#include "common/c_test.h"
#include "common/logger.h"
#include "core/utils/stack.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static void infix2rpn(const char *exp);

// JUST_RUN_TEST(rpn, test)
TEST(rpn, test)
{
    infix2rpn("(1+2)*3");
    infix2rpn("4+(13/5)");
}

static int priority(char op) {
    if (op == '+' || op == '-') {
        return 2;
    } else if (op == '*' || op == '/' || op == '%') {
        return 3;
    } else {
        return 0;
    }
}

static void infix2rpn(const char *exp) {
    stack_t *op_stack = stack_create();
    for(int i = 0; exp[i] != '\0'; i++) {
        if(exp[i] == ' ') continue;

        if(isdigit(exp[i])) {
            while(isdigit(exp[i])) {
                printf("%c", exp[i]);
                i++;
            }
            printf(" ");
            i--;
        } else if(exp[i] == '(') {
            stack_push(op_stack, (void *)&exp[i]);
        } else if(exp[i] == ')') {
            while(*(char *)stack_peek(op_stack) != '(') {
                printf("%c", *(char *)stack_pop(op_stack));
            }
            stack_pop(op_stack);
        } else {
            while(stack_size(op_stack) > 0 && priority(exp[i]) <= priority(*(char *)stack_peek(op_stack))) {
                printf("%c", *(char *)stack_pop(op_stack));
            }
            stack_push(op_stack, (void *)&exp[i]);
        }
    }

    while(stack_size(op_stack) > 0) 
        printf("%c", *(char *)stack_pop(op_stack));
    printf("\n");
}
