/**
 * @file modbus_PDU.c
 * @author niwciu (niwciu@gmail.com)
 * @brief
 * @date 2024-05-06
 *
 * @copyright Copyright (c) 2024
 *
 */

#include "modbus_PDU.h"
#include "buf_rw.h"
// #include "modbus_data.h"  // dodane w hederze modsbus_PDU.h
#include <stdio.h>

static modbus_ret_t read_reg_request(modbus_buf_t *send_buf, modbus_req_t req_code, modbus_adr_t adr, modbus_data_t len);
static modbus_ret_t write_single_reg_coil_request(modbus_buf_t *send_buf, modbus_req_t req_code, modbus_adr_t adr);
static modbus_data_t modbus_get_max_len(modbus_req_t req_code);
static modbus_byte_count_t get_coil_din_byte_count(modbus_data_qty_t coil_qty);
static void clear_coil_din_status_byte(modbus_buf_t *buf, modbus_data_qty_t qty);
static void set_coil_din_value_from_modbus_msg(const modbus_buf_t *data_state_ptr, modbus_adr_t start_adr, modbus_data_qty_t coil_din_qty, modbus_coil_disin_t **data_tab);
static void set_inreg_hreg_value_from_modbus_msg(const modbus_buf_t *msg_data_ptr, modbus_adr_t start_adr, modbus_data_qty_t reg_qty, modbus_reg_t **data_tab);
static modbus_ret_t update_master_data_from_modbus_msg(const modbus_buf_t *resp_msg, const modbus_buf_t *req_msg, modbus_fun_code_t fun_code, void **data_tab);
static void update_master_specific_data_type_from_modbus_msg(const modbus_buf_t *resp_msg, const modbus_buf_t *req_msg, modbus_fun_code_t fun_code, void **data_tab);
static modbus_byte_count_t get_expected_byte_cnt(modbus_fun_code_t func_code, modbus_data_qty_t req_data_qty);
static modbus_ret_t check_slave_resp_vs_req_for_sigle_write(const modbus_buf_t *resp_msg, const modbus_buf_t *req_msg);

static modbus_ret_t read_reg_request(modbus_buf_t *send_buf, modbus_req_t req_code, modbus_adr_t adr, modbus_data_t len)
{
    if ((len <= modbus_get_max_len(req_code)) && (len >= MODBUS_MIN_REG_COIL_QTY))
    {
        send_buf[MODBUS_FUNCTION_CODE_IDX] = req_code;
        write_u16_to_buf(send_buf + MODBUS_REQUEST_ADR_IDX, adr);
        write_u16_to_buf(send_buf + MODBUS_REQUEST_LEN_IDX, len);
        return MODBUS_READ_REQUEST_LEN;
    }
    else
    {
        return RET_ERROR;
    }
}

static modbus_ret_t write_single_reg_coil_request(modbus_buf_t *send_buf, modbus_req_t req_code, modbus_adr_t adr)
{
    send_buf[MODBUS_FUNCTION_CODE_IDX] = req_code;
    write_u16_to_buf(send_buf + MODBUS_REQUEST_ADR_IDX, adr);
    if( MODBUS_WRITE_SINGLE_COIL_FUNC_CODE == req_code)
    {
        write_u16_to_buf(send_buf + MODBUS_REQUEST_LEN_IDX, (get_coil_state(Master_Coils,adr) * COIL_ON));
    }
    else
    {
        write_u16_to_buf(send_buf + MODBUS_REQUEST_LEN_IDX, get_holding_register_value(Master_Holding_Registers,adr));
    }
    return MODBUS_WRITE_SINGLE_REQUEST_LEN;
}

