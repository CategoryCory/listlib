#include "list.h"
#include "unity.h"

static list test_list;
static list_status test_status;

void setUp(void) {
    test_status = list_init(&test_list, sizeof(int32_t));
}

void tearDown(void) {
    list_destroy(&test_list);
}

static void populate_list_with_data(void) {
    for (int32_t i = 0; i < 10; i++) {
        int32_t value = i * 10;
        list_push(&test_list, &value);
    }
}

static void assert_list_get_status_and_value(
    const size_t index,
    const list_status expected_status,
    const int32_t expected_value)
{
    int32_t result = -1;
    const list_status actual_status = list_get(&test_list, index, &result);
    TEST_ASSERT_EQUAL(expected_status, actual_status);

    if (expected_status == LIST_OK) {
        TEST_ASSERT_EQUAL_INT32(expected_value, result);
    }
}

static void assert_list_set_status_and_value(
    const size_t index,
    const list_status expected_status,
    const int32_t value_to_set)
{
    const list_status actual_status = list_set(&test_list, index, &value_to_set);
    TEST_ASSERT_EQUAL(expected_status, actual_status);

    if (expected_status == LIST_OK) {
        int32_t result = -1;
        list_get(&test_list, index, &result);
        TEST_ASSERT_EQUAL_INT32(value_to_set, result);
    }
}

void test_list_init_sets_defaults(void) {
    list lst;
    const list_status status = list_init(&lst, sizeof(int32_t));

    TEST_ASSERT_EQUAL(LIST_OK, status);
    TEST_ASSERT_NULL(lst.data);
    TEST_ASSERT_EQUAL_UINT64(0, lst.size);
    TEST_ASSERT_EQUAL_UINT64(0, lst.capacity);
    TEST_ASSERT_EQUAL_UINT64(sizeof(int32_t), lst.elem_size);

    list_destroy(&lst);
}

void test_list_push_and_get_value(void) {
    constexpr int32_t value = 123;
    const list_status push_status = list_push(&test_list, &value);
    TEST_ASSERT_EQUAL(LIST_OK, push_status);
    TEST_ASSERT_EQUAL_UINT64(1, test_list.size);

    int32_t result = 0;
    const list_status get_status = list_get(&test_list, 0, &result);
    TEST_ASSERT_EQUAL(LIST_OK, get_status);
    TEST_ASSERT_EQUAL(value, result);
}

void test_list_push_and_pop_value(void) {
    constexpr int32_t value = 123;
    list_push(&test_list, &value);

    int32_t popped_value = 0;
    const list_status pop_status = list_pop(&test_list, &popped_value);
    TEST_ASSERT_EQUAL(LIST_OK, pop_status);
    TEST_ASSERT_EQUAL(value, popped_value);
    TEST_ASSERT_EQUAL_UINT64(0, test_list.size);
}

void test_list_populates_and_resizes_when_needed(void) {
    populate_list_with_data();

    TEST_ASSERT_EQUAL(10, test_list.size);
    TEST_ASSERT_EQUAL(16, test_list.capacity);
}

void test_list_get_returns_value_at_index(void) {
    populate_list_with_data();
    assert_list_get_status_and_value(7, LIST_OK, 70);
}

void test_list_get_returns_error_when_index_out_of_bounds(void) {
    populate_list_with_data();
    assert_list_get_status_and_value(15, LIST_OUT_OF_BOUNDS, 0);
}

void test_list_set_sets_value_at_index(void) {
    populate_list_with_data();
    assert_list_set_status_and_value(7, LIST_OK, 123);
}

void test_list_set_returns_error_when_index_out_of_bounds(void) {
    populate_list_with_data();
    assert_list_set_status_and_value(15, LIST_OUT_OF_BOUNDS, 0);
}

void test_list_clear_empties_list(void) {
    populate_list_with_data();
    list_clear(&test_list);
    TEST_ASSERT_EQUAL_UINT64(0, test_list.size);
}

int main(void) {
    UNITY_BEGIN();

    RUN_TEST(test_list_init_sets_defaults);
    RUN_TEST(test_list_push_and_get_value);
    RUN_TEST(test_list_push_and_pop_value);
    RUN_TEST(test_list_populates_and_resizes_when_needed);
    RUN_TEST(test_list_get_returns_value_at_index);
    RUN_TEST(test_list_get_returns_error_when_index_out_of_bounds);
    RUN_TEST(test_list_set_sets_value_at_index);
    RUN_TEST(test_list_set_returns_error_when_index_out_of_bounds);
    RUN_TEST(test_list_clear_empties_list);

    return UNITY_END();
}
