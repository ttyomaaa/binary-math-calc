#include "stcks.h"
#include "tokens.h"
#include "calc.h"

#define MAX 255

bool make_rpn(token *tokens, char *outstring);
bool calc(token *tokens);

int main(void) {
    char expression[MAX], filename[MAX], *outstring;
    token *tokens = tokenization(expression);
    FILE *fp;

    printf("ALLOWED OPERANDS: BINARY NUMBERS\nALLOWED OPERATORS: &=AND, |=OR, ^=XOR, ~=NOT, f=MAX\n");
    printf("ENTER FILE NAME:\n");
    scanf("%s", filename);

    if (!(fp = fopen(filename, "r"))) {
        printf("FILE %s DOES NOT EXIST!\n", filename);
        return 0;
    }
    if (!fgets(expression, MAX, fp)) {
        fclose(fp);
        return 0;
    }
    fclose(fp);

    printf("EXPRESSION = %s\n", expression);
    tokens = tokenization(expression);
    //print_tokens(tokens);
    outstring = (char*) malloc((2 * strlen(expression) + 1) * sizeof (char));
    if (make_rpn(tokens, outstring)) {
        printf("\n\n\tCONVERSION SUCCESSFUL!\n\n");
        free_tokens(tokens);
        tokens = tokenization(outstring);
        //print_tokens(tokens);
        if (calc(tokens)) printf("\n\tCALCULATION SUCCESSFUL!\n");
    }
    free(outstring);
    free_tokens(tokens);
    return 0;
}

bool make_rpn(token *tokens, char *outstring) {
    opstck *OPSstck = NULL;
    int k = 0, point = 0;

    while (tokens[k].type != -1) {
        if (tokens[k].type == 2) {
            for (int i = 0; i < (int) strlen(tokens[k].value); i++) {
                outstring[point++] = tokens[k].value[i];
            }
            outstring[point++] = ' ';
        } else if (tokens[k].type == 5) {
            OPSstck = op_push(OPSstck, 'f');
        } else if (tokens[k].type == 6) {
            while ((OPSstck != NULL)&&((OPSstck->c) != '(')) {
                outstring[point++] = op_del(&OPSstck);
                outstring[point++] = ' ';
            }
            if (OPSstck == NULL) {
                printf("ERROR: STACK IS EMPTY(COMMA)!\n");
                return false;
            }
        } else if (tokens[k].type == 1) {
            if (OPSstck == NULL)
                OPSstck = op_push(OPSstck, tokens[k].value[0]);
            else {
                if (prior(OPSstck->c) < prior(tokens[k].value[0]))
                    OPSstck = op_push(OPSstck, tokens[k].value[0]);
                else {
                    while ((OPSstck != NULL)&&(prior(OPSstck->c) >= prior(tokens[k].value[0]))) {
                        outstring[point++] = op_del(&OPSstck);
                        outstring[point++] = ' ';
                    }
                    OPSstck = op_push(OPSstck, tokens[k].value[0]);
                }
            }
        } else if (tokens[k].type == 3) {
            if (OPSstck != NULL)
                if ((OPSstck->c) == 'f') {
                    outstring[point++] = '(';
                    outstring[point++] = ' ';
                }
            OPSstck = op_push(OPSstck, '(');
        } else if (tokens[k].type == 4) {
            while ((OPSstck != NULL)&&((OPSstck->c) != '(')) {
                outstring[point++] = op_del(&OPSstck);
                outstring[point++] = ' ';
            }
            if (OPSstck == NULL) {
                printf("ERROR: EMPTY STACK(RIGHT_PARENTHESIS)!\n");
                return false;
            }
            op_del(&OPSstck);

            if (OPSstck != NULL) {
                if ((OPSstck->c) == 'f') {
                    outstring[point++] = op_del(&OPSstck);
                    outstring[point++] = ' ';
                }
            }
        } else {
            printf("ERROR: UNKNOWN TOKEN!\n");
            free_opstck(OPSstck);
            return false;
        }
        k++;
    }
    while (OPSstck != NULL) {
        if ((OPSstck->c) == '(' || (OPSstck->c) == ')') {
            printf("ERROR: PARENTHESIS MISMATCHED!\n");
            free_opstck(OPSstck);
            return false;
        }
        outstring[point++] = op_del(&OPSstck);
        outstring[point++] = ' ';
    }

    outstring[point] = '\0';
    printf("EXPRESSION IN RPN = ");
    for (int i = 0; i < point; i++) {
        if (outstring[i] != '(')
            printf("%c", outstring[i]);
    }
    return true;
}

