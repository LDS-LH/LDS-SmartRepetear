#include "unity.h"
#include "port_control.h"
#include "mcc.h"

void setUp(void)
{
    PORT_1_SetLow();
    PORT_2_SetLow();
    PORT_3_SetLow();
    PORT_4_SetLow();
}
void tearDown(void) {}

void test_set_high_port_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(-1, set_high_port(999));
}

void test_set_high_port_sets_each_port(void)
{
    TEST_ASSERT_EQUAL_INT(0, set_high_port(PORT_CONTROL_1));
    TEST_ASSERT_EQUAL_INT(1, PORT_1_GetValue());

    TEST_ASSERT_EQUAL_INT(0, set_high_port(PORT_CONTROL_2));
    TEST_ASSERT_EQUAL_INT(1, PORT_2_GetValue());

    TEST_ASSERT_EQUAL_INT(0, set_high_port(PORT_CONTROL_3));
    TEST_ASSERT_EQUAL_INT(1, PORT_3_GetValue());

    TEST_ASSERT_EQUAL_INT(0, set_high_port(PORT_CONTROL_4));
    TEST_ASSERT_EQUAL_INT(1, PORT_4_GetValue());
}

void test_set_low_port_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(-1, set_low_port(999));
}

void test_set_low_port_sets_each_port(void)
{
    set_high_all();

    TEST_ASSERT_EQUAL_INT(0, set_low_port(PORT_CONTROL_1));
    TEST_ASSERT_EQUAL_INT(0, PORT_1_GetValue());

    TEST_ASSERT_EQUAL_INT(0, set_low_port(PORT_CONTROL_2));
    TEST_ASSERT_EQUAL_INT(0, PORT_2_GetValue());

    TEST_ASSERT_EQUAL_INT(0, set_low_port(PORT_CONTROL_3));
    TEST_ASSERT_EQUAL_INT(0, PORT_3_GetValue());

    TEST_ASSERT_EQUAL_INT(0, set_low_port(PORT_CONTROL_4));
    TEST_ASSERT_EQUAL_INT(0, PORT_4_GetValue());
}

void test_set_high_all_sets_all_ports_high(void)
{
    TEST_ASSERT_EQUAL_INT(0, set_high_all());
    TEST_ASSERT_EQUAL_INT(1, PORT_1_GetValue());
    TEST_ASSERT_EQUAL_INT(1, PORT_2_GetValue());
    TEST_ASSERT_EQUAL_INT(1, PORT_3_GetValue());
    TEST_ASSERT_EQUAL_INT(1, PORT_4_GetValue());
}

void test_set_low_all_sets_all_ports_low(void)
{
    set_high_all();

    TEST_ASSERT_EQUAL_INT(0, set_low_all());
    TEST_ASSERT_EQUAL_INT(0, PORT_1_GetValue());
    TEST_ASSERT_EQUAL_INT(0, PORT_2_GetValue());
    TEST_ASSERT_EQUAL_INT(0, PORT_3_GetValue());
    TEST_ASSERT_EQUAL_INT(0, PORT_4_GetValue());
}

