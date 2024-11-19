/**
 * @file modbus_slave.c
 * @brief Implementation of Modbus slave functions for handling communication and data management.
 * @date 2024-05-31
 *
 * This file contains the implementation of Modbus slave functions that manage communication
 * with a Modbus master. It includes functionalities for reading and writing coils, discrete
 * inputs, holding registers, and input registers. Additionally, it implements a state machine
 * and callbacks for handling Modbus RTU communication.
 *
 * This implementation assumes the use of a Modbus RTU driver for serial communication. It provides
 * functions to initialize the Modbus slave, register application data to Modbus tables, check Modbus
 * requests, and manage various states during the Modbus communication cycle.
 *
 * @see modbus_slave.h
 * @see modbus_public.type.h
 * @see modbus_config.h
 * @see modbus_driver_interface.h
 *
 * @copyright Copyright (c) 2024
 */
#include "modbus_slave.h"
#include "modbus_slave_PDU.h"
#include "modbus_slave_RTU.h"
#include "modbus_RTU.h"
#include "modbus_type.h"
#include "modbus_driver_interface.h"
#include <stddef.h>

#ifdef UNIT_TEST
#define PRIVATE
#else
#define PRIVATE static
#endif
PRIVATE modbus_device_ID_t modbus_slave_ID;
PRIVATE modbus_slave_state_t slave_manager_state_machine = MODBUS_SLAVE_UNKNOWN;
PRIVATE const modbus_RTU_driver_struct_t *slave_RTU_driver = NULL;
PRIVATE modbus_msg_t slave_msg;
PRIVATE modbus_msg_t *slave_msg_ptr = NULL;
PRIVATE modbus_status_flag_t TIMER_1_5_CHAR_FLAG = MODBUS_FLAG_UNKNOWN;
PRIVATE modbus_status_flag_t TIMER_3_5_CHAR_FLAG = MODBUS_FLAG_UNKNOWN;
PRIVATE modbus_status_flag_t FRAME_ERROR_FLAG = MODBUS_FLAG_UNKNOWN;
PRIVATE modbus_status_flag_t RESP_TRANSMITION_FLAG = MODBUS_FLAG_UNKNOWN;

static void handle_modbus_slave_idle_state(void);
static void handle_modbus_slave_msg_recived_state(void);
static void handle_modbus_slave_transmitting_resp_state(void);
static void handle_modbus_slave_default_state(void);
static void init_modbus_data_buffers_and_queues(modbus_mode_t mode);
static void register_msg_req_resp_data_buffers(modbus_mode_t mode);
static void init_modbus_driver(modbus_mode_t mode, baud_t baud_rate, parity_t parity);
static void init_modbus_slave_internall_data(modbus_device_ID_t Slave_ID);
static void modbus_resp_send_callback(void);
static void modbus_T_1_5_char_expired_callback(void);
static void modbus_T_3_5_char_expired_callback(void);
static void modbus_frame_error_callback(void);

/**
 * @brief Registers application data to Modbus slave coils table.
 *
 * This function registers application data to the Modbus slave coils table at the specified coil address.
 *
 * @param coil_adr Coil address where the application data will be registered.
 * @param app_data_ptr Pointer to the application data to be registered.
 */
void register_app_data_to_modbus_slave_coils_table(modbus_adr_t coil_adr, modbus_coil_disin_t *app_data_ptr)
{
    uint8_t a[3];
    a[4]=5;
    register_app_data_to_modbus_coils_din_table(Slave_Coils, coil_adr, app_data_ptr);
}

/**
 * @brief Registers application data to Modbus slave discrete inputs table.
 *
 * This function registers application data to the Modbus slave discrete inputs table at the specified address.
 *
 * @param disin_adr Discrete input address where the application data will be registered.
 * @param app_data_ptr Pointer to the application data to be registered.
 */
