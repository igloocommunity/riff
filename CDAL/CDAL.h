/*
 * CDAL.h
 *
 * Copyright (C) ST-Ericsson SA 2011
 * Authors: Srimanta Panda <srimanta.panda@stericsson.com>,
 *          Ola Borgelin <ola.borgelin@stericsson.com>,
 *          Karin Hedlund <karin.hedlund@stericsson.com>,
 *          Markus Andersson <markus.m.andersson@stericsson.com> for ST-Ericsson.
 * License terms: 3-clause BSD license
 *
 * CDAL API is implementation of communication device abstraction library,
 * which adapts the usb driver interface. This device communication abstraction
 * library is used to communicate with the device through the communication
 * channel.
 */
#pragma once

#if defined(_WIN32)
#ifdef CDAL_EXPORTS
#define CDAL_API __declspec(dllexport)
#else
#define CDAL_API __declspec(dllimport)
#endif
#elif defined(__linux__)
#include <cstddef>
#ifdef CDAL_EXPORTS
#define CDAL_API __attribute__((visibility("default")))
#else
#define CDAL_API
#endif
#endif

// type of the device
typedef enum {
    USB_DEVICE,
    UART_DEVICE,
    UNKNOWN_DEVICE
} DeviceType_t;

// error status codes
typedef enum {
    COMM_DEVICE_SUCCESS = 0,
    COMM_DEVICE_GENERAL_ERROR = 1,
    COMM_DEVICE_UNSUPPORTED_OPERATION = 2,
    COMM_DEVICE_OBJECT_NOT_MAPPED = 3,
    COMM_DEVICE_UART_FAILED_TO_OPEN_PORT = 4,
    COMM_DEVICE_UART_FAILED_TO_CONFIGURE_PORT = 5,
    COMM_DEVICE_LIBUSB_FAILED_TO_OPEN_PORT = 6,
    COMM_DEVICE_LIBUSB_FAILED_TO_CLAIM_INTERFACE = 7,
    COMM_DEVICE_PEEK_BUFFER_SIZE_ERROR = 8,
    COMM_DEVICE_LIBUSB_FAILED_TO_SET_CONFIG = 9
} DeviceStatus_t;

// device events codes
typedef enum {
    COMM_DEVICE_UNDEFINED_EVENT = -1,
    MEFLASH_DEVICE_CONNECTED = 0,
    MEFLASH_DEVICE_DISCONNECTED = 1,
    LIBUSB_DEVICE_CONNECTED = 2,
    LIBUSB_DEVICE_DISCONNECTED = 3,
    NOMADIK_DEVICE_CONNECTED = 4,
    NOMADIK_DEVICE_DISCONNECTED = 5
} DeviceEvent_t;

typedef struct CommDevice* Device_t;

// event and id parameters are valid only if status is 0 (COMM_DEVICE_SUCCESS)
typedef void (*EventCallback_t)(DeviceStatus_t status, DeviceEvent_t event, Device_t id);

typedef void (*DataCallback_t)(void* buffer, size_t len, void* param);

extern "C" {
    CDAL_API void usb_init_driver(const char* vendorID, const char* productID);
    CDAL_API void usb_set_listen_callback(EventCallback_t callback);
    CDAL_API void usb_deinit_driver();
    CDAL_API void usb_destroy_device(Device_t device, int error_code);

    CDAL_API int comm_read(Device_t device, void* buffer, size_t size);
    CDAL_API int comm_read_nowait(void* buffer, size_t size, DataCallback_t cb, void* param);
    CDAL_API int comm_write(Device_t device, void* buffer, size_t len);
    CDAL_API int comm_write_nowait(void* buffer, size_t size, DataCallback_t cb, void* param);
    CDAL_API int comm_cancel(void* param);
    CDAL_API unsigned long comm_get_physical_address(Device_t device);
    CDAL_API void** comm_get_object_storage(Device_t device);
    CDAL_API void comm_progress(void *Communication_p, unsigned long long totalbytes, unsigned long long tranferedbytes);
}
