#include "word_info.h"
#define GET_WORD_ERROR_EXIT(msg) printf("[error] get_word:%s : %s : %d\n", msg, info->line , info->line_num)
static char* read_file(const char* path)
{
    FILE* fp = fopen(path, "r");
    if (fp == NULL) {
        printf("open file %s failed\n", path);
        return NULL;
    }

    fseek(fp, 0, SEEK_END);
    int file_size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char* code_str = (char*)malloc(file_size + 1);
    fread(code_str, 1, file_size, fp);
    code_str[file_size] = '\0';

    fclose(fp);
    return code_str;
}


static char* new_line(const char* code_str, int start_pos)
{
    char* line = (char*)malloc(MAX_LINE_NUM + 1);
    int i = start_pos;
    while (code_str[i] != '\n' && code_str[i] != '\0') {
        line[i - start_pos] = code_str[i];
        i++;
    }
    line[i - start_pos] = '\0';
    return line;
}

struct WordInfo create_word_info(const char* code_path)
{
    struct WordInfo info;
    info.word = "";
    info.last_shortcut = '\0';
    info.shortcut = '\0';
    info.code_str = read_file(code_path);
    info.start_pos = 0;
    info.code_path = code_path;
    info.func_name = "";
    info.line = new_line(info.code_str, info.start_pos);
    info.line_num = 1;

    return info;
}

void clear_word_info(struct WordInfo* info)
{
    info->word = "";
    info->last_shortcut = '\0';
    info->shortcut = '\0';
    info->code_str = "\0";
    info->start_pos = 0;
    info->code_path = "\0";
    info->func_name = "";
    info->line = "";
    info->line_num = 0;
}

static int handle_comment(struct WordInfo* info, char c, int  skip_len);
static int handle_string(struct WordInfo* info, char c, char* skip_str, int* skip_len);
static int handle_back_slash(struct WordInfo* info, char c, int* skip_len);
static int handle_special_chars(struct WordInfo* info, char c, char* skip_str, int* skip_len, const char *special_chars);

void next_word_info(struct WordInfo* info)
{
    const char special_chars[] = " ~!#%^&*()[]{}-+=/|<>,.:;\?\'\"\n\t\v\f\0\r";
    next_special_word_info(info, special_chars);
}
void next_special_word_info(struct WordInfo* info, const char *special_chars)
{
    char skip_str[1024] = {0};
    int  skip_len = 0;
    
    while (1) {
        char c = info->code_str[info->start_pos];
        info->start_pos++;
        if (c == '\0') { return; }

        if(handle_comment(info, c, skip_len)) continue;
        if(handle_back_slash(info, c, &skip_len)) continue;

        skip_str[skip_len++] = c;

        if(handle_string(info, c, skip_str, &skip_len)) continue;

        if(handle_special_chars(info, c, skip_str, &skip_len, special_chars)) return; 
    }
}

static int handle_comment(struct WordInfo* info, char c, int skip_len)
{
    if(c == ' ' && skip_len == 0) {
        return 1;
    } else if(c == '/') {
        if(info->code_str[info->start_pos] == '/') {
            while(info->code_str[info->start_pos] != '\n') info->start_pos++;
            return 1;
        } else if(info->code_str[info->start_pos] == '*') {
            while(info->code_str[info->start_pos] != '*' || info->code_str[info->start_pos + 1] != '/') {
                if(info->code_str[info->start_pos] == '\n') {
                    info->line = new_line(info->code_str, info->start_pos);
                    info->line_num++;
                }
                info->start_pos++;
            }
            info->start_pos+=2;
            return 1;
        }
    } 
    return 0;
}


static int handle_string(struct WordInfo* info, char c, char* skip_str, int* skip_len)
{
    if(info->shortcut == '\"') {
        if(info->last_shortcut != '\"') {
            if(c == '\"') {
                skip_str[*skip_len-1] = '\0';
                info->word = skip_str;
                info->last_shortcut = info->shortcut;
                info->shortcut = c;
                return 0;
            }
            return 1;
        }
    }
    return 0;
}

static int handle_special_chars(struct WordInfo* info, char c, char* skip_str, int* skip_len, const char *special_chars)
{
    for (int i = 0; i < strlen(special_chars); i++) {
        if (c == special_chars[i]) {
            
            skip_str[*skip_len-1] = '\0';
            info->word = skip_str;
            info->last_shortcut = info->shortcut;
            info->shortcut = c;

            // 换行更新
            if(c == '\n') {
                info->line = new_line(info->code_str, info->start_pos);
                info->line_num++;
            }
            return 1;
        }
    }
    return 0;
}


static int handle_back_slash(struct WordInfo* info, char c, int* skip_len)
{
    if(c == '\\') {
        if(info->code_str[info->start_pos] == '\n') {
            *skip_len--;
            info->line_num++;
            info->start_pos++;
            return 1;
        }else{
            GET_WORD_ERROR_EXIT("backslash");
            clear_word_info(info);
            return 2;
        }
    }
    return 0;
}

void print_word_info(struct WordInfo* info)
{
    // printf("%d|%s|%c|%s|%c\n", info->line_num, info->line, info->last_shortcut, info->word, info->shortcut);
    printf("%d|%c|%s|%c\n", info->line_num, info->last_shortcut, info->word, info->shortcut);
}