static modbus_data_t modbus_get_max_len(modbus_req_t req_code)
{
    modbus_data_t max_len = 0;
    switch (req_code)
    {
    case MODBUS_READ_COILS_FUNC_CODE:
        max_len = MODBUS_MAX_READ_COILS_QTY;
        break;
    case MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE:
        max_len = MODBUS_MAX_WRITE_COILS_QTY;
        break;
    case MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE:
        max_len = MODBUS_MAX_READ_DISCRETE_INPUTS_QTY;
        break;
    case MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE:
    case MODBUS_READ_INPUT_REGISTERS_FUNC_CODE:
        max_len = MODBUS_MAX_READ_REG_QTY;
        break;
    case MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE:
        max_len = MODBUS_MAX_WRITE_REG_QTY;
        break;
    case MODBUS_WRITE_SINGLE_COIL_FUNC_CODE:
    case MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE:
    default:
        max_len = MODBUS_MIN_REG_COIL_QTY;
        break;
    }
    return max_len;
}

static modbus_byte_count_t get_coil_din_byte_count(modbus_data_qty_t coil_qty)
{
    modbus_byte_count_t byte_count;

    byte_count = (coil_qty / 8);
    if (coil_qty % 8)
    {
        byte_count++;
    }
    return byte_count;
}

static void clear_coil_din_status_byte(modbus_buf_t *buf, modbus_data_qty_t qty)
{
    for (uint8_t i = 0; i < qty; i++)
    {
        *(buf + i) = 0;
    }
}

static void set_coil_din_value_from_modbus_msg(const modbus_buf_t *data_state_ptr, modbus_adr_t start_adr, modbus_data_qty_t coil_din_qty, modbus_coil_disin_t **data_tab)
{
    for (modbus_data_qty_t i = 0; i < coil_din_qty; i++)
    {
        if (0 != (*(data_state_ptr + (i / 8)) & (1 << (i % 8))))
        {
            set_coil_state(data_tab, (start_adr + i), 1);
        }
        else
        {
            set_coil_state(data_tab, (start_adr + i), 0);
        }
    }
}
static void set_inreg_hreg_value_from_modbus_msg(const modbus_buf_t *msg_data_ptr, modbus_adr_t start_adr, modbus_data_qty_t reg_qty, modbus_reg_t **data_tab)
{
    for (modbus_data_qty_t i = 0; i < reg_qty; i++)
    {
        modbus_data_t reg_data = read_u16_from_buf(msg_data_ptr + (i * 2));
        set_register_value(data_tab, start_adr + i, reg_data);
    }
}
static modbus_ret_t update_master_data_from_modbus_msg(const modbus_buf_t *resp_msg, const modbus_buf_t *req_msg, modbus_fun_code_t fun_code, void **data_tab)
{

    modbus_byte_count_t slave_resp_byte_cnt = resp_msg[MODBUS_RESP_READ_BYTE_CNT_IDX];
    modbus_data_qty_t requested_data_qty = read_u16_from_buf(req_msg + MODBUS_REQUEST_LEN_IDX);
    modbus_data_qty_t expected_byte_cnt;
    modbus_reg_t status;
    expected_byte_cnt = get_expected_byte_cnt(fun_code, requested_data_qty);
    if (resp_msg[MODBUS_FUNCTION_CODE_IDX] == fun_code)
    {
        if (slave_resp_byte_cnt == expected_byte_cnt)
        {
            if (NULL != data_tab)
            {
                update_master_specific_data_type_from_modbus_msg(resp_msg, req_msg, fun_code, data_tab);
                status = RET_OK;
            }
            else
                status = RET_ERROR_DATA_TAB_NULL_PTR;
        }
        else
        {
            status = RET_ERROR_BYTE_CNT;
        }
    }
    else
    {
        status = RET_ERROR_FUN_CODE;
    }
    return status;
}
static void update_master_specific_data_type_from_modbus_msg(const modbus_buf_t *resp_msg, const modbus_buf_t *req_msg, modbus_fun_code_t fun_code, void **data_tab)
{
    modbus_data_qty_t data_qty = read_u16_from_buf(req_msg + MODBUS_REQUEST_LEN_IDX);
    modbus_adr_t data_start_adr = read_u16_from_buf(req_msg + MODBUS_REQUEST_ADR_IDX);
    switch (fun_code)
    {
    case MODBUS_READ_INPUT_REGISTERS_FUNC_CODE:
    case MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE:
        set_inreg_hreg_value_from_modbus_msg(&resp_msg[MODBUS_RESP_READ_DATA_IDX], data_start_adr, data_qty, (modbus_reg_t **)(data_tab));
        break;
    default:
        set_coil_din_value_from_modbus_msg(&resp_msg[MODBUS_RESP_READ_DATA_IDX], data_start_adr, data_qty, (modbus_coil_disin_t **)(data_tab));
        break;
    }
}
static modbus_byte_count_t get_expected_byte_cnt(modbus_fun_code_t func_code, modbus_data_qty_t req_data_qty)
{
    modbus_byte_count_t byte_cnt;
    switch (func_code)
    {
    case MODBUS_READ_INPUT_REGISTERS_FUNC_CODE:
    case MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE:
        byte_cnt = 2 * req_data_qty;
        break;
    default:
        byte_cnt = get_coil_din_byte_count(req_data_qty);
        break;
    }
    return byte_cnt;
}

