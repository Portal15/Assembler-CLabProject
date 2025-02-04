#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lineAnalyzer.h"
#include "Validations.h"
#include "objectTableAnalyzer.h"
#include "errorHandler.h"
#include "preProcessor.h"

int main(void) {
    // char *preForbiddenWords = "jmp\nmov";
    // char *forbiddenWords;
    // forbiddenWords = malloc(strlen(preForbiddenWords) + 1);
    //
    // strcpy(forbiddenWords, preForbiddenWords);
    // char *mcros = preProcessor("bla.as", forbiddenWords);
    // printf("%s\n", mcros);
    // free(mcros);
    // free(forbiddenWords);
    objectTable *ot = createObjectTable();
    for (int i = 0; i < 10; i++) {
        char* input = malloc(sizeof(char) * 100);
        fgets(input, 100, stdin);
        int isValid = 1;
        line l = analyzeLine(input, i, ot);
        isValid = checkLineValidation(&l, "jmp\nmov", "bla") == 1 ? isValid : 0;
        isValid = checkLabelErrors(*ot, &l) == 1 ? isValid : 0;

        if (isValid) {
            addLineToObjectTable(&l, ot);
            isValid = checkObjectTableErrors(*ot);
            if (isValid) {
                printf("Valid\n");
            }else {
                printf("Invalid in object table\n");
            }
        } else {
            printf("Invalid\n");
        }
        free(input);
        free(l.cmd);
    }
    return 0;
}
