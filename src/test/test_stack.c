#include "common/c_test.h"
#include "common/logger.h"
#include "core/utils/stack.h"

// JUST_RUN_TEST(stack, test)
TEST(stack, test)
{
    stack_t *stack = stack_create();
    ASSERT_EQ(stack_size(stack), 0);

    stack_push(stack, (void *) 1);
    ASSERT_EQ(stack_size(stack), 1);

    ASSERT_EQ(stack_peek(stack), (void *) 1);
    ASSERT_EQ(stack_size(stack), 1);

    ASSERT_EQ(stack_pop(stack), (void *) 1);
    ASSERT_EQ(stack_size(stack), 0);

    stack_destroy(stack);
}