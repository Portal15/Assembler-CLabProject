#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headerFiles/inputAnalyzer.h"
#include "headerFiles/validationsChecker.h"
#include "headerFiles/binaryConvertor.h"
#include "headerFiles/objectTableAnalyzer.h"
#include "headerFiles/second.h"
#include "headerFiles/createFiles.h"
#include "headerFiles/errorHandler.h"
#include "headerFiles/preProcessor.h"
#include "headerFiles/utils.h"

#define NUM_OF_DASHES 40

void runOnFile(char* fileName);

void freeAll(objectTable *ot, externsAppearance *externs, binaryConvertedLines *list);
void freeLine(line *l);

int main(int argc, char** argv) {
    int i;
    while (argc > 1) {
        for (i = 0; i < NUM_OF_DASHES - strlen(argv[argc - 1]); i++) {
            if (i == (NUM_OF_DASHES - strlen(argv[argc - 1])) / 2) {
                printf(" %s ", argv[argc - 1]);
            }else {
                printf("-");
            }
        }
        printf("\n");
        runOnFile(argv[argc - 1]);
        argc--;
    }
    for (i = 0; i < NUM_OF_DASHES; i++) {
        printf("-");
    }
    printf("\n");
    return 0;
}


void runOnFile(char* fileName) {
    char fileNameAM[256];
    FILE *fileAM;
    objectTable *ot = createObjectTable();
    binaryConvertedLines *list = malloc(sizeof(binaryConvertedLines));
    int address = 100, tempAddress = address, dataWithNoLabel = 0, error = 0, i = 0, numOfLines = 0, lineNum = 0;

    char* firstForbiddenWords = "mov\ncmp\nadd\nsub\nlea\nclr\nnot\ninc\ndec\njmp\nbne\njsr\nred\nprn\nrts\nstop\nr1\nr2\nr3\nr4\nr5\nr6\nr7\nr8\n";
    /* Preprocessor */
    char* macros = preProcessor(fileName, firstForbiddenWords);
    if (macros == NULL) {
        printf("-X-     failed to create macro file\n");
        return;
    }
    printf("-V-     after macro file successfully created\n");
    /* End of Preprocessor */
    externsAppearance *externs;
    char *forbiddenWords = malloc(strlen(firstForbiddenWords) + strlen(macros) + 1);
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
    numOfLines = countLinesInFile(fileNameAM) + address;
    list->data = NULL;
    for (lineNum = address; lineNum < numOfLines; lineNum++) { /* This for loop presents the first pass */
	    i = lineNum;
        char* input = malloc(sizeof(char) * 100);
        int isValid = 1;
        line l;
        fgets(input, MAX_LINE_LENGTH, fileAM); /* Gets the input from the file */
        if (isSentence(input)) {
            l = analyzeInput(input, i);

            isValid = checkInputValidation(&l, forbiddenWords) == 1 ? isValid : 0;
            isValid = checkLabelErrors(*ot, &l) == 1 ? isValid : 0;
            if (!isValid) {
                error = 1;
            }

            addLineToObjectTable(&l, ot);
            isValid = checkObjectTableErrors(*ot);
            if (!isValid) {
                error = 1;
            }

            isValid = binaryConvertor(&l, &address, list);
            if (isValid) {
                error = 1;
            }

            if (l.label == NULL && l.vars != NULL && (l.vars->varType == 0 || l.vars->varType == 1)) {
                dataWithNoLabel += 1;
            }

            fillAddresses(ot, &tempAddress, i);
            free(input);
            freeLine(&l);
            tempAddress = address;
        }
    }
    free(macros);
    fclose(fileAM);
    free(forbiddenWords);

    if (checkForDoubles(ot)) {
        error = 1;
    }

    externs = malloc(sizeof(externsAppearance));
    externs->name = NULL;
    if (SecondPass(list, ot, externs) && !error) { /* This function presents the second pass */
       error = 1;
    }
    if (!error) {
        CreateFiles(list, fileNameAM, ot, externs, dataWithNoLabel);
    }else {
        printf("-X-     failed to create files because previous errors\n");
    }

    freeAll(ot, externs, list);
}

void freeAll(objectTable *ot, externsAppearance *externs, binaryConvertedLines *list) {
    objectTableEntry *entry = ot->entries;
    objectTableExtern *ext = ot->externs;
    objectTableData *data = ot->data;
    objectTableCommands *command = ot->commands;

    while (entry) {
        objectTableEntry *tempEntry = entry->next;
        if (entry->name != NULL) free(entry->name);
        free(entry);
        entry = tempEntry;
    }


    while (ext) {
        objectTableExtern *tempExt = ext->next;
        if (ext->name != NULL) free(ext->name);
        free(ext);
        ext = tempExt;
    }

    while (data) {
        objectTableData *tempData = data->next;
        if (data->name != NULL) free(data->name);
        if (data->value != NULL) freeVar(data->value);
        free(data);
        data = tempData;
    }

    while (command) {
        objectTableCommands *tempCommand = command->next;
        if (command->name != NULL) free(command->name);
        free(command);
        command = tempCommand;
    }

    while (externs) {
        externsAppearance *tempExtern = externs;
        if (externs->name != NULL) free(externs->name);
        externs = externs->next;
        free(tempExtern);
    }

    while (list) {
        binaryConvertedLines *tempLine = list->next;
        if(list->data != NULL) free(list->data);
        free(list);
        list = tempLine;
    }
    free(ot);
}

void freeLine(line *l) {
    if (l == NULL) return;
    if (l->label != NULL) free(l->label);
    if (l->cmd != NULL) {
        freeCommand(l->cmd);
    }
    if (l->vars != NULL) {
        freeVar(l->vars);
    }
}