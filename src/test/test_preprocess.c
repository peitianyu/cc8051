#include "common/c_test.h"
#include "common/logger.h"
#include "core/preprocess.h"

static void process(const char* code_path);

JUST_RUN_TEST(preprocess, test)
TEST(preprocess, test)
{
    const char* code_path = "../data/1_preprocess.c";
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
        // if(word_info.last_shortcut == '\n' && strlen(word_info.word) == 0) continue;
        
        print_word_info(&word_info);
    } 
}