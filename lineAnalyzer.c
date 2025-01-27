#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "lineAnalyzer.h"
#include "errorHandler.h"

char *removeSpacesUntilChar(char *line);
char *getStringUntilNonLetterChar(char *str);
char *removeUntilNonLetter(char *str);
int isEndOfString(const char *str);
void freeCommand(command *cmd);

line analyzeLine(char *inputedLine, int lineNum) {
    line newLine;
    char *label = NULL;
    command *cmd = NULL;
    char* currentParm = NULL;
    var *vars = NULL;

    /* Initialize newLine structure */
    memset(&newLine, 0, sizeof(line));
    newLine.lineNum = lineNum;

    cmd = (command *)malloc(sizeof(command));
    if (cmd == NULL) {
        printErrorMsg(MEMORY_ALLOCATION_ERROR, lineNum);
        return newLine;
    }
    /* Initialize command structure */
    memset(cmd, 0, sizeof(command));

    /* Step 1: Remove leading spaces */
    inputedLine = removeSpacesUntilChar(inputedLine);

    /* Step 2: Check if line is empty */
    if (isEndOfString(inputedLine)) {
        free(cmd);
        return newLine;
    }

    /* Step 3: Extract first parameter */
    currentParm = getStringUntilNonLetterChar(inputedLine);
    if (currentParm == NULL) {
        free(cmd);
        return newLine;
    }
    inputedLine = removeUntilNonLetter(inputedLine);

    /* Step 4: Check for label */
    if (currentParm[strlen(currentParm) - 1] == ':') {
        currentParm[strlen(currentParm) - 1] = '\0'; /* Step 4.1: Remove ':' */
        label = strdup(currentParm); /* Step 4.2: Set label */

        if (label == NULL) {
            free(currentParm);
            free(cmd);
            return newLine;
        }
        free(currentParm);

        inputedLine = removeSpacesUntilChar(inputedLine); /* Step 4.3: Remove spaces after label */
        if (isEndOfString(inputedLine)) { /* Step 4.4: Check if line is empty */
            newLine.label = label;
            free(cmd);
            return newLine;
        }

        currentParm = getStringUntilNonLetterChar(inputedLine); /* Step 4.5: Extract command name */
        if (currentParm == NULL) {
            free(label);
            free(cmd);
            return newLine;
        }
        inputedLine = removeUntilNonLetter(inputedLine);
    }

    /* Step 5: Skip assembler directives */
    if (currentParm[0] == '.') {
        currentParm++;
        free(currentParm);
        free(label);
        free(cmd);
        return newLine;
    }

    /* Step 6: Set command name */
    cmd->name = strdup(currentParm);
    free(currentParm);
    currentParm = NULL;

    if (cmd->name == NULL) {
        free(label);
        free(cmd);
        return newLine;
    }
    cmd->isFilled[0] = 1;

    /* Step 7: Remove landing spaces */
    inputedLine = removeSpacesUntilChar(inputedLine);

    if (isEndOfString(inputedLine)) { /* Step 8:  Check if line is empty */
        newLine.label = label;
        newLine.cmd = cmd;
        return newLine;
    }

    if (inputedLine[0] == ',') { /* Step 9: Check for illegal comma */
        cmd->error = ILLEGAL_COMMA_ERROR;
        printErrorMsg(ILLEGAL_COMMA_ERROR, lineNum);
        newLine.label = label;
        newLine.cmd = cmd;
        return newLine;
    }

    currentParm = getStringUntilNonLetterChar(inputedLine); /* Step 10: Extract first parameter */

    if (currentParm == NULL) {
        free(label);
        freeCommand(cmd);
        return newLine;
    }
    inputedLine = removeUntilNonLetter(inputedLine);

    /* Step 11: Process first parameter addressing */
    if (currentParm[0] == '#') {
        cmd->parm1Addressing0 = strdup(currentParm + 1);
        cmd->isFilled[1] = 0;
    } else if (currentParm[0] == '&') {
        cmd->parm1Addressing2 = strdup(currentParm + 1);
        cmd->isFilled[1] = 2;
    } else if (currentParm[0] == 'r') {
        cmd->parm1Addressing3 = strdup(currentParm + 1);
        cmd->isFilled[1] = 3;
    } else {
        cmd->parm1Addressing1 = strdup(currentParm);
        cmd->isFilled[1] = 1;
    }

    free(currentParm);
    currentParm = NULL;

    /* Step 12: Remove landing spaces */
    inputedLine = removeSpacesUntilChar(inputedLine);

    /* Step 13: Check if line is empty */
    if (isEndOfString(inputedLine)) {
        newLine.label = label;
        newLine.cmd = cmd;
        return newLine;
    }

    /* Step 14: Check for missing comma */
    if (inputedLine[0] != ',') {
        cmd->error = MISSING_COMMA_ERROR;
        printErrorMsg(MISSING_COMMA_ERROR, lineNum);
        newLine.label = label;
        newLine.cmd = cmd;
        return newLine;
    }
    inputedLine++;

    /* Step 15: Remove landing spaces */
    inputedLine = removeSpacesUntilChar(inputedLine);

    /* Step 16: Check if line is empty */
    if (isEndOfString(inputedLine)) {
        cmd->error = UNEXPECTED_TEXT_AFTER_END_OF_LINE_ERROR;
        printErrorMsg(UNEXPECTED_TEXT_AFTER_END_OF_LINE_ERROR, lineNum);
        newLine.label = label;
        newLine.cmd = cmd;
        return newLine;
    }

    /* Step 17: Check for multiple comma */
    if (inputedLine[0] == ',') {
        cmd->error = MULTIPLE_COMMA_ERROR;
        printErrorMsg(MULTIPLE_COMMA_ERROR, lineNum);
        newLine.label = label;
        newLine.cmd = cmd;
        return newLine;
    }

    /* Step 18: Extract second parameter */
    currentParm = getStringUntilNonLetterChar(inputedLine);
    if (currentParm == NULL) {
        free(label);
        freeCommand(cmd);
        return newLine;
    }
    inputedLine = removeUntilNonLetter(inputedLine);

    /* Step 19: Process second parameter addressing */
    if (currentParm[0] == '#') {
        cmd->parm2Addressing0 = strdup(currentParm + 1);
        cmd->isFilled[2] = 0;
    } else if (currentParm[0] == '&') {
        cmd->parm2Addressing2 = strdup(currentParm + 1);
        cmd->isFilled[2] = 2;
    } else if (currentParm[0] == 'r') {
        cmd->parm2Addressing3 = strdup(currentParm + 1);
        cmd->isFilled[2] = 3;
    } else {
        cmd->parm2Addressing1 = strdup(currentParm);
        cmd->isFilled[2] = 1;
    }

    free(currentParm);
    currentParm = NULL;

    /* Step 20: Remove landing spaces */
    inputedLine = removeSpacesUntilChar(inputedLine);

    /* Step 21: Check if line is empty */
    if (!isEndOfString(inputedLine)) {
        cmd->error = UNEXPECTED_TEXT_AFTER_END_OF_LINE_ERROR;
        printErrorMsg(UNEXPECTED_TEXT_AFTER_END_OF_LINE_ERROR, lineNum);
    }

    newLine.label = label;
    newLine.cmd = cmd;
    newLine.vars = vars;
    return newLine;
}

