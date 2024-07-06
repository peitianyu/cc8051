#include "preprocess.h"

#define PREPROCESS_ERROR_EXIT(msg) printf("[error] preprocess:%s : %s : %d\n", msg, word_info->line , word_info->line_num)

HashMap g_define_map;
int g_def_start = 1;
struct DefineInfo
{
    char* arg_list[2];
    int arg_num;
    char* define_body;
    int define_start;
};

static char* get_date_time(int is_date);
static int is_blank(char ch);
static char* trim_all_blank(const char* str);
static char* skip_last_str(const char* code_str, const char* str, int str_len);
static char* get_arg_str(struct WordInfo* word_info, struct DefineInfo* define_info, int id);
static void predefined_macros(struct WordInfo* word_info);
static void pre_include_parse(struct WordInfo* word_info, void(func)(const char* code_path));
static int pre_define_parse(struct WordInfo* word_info);
static void define_parse(struct WordInfo* word_info);
static int pre_undef_parse(struct WordInfo* word_info);
static void pre_error_parse(struct WordInfo* word_info);
static void pre_warning_parse(struct WordInfo* word_info);
static void pre_if_parse(struct WordInfo* word_info);
static void pre_else_parse(struct WordInfo* word_info);
static void pre_endif_parse(struct WordInfo* word_info);
static void pre_ifdef_parse(struct WordInfo* word_info);
static void pre_ifndef_parse(struct WordInfo* word_info);

int preprocess(struct WordInfo* word_info, void(func)(const char* code_path))
{
    predefined_macros(word_info);

    if(word_info->shortcut == '#') {
        next_word_info(word_info);
        if(strcmp(word_info->word, "include") == 0) {
            pre_include_parse(word_info, func); return 1;
        } else if(strcmp(word_info->word, "define") == 0) {
            if(pre_define_parse(word_info)) return 1;
        } else if(strcmp(word_info->word, "undef") == 0) {
            if(pre_undef_parse(word_info)) return 1;
        } else if(strcmp(word_info->word, "error") == 0) {
            pre_error_parse(word_info); return 1;
        } else if(strcmp(word_info->word, "warning") == 0) {
            pre_warning_parse(word_info); return 1;
        } else if(strcmp(word_info->word, "if") == 0) {
            pre_if_parse(word_info); return 1;
        } else if(strcmp(word_info->word, "else") == 0) {
            pre_else_parse(word_info); return 1;
        } else if(strcmp(word_info->word, "endif") == 0) {
            pre_endif_parse(word_info); return 1;
        } else if(strcmp(word_info->word, "ifdef") == 0) {
            pre_ifdef_parse(word_info); return 1;
        } else if(strcmp(word_info->word, "ifndef") == 0) {
            pre_ifndef_parse(word_info); return 1;
        }
    }

    if(!g_def_start) return 1;
    define_parse(word_info);

    return 0;
}

static void predefined_macros(struct WordInfo* word_info)
{
    if(strcmp(word_info->word, "__LINE__") == 0)              sprintf(word_info->word, "%d", word_info->line_num);
    else if(strcmp(word_info->word, "__FILE__") == 0)         word_info->word = (char*)word_info->code_path;
    else if(strcmp(word_info->word, "__FUNCTION__") == 0)     word_info->word = word_info->func_name;
    else if(strcmp(word_info->word, "__VA_ARGS__") == 0)      word_info->word = "...";
    else if(strcmp(word_info->word, "__DATE__") == 0)         word_info->word = get_date_time(1);
    else if(strcmp(word_info->word, "__TIME__") == 0)         word_info->word = get_date_time(0);
}

static void pre_include_parse(struct WordInfo* word_info, void(func)(const char* code_path))
{
    next_special_word_info(word_info, "<\"");
    next_special_word_info(word_info, ">\"");
    char* include_path = skip_last_str(word_info->code_path, "/", 1);
    strcat(include_path, "/");
    strcat(include_path, word_info->word);
    func(include_path);
}

