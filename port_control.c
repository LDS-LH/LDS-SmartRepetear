#include "port_control.h"

int set_high_port(uint16_t port){
    switch(port){
        case PORT_CONTROL_1:
            PORT_1_SetHigh();
            break;
        case PORT_CONTROL_2:
            PORT_2_SetHigh();
            break;
        case PORT_CONTROL_3:
            PORT_3_SetHigh();
            break;
        case PORT_CONTROL_4:
            PORT_4_SetHigh();
            break;
        default:
            return -1; // Invalid port number
    }
    return 0 ;
}

int set_high_all(){
    PORT_1_SetHigh();
    PORT_2_SetHigh();
    PORT_3_SetHigh();
    PORT_4_SetHigh();
    
    return 0;
}

int set_low_port(uint16_t port){
    switch(port){
        case PORT_CONTROL_1:
            PORT_1_SetLow();
            break;
        case PORT_CONTROL_2:
            PORT_2_SetLow();
            break;
        case PORT_CONTROL_3:
            PORT_3_SetLow();
            break;
        case PORT_CONTROL_4:
            PORT_4_SetLow();
            break;
        default:
            return -1; // Invalid port number
    }
    return 0;
}

int set_low_all(){
    PORT_1_SetLow();
    PORT_2_SetLow();
    PORT_3_SetLow();
    PORT_4_SetLow();
    return 0;
}
