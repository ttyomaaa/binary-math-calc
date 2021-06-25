#include <stdio.h>
#include <stdlib.h>
struct opstck {
    char c;
    opstck *next;
};

struct numstck {
    int n;
    numstck *next;
};

numstck *num_push(numstck *HEAD, int a);
int num_del(numstck **HEAD);
opstck *op_push(opstck *HEAD, char a);
char op_del(struct opstck **HEAD);
void free_numstck(numstck *NUMSstck);
void free_opstck(opstck *OPSstck);