static modbus_ret_t check_slave_resp_vs_req_for_sigle_write(const modbus_buf_t *resp_msg, const modbus_buf_t *req_msg)
{
        modbus_ret_t status = RET_OK;
    if (resp_msg[MODBUS_FUNCTION_CODE_IDX] == req_msg[MODBUS_FUNCTION_CODE_IDX])
    {
        if (read_u16_from_buf(&resp_msg[MODBUS_RESP_WRITE_ADR_IDX]) == read_u16_from_buf(&req_msg[MODBUS_REQUEST_ADR_IDX]))
        {
            if (read_u16_from_buf(&resp_msg[MODBUS_RESP_WRITE_SINGLE_DATA_IDX]) == read_u16_from_buf(&req_msg[MODBUS_REQUEST_WRITE_SINGLE_DATA_IDX]))
                status = RET_OK;
            else
                status = RET_ERROR_WRITE_SINGLE_OUT_VAL;
        } 
        else
        {
            status = RET_ERROR_WRITE_OUT_ADR;
        }
            
    }
    else
    {
        status = RET_ERROR_FUN_CODE;
    }
    return status; 
}


// Master API functions
void register_app_data_to_slave_coils_table(modbus_adr_t coil_adr, modbus_coil_disin_t *app_data_ptr)
{
    Slave_Coils[coil_adr] = app_data_ptr;
}
void register_app_data_to_slave_discrete_inputs_table(modbus_adr_t din_adr, modbus_coil_disin_t *app_data_ptr)
{
    Slave_Discrete_Inputs[din_adr] = app_data_ptr;
}
void register_app_data_to_slave_input_registers_table(modbus_adr_t input_reg_adr, modbus_reg_t *app_data_ptr)
{
    Slave_Input_Registers[input_reg_adr] = app_data_ptr;
}
void register_app_data_to_slave_holding_registers_table(modbus_adr_t hreg_reg_adr, modbus_reg_t *app_data_ptr)
{
    Slave_Holding_Registers[hreg_reg_adr] = app_data_ptr;
}
modbus_ret_t modbus_master_read_holding_reg_req(modbus_buf_t *send_buf, modbus_adr_t adr, modbus_data_qty_t hreg_qty)
{
    return read_reg_request(send_buf, MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE, adr, hreg_qty);
}

modbus_ret_t modbus_master_read_input_reg_req(modbus_buf_t *send_buf, modbus_adr_t adr, modbus_data_qty_t reg_qty)
{
    return read_reg_request(send_buf, MODBUS_READ_INPUT_REGISTERS_FUNC_CODE, adr, reg_qty);
}

modbus_ret_t modbus_master_read_discrete_inputs_req(modbus_buf_t *send_buf, modbus_adr_t adr, modbus_data_qty_t discrete_input_qty)
{
    return read_reg_request(send_buf, MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, adr, discrete_input_qty);
}