void register_app_data_to_modbus_slave_din_table(modbus_adr_t disin_adr, modbus_coil_disin_t *app_data_ptr)
{
    register_app_data_to_modbus_coils_din_table(Slave_Discrete_Inputs, disin_adr, app_data_ptr);
}

/**
 * @brief Registers application data to Modbus slave input registers table.
 *
 * This function registers application data to the Modbus slave input registers table at the specified register address.
 *
 * @param reg_adr Register address where the application data will be registered.
 * @param app_data_ptr Pointer to the application data to be registered.
 */
void register_app_data_to_modbus_slave_inreg_table(modbus_adr_t reg_adr, modbus_reg_t *app_data_ptr)
{
    register_app_data_to_modbus_reg_table(Slave_Input_Registers, reg_adr, app_data_ptr);
}

/**
 * @brief Registers application data to Modbus slave holding registers table.
 *
 * This function registers application data to the Modbus slave holding registers table at the specified register address.
 *
 * @param reg_adr Register address where the application data will be registered.
 * @param app_data_ptr Pointer to the application data to be registered.
 */
void register_app_data_to_modbus_slave_hreg_table(modbus_adr_t reg_adr, modbus_reg_t *app_data_ptr)
{
    register_app_data_to_modbus_reg_table(Slave_Holding_Registers, reg_adr, app_data_ptr);
}

/**
 * @brief Initializes the Modbus slave.
 *
 * This function initializes the Modbus slave based on the specified mode, baud rate, parity, and slave ID.
 * It sets up data buffers, initializes the Modbus driver for communication, and prepares internal data structures.
 *
 * @param mode Modbus mode (e.g., RTU).
 * @param baud_rate Baud rate for serial communication.
 * @param parity Parity for serial communication.
 * @param slave_ID Modbus slave ID.
 */
void modbus_slave_init(modbus_mode_t mode, baud_t baud_rate, parity_t parity, modbus_device_ID_t slave_ID)
{
    init_modbus_data_buffers_and_queues(mode);
    init_modbus_driver(mode, baud_rate, parity);
    init_modbus_slave_internall_data(slave_ID);
}

/**
 * @brief Checks for incoming Modbus requests and processes them.
 *
 * This function checks the Modbus slave state machine and handles Modbus requests accordingly.
 * It transitions between different states (idle, message received, transmitting response) based on
 * the state of flags indicating timer expirations and frame errors during Modbus communication.
 *
 * @warning This function needs to be called periodically to keep the Modbus slave application running and operational.
 */
void check_modbus_request(void)
{
    switch (slave_manager_state_machine)
    {
    case MODBUS_SLAVE_IDLE:
        handle_modbus_slave_idle_state();
        break;
    case MODBUS_SLAVE_MSG_RECIVED:
        handle_modbus_slave_msg_recived_state();
        break;
    case MODBUS_SLAVE_TRANSMITTING_RESP:
        handle_modbus_slave_transmitting_resp_state();
        break;
    default:
        handle_modbus_slave_default_state();
        break;
    }
}

/**
 * @brief Handles the Modbus slave state machine in the idle state.
 *
 * This function handles the Modbus slave state machine when it is in the idle state.
 * It checks for the expiration of the 1.5 character time and initiates reception of Modbus messages.
 */
static void handle_modbus_slave_idle_state(void)
{
    if (TIMER_1_5_CHAR_FLAG == MODBUS_FLAG_SET)
    {
        modbus_ret_t RTU_status;
        TIMER_1_5_CHAR_FLAG = MODBUS_FLAG_CLEARED;
        RTU_status = modbus_RTU_recv(slave_msg_ptr->req.data, slave_msg_ptr->req.len, modbus_slave_ID);
        if (RTU_status != RET_OK)
        {
            FRAME_ERROR_FLAG = MODBUS_FLAG_SET;
        }
        slave_manager_state_machine = MODBUS_SLAVE_MSG_RECIVED;
    }
}

