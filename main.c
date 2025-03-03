#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headerFiles/lineAnalyzer.h"
#include "headerFiles/Validations.h"
#include "headerFiles/binaryConvertor.h"
#include "headerFiles/objectTableAnalyzer.h"
#include "headerFiles/second.h"
#include "headerFiles/createFiles.h"
#include "headerFiles/errorHandler.h"
#include "headerFiles/preProcessor.h"

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
    binaryConvertedLines *list = malloc(sizeof(binaryConvertedLines));
    int invisibleLineNum = 100;
    int tempInvisibleLineNum = invisibleLineNum;

    for (int i = 100; i < 121; i++) {
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
        int hava = binaryConvertor(&l, &invisibleLineNum, list);
        if (hava) {
            printf("Error in binary convertor\n");
        }
        fillInvisibleLineNum(ot, &tempInvisibleLineNum, i);
        free(input);
        free(l.cmd);
        tempInvisibleLineNum = invisibleLineNum;
    }
    if (checkForDoubles(ot)) {
        printf("error at doubles\n");
    }

    if (SecondPass(list, ot)) {
        printf("error\n");
    }

    CreateFiles(list, "bla.as", ot);
    // free(list);
    return 0;
}
