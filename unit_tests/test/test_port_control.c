#include "unity.h"
#include "port_control.h"
#include "mcc.h"

void setUp(void) {}
void tearDown(void) {}

void test_set_high_port_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(-1, set_high_port(999));
}

void test_set_low_port_invalid(void)
{
    TEST_ASSERT_EQUAL_INT(-1, set_low_port(999));
}

void test_suma(void)
{
    TEST_ASSERT_EQUAL_INT(4, suma(2, 2));
}