modbus_ret_t modbus_master_read_coils_req(modbus_buf_t *send_buf, modbus_adr_t adr, modbus_data_qty_t coils_qty)
{
    return read_reg_request(send_buf, MODBUS_READ_COILS_FUNC_CODE, adr, coils_qty);
}

modbus_ret_t modbus_master_write_single_reg_req(modbus_buf_t *send_buf, modbus_adr_t adr)
{
    return write_single_reg_coil_request(send_buf, MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE, adr);
}

modbus_ret_t modbus_master_write_single_coil_req(modbus_buf_t *send_buf, modbus_adr_t adr)
{
    return write_single_reg_coil_request(send_buf, MODBUS_WRITE_SINGLE_COIL_FUNC_CODE, adr);
}

modbus_ret_t modbus_master_write_multiple_reg_req(modbus_buf_t *send_buf, modbus_adr_t adr, modbus_data_qty_t reg_qty)
{
    if ((reg_qty <= MODBUS_MAX_WRITE_REG_QTY) && (reg_qty >= MODBUS_MIN_REG_COIL_QTY))
    {
        send_buf[MODBUS_FUNCTION_CODE_IDX] = MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE;
        write_u16_to_buf(send_buf + MODBUS_REQUEST_ADR_IDX, adr);
        write_u16_to_buf(send_buf + MODBUS_REQUEST_LEN_IDX, reg_qty);
        send_buf[MODBUS_REQUEST_BYTE_CNT_IDX] = reg_qty * 2;

        for (modbus_data_qty_t i = 0; i < reg_qty; i++)
        {
            write_u16_to_buf(send_buf + (MODBUS_REQUEST_WRITE_MULTI_DATA_IDX + (i * 2)), get_holding_register_value(Master_Holding_Registers,adr+i));
        }
        return MODBUS_WRITE_MULTI_REQUEST_LEN + send_buf[MODBUS_REQUEST_BYTE_CNT_IDX];
    }
    else
    {
        return RET_ERROR;
    }
}

modbus_ret_t modbus_master_write_multiple_coils_req(modbus_buf_t *send_buf, modbus_adr_t adr, modbus_data_qty_t coils_qty)
{
    modbus_byte_count_t byte_count = get_coil_din_byte_count(coils_qty);
    if ((coils_qty <= MODBUS_MAX_WRITE_COILS_QTY) && (coils_qty >= MODBUS_MIN_REG_COIL_QTY))
    {
        send_buf[MODBUS_FUNCTION_CODE_IDX] = MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE;
        write_u16_to_buf(send_buf + MODBUS_REQUEST_ADR_IDX, adr);
        write_u16_to_buf(send_buf + MODBUS_REQUEST_LEN_IDX, coils_qty);
        send_buf[MODBUS_REQUEST_BYTE_CNT_IDX] = byte_count;
        
        clear_coil_din_status_byte(&send_buf[MODBUS_REQUEST_WRITE_MULTI_DATA_IDX], byte_count);
        for (modbus_data_qty_t i = 0; i < coils_qty; i++)
        {

            send_buf[MODBUS_REQUEST_WRITE_MULTI_DATA_IDX + (i/8)] |= (get_coil_state(Master_Coils, adr + i) << (i % 8));
        }
        return RET_OK;
    }
    else
    {
        return RET_ERROR;
    }
}

modbus_ret_t modbus_master_read_coils_resp(const modbus_buf_t *resp_buf, const modbus_buf_t *req_buf)
{
    return update_master_data_from_modbus_msg(resp_buf, req_buf, MODBUS_READ_COILS_FUNC_CODE, (void **)(Master_Coils));
}

modbus_ret_t modbus_master_read_discrete_inputs_resp(const modbus_buf_t *resp_buf, const modbus_buf_t *req_buf)
{
    return update_master_data_from_modbus_msg(resp_buf, req_buf, MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE, (void **)(Master_Discrete_Inputs));
}

