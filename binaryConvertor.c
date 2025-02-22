#include "binaryConvertor.h"
#include "errorHandler.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Constants */
#define A 4 /* 100 */
#define R 2 /* 010 */
#define E 1 /* 001 */
#define MAX_NUM_LENGTH 20
#define MAX_LINE_LENGTH 256

/* Forward declarations */
int isCommand(const line *l);
int getBinaryCodeFromNumber(int number);
int getOpcodeAndFunctCode(const line *l);
int getAddressingCode(const line *l, int *invisibleLineNum);
int getRegistersCode(const line *l);
int addLineToList(binaryConvertedLines *list, char *data);

/* Main binary converter function */
int binaryConvertor(line *l_param, int *invisibleLineNum_param, binaryConvertedLines *list_param) {
    int binaryCode = 0;
    char tempBuffer[MAX_LINE_LENGTH];
    char *str;


    if (isCommand(l_param)) {
        char *binaryStr;

        binaryCode = getOpcodeAndFunctCode(l_param);
        binaryCode |= getAddressingCode(l_param, invisibleLineNum_param);
        binaryCode |= getRegistersCode(l_param);
        binaryCode |= A;

        binaryStr = (char *)malloc(MAX_NUM_LENGTH * sizeof(char));
        if (binaryStr == NULL) {
            printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
            return 1;
        }

        sprintf(binaryStr, "%d", binaryCode);
        if (addLineToList(list_param, binaryStr)) {
            free(binaryStr);
            return 1;
        }

        /* Handle parameter 1 */
        if (l_param->cmd->parm1Addressing0 != NULL) {
            char *numStr;
            binaryCode = atoi(l_param->cmd->parm1Addressing0);

            numStr = (char *)malloc(MAX_NUM_LENGTH * sizeof(char));
            if (numStr == NULL) {
                printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
                return 1;
            }

            sprintf(numStr, "%d", binaryCode);
            if (addLineToList(list_param, numStr)) {
                free(numStr);
                return 1;
            }
        }
        else if (l_param->cmd->parm1Addressing1 != NULL) {
            sprintf(tempBuffer, "!%s!%d", l_param->cmd->parm1Addressing1, l_param->lineNum);
            str = strdup(tempBuffer);
            if (str == NULL || addLineToList(list_param, str)) {
                free(str);
                return 1;
            }
        }
        else if (l_param->cmd->parm1Addressing2 != NULL) {
            sprintf(tempBuffer, "!%s!%d!%d",
                l_param->cmd->parm1Addressing2,
                l_param->lineNum,
                *invisibleLineNum_param);
            str = strdup(tempBuffer);
            if (str == NULL || addLineToList(list_param, str)) {
                free(str);
                return 1;
            }
        }

        /* Handle parameter 2 */
        if (l_param->cmd->parm2Addressing0 != NULL) {
            char *numStr;
            binaryCode = atoi(l_param->cmd->parm2Addressing0);

            numStr = (char *)malloc(MAX_NUM_LENGTH * sizeof(char));
            if (numStr == NULL) {
                printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
                return 1;
            }

            sprintf(numStr, "%d", binaryCode);
            if (addLineToList(list_param, numStr)) {
                free(numStr);
                return 1;
            }
        }
        else if (l_param->cmd->parm2Addressing1 != NULL) {
            sprintf(tempBuffer, "!%s!%d", l_param->cmd->parm2Addressing1, l_param->lineNum);
            str = strdup(tempBuffer);
            if (str == NULL || addLineToList(list_param, str)) {
                free(str);
                return 1;
            }
        }
        else if (l_param->cmd->parm2Addressing2 != NULL) {
            sprintf(tempBuffer, "!%s!%d!%d",
                l_param->cmd->parm2Addressing2,
                l_param->lineNum,
                *invisibleLineNum_param);
            str = strdup(tempBuffer);
            if (str == NULL || addLineToList(list_param, str)) {
                free(str);
                return 1;
            }
        }
    }
    return 0;
}

/* Helper function implementations */
int isCommand(const line *l) {
    return l != NULL && l->cmd != NULL;
}

