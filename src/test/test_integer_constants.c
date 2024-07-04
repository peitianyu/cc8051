#include "common/c_test.h"
#include "common/logger.h"
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

static int is_digit_str(char* str, int *num);

JUST_RUN_TEST(integer_constants, test)
TEST(integer_constants, test)
{
    // 暂不支持科学计数法
    char* strs[7] = {"0x89", "0b10100011", "123", "1", "true", "false", "NULL"};

    for(int i = 0; i < sizeof(strs) / sizeof(char*); i++) {
        int num = 0;
        int ret = is_digit_str(strs[i], &num);
        LOG_DEBUG("str: %s, num: %d, ret: %d", strs[i], num, ret);
    }
}


static int is_digit_str(char* str, int *num)
{   
    if(!str) return 0;

    if(strcmp(str, "false") == 0) {
        *num = 0; return 1;
    } else if(strcmp(str, "true") == 0) {
        *num = 1; return 1;
    } else if(strcmp(str, "NULL") == 0) {
        *num = 0; return 1;
    }

    if(!isdigit(str[0])) return 0;

    if(strlen(str) == 1) {
        if(isdigit(str[0])) {
            *num = str[0] - '0'; 
            return 1;
        } else {
            return 0;
        }
    } else if(strlen(str) >= 2) {
        if(str[0] == '0') {
            if(str[1] == 'x' || str[1] == 'X') {
                *num = strtol(str, NULL, 16); 
                return 1;
            } else if(str[1] == 'b' || str[1] == 'B') {
                for (int i = 2; i < strlen(str); i++) {
                    if (str[i] == '0' || str[i] == '1') {
                        *num = *num * 2 + (str[i] - '0');
                    } else {
                        return 0;
                    }
                }
                return 1;
            } 
        }

        for(int i = 0; i < strlen(str); i++) 
            if(!isdigit(str[i])) return 0;
        *num = strtol(str, NULL, 10);
        return 1;
    }
    return 0;
}