modbus_ret_t modbus_master_read_input_reg_resp(const modbus_buf_t *resp_buf, const modbus_buf_t *req_buf)
{
    return update_master_data_from_modbus_msg(resp_buf, req_buf, MODBUS_READ_INPUT_REGISTERS_FUNC_CODE, (void **)(Master_Input_Registers));
}

modbus_ret_t modbus_master_read_holding_reg_resp(const modbus_buf_t *resp_buf, const modbus_buf_t *req_buf)
{
    return update_master_data_from_modbus_msg(resp_buf, req_buf, MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE, (void **)(Master_Holding_Registers));
}

modbus_ret_t modbus_master_write_single_coil_resp(const modbus_buf_t *resp_buf, const modbus_buf_t *req_buf)
{
    return check_slave_resp_vs_req_for_sigle_write(resp_buf,req_buf);
}

modbus_ret_t modbus_master_write_single_reg_resp(const  modbus_buf_t *resp_buf, const modbus_buf_t *req_buf)
{
    return check_slave_resp_vs_req_for_sigle_write(resp_buf,req_buf);
}

modbus_ret_t modbus_master_write_multiple_coils_resp(const  modbus_buf_t *resp_buf, const modbus_buf_t *req_buf)
{
    modbus_ret_t status;
    if(resp_buf[MODBUS_FUNCTION_CODE_IDX] == req_buf[MODBUS_FUNCTION_CODE_IDX])
    {
        if((read_u16_from_buf(resp_buf+MODBUS_RESP_WRITE_ADR_IDX))==(read_u16_from_buf(req_buf+MODBUS_RESP_WRITE_ADR_IDX)))
        {
            if((read_u16_from_buf(resp_buf+MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX))==(read_u16_from_buf(req_buf+MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX)))
            {
                status = RET_OK;
            }
            else
            {
                status = RET_ERROR_WRITE_MULTI_OUT_QTY;
            }
            
        }
        else
        {
            status = RET_ERROR_WRITE_OUT_ADR;
        }
        
    }
    else
    {
        status = RET_ERROR_FUN_CODE;
    }
    return status;
}
modbus_ret_t modbus_master_write_multiple_reg_resp(const  modbus_buf_t *resp_buf, const modbus_buf_t *req_buf)
{
    return RET_OK;
}

// Slave API functions

void register_app_data_to_master_coils_table(modbus_adr_t coil_adr, modbus_coil_disin_t *app_data_ptr)
{
    Master_Coils[coil_adr] = app_data_ptr;
}
void register_app_data_to_master_discrete_inputs_table(modbus_adr_t din_adr, modbus_coil_disin_t *app_data_ptr)
{
    Master_Discrete_Inputs[din_adr] = app_data_ptr;
}
void register_app_data_to_master_input_registers_table(modbus_adr_t input_reg_adr, modbus_reg_t *app_data_ptr)
{
    Master_Input_Registers[input_reg_adr] = app_data_ptr;
}
void register_app_data_to_master_holding_registers_table(modbus_adr_t hreg_reg_adr, modbus_reg_t *app_data_ptr)
{
    Master_Holding_Registers[hreg_reg_adr] = app_data_ptr;
}

modbus_ret_t modbus_slave_read_coils(modbus_buf_t *resp_buf, const modbus_buf_t *req_buf)
{
    modbus_adr_t adr = read_u16_from_buf(req_buf + MODBUS_REQUEST_ADR_IDX);
    modbus_data_qty_t coil_qty = read_u16_from_buf(req_buf + MODBUS_REQUEST_LEN_IDX);
    modbus_byte_count_t byte_cnt = get_coil_din_byte_count(coil_qty);

    resp_buf[MODBUS_FUNCTION_CODE_IDX] = MODBUS_READ_COILS_FUNC_CODE;
    resp_buf[MODBUS_RESP_READ_BYTE_CNT_IDX] = byte_cnt;

    clear_coil_din_status_byte(&resp_buf[MODBUS_RESP_READ_DATA_IDX], byte_cnt);

    for (modbus_byte_count_t i = 0; i < coil_qty; i++)
    {
        resp_buf[MODBUS_RESP_READ_DATA_IDX + (i / 8)] |= (get_coil_state(Slave_Coils, adr + i) << (i % 8));
    }
    return MODBUS_READ_RESP_LEN + byte_cnt;
}