static int pre_define_parse(struct WordInfo* word_info)
{
    struct DefineInfo* define_info = (struct DefineInfo*)malloc(sizeof(struct DefineInfo));
    next_word_info(word_info);
    char *define_body = (char*)malloc(1024*sizeof(char));
    memset(define_body, 0, 1024);
    char* define_name = (char*)malloc(strlen(word_info->word) + 1);
    strcpy(define_name, word_info->word);
    if(word_info->shortcut == ' ') {
        next_word_info(word_info);
        if(strlen(trim_all_blank(word_info->word)) == 0) {
            *define_info = (struct DefineInfo) {{}, 0, 0};
            hash_map_insert(&g_define_map, define_name, define_info);
            // printf("define %s \n", define_name);
        } else {
            strcpy(define_body, word_info->word);
            *define_info = (struct DefineInfo) {{}, 0, define_body};
            hash_map_insert(&g_define_map, define_name, define_info);
            // printf("define %s : %s : %d\n", define_name, word_info->word, define_info->arg_num);
        }
        return 1;
    } else if(word_info->shortcut == '(') {
        next_special_word_info(word_info, ",)");
        char* word = trim_all_blank(word_info->word);
        char* word1 = (char*)malloc(strlen(word) + 1);
        strcpy(word1, word);
        if(word_info->shortcut == ')') 
        {
            if(strlen(word) == 0) {
                PREPROCESS_ERROR_EXIT(")");
                clear_word_info(word_info);
                return 1;
            } else {
                next_special_word_info(word_info, "\n");
                strcpy(define_body, word_info->word);
                *define_info = (struct DefineInfo) {{word1}, 1, define_body};
                hash_map_insert(&g_define_map, define_name, define_info);
                // printf("define %s : %s : %s : %d\n", define_name, word1, word_info->word, define_info->arg_num);
            }
        }else {
            next_special_word_info(word_info, ",)");
            word = trim_all_blank(word_info->word);
            char* word2 = (char*)malloc(strlen(word) + 1);
            strcpy(word2, word);
            next_special_word_info(word_info, "\n");
            strcpy(define_body, word_info->word);
            *define_info = (struct DefineInfo) {{word1, word2}, 2, define_body};
            hash_map_insert(&g_define_map, define_name, define_info);
            // printf("define %s : %s : %s : %s : %d\n", define_name, word1, word2, word_info->word, define_info->arg_num);
        }
        return 1;
    }
    return 0;
}


static void define_parse(struct WordInfo* word_info) {
    struct DefineInfo* define_info = (struct DefineInfo*)hash_map_get(&g_define_map, word_info->word);
    // printf("word: %s\n", word_info->word);
    char* defined_body;
    
    if(define_info == NULL) return ;
    if(define_info->define_body == 0) return ;

    // printf("define_info: %s\n", define_info->define_body);

    if(define_info->arg_num == 0) {
        defined_body = define_info->define_body;
    } else if (define_info->arg_num == 1) {
        next_special_word_info(word_info, ")");
        defined_body = get_arg_str(word_info, define_info, 0);
        if(defined_body == NULL) PREPROCESS_ERROR_EXIT("define");
    } else if (define_info->arg_num == 2) {
        next_special_word_info(word_info, ",");
        defined_body = get_arg_str(word_info, define_info, 0);
        if(defined_body == NULL) PREPROCESS_ERROR_EXIT("define");
        define_info->define_body = defined_body;
        next_special_word_info(word_info, ")");
        if(defined_body == NULL) PREPROCESS_ERROR_EXIT("define");
        defined_body = get_arg_str(word_info, define_info, 1);
    }
    
    char* code_str = (char*)malloc(strlen(word_info->code_str)+strlen(defined_body)); 
    strncpy(code_str, word_info->code_str, word_info->start_pos);
    strcat(code_str, defined_body);
    if(word_info->shortcut == '\n') strcat(code_str, " ");
    strcat(code_str, word_info->code_str+word_info->start_pos);
    word_info->code_str = code_str;
    next_word_info(word_info);
}

static int pre_undef_parse(struct WordInfo* word_info)
{
    next_word_info(word_info);
    if (hash_map_get(&g_define_map, word_info->word) == NULL) return 0;
    hash_map_erase(&g_define_map, word_info->word);
    return 1;
}

