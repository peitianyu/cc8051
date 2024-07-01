#ifndef __WORD_INFO_H__
#define __WORD_INFO_H__

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE_NUM (1024)

struct WordInfo
{
    char* word;
    char last_shortcut;
    char shortcut;
    char* code_str;
    int start_pos;

    const char* code_path;
    char* func_name;
    char* line;
    int line_num;
};


struct WordInfo create_word_info(const char* code_path);
void clear_word_info(struct WordInfo* info);
void next_word_info(struct WordInfo* info);
void next_special_word_info(struct WordInfo* info, const char *special_chars);
void print_word_info(struct WordInfo* info);


#endif // __WORD_INFO_H__