/**
 * @brief Handles the Modbus slave state machine in the message received state.
 *
 * This function handles the Modbus slave state machine when it is in the message received state.
 * It checks for frame errors and the expiration of the 3.5 character time before preparing
 * and transmitting the Modbus response message.
 */
static void handle_modbus_slave_msg_recived_state(void)
{
    if ((MODBUS_FLAG_SET == FRAME_ERROR_FLAG) && (MODBUS_FLAG_SET == TIMER_3_5_CHAR_FLAG))
    {
        slave_msg_ptr->req.len = 0;
        slave_manager_state_machine = MODBUS_SLAVE_IDLE;
        FRAME_ERROR_FLAG = MODBUS_FLAG_CLEARED;
        TIMER_3_5_CHAR_FLAG = MODBUS_FLAG_CLEARED;
    }
    else if ((MODBUS_FLAG_CLEARED == FRAME_ERROR_FLAG) && (MODBUS_FLAG_SET == TIMER_3_5_CHAR_FLAG))
    {
        parse_master_request_and_prepare_resp(slave_msg_ptr);
        modbus_RTU_send(slave_msg_ptr->resp.data, &slave_msg_ptr->resp.len, modbus_slave_ID);
        slave_RTU_driver->send(slave_msg_ptr->resp.data, slave_msg_ptr->resp.len);
        RESP_TRANSMITION_FLAG = MODBUS_FLAG_SET;
        slave_manager_state_machine = MODBUS_SLAVE_TRANSMITTING_RESP;
        TIMER_3_5_CHAR_FLAG = MODBUS_FLAG_CLEARED;
    }
    else
    {
        // Do nothing until flags are set as expected
    }
}

/**
 * @brief Handles the Modbus slave state machine in the transmitting response state.
 *
 * This function handles the Modbus slave state machine when it is in the transmitting response state.
 * It checks for the completion of response transmission before transitioning back to the idle state.
 */
static void handle_modbus_slave_transmitting_resp_state(void)
{
    if (MODBUS_FLAG_CLEARED == RESP_TRANSMITION_FLAG)
    {
        slave_manager_state_machine = MODBUS_SLAVE_IDLE;
        slave_msg_ptr->resp.len = 0;
        slave_msg_ptr->req.len = 0;
    }
}

/**
 * @brief Handles the Modbus slave state machine in the default state.
 *
 * This function handles the Modbus slave state machine when it is in an unknown state.
 * It resets all flags and clears data buffers to ensure a clean state transition.
 */
static void handle_modbus_slave_default_state(void)
{
    slave_manager_state_machine = MODBUS_SLAVE_IDLE;
    TIMER_1_5_CHAR_FLAG = MODBUS_FLAG_CLEARED;
    TIMER_3_5_CHAR_FLAG = MODBUS_FLAG_CLEARED;
    FRAME_ERROR_FLAG = MODBUS_FLAG_CLEARED;
    RESP_TRANSMITION_FLAG = MODBUS_FLAG_CLEARED;
    slave_msg_ptr->resp.len = 0;
    slave_msg_ptr->req.len = 0;
}

/**
 * @brief Initializes Modbus data buffers and queues based on the specified mode.
 *
 * This function initializes Modbus data buffers and queues based on the specified mode (e.g., RTU).
 * It registers request and response data buffers to be used for Modbus communication.
 *
 * @param mode Modbus mode (e.g., RTU).
 */
static void init_modbus_data_buffers_and_queues(modbus_mode_t mode)
{
    register_msg_req_resp_data_buffers(mode);
    slave_msg_ptr = &slave_msg;
}

/**
 * @brief Registers message request and response data buffers based on the specified mode.
 *
 * This function registers message request and response data buffers based on the specified mode (e.g., RTU).
 * It sets up the data buffers used for Modbus communication.
 *
 * @param mode Modbus mode (e.g., RTU).
 */
