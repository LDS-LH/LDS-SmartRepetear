#include "mcc.h"

static int port_1_value;
static int port_2_value;
static int port_3_value;
static int port_4_value;

void PORT_1_SetHigh(void) { port_1_value = 1; }
void PORT_1_SetLow(void) { port_1_value = 0; }
int PORT_1_GetValue(void) { return port_1_value; }

void PORT_2_SetHigh(void) { port_2_value = 1; }
void PORT_2_SetLow(void) { port_2_value = 0; }
int PORT_2_GetValue(void) { return port_2_value; }

void PORT_3_SetHigh(void) { port_3_value = 1; }
void PORT_3_SetLow(void) { port_3_value = 0; }
int PORT_3_GetValue(void) { return port_3_value; }

void PORT_4_SetHigh(void) { port_4_value = 1; }
void PORT_4_SetLow(void) { port_4_value = 0; }
int PORT_4_GetValue(void) { return port_4_value; }