modbus_ret_t modbus_slave_read_discrete_inputs(modbus_buf_t *resp_buf, const modbus_buf_t *req_buf)
{
    modbus_adr_t adr = read_u16_from_buf(&req_buf[MODBUS_REQUEST_ADR_IDX]);
    modbus_data_qty_t din_qty = read_u16_from_buf(&req_buf[MODBUS_REQUEST_LEN_IDX]);
    modbus_byte_count_t byte_cnt = get_coil_din_byte_count(din_qty);

    resp_buf[MODBUS_FUNCTION_CODE_IDX] = MODBUS_READ_DISCRETE_INPUTS_FUNC_CODE;
    resp_buf[MODBUS_RESP_READ_BYTE_CNT_IDX] = byte_cnt;

    clear_coil_din_status_byte(&resp_buf[MODBUS_RESP_READ_DATA_IDX], byte_cnt);

    for (modbus_byte_count_t i = 0; i < din_qty; i++)
    {
        resp_buf[MODBUS_RESP_READ_DATA_IDX + (i / 8)] |= (get_discrete_input_state(Slave_Discrete_Inputs, adr + i) << (i % 8));
    }
    return MODBUS_READ_RESP_LEN + byte_cnt;
}

modbus_ret_t modbus_slave_read_holding_reg(modbus_buf_t *resp_buf, const modbus_buf_t *req_buf)
{
    modbus_adr_t adr = read_u16_from_buf(&req_buf[MODBUS_REQUEST_ADR_IDX]);
    modbus_data_qty_t reg_qty = read_u16_from_buf(&req_buf[MODBUS_REQUEST_LEN_IDX]);
    modbus_byte_count_t byte_cnt = 2 * reg_qty;

    resp_buf[MODBUS_FUNCTION_CODE_IDX] = MODBUS_READ_HOLDING_REGISTERS_FUNC_CODE;
    resp_buf[MODBUS_RESP_READ_BYTE_CNT_IDX] = byte_cnt;

    for (modbus_byte_count_t i = 0; i < reg_qty; i++)
    {
        write_u16_to_buf(&resp_buf[MODBUS_RESP_READ_DATA_IDX + (i * 2)], get_holding_register_value(Slave_Holding_Registers, adr + i));
    }
    return MODBUS_READ_RESP_LEN + byte_cnt;
}

modbus_ret_t modbus_slave_read_input_reg(modbus_buf_t *resp_buf, const modbus_buf_t *req_buf)
{
    modbus_adr_t adr = read_u16_from_buf(&req_buf[MODBUS_REQUEST_ADR_IDX]);
    modbus_data_qty_t reg_qty = read_u16_from_buf(&req_buf[MODBUS_REQUEST_LEN_IDX]);
    modbus_byte_count_t byte_cnt = 2 * reg_qty;

    resp_buf[MODBUS_FUNCTION_CODE_IDX] = MODBUS_READ_INPUT_REGISTERS_FUNC_CODE;
    resp_buf[MODBUS_RESP_READ_BYTE_CNT_IDX] = byte_cnt;

    for (modbus_byte_count_t i = 0; i < reg_qty; i++)
    {
        write_u16_to_buf(&resp_buf[MODBUS_RESP_READ_DATA_IDX + (i * 2)], get_input_register_state(Slave_Input_Registers, adr + i));
    }
    return MODBUS_READ_RESP_LEN + byte_cnt;
}

