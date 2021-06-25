#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

struct token {
    int type;
    //1-op
    //2-num
    //3-(
    //4-)
    //5-f
    //6-,
    //0-?
    //-1-end
    char *value;
};

token *tokenization(const char *expression);
void print_tokens(token *tokens);
void free_tokens(token *tokens);