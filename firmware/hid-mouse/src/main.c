/*
 * Copyright (c) 2025 ZiTe SideraKB
 * Copyright (c) 2018 qianfan Zhao
 * Copyright (c) 2018, 2023 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <sample_usbd.h>

#include <string.h>

#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/input/input.h>
#include <zephyr/sys/util.h>

#include <zephyr/usb/usb_device.h>
#include <zephyr/usb/usbd.h>
#include <zephyr/usb/class/usb_hid.h>

#include <zephyr/logging/log.h>
LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

static const struct device *motion_sensor = DEVICE_DT_GET(DT_NODELABEL(pmw3610));
static const struct gpio_dt_spec led0 = GPIO_DT_SPEC_GET(DT_ALIAS(led0), gpios);
static const struct gpio_dt_spec led1 = GPIO_DT_SPEC_GET(DT_ALIAS(led1), gpios);

static const uint8_t hid_report_desc[] = HID_MOUSE_REPORT_DESC(2);
static enum usb_dc_status_code usb_status;

#define MOUSE_BTN_LEFT  0
#define MOUSE_BTN_RIGHT 1

enum mouse_report_idx {
    MOUSE_BTN_REPORT_IDX = 0,
    MOUSE_X_REPORT_IDX = 1,
    MOUSE_Y_REPORT_IDX = 2,
    MOUSE_WHEEL_REPORT_IDX = 3,
    MOUSE_REPORT_COUNT = 4,
};

K_MSGQ_DEFINE(mouse_msgq, MOUSE_REPORT_COUNT, 2, 1);
static K_SEM_DEFINE(ep_write_sem, 0, 1);

#if defined(CONFIG_USB_DEVICE_STACK_NEXT)
static int enable_usb_device_next(void) {
    struct usbd_context *sample_usbd;
    int err;

    sample_usbd = sample_usbd_init_device(NULL);
    if (sample_usbd == NULL) {
        LOG_ERR("Failed to initialize USB device");
        return -ENODEV;
    }

    err = usbd_enable(sample_usbd);
    if (err) {
        LOG_ERR("Failed to enable device support");
        return err;
    }

    LOG_DBG("USB device support enabled");

    return 0;
}
#endif /* defined(CONFIG_USB_DEVICE_STACK_NEXT) */

static inline void hid_status_cb(enum usb_dc_status_code status, const uint8_t *param) {
    usb_status = status;
}

static void hid_int_in_ready_cb(const struct device *dev) {
    ARG_UNUSED(dev);
    k_sem_give(&ep_write_sem);
}

static const struct hid_ops ops = {
    .int_in_ready = hid_int_in_ready_cb,
};

static void hid_thread_entry(void) {
    const struct device *hid_dev;
    int ret;

#if defined(CONFIG_USB_DEVICE_STACK_NEXT)
    hid_dev = DEVICE_DT_GET_ONE(zephyr_hid_device);
#else
    hid_dev = device_get_binding("HID_0");
#endif
    if (hid_dev == NULL) {
        LOG_ERR("Cannot get USB HID Device");
        return;
    }

    usb_hid_register_device(hid_dev, hid_report_desc, sizeof(hid_report_desc), &ops);
    ret = usb_hid_init(hid_dev);
    if (ret) {
        LOG_ERR("Failed to initialize USB HID");
        return;
    }

#if defined(CONFIG_USB_DEVICE_STACK_NEXT)
    ret = enable_usb_device_next();
#else
    ret = usb_enable(hid_status_cb);
#endif
    if (ret) {
        LOG_ERR("Failed to enable USB");
        return;
    }

    LOG_INF("USB HID Initialized");

    while (true) {
        UDC_STATIC_BUF_DEFINE(report, MOUSE_REPORT_COUNT);

        k_msgq_get(&mouse_msgq, &report, K_FOREVER);

        ret = hid_int_ep_write(hid_dev, report, MOUSE_REPORT_COUNT, NULL);
        if (ret) {
            LOG_ERR("HID write error, %d", ret);
        } else {
            k_sem_take(&ep_write_sem, K_FOREVER);
            (void)gpio_pin_toggle(led1.port, led1.pin);
        }
    }
}

