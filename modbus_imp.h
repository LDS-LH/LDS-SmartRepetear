
#ifndef MODBUS_IMP_H
#define MODBUS_IMP_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "nanomodbus.h"

#define COILS_ADDR_MAX             5
#define REGS_INPUT_ADDR_MAX        21
#define REGS_HOLDING_ADDR_MAX      5

// Our RTU address
#define RTU_SERVER_ADDRESS_DEFAULT 3
#define RTU_BAUDRATE_DEFAULT       9600

#define RTU_SERIAL_NUMBER_DEFAULT  0
#define RTU_SENSOR_TYPE_DEFAULT    200

#define MAX_SLAVE_VALUE            255
#define MIN_SLAVE_VALUE            1

#define IS_IN_MEMORY_VALUE         0x00
#define IS_IN_MEMORY_EPP_ADDR      0x7001
#define SLAVE_EPP_ADDR             0x7002
#define BAUDRATE_EPP_ADDR          0x7004
#define SERIAL_NUMBER_ADDR         0x7006
#define SENSOR_TYPE_ADDR           0x7008

// Holding registers for serial number write operations
#define SN_PASSWORD_CORRECT         8336
#define SNW_STATUS_IDLE             0 
#define SNW_STATUS_SUCCESS          1 
#define SNW_STATUS_WRONG_PASS       2 
#define SNW_STATUS_NOT_AUTHORIZED   3 

typedef struct
{
    // 0 t0 255
    uint16_t addr_slave;
    uint16_t baudrate;

    uint16_t serial_number_in;
    uint16_t sn_password;
    uint16_t sn_write_status;         
} holding_register;

typedef struct
{
    uint16_t sensor_type;
    uint16_t serial_number;
    uint16_t sensor1_value;
    uint16_t sensor2_value;
    uint16_t sensor3_valueH;
    uint16_t sensor3_valueL;
    uint16_t sensor4_valueH;
    uint16_t sensor4_valueL;
    uint16_t coeff[N_COEF];
} input_register;

typedef struct
{
    // A single nmbs_bitfield variable can keep 2000 coils
    nmbs_bitfield coils;
} coils;

typedef struct
{
    coils server_coils;
    input_register server_input_register;
    holding_register server_holding_register;
} mod_bus_registers;

void default_values_register(mod_bus_registers *modbus_data);
void holding_register_change_handler(mod_bus_registers* modbus_data, holding_register* prev_holding_regs, nmbs_t* nmbs); // nmbs_t* nmbs 
void check_error_modbus(nmbs_error err);

int32_t read_serial(uint8_t *buf, uint16_t count, int32_t byte_timeout_ms, void *arg);
int32_t write_serial(const uint8_t *buf, uint16_t count, int32_t byte_timeout_ms, void *arg);
nmbs_error handle_read_coils(uint16_t address, uint16_t quantity, nmbs_bitfield coils_out, uint8_t unit_id, void *arg);
nmbs_error handle_write_single_coil(uint16_t address, bool coils, uint8_t unit_id, void *arg);
nmbs_error handler_read_input_registers(uint16_t address, uint16_t quantity, uint16_t *registers_out, uint8_t unit_id, void *arg);
nmbs_error handler_read_holding_registers(uint16_t address, uint16_t quantity, uint16_t *registers_out, uint8_t unit_id, void *arg);
nmbs_error handle_write_single_register(uint16_t address, uint16_t value, uint8_t unit_id, void *arg);
void check_error_modbus(nmbs_error err);





#ifdef __cplusplus
}
#endif

#endif /* MODBUS_IMP_H */