bool calc(token *tokens) {
    numstck *NUMSstck = NULL;
    int k = 0, count = 0, res;
    int n;
    while (tokens[k].type != -1) {
        if (tokens[k].type == 2) {
            n = str2num(tokens[k].value);
            NUMSstck = num_push(NUMSstck, n);
            count++;
        } else if (tokens[k].type == 3) {
            n = -1;
            NUMSstck = num_push(NUMSstck, n);
            count++;
        } else if ((tokens[k].type == 1) || (tokens[k].type == 5)) {
            int args;
            if (tokens[k].type == 1) {
                args = arg_count(tokens[k].value[0]);
                if (count < args) {
                    printf("ERROR: NOT ENOUGH ARGUMENTS!\n");
                    free_numstck(NUMSstck);
                    return false;
                }
                if (tokens[k].value[0] == '~') {
                    if (NUMSstck != NULL) {
                        int tmp = negation((unsigned int) num_del(&NUMSstck));
                        count--;
                        NUMSstck = num_push(NUMSstck, tmp);
                        count++;
                    }
                } else if (tokens[k].value[0] == '|') {
                    int tmp1 = num_del(&NUMSstck);
                    count--;
                    int tmp2 = num_del(&NUMSstck);
                    count--;
                    NUMSstck = num_push(NUMSstck, tmp2 | tmp1);
                    count++;
                } else if (tokens[k].value[0] == '&') {
                    int tmp1 = num_del(&NUMSstck);
                    count--;
                    int tmp2 = num_del(&NUMSstck);
                    count--;
                    NUMSstck = num_push(NUMSstck, tmp2 & tmp1);
                    count++;
                } else if (tokens[k].value[0] == '^') {
                    int tmp1 = num_del(&NUMSstck);
                    count--;
                    int tmp2 = num_del(&NUMSstck);
                    count--;
                    NUMSstck = num_push(NUMSstck, tmp2^tmp1);
                    count++;
                }
            } else if (tokens[k].type == 5) {
                if ((count == 1) || (count == 0)) {
                    printf("ERROR: ZERO ARGUMENTS IN FUNCTION!\n");
                    free_numstck(NUMSstck);
                    return false;
                }
                int *arr = (int*) malloc((count - 1) * sizeof (int));
                int i = 0;
                while ((NUMSstck != NULL)&&((NUMSstck->n) != -1)) {
                    arr[i] = num_del(&NUMSstck);
                    i++;
                    count--;
                }
                if (NUMSstck != NULL)
                    num_del(&NUMSstck);
                count--;
                NUMSstck = num_push(NUMSstck, nfunc(arr, i));
                count++;
                free(arr);
            }
        }
        k++;
    }
    res = num_del(&NUMSstck);
    if (res != -2)
        count--;
    else {
        printf("ERROR: EMPTY STACK(NO RESULT)!\n");
        return false;
    }

    if (num_del(&NUMSstck) != -2) {
        printf("ERROR: MORE THAN ONE ELEMENT IN STACK AT THE END OF CALCULATIONS!\n");
        free_numstck(NUMSstck);
        return false;
    }
    printf("RESULT = %d\n", num2bin(res));
    return true;
}