K_THREAD_STACK_DEFINE(hid_thread_stack, 1024);
K_THREAD_DEFINE(hid_thread, K_THREAD_STACK_SIZEOF(hid_thread_stack), hid_thread_entry, NULL, NULL, NULL, K_PRIO_PREEMPT(0), 0, 0);

static ALWAYS_INLINE void rwup_if_suspended(void) {
    if (IS_ENABLED(CONFIG_USB_DEVICE_REMOTE_WAKEUP)) {
        if (usb_status == USB_DC_SUSPEND) {
            usb_wakeup_request();
            return;
        }
    }
}

static void input_cb(struct input_event *evt, void *user_data) {
    ARG_UNUSED(user_data);

    static int8_t tmp[MOUSE_REPORT_COUNT];
    bool should_send_report = false;

    switch (evt->code) {
    case INPUT_KEY_0:
        rwup_if_suspended();
        WRITE_BIT(tmp[MOUSE_BTN_REPORT_IDX], MOUSE_BTN_LEFT, evt->value);
        should_send_report = true;
        break;

    case INPUT_KEY_1:
        rwup_if_suspended();
        WRITE_BIT(tmp[MOUSE_BTN_REPORT_IDX], MOUSE_BTN_RIGHT, evt->value);
        should_send_report = true;
        break;

    case INPUT_REL_X: {
        int v = CLAMP(evt->value, -127, 127);
        tmp[MOUSE_X_REPORT_IDX] = (int8_t)(tmp[MOUSE_X_REPORT_IDX] + v);
        LOG_INF("Motion X: %d", v);
        should_send_report = false; /* Wait for possible Y axis data */
        break;
    }

    case INPUT_REL_Y: {
        int v = CLAMP(evt->value, -127, 127);
        tmp[MOUSE_Y_REPORT_IDX] = (int8_t)(tmp[MOUSE_Y_REPORT_IDX] + v);
        LOG_INF("Motion Y: %d", v);
        should_send_report = true;
        break;
    }

    default:
        LOG_INF("Unrecognized input code %u value %d", evt->code, evt->value);
        return;
    }

    if (should_send_report && (usb_status == USB_DC_CONFIGURED)) {
        if (k_msgq_put(&mouse_msgq, tmp, K_NO_WAIT) != 0) {
            LOG_ERR("Failed to put new input event");
        }

        tmp[MOUSE_X_REPORT_IDX] = 0;
        tmp[MOUSE_Y_REPORT_IDX] = 0;
    }
}

INPUT_CALLBACK_DEFINE(NULL, input_cb, NULL);

static int led_init(void) {
    int ret;

    if (!gpio_is_ready_dt(&led0)) {
        LOG_ERR("LED device %s is not ready", led0.port->name);
        return -1;
    }

    ret = gpio_pin_configure_dt(&led0, GPIO_OUTPUT);
    if (ret) {
        LOG_ERR("Failed to configure the LED pin, error: %d", ret);
        return -1;
    }

    if (!gpio_is_ready_dt(&led1)) {
        LOG_ERR("LED device %s is not ready", led1.port->name);
        return -1;
    }

    ret = gpio_pin_configure_dt(&led1, GPIO_OUTPUT);
    if (ret) {
        LOG_ERR("Failed to configure the LED pin, error: %d", ret);
        return -1;
    }

    return 0; /* Ok */
}

static int motion_sensor_init(void) {
    if (!device_is_ready(motion_sensor)) {
        LOG_ERR("Motion sensor device not ready");
        return -1;
    }

    return 0; /* Ok */
}

int main(void) {
    int ret;

    LOG_INF("Starting PMW3610 example");

    ret = led_init();
    if (ret) {
        LOG_ERR("Failed to initialize LEDs");
        return 0;
    }

    ret = motion_sensor_init();
    if (ret) {
        LOG_ERR("Failed to initialize motion sensor");
        return 0;
    }

    while (true) {
        (void)gpio_pin_toggle(led0.port, led0.pin);
        k_msleep(1000);
    }
    return 0;
}
