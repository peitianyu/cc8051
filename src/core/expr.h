#ifndef __EXPR_H__
#define __EXPR_H__

#include "utils/stack.h"
#include "word_info.h"
#include <ctype.h>

struct VarType {
    int base_type;
    int* decorate_type;
    int decorate_type_size;
};

struct VarInfo {
    struct VarType type;
    void *val;
};

void update_token(struct WordInfo *word_info);

#endif // __EXPR_H__