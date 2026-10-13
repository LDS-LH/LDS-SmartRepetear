/*
 * File:   modbus_imp.c
 * Author: asus
 *
 * Created on 24 de agosto de 2023, 12:00
 */

#include "device.h"
#include <xc.h>
/*
   This example application sets up an RTU server and handles modbus requests

   This server supports the following function codes:
   FC 01 (0x01) Read Coils
   FC 03 (0x03) Read Holding Registers
   FC 15 (0x0F) Write Multiple Coils
   FC 16 (0x10) Write Multiple registers
*/

#include "modbus_imp.h"

bool sn_write_enabled = false;

int32_t read_serial(uint8_t* buf, uint16_t count, int32_t byte_timeout_ms, void* arg) 
{
    int32_t charCount = 0;
    uint8_t c;
    
    while(charCount != count)
    {
            c = EUSART1_Read();
            buf[charCount++] = c;
    }

    return charCount;
}

int32_t write_serial(const uint8_t* buf, uint16_t count, int32_t byte_timeout_ms, void* arg)
{ 
    TxDeviceEnable();
    int32_t number_of_byte_send = 0;

    for(int i=0; i<count; i++)
    {
        if(EUSART1_is_tx_ready())
        {
            EUSART1_Write(*buf);
            while(!EUSART1_is_tx_done());
            buf++;
            number_of_byte_send++;
        }
    }
    TxDeviceDisable();
    
    return number_of_byte_send;
}

nmbs_error handle_read_coils(uint16_t address, uint16_t quantity, nmbs_bitfield coils_out, uint8_t unit_id, void *arg) 
{
  if (address + quantity > COILS_ADDR_MAX)
    return NMBS_EXCEPTION_ILLEGAL_DATA_ADDRESS;

  nmbs_bitfield *server_coils = &((mod_bus_registers*) arg)->server_coils.coils;
  // Read our coils values into coils_out
  for (int i = 0; i < quantity; i++) {
    bool value = nmbs_bitfield_read(*server_coils, address + i);
    nmbs_bitfield_write(coils_out, i, value);
  }

  return NMBS_ERROR_NONE;
}

nmbs_error handle_write_single_coil(uint16_t address, bool coils, uint8_t unit_id, void *arg) 
{
  if (address > COILS_ADDR_MAX)
    return NMBS_EXCEPTION_ILLEGAL_DATA_ADDRESS;

  // Write coils values to our server_coils
  nmbs_bitfield *server_coils = &((mod_bus_registers*) arg)->server_coils.coils;
  nmbs_bitfield_write(*server_coils, address, coils);
  
  return NMBS_ERROR_NONE;
}

nmbs_error handler_read_input_registers(uint16_t address, uint16_t quantity, uint16_t* registers_out, uint8_t unit_id, void *arg) 
{
  if (address + quantity > REGS_INPUT_ADDR_MAX)
    return NMBS_EXCEPTION_ILLEGAL_DATA_ADDRESS;

  // Read our registers values into registers_out
  uint16_t* server_registers = (uint16_t *)(&(((mod_bus_registers*) arg)->server_input_register));
  for (int i = 0; i < quantity; i++)
    registers_out[i] = server_registers[address + i];

  return NMBS_ERROR_NONE;
}

nmbs_error handler_read_holding_registers(uint16_t address, uint16_t quantity, uint16_t* registers_out, uint8_t unit_id, void *arg) 
{
  if (address + quantity > REGS_HOLDING_ADDR_MAX)
    return NMBS_EXCEPTION_ILLEGAL_DATA_ADDRESS;

  // Read our registers values into registers_out
  uint16_t* server_registers = (uint16_t *)(&(((mod_bus_registers*) arg)->server_holding_register));
  for (int i = 0; i < quantity; i++)
    registers_out[i] = server_registers[address + i];

  return NMBS_ERROR_NONE;
}

nmbs_error handle_write_single_register(uint16_t address, uint16_t value, uint8_t unit_id, void *arg) 
{
  if (address >= REGS_HOLDING_ADDR_MAX)
    return NMBS_EXCEPTION_ILLEGAL_DATA_ADDRESS;

  // Write registers values to our server_registers
  uint16_t* server_registers = (uint16_t *)(&(((mod_bus_registers*) arg)->server_holding_register));

  server_registers[address] = value;

  return NMBS_ERROR_NONE;
}

void default_values_register(mod_bus_registers* modbus_data)
{
    modbus_data->server_holding_register.addr_slave       = RTU_SERVER_ADDRESS_DEFAULT;
    modbus_data->server_holding_register.baudrate         = RTU_BAUDRATE_DEFAULT;
    modbus_data->server_holding_register.sn_password      = 0;
    modbus_data->server_holding_register.serial_number_in = 0;    
    
    m_memset(&(modbus_data->server_coils)         , 0, sizeof(modbus_data->server_coils));
    m_memset(&(modbus_data->server_input_register), 0, sizeof(modbus_data->server_input_register));

    modbus_data->server_input_register.serial_number = RTU_SERIAL_NUMBER_DEFAULT;
    modbus_data->server_input_register.sensor_type   = RTU_SENSOR_TYPE_DEFAULT;
    
   // readBMP280coeff(modbus_data->server_input_register.coeff);
}

