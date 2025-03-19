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
    char* firstForbiddenWords = "mov\ncmp\nadd\nsub\nlea\nclr\nnot\ninc\ndec\njmp\nbne\njsr\nred\nprn\nrts\nstop\nr1\nr2\nr3\nr4\nr5\nr6\nr7\nr8";
    char* macros = preProcessor(fileName, firstForbiddenWords);
    if (macros == NULL) {
        return;
    }
    externsAppearance *externs;
    char *forbiddenWords = malloc(strlen(firstForbiddenWords) + strlen(macros) + 1); // +1 for the null terminator
    if (forbiddenWords == NULL) {
        printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
        return;
    }
    strcpy(forbiddenWords, firstForbiddenWords);
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
        if (isSentence(input)) {
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
            freeCommand(l.cmd);
            tempAddress = address;
        }
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