char *removeSpacesUntilChar(char *line) {
    while (isspace(*line)) {
        line++;
    }
    return line;
}

char *getStringUntilNonLetterChar(char *str) {
    char *string = malloc(sizeof(char) * MAX_SIZE_OF_STRING);
    char *start;

    if (string == NULL) {
        printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
        return NULL;
    }
    start = string;
    while (*str != '\0' && *str != ',' && *str != ' ' && *str != '\n' && *str != '\t') {
        *start = *str;
        start++;
        str++;
    }
    *start = '\0';
    return string;
}

char *removeUntilNonLetter(char *str) {
    while (*str != '\0' && *str != ',' && *str != ' ' && *str != '\n' && *str != '\t') {
        str++;
    }
    return str;
}

int isEndOfString(const char *str) {
    return *str == '\0' || *str == '\n' || *str == EOF;
}

void freeCommand(command *cmd) {
    if (cmd) {
        free(cmd->name);
        free(cmd->parm1Addressing0);
        free(cmd->parm1Addressing1);
        free(cmd->parm1Addressing2);
        free(cmd->parm1Addressing3);
        free(cmd->parm2Addressing0);
        free(cmd->parm2Addressing1);
        free(cmd->parm2Addressing2);
        free(cmd->parm2Addressing3);
        free(cmd);
    }
}