void test_set_high_all_except_keeps_selected_port_low(void)
{
    TEST_ASSERT_EQUAL_INT(0, set_high_all_except(PORT_CONTROL_1));
    TEST_ASSERT_EQUAL_INT(0, PORT_1_GetValue());
    TEST_ASSERT_EQUAL_INT(1, PORT_2_GetValue());
    TEST_ASSERT_EQUAL_INT(1, PORT_3_GetValue());
    TEST_ASSERT_EQUAL_INT(1, PORT_4_GetValue());

    set_low_all();
    TEST_ASSERT_EQUAL_INT(0, set_high_all_except(PORT_CONTROL_2));
    TEST_ASSERT_EQUAL_INT(1, PORT_1_GetValue());
    TEST_ASSERT_EQUAL_INT(0, PORT_2_GetValue());
    TEST_ASSERT_EQUAL_INT(1, PORT_3_GetValue());
    TEST_ASSERT_EQUAL_INT(1, PORT_4_GetValue());

    set_low_all();
    TEST_ASSERT_EQUAL_INT(0, set_high_all_except(PORT_CONTROL_3));
    TEST_ASSERT_EQUAL_INT(1, PORT_1_GetValue());
    TEST_ASSERT_EQUAL_INT(1, PORT_2_GetValue());
    TEST_ASSERT_EQUAL_INT(0, PORT_3_GetValue());
    TEST_ASSERT_EQUAL_INT(1, PORT_4_GetValue());

    set_low_all();
    TEST_ASSERT_EQUAL_INT(0, set_high_all_except(PORT_CONTROL_4));
    TEST_ASSERT_EQUAL_INT(1, PORT_1_GetValue());
    TEST_ASSERT_EQUAL_INT(1, PORT_2_GetValue());
    TEST_ASSERT_EQUAL_INT(1, PORT_3_GetValue());
    TEST_ASSERT_EQUAL_INT(0, PORT_4_GetValue());
}

void test_set_low_all_except_keeps_selected_port_high(void)
{
    set_high_all();

    TEST_ASSERT_EQUAL_INT(0, set_low_all_except(PORT_CONTROL_1));
    TEST_ASSERT_EQUAL_INT(1, PORT_1_GetValue());
    TEST_ASSERT_EQUAL_INT(0, PORT_2_GetValue());
    TEST_ASSERT_EQUAL_INT(0, PORT_3_GetValue());
    TEST_ASSERT_EQUAL_INT(0, PORT_4_GetValue());

    set_high_all();
    TEST_ASSERT_EQUAL_INT(0, set_low_all_except(PORT_CONTROL_2));
    TEST_ASSERT_EQUAL_INT(1, PORT_2_GetValue());
    TEST_ASSERT_EQUAL_INT(0, PORT_1_GetValue());
    TEST_ASSERT_EQUAL_INT(0, PORT_3_GetValue());
    TEST_ASSERT_EQUAL_INT(0, PORT_4_GetValue());

    set_high_all();
    TEST_ASSERT_EQUAL_INT(0, set_low_all_except(PORT_CONTROL_3));
    TEST_ASSERT_EQUAL_INT(0, PORT_1_GetValue());
    TEST_ASSERT_EQUAL_INT(0, PORT_2_GetValue());
    TEST_ASSERT_EQUAL_INT(1, PORT_3_GetValue());
    TEST_ASSERT_EQUAL_INT(0, PORT_4_GetValue());

    set_high_all();
    TEST_ASSERT_EQUAL_INT(0, set_low_all_except(PORT_CONTROL_4));
    TEST_ASSERT_EQUAL_INT(0, PORT_1_GetValue());
    TEST_ASSERT_EQUAL_INT(0, PORT_2_GetValue());
    TEST_ASSERT_EQUAL_INT(0, PORT_3_GetValue());
    TEST_ASSERT_EQUAL_INT(1, PORT_4_GetValue());
}

void test_all_except_rejects_invalid_port(void)
{
    set_high_all();

    TEST_ASSERT_EQUAL_INT(-1, set_high_all_except(999));
    TEST_ASSERT_EQUAL_INT(1, PORT_1_GetValue());
    TEST_ASSERT_EQUAL_INT(1, PORT_2_GetValue());
    TEST_ASSERT_EQUAL_INT(1, PORT_3_GetValue());
    TEST_ASSERT_EQUAL_INT(1, PORT_4_GetValue());

    TEST_ASSERT_EQUAL_INT(-1, set_low_all_except(999));
    TEST_ASSERT_EQUAL_INT(1, PORT_1_GetValue());
    TEST_ASSERT_EQUAL_INT(1, PORT_2_GetValue());
    TEST_ASSERT_EQUAL_INT(1, PORT_3_GetValue());
    TEST_ASSERT_EQUAL_INT(1, PORT_4_GetValue());
}