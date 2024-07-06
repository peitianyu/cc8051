#include "expr.h"

enum Operator {
#define DEF(id, str) id, 
#include "cc_operator.h"
#undef DEF
};

static const char* g_op_keywords[] = {
#define DEF(id, str) str,
#include "cc_operator.h"
#undef DEF
};

enum cc_token {
#define DEF(id, str) id,
#include "cc_token.h"
#undef DEF
};

static const char* g_token_keywords[] = {
#define DEF(id, str) str, 
#include "cc_token.h"
#undef DEF
};

static int is_digit_str(char* str, int *num);
static int is_token(char* str);
void update_token(struct WordInfo *word_info)
{
    int token_id = is_token(word_info->word);
    if(token_id != -1) {
        printf("token: %s\n", g_token_keywords[token_id]);
        return ;
    }
    
    int num = 0;
    if(is_digit_str(word_info->word, &num)) {
        printf("num: %d\n", num);
        return;
    }

    if(strlen(word_info->word) == 0) {
        // TODO: 判断运算符
    } else if(strlen(word_info->word)>0) {
        printf("name: %s\n", word_info->word);
    }
}


static int is_token(char* str)
{
    for(int i = 0; i < sizeof(g_token_keywords) / sizeof(g_token_keywords[0]); i++) 
        if(strcmp(str, g_token_keywords[i]) == 0) return i;
    return -1;
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

    if(strlen(str) == 1) {
        if(isdigit(str[0])) {
            *num = str[0] - '0'; return 1;
        } else {  return 0; }
    } else if(strlen(str) >= 2) {
        if(str[0] == '-') {
            if(is_digit_str(str + 1, num)) {
                *num = -*num;  return 1;
            } else { return 0; }
        } else if(str[0] == '!') {
            if(is_digit_str(str + 1, num)) {
                *num = !*num;  return 1;
            } else { return 0; }
        }
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