static void register_msg_req_resp_data_buffers(modbus_mode_t mode)
{
    if (RTU == mode)
    {
        slave_msg.req.data = slave_RTU_req_buf;
        slave_msg.resp.data = slave_RTU_resp_buf;
    }
}

/**
 * @brief Initializes the Modbus driver based on the specified mode, baud rate, and parity.
 *
 * This function initializes the Modbus driver based on the specified mode (e.g., RTU), baud rate,
 * and parity for serial communication.
 *
 * @param mode Modbus mode (e.g., RTU).
 * @param baud_rate Baud rate for serial communication.
 * @param parity Parity for serial communication.
 */
static void init_modbus_driver(modbus_mode_t mode, baud_t baud_rate, parity_t parity)
{
    if (RTU == mode)
    {
        slave_RTU_driver = get_slave_RTU_driver_interface();
        slave_RTU_driver->init(baud_rate, parity);
        slave_RTU_driver->enable_rcev(&slave_msg_ptr->req);
        slave_RTU_driver->subscribe_msg_tx_done_cb(modbus_resp_send_callback);
        slave_RTU_driver->subscribe_t_1_5_char_expired_cb(modbus_T_1_5_char_expired_callback);
        slave_RTU_driver->subscribe_t_3_5_char_expired_cb(modbus_T_3_5_char_expired_callback);
        slave_RTU_driver->subscribe_modbus_frame_error_cb(modbus_frame_error_callback);
    }
    // else if (OTHER_MODE == mode)
    else
    {
        slave_RTU_driver = NULL;
    }
}

/**
 * @brief Initializes internal Modbus slave data based on the specified slave ID.
 *
 * This function initializes internal Modbus slave data based on the specified slave ID.
 * It sets up the slave ID and initializes state machine flags for communication handling.
 *
 * @param Slave_ID Modbus slave ID.
 */
static void init_modbus_slave_internall_data(modbus_device_ID_t Slave_ID)
{
    modbus_slave_ID = Slave_ID;

    slave_manager_state_machine = MODBUS_SLAVE_IDLE;

    TIMER_1_5_CHAR_FLAG = MODBUS_FLAG_CLEARED;
    TIMER_3_5_CHAR_FLAG = MODBUS_FLAG_CLEARED;
    FRAME_ERROR_FLAG = MODBUS_FLAG_CLEARED;
    RESP_TRANSMITION_FLAG = MODBUS_FLAG_CLEARED;
}

/**
 * @brief Modbus response send callback function.
 *
 * This function serves as a callback handler when a Modbus response is successfully sent.
 * It clears the response transmission flag to indicate completion of response transmission.
 */
static void modbus_resp_send_callback(void)
{
    RESP_TRANSMITION_FLAG = MODBUS_FLAG_CLEARED;
}

/**
 * @brief Modbus T_1_5 character time expired callback function.
 *
 * This function serves as a callback handler when the T_1_5 character time has expired in Modbus communication.
 * It sets the timer flag to initiate the reception of Modbus messages.
 */
static void modbus_T_1_5_char_expired_callback(void)
{
    TIMER_1_5_CHAR_FLAG = MODBUS_FLAG_SET;
}

/**
 * @brief Modbus T_3_5 character time expired callback function.
 *
 * This function serves as a callback handler when the T_3_5 character time has expired in Modbus communication.
 * It sets the timer flag to handle frame errors and prepare for response transmission.
 */
static void modbus_T_3_5_char_expired_callback(void)
{
    TIMER_3_5_CHAR_FLAG = MODBUS_FLAG_SET;
}

/**
 * @brief Modbus frame error callback function.
 *
 * This function serves as a callback handler when a frame error occurs in Modbus communication.
 * It sets the frame error flag to handle communication errors and reset message reception.
 */
static void modbus_frame_error_callback(void)
{
    FRAME_ERROR_FLAG = MODBUS_FLAG_SET;
}