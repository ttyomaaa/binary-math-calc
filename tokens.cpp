#include "tokens.h"

token *tokenization(const char *expression) {
    int len = 0;
    const char *c = expression;
    token *tokens = (token*) malloc((strlen(expression) + 1) * sizeof (token));
    token currtoken;
    while (*c) {
        currtoken = {0, NULL};
        if (*c == '(') {
            currtoken.type = 3;
        } else if (*c == ')') {
            currtoken.type = 4;
        } else if (*c == ',') {
            currtoken.type = 6;
        } else if (strchr("&|~^", *c)) {
            currtoken.type = 1;
            currtoken.value = strndup(c, 1);

        } else if (sscanf(c, "%m[0-1]", &currtoken.value)) {
            currtoken.type = 2;
        } else if (*c == 'f') {
            currtoken.type = 5;
            currtoken.value = strndup(c, 1);
        }
        if (!isspace(*c)) {
            tokens[len] = currtoken;
            len++;
        }
        if (currtoken.type == 2)
            c += strlen(currtoken.value);
        else c += 1;
    }
    currtoken = {-1, NULL};
    tokens[len] = currtoken;
    return tokens;
}

void print_tokens(token *tokens) {
    int i = 0;
    for (token *Token = tokens; Token->type != -1; Token++, i++) {
        if (Token->value)
            printf("%d: %d  %s\n", i, Token->type, Token->value);
        else
            printf("%d: %d\n", i, Token->type);
        //printf("%d: %d \n",i+1,Token->type);
    }
}

void free_tokens(token *tokens) {
    for (token *Token = tokens; Token->type != -1; Token++)
        if((Token->type==1)||(Token->type==2)||(Token->type==5)) free(Token->value);
    free(tokens);
}