modbus_ret_t modbus_slave_write_single_coil(modbus_buf_t *resp_buf, const modbus_buf_t *req_buf)
{
    modbus_adr_t adr = read_u16_from_buf(&req_buf[MODBUS_REQUEST_ADR_IDX]);
    modbus_w_coil_t coils_state = read_u16_from_buf(&req_buf[MODBUS_RESP_WRITE_SINGLE_DATA_IDX]);

    resp_buf[MODBUS_FUNCTION_CODE_IDX] = MODBUS_WRITE_SINGLE_COIL_FUNC_CODE;

    set_coil_state(Slave_Coils, adr, !!coils_state); // double logic negation make 1 from var that is different then 0 and 0 from var equal 0

    write_u16_to_buf(&resp_buf[MODBUS_RESP_WRITE_ADR_IDX], adr);
    write_u16_to_buf(&resp_buf[MODBUS_RESP_WRITE_SINGLE_DATA_IDX], coils_state);
    return MODBUS_WRITE_SINGLE_RESP_LEN;
}

modbus_ret_t modbus_slave_write_multiple_coils(modbus_buf_t *resp_buf, const modbus_buf_t *req_buf)
{
    modbus_adr_t adr = read_u16_from_buf(&req_buf[MODBUS_REQUEST_ADR_IDX]);
    modbus_data_qty_t coils_qty = read_u16_from_buf(&req_buf[MODBUS_REQUEST_LEN_IDX]);

    set_coil_din_value_from_modbus_msg(&req_buf[MODBUS_REQUEST_WRITE_MULTI_DATA_IDX], adr, coils_qty, Slave_Coils);

    resp_buf[MODBUS_FUNCTION_CODE_IDX] = MODBUS_WRITE_MULTIPLE_COILS_FUNC_CODE;
    write_u16_to_buf(&resp_buf[MODBUS_RESP_WRITE_ADR_IDX], adr);
    write_u16_to_buf(&resp_buf[MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX], coils_qty);
    return MODBUS_WRITE_MULTI_RESP_LEN;
}

modbus_ret_t modbus_slave_write_single_reg(modbus_buf_t *resp_buf, const modbus_buf_t *req_buf)
{
    modbus_adr_t adr = read_u16_from_buf(&req_buf[MODBUS_REQUEST_ADR_IDX]);
    modbus_reg_t reg_val_to_save = read_u16_from_buf(&req_buf[MODBUS_REQUEST_LEN_IDX]);

    set_register_value(Slave_Holding_Registers, adr, reg_val_to_save);

    resp_buf[MODBUS_FUNCTION_CODE_IDX] = MODBUS_WRITE_SINGLE_REGISTER_FUNC_CODE;
    write_u16_to_buf(&resp_buf[MODBUS_RESP_WRITE_ADR_IDX], adr);
    write_u16_to_buf(&resp_buf[MODBUS_RESP_WRITE_SINGLE_DATA_IDX], reg_val_to_save);
    return MODBUS_WRITE_SINGLE_RESP_LEN;
}

modbus_ret_t modbus_slave_write_multiple_reg(modbus_buf_t *resp_buf, const modbus_buf_t *req_buf)
{
    modbus_adr_t adr = read_u16_from_buf(&req_buf[MODBUS_REQUEST_ADR_IDX]);
    modbus_data_qty_t hreg_qty = read_u16_from_buf(&req_buf[MODBUS_REQUEST_LEN_IDX]);

    for (modbus_data_qty_t i = 0; i < hreg_qty; i++)
    {
        set_register_value(Slave_Holding_Registers, adr + i, read_u16_from_buf(&req_buf[MODBUS_REQUEST_WRITE_MULTI_DATA_IDX + (i * 2)]));
    }

    resp_buf[MODBUS_FUNCTION_CODE_IDX] = MODBUS_WRITE_MULTIPLE_REGISTER_FUNC_CODE;
    write_u16_to_buf(&resp_buf[MODBUS_RESP_WRITE_ADR_IDX], adr);
    write_u16_to_buf(&resp_buf[MODBUS_RESP_WRITE_MULTIPLE_DATA_QTY_IDX], hreg_qty);
    return MODBUS_WRITE_MULTI_RESP_LEN;
}
