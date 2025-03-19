/**
 * @file modbus_public_type.h
 * @author niwciu (niwciu@gmail.com)
 * @brief Defines public types and enums used in Modbus communication.
 * @date 2024-05-30
 *
 * This header file defines various enums and typedefs that are essential for Modbus communication
 * and error handling. It includes enums for Modbus modes (RTU and unknown), parity settings,
 * Modbus exception codes, Modbus function codes, and other types used in Modbus data communication.
 *
 * This file serves as a public interface to define Modbus-specific types, making the codebase more
 * readable and maintainable by providing clear abstractions for Modbus-related concepts.
 *
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
    /**
     * @brief Enum defining Modbus communication modes.
     */
    typedef enum
    {
        RTU,         /**< Modbus RTU mode. */
        UNKNOWN_MODE /**< Unknown Modbus mode. */
    } modbus_mode_t;

    /**
     * @brief Enum defining parity settings for serial communication.
     */
    typedef enum
    {
        NONE, /**< No parity. */
        ODD,  /**< Odd parity. */
        EVEN  /**< Even parity. */
    } parity_t;

    /**
     * @brief Enum defining Modbus exception codes.
     */
    typedef enum
    {
        MODBUS_ILLEGAL_FUNCTION_ERROR = 0x01U,     /**< Illegal function error code. */
        MODBUS_ILLEGAL_DATA_ADDRESS_ERROR = 0x02U, /**< Illegal data address error code. */
        MODBUS_ILLEGAL_DATA_VALUE_ERROR = 0x03U,   /**< Illegal data value error code. */
        MODBUS_SERVER_DEVICE_FAILURE_ERROR = 0x04U /**< Server device failure error code. */
    } modbus_exception_code_t;

    /**
     * @brief Enum defining Modbus master request return error codes.
     */
    typedef enum
    {
        MODBUS_MASTER_REQUEST_SENDED_TO_QUEUE,  /**< OK - request was added to queue succesfully. */
        MODBUS_MASTER_FREE_QUEUE_EMPTY_ERROR,   /**< Error when free queue is empty. */
        MODBUS_MASTER_LIB_PDU_REQ_ERROR,        /**< Error in Modbus PDU request. */
        MODBUS_MASTER_LIB_RTU_SEND_ERROR        /**< Error sending Modbus RTU message. */
    } modbus_master_req_ret_t;

    /**
     * @brief Enum defining Modbus master response read error codes.
     */
    typedef enum
    {
        MODBUS_MASTER_RESP_RTU_CRC_ERR, /**< Modbus RTU CRC error in response. */
        MODBUS_MASTER_RESP_TIMEOUT_ERR, /**< Modbus response timeout error. */
        MODBUS_MASTER_RESP_FRAME_ERR    /**< Modbus response frame error. */
    } modbus_master_resp_read_error_t;

    /**
     * @brief Modbus function codes enumeration.
     */
    typedef enum
    {
        MODBUS_READ_COILS_FUNC_CODE = 0x01U,              /**< Function code for reading coils. */
        MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE = 0x02U,    /**< Function code for reading discrete inputs. */
        MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE = 0x03U,  /**< Function code for reading holding registers. */
        MODBUS_READ_INPUT_REGISTERS_FUNC_CODE = 0x04U,    /**< Function code for reading input registers. */
        MODBUS_WRITE_SINGLE_COIL_FUNC_CODE = 0x05U,       /**< Function code for writing a single coil. */
        MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE = 0x06U,   /**< Function code for writing a single register. */
        MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE = 0x0FU,    /**< Function code for writing multiple coils. */
        MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE = 0x10U, /**< Function code for writing multiple registers. */
    } modbus_fun_code_t;

    typedef uint16_t modbus_adr_t;      /**< Modbus address type. */
    typedef bool modbus_coil_disin_t;   /**< Modbus coil and discrete input type (boolean). */
    typedef uint16_t modbus_reg_t;      /**< Modbus register type. */
    typedef uint16_t modbus_data_qty_t; /**< Modbus data quantity type. */
    typedef uint8_t modbus_device_ID_t; /**< Modbus device ID type. */
    typedef uint32_t baud_t;            /**< Baud rate type. */

    /**
     * @brief Structure defining Modbus error report.
     */
    typedef struct
    {
        modbus_device_ID_t slave_ID;                     /**< Modbus slave ID. */
        modbus_fun_code_t fun_conde;                     /**< Modbus function code. */
        modbus_adr_t data_adr;                           /**< Address of the data that caused the error. */
        modbus_data_qty_t data_qty;                      /**< Quantity of data objects that caused the error. */
        modbus_master_resp_read_error_t resp_read_error; /**< Modbus master response read error. */
    } modbus_master_error_report_t;

    typedef struct
    {
        modbus_device_ID_t slave_ID;            /**< Modbus slave ID. */
        modbus_fun_code_t fun_conde;            /**< Modbus function code. */
        modbus_exception_code_t exception_code; /**< Modbus exception code. */
        modbus_adr_t data_adr;                  /**< Modbus adress of processed object. */
        modbus_data_qty_t data_qty;             /**< Quantitt of processed object. First object located under data_adr position. */
        // modbus_coil_disin_t readed_coil_val;
        // modbus_reg_t readed_reg_val;

    } modbus_read_data_t;

    /**
     * @brief Typedef for Modbus master error callback function.
     *
     * This callback function is invoked when there is an error reported by the Modbus master.
     * It provides a pointer to a structure containing details about the error.
     *
     * @param error_report Pointer to the Modbus error report structure.
     */
    typedef void (*modbus_master_error_cb_t)(const modbus_master_error_report_t *error_report);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif /* _MODBUS_PUBLIC_TYPE_H_ */