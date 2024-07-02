#ifndef __PREPROCESS_H__
#define __PREPROCESS_H__

#include "word_info.h"
#include "utils/hash_map.h"
#include <time.h>


int preprocess(struct WordInfo* word_info, void(func)(const char* code_path));

#endif // __PREPROCESS_H__