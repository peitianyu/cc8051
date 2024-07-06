#include "common/c_test.h"
#include "common/logger.h"
#include "core/preprocess.h"
#include "core/expr.h"

static void process(const char* code_path);

JUST_RUN_TEST(data_type, test)
TEST(data_type, test)
{
    const char* code_path = "../data/2_data_type.c";
    
    process(code_path);
}

static void process(const char* code_path)
{
    struct WordInfo word_info = create_word_info(code_path);

    while(1)
    {
        if(word_info.code_str[word_info.start_pos] == '\0') break;

        next_word_info(&word_info);

        if(preprocess(&word_info, process)) continue;

        if(word_info.shortcut == '\n' && strlen(word_info.word) == 0) continue;

        update_token(&word_info);
    } 
}