void holding_register_change_handler(mod_bus_registers* modbus_data, holding_register* prev_holding_regs, nmbs_t* nmbs)
{
  // Check whether the correct password has ben submitted in orde to enable a serial number write operation
  if(modbus_data->server_holding_register.sn_password != prev_holding_regs->sn_password)
  {
    if(modbus_data->server_holding_register.sn_password == SN_PASSWORD_CORRECT)
    {
      sn_write_enabled = true;
    }
    else
    {
      modbus_data->server_holding_register.sn_write_status = SNW_STATUS_WRONG_PASS;
      sn_write_enabled = false;
    }
    modbus_data->server_holding_register.sn_password = 0;
    prev_holding_regs->sn_password = 0;
  }

  // Check whether a serial number was write was attempted and handle it
  if(modbus_data->server_holding_register.serial_number_in != prev_holding_regs->serial_number_in)
  {  
    if((!sn_write_enabled) && (modbus_data->server_holding_register.sn_write_status != SNW_STATUS_WRONG_PASS))
    {
      modbus_data->server_holding_register.sn_write_status = SNW_STATUS_NOT_AUTHORIZED;
    }
  
    if(sn_write_enabled)
    {
      modbus_data->server_input_register.serial_number     = modbus_data->server_holding_register.serial_number_in;
      modbus_data->server_holding_register.sn_write_status = SNW_STATUS_SUCCESS;
      sn_write_enabled = false;
      DATAEE_WriteWord(SERIAL_NUMBER_ADDR, modbus_data->server_input_register.serial_number);
      DATAEE_WriteByte(IS_IN_MEMORY_EPP_ADDR, IS_IN_MEMORY_VALUE);
    }
    modbus_data->server_holding_register.serial_number_in = 0;
    prev_holding_regs->serial_number_in = 0;
  }

  if(modbus_data->server_holding_register.addr_slave != prev_holding_regs->addr_slave)
  {
    if(modbus_data->server_holding_register.addr_slave <= MAX_SLAVE_VALUE  
            & modbus_data->server_holding_register.addr_slave > MIN_SLAVE_VALUE - 1)
    {
      nmbs->address_rtu = modbus_data->server_holding_register.addr_slave;
      DATAEE_WriteWord(SLAVE_EPP_ADDR, modbus_data->server_holding_register.addr_slave);
      DATAEE_WriteByte(IS_IN_MEMORY_EPP_ADDR, IS_IN_MEMORY_VALUE);
    }
    else
    {
      modbus_data->server_holding_register.addr_slave = prev_holding_regs->addr_slave;
    }  
  }
  else if(modbus_data->server_holding_register.baudrate != prev_holding_regs->baudrate)
  {
    if(setBaudRateEUSART1(modbus_data->server_holding_register.baudrate))
    {
      modbus_data->server_holding_register.baudrate = prev_holding_regs->baudrate;
    }
    else
    {
      DATAEE_WriteWord(BAUDRATE_EPP_ADDR, modbus_data->server_holding_register.baudrate);
      DATAEE_WriteByte(IS_IN_MEMORY_EPP_ADDR, IS_IN_MEMORY_VALUE);
    }
  }  
}

void check_error_modbus(nmbs_error err)
{
//    switch(err)
//    {
//        MBS_ERROR_INVALID_UNIT_ID = -7,  /**< Received invalid unit ID in response from server */
//        NMBS_ERROR_INVALID_TCP_MBAP = -6, /**< Received invalid TCP MBAP */
//        NMBS_ERROR_CRC = -5,              /**< Received invalid CRC */
//        NMBS_ERROR_TRANSPORT = -4,        /**< Transport error */
//        NMBS_ERROR_TIMEOUT = -3,          /**< Read/write timeout occurred */
//        NMBS_ERROR_INVALID_RESPONSE = -2, /**< Received invalid response from server */
//        NMBS_ERROR_INVALID_ARGUMENT = -1, /**< Invalid argument provided */
//        NMBS_ERROR_NONE = 0,              /**< No error */
//
//        // Modbus exceptions
//        NMBS_EXCEPTION_ILLEGAL_FUNCTION = 1,      /**< Modbus exception 1 */
//        NMBS_EXCEPTION_ILLEGAL_DATA_ADDRESS = 2,  /**< Modbus exception 2 */
//        NMBS_EXCEPTION_ILLEGAL_DATA_VALUE = 3,    /**< Modbus exception 3 */
//        NMBS_EXCEPTION_SERVER_DEVICE_FAILURE = 4, /**< Modbus exception 4 */
//        case NMBS_ERROR_CRC:
//            //sendPacket((char*)&sensor.sensor_info, sizeof(sensor_info_t));
//            sendSensorInfo(&sensor.sensor_info);
//        case CMD_SENSE:
//            sensor_data = sensor.sense(&sensor.sensor_info.ADCChannel);
//            break;
//        case CMD_GET_DATA:
//            sendSensorData(sensor_data);
//            break;
//        default:
//            //sendPacket("INVALIDO", 8);
//            break;
//       }
}

