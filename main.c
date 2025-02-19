#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lineAnalyzer.h"
#include "Validations.h"
#include "binaryConvertor.h"
#include "objectTableAnalyzer.h"
#include "second.h"
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

    /* */
    objectTable *ot = createObjectTable();
    for (int i = 0; i < 2; i++) {
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
        int hava = binaryConvertor(&l, 0);
        free(input);
        free(l.cmd);
    }
    binaryConvertedLines *list = malloc(sizeof(binaryConvertedLines));
    char *data = malloc(sizeof(char) * 100);
    strcpy(data, "12312321");
    list->data = data;
    binaryConvertedLines *next = malloc(sizeof(binaryConvertedLines));
    list->next = next;
    char *data1 = malloc(sizeof(char) * 100);
    strcpy(data1, "!xasda!232");
    next->data = data1;

    if (SecondPass(list, ot)) {
        printf("error\n");
    }
    free(list);
    return 0;
}
