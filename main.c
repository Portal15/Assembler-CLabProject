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

    /* */
    objectTable *ot = createObjectTable();
    binaryConvertedLines *list = malloc(sizeof(binaryConvertedLines));
    int invisibleLineNum = 100;
    int tempInvisibleLineNum = invisibleLineNum;
    int dataWithNoLabel = 0;
    int i = 0;

    for (i = 100; i < 121; i++) {
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
        if (l.label == NULL && l.vars != NULL && (l.vars->varType == 0 || l.vars->varType == 1)) {
            dataWithNoLabel += 1;
        }

        fillInvisibleLineNum(ot, &tempInvisibleLineNum, i);
        free(input);
        free(l.cmd);
        tempInvisibleLineNum = invisibleLineNum;
    }
    if (checkForDoubles(ot)) {
        printf("error at doubles\n");
    }
    externsAppearance *externs = malloc(sizeof(externsAppearance));
    if (SecondPass(list, ot, externs)) {
        printf("error\n");
    }

    CreateFiles(list, "bla.as", ot, externs, dataWithNoLabel);
    return 0;
}