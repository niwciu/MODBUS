/**
 * @file modbus_public_type.h
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-05-30
 *
 * @copyright Copyright (c) 2024
 *
 */
#ifndef _MODBUS_PUBLIC_TYPE_H_
#define _MODBUS_PUBLIC_TYPE_H_

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
#include <stdint.h>
#include <stdbool.h>
    typedef enum
    {
        RTU,
        UNKNOWN
    } modbus_mode_t;

    typedef enum
    {
        NONE,
        ODD,
        EVEN
    } parity_t;
    typedef enum
    {
        MODBUS_MASTER_REQUEST_SEND,
        MODBUS_MASTER_FREE_QUEUE_EMPTY_ERR,
        MODBUS_MASTER_LIB_REQ_ERROR,
        MODBUS_MASTER_LIB_RTU_SEND_ERROR,
    } modbus_master_error_t;

    typedef uint16_t modbus_adr_t;
    typedef bool modbus_coil_disin_t;
    typedef uint16_t modbus_reg_t;
    typedef uint16_t modbus_data_qty_t;
    typedef uint8_t modbus_device_ID_t;
    typedef uint32_t baud_t;
#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_PUBLIC_TYPE_H_ */