static void pre_error_parse(struct WordInfo* word_info)
{
    next_special_word_info(word_info, "\n");
    printf("error: %s\n", word_info->word);
}

static void pre_warning_parse(struct WordInfo* word_info)
{
    next_special_word_info(word_info, "\n");
    printf("warning: %s\n", word_info->word);
}

static void pre_if_parse(struct WordInfo* word_info)
{
    next_word_info(word_info);
    struct DefineInfo* define_info = (struct DefineInfo*)hash_map_get(&g_define_map, word_info->word);
    if (define_info == NULL) PREPROCESS_ERROR_EXIT("#if (x) : define not found");
    g_def_start = atoi(define_info->define_body);
    next_word_info(word_info);
}

static void pre_else_parse(struct WordInfo* word_info) {
    g_def_start = !g_def_start;
} 
static void pre_endif_parse(struct WordInfo* word_info) {
    g_def_start = 1;
}

static void pre_ifdef_parse(struct WordInfo* word_info) {
    next_word_info(word_info);
    struct DefineInfo* define_info = (struct DefineInfo*)hash_map_get(&g_define_map, word_info->word);
    g_def_start = (define_info != NULL);
}
static void pre_ifndef_parse(struct WordInfo* word_info)
{
    next_word_info(word_info);
    struct DefineInfo* define_info = (struct DefineInfo*)hash_map_get(&g_define_map, word_info->word);
    g_def_start = (define_info == NULL);
}

static int is_blank(char ch) {
    return ch == ' ' || ch == '\t' || ch == '\v' || ch == '\f' || ch == '\r';
}

static char* trim_all_blank(const char* str)
{
    static char trim_str[100];
    int i = 0, j = 0;
    while(i < strlen(str)) {
        if(!is_blank(str[i])) {
            trim_str[j] = str[i], j++;
        }
        i++;
    }
    trim_str[j] = '\0';
    return trim_str;
}

static char* get_date_time(int is_date) {
    time_t now = time(NULL); 
    struct tm *local_time = localtime(&now); 

    static char date_str[11];
    if(is_date)     strftime(date_str, sizeof(date_str), "%Y-%m-%d", local_time);
    else            strftime(date_str, sizeof(date_str), "%H:%M:%S", local_time);
    
    return date_str; 
}

static char* skip_last_str(const char* code_str, const char* str, int str_len)
{
    char* skip_str = malloc(sizeof(char) * 100); 
    int end_pos = strlen(code_str);
    char c = code_str[--end_pos];
    while(1) {
        c = code_str[end_pos--];
        for(int i = 0; i < str_len; i++) {
            if (c == '\0') return NULL;
            if (c == str[i]) {
                for(int j = 0; j <= end_pos; j++) skip_str[j] = code_str[j];
                return skip_str;
            }
        }
    }
}


static char* get_arg_str(struct WordInfo* word_info, struct DefineInfo* define_info, int id)
{
    static char* arg_str;
    if(strcmp(define_info->arg_list[id], "...") == 0) arg_str = "__VA_ARGS__";
    else                                             arg_str = define_info->arg_list[id];

    char* found = strstr(define_info->define_body, arg_str);
    if (found == NULL) return NULL;
    
    int pos = found - define_info->define_body;
    char* new_define_body = (char*)malloc(sizeof(char)*1024);
    if(define_info->define_body[pos-1] == '#') {
        if(pos-2 >= 0 &&(define_info->define_body[pos-2] == '#')) {
            strncpy(new_define_body, define_info->define_body, pos-2);
            strcat(new_define_body, word_info->word);
            strcat(new_define_body, found+strlen(arg_str));
        } else {
            strncpy(new_define_body, define_info->define_body, pos-1);
            strcat(new_define_body, "\"");
            strcat(new_define_body, word_info->word);
            strcat(new_define_body, "\"");
            strcat(new_define_body, found+strlen(arg_str));
        }
    } else {
        strncpy(new_define_body, define_info->define_body, pos);
        strcat(new_define_body, word_info->word);
        strcat(new_define_body, found+strlen(arg_str));
    }
}