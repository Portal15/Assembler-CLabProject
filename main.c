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
#include "headerFiles/utils.h"

void runOnFile(char* fileName);

int main(int argc, char** argv) {

    while (argc > 1) {
        runOnFile(argv[argc - 1]);
        argc--;
    }

    /*
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
        isValid = checkLineValidation(&l, "jmp\nmov") == 1 ? isValid : 0;
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

        fillAddresses(ot, &tempInvisibleLineNum, i);
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
    */
    return 0;
}

void runOnFile(char* fileName) {
    char fileNameAM[256];
    FILE *fileAM;
    objectTable *ot = createObjectTable();
    binaryConvertedLines *list = malloc(sizeof(binaryConvertedLines));
    int address = 100;
    int tempAddress = address;
    int dataWithNoLabel = 0;
    int i = 0;
    char* commands = "jmp\nmov";
    char* macros = preProcessor(fileName, commands);
    if (macros == NULL) {
        return;
    }
    externsAppearance *externs;
    char *forbiddenWords = malloc(strlen(commands) + strlen(macros) + 1); // +1 for the null terminator
    if (forbiddenWords == NULL) {
        printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
        return;
    }
    strcpy(forbiddenWords, commands);
    strcat(forbiddenWords, macros);

    CreateFileName(fileName, fileNameAM, 3);
    fileAM = fopen(fileNameAM, "r");
    if (fileAM == NULL) {
        printErrorMsg(FAILED_TO_OPEN_FILE_ERROR, -1);
        return;
    }

    for (i = address; i < countLinesInFile(fileNameAM) + address; i++) {
        char* input = malloc(sizeof(char) * 100);
        int isValid = 1;
        line l;
        fgets(input, MAX_LINE_LENGTH, fileAM);
        l = analyzeLine(input, i, ot);

        isValid = checkLineValidation(&l, forbiddenWords) == 1 ? isValid : 0;
        isValid = checkLabelErrors(*ot, &l) == 1 ? isValid : 0;
        if (!isValid) {
            return;
        }

        addLineToObjectTable(&l, ot);
        isValid = checkObjectTableErrors(*ot);
        if (!isValid) {
            return;
        }

        isValid = binaryConvertor(&l, &address, list);
        if (isValid) {
            return;
        }

        if (l.label == NULL && l.vars != NULL && (l.vars->varType == 0 || l.vars->varType == 1)) {
            dataWithNoLabel += 1;
        }

        fillAddresses(ot, &tempAddress, i);
        free(input);
        free(l.cmd);
        tempAddress = address;
    }

    if (checkForDoubles(ot)) {
        return;
    }

    externs = malloc(sizeof(externsAppearance));
    if (SecondPass(list, ot, externs)) {
       return;
    }

    CreateFiles(list, fileNameAM, ot, externs, dataWithNoLabel);
}