int getBinaryCodeFromNumber(int number) {
    int binaryCode = 0;
    int i = 1;

    while (number != 0) {
        binaryCode += (number % 2) * i;
        number /= 2;
        i *= 10;
    }
    return binaryCode;
}

int getOpcodeAndFunctCode(const line *l) {
    struct commandsOpcode {
        const char *name;
        int opcode;
        int funct;
    };

    /* Static command table */
    static const struct commandsOpcode commandsOpcodes[] = {
        {"mov", 0, 0},
        {"cmp", 1, 0},
        {"add", 2, 1},
        {"sub", 2, 1},
        {"lea", 4, 0},
        {"clr", 5, 1},
        {"not", 5, 2},
        {"inc", 5, 3},
        {"dec", 5, 4},
        {"jmp", 9, 1},
        {"bne", 9, 2},
        {"jsr", 9, 3},
        {"red", 12, 0},
        {"prn", 13, 0},
        {"rts", 14, 0},
        {"stop", 15, 0},
        {NULL, -1, -1}
    };

    int i = 0;
    int opcode = -1;
    int funct = -1;
    int final;
    int found = 0;

    /* Find matching command */
    while (commandsOpcodes[i].name != NULL && !found) {
        if (strcmp(commandsOpcodes[i].name, l->cmd->name) == 0) {
            opcode = commandsOpcodes[i].opcode;
            funct = commandsOpcodes[i].funct;
            found = 1;
        }
        i++;
    }

    /* Shift bits to proper positions */
    opcode <<= 18;
    funct <<= 3;
    final = opcode | funct;

    return final;
}

int getAddressingCode(const line *l, int *invisibleLineNum) {
    int addressingCode1 = 0;
    int addressingCode2 = 0;
    int final;

    if (l->cmd->parm1Addressing0 != NULL) {
        addressingCode1 = 0;
        (*invisibleLineNum)++;
    }
    else if (l->cmd->parm1Addressing1 != NULL) {
        addressingCode1 = 1;
        (*invisibleLineNum)++;
    }
    else if (l->cmd->parm1Addressing2 != NULL) {
        addressingCode1 = 2;
        (*invisibleLineNum)++;
    }
    else if (l->cmd->parm1Addressing3 != NULL) {
        addressingCode1 = 3;
    }

    addressingCode1 <<= 16;

    if (l->cmd->parm2Addressing0 != NULL) {
        addressingCode2 = 0;
        (*invisibleLineNum)++;
    }
    else if (l->cmd->parm2Addressing1 != NULL) {
        addressingCode2 = 1;
        (*invisibleLineNum)++;
    }
    else if (l->cmd->parm2Addressing2 != NULL) {
        addressingCode2 = 2;
        (*invisibleLineNum)++;
    }
    else if (l->cmd->parm2Addressing3 != NULL) {
        addressingCode2 = 3;
    }

    addressingCode2 <<= 11;
    final = addressingCode1 | addressingCode2;
    return final;
}

int getRegistersCode(const line *l) {
    int registerCode1 = 0;
    int registerCode2 = 0;
    int final;

    if (l->cmd->parm1Addressing3 != NULL) {
        registerCode1 = atoi(l->cmd->parm1Addressing3);
    }
    if (l->cmd->parm2Addressing3 != NULL) {
        registerCode2 = atoi(l->cmd->parm2Addressing3);
    }

    registerCode1 <<= 13;
    registerCode2 <<= 8;
    final = registerCode1 | registerCode2;
    return final;
}

int addLineToList(binaryConvertedLines *list, char *data) {
    binaryConvertedLines *newNode;
    binaryConvertedLines *temp;

    if (list == NULL || data == NULL) {
        return 1;
    }

    newNode = (binaryConvertedLines *)malloc(sizeof(binaryConvertedLines));
    if (newNode == NULL) {
        printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
        return 1;
    }

    newNode->data = data;
    newNode->next = NULL;

    temp = list;
    if (temp->data == NULL) {
        *list = *newNode;
        return 0;
    }

    while (temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
    return 0;
}