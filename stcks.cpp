#include "stcks.h"

numstck *num_push(numstck *HEAD, int a) {
    numstck *PTR;
    PTR = (numstck*) malloc(sizeof (numstck));
    PTR->n = a;
    PTR->next = HEAD;
    return PTR;
}

int num_del(numstck **HEAD) {
    numstck *PTR;
    int a;
    if (*HEAD == NULL) return -2;
    PTR = *HEAD;
    a = PTR->n;
    *HEAD = PTR->next;
    free(PTR);
    return a;
}

opstck *op_push(opstck *HEAD, char a) {
    opstck *PTR;
    PTR = (opstck*) malloc(sizeof (opstck));
    PTR->c = a;
    PTR->next = HEAD;
    return PTR;
}

char op_del(struct opstck **HEAD) {
    opstck *PTR;
    char a;
    if (*HEAD == NULL) return '\0';
    PTR = *HEAD;
    a = PTR->c;
    *HEAD = PTR->next;
    free(PTR);
    return a;
}

void free_numstck(numstck *NUMSstck) {
    while (NUMSstck != NULL)
        num_del(&NUMSstck);
}

void free_opstck(opstck *OPSstck) {
    while (OPSstck != NULL)
        op_del(&OPSstck);
}