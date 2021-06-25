#include "calc.h"

int arg_count(char c) {
    switch (c) {
        case '~':
            return 1;
        case '&':
            return 2;

        case '^':
            return 2;
        case '|':
            return 2;
    }
    return -1;
}
int str2num(char *s) {
    int res = 0;
    for (int i = 0; i < (int) strlen(s); i++) {
        res = res * 2 + (int) (s[i] - '0');
    }
    return res;
}

int num2bin(int num) {
    int bin = 0, k = 1;

    while (num) {
        bin += (num % 2) * k;
        k *= 10;
        num /= 2;
    }

    return bin;
}

int nfunc(int *arr, int n) {
    int res = 0;
    for (int i = 0; i < n; i++) {
        if (arr[i] > res) res = arr[i];
    }
    return res;
}

int prior(char a) {
    switch (a) {
        case '~':
            return 4;
        case '&':
            return 3;
        case '^':
            return 2;
        case '|':
            return 1;
        case '(':
            return 0;
    }
    return -1;
}

int negation(unsigned int n) {
    unsigned int mask;
    unsigned int tmp = n;
    unsigned int bit = 0;

    if (tmp == 0) return 1;
    while (tmp) {
        tmp >>= 1;
        bit++;
    }
    mask = (1U << bit) - 1;
    n = n ^ mask;
    return (int) n;
}
