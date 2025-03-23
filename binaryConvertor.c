#include "headerFiles/binaryConvertor.h"
#include "headerFiles/errorHandler.h"
#include "headerFiles/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Constants */
#define A 4 /* 100 */
#define R 2 /* 010 */
#define E 1 /* 001 */
#define MAX_NUM_LENGTH 20
#define MAX_LINE_LENGTH 256

/* This function checks if the line is a command.
 * l - the line to be checked
 * return - 1 if the line is a command, 0 otherwise
 */
int isCommand(const line *l);

/* This function gets the opcode and funct code of the command.
 * l - the line to be checked
 * return - the opcode and funct code as integer
 */
int getOpcodeAndFunctCode(const line *l);

/* This function gets the addressing code of the command.
 * l - the line to be checked
 * address - a pointer to the address
 * return - the addressing code as integer
 */
int getAddressingCode(const line *l, int *address);

/* This function gets the registers code of the command.
 * l - the line to be checked
 * return - the registers code as integer
 */
int getRegistersCode(const line *l);

/* This function adds a line to the binary list.
 * list - the list to add the line to
 * data - the data to be added
 * return - 1 if there is an error, 0 otherwise
 */
int addLineToList(binaryConvertedLines *list, char *data);

int binaryConvertor(const line *l, int *address, binaryConvertedLines *list) {
    int binaryCode = 0;
    char tempBuffer[MAX_LINE_LENGTH];
    char *str;


    if (isCommand(l)) {
        char *binaryStr;

        binaryCode = getOpcodeAndFunctCode(l);
        binaryCode |= getAddressingCode(l, address);
        binaryCode |= getRegistersCode(l);
        binaryCode |= A;

        binaryStr = (char *)malloc(MAX_NUM_LENGTH * sizeof(char));
        if (binaryStr == NULL) {
            printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
            return 1;
        }

        sprintf(binaryStr, "%d", binaryCode);
        if (addLineToList(list, binaryStr)) {
            printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
            free(binaryStr);
            return 1;
        }

        /* Handle parameter 1 */
        if (l->cmd->parm1Addressing0 != NULL) {
            char *numStr;
            binaryCode = atoi(l->cmd->parm1Addressing0);
            binaryCode = binaryCode << 3;
            binaryCode |= A;
            numStr = (char *)malloc(MAX_NUM_LENGTH * sizeof(char));
            if (numStr == NULL) {
                printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
                return 1;
            }

            sprintf(numStr, "%d", binaryCode);
            if (addLineToList(list, numStr)) {
                free(numStr);
                printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
                return 1;
            }
        }
        else if (l->cmd->parm1Addressing1 != NULL) {
            sprintf(tempBuffer, "!%s!%d", l->cmd->parm1Addressing1, l->lineNum);
            str = strdup(tempBuffer);
	    if (str == NULL) {
    		printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
    		return 1;
	    }

	    if (addLineToList(list, str)) {
    		free(str);
    		printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
    		return 1;
	    }
        }
        else if (l->cmd->parm1Addressing2 != NULL) {
            sprintf(tempBuffer, "!%s!%d!%d",
                l->cmd->parm1Addressing2,
                l->lineNum,
                *address);
            str = strdup(tempBuffer);
            if (str == NULL || addLineToList(list, str)) {
                free(str);
                printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
                return 1;
            }
        }

        /* Handle parameter 2 */
        if (l->cmd->parm2Addressing0 != NULL) {
            char *numStr;
            binaryCode = atoi(l->cmd->parm2Addressing0);
            binaryCode = binaryCode << 3;
            binaryCode |= A;

            numStr = (char *)malloc(MAX_NUM_LENGTH * sizeof(char));
            if (numStr == NULL) {
                printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
                return 1;
            }

            sprintf(numStr, "%d", binaryCode);
            if (addLineToList(list, numStr)) {
                free(numStr);
                printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
                return 1;
            }
        }
        else if (l->cmd->parm2Addressing1 != NULL) {
            sprintf(tempBuffer, "!%s!%d", l->cmd->parm2Addressing1, l->lineNum);
            str = strdup(tempBuffer);
            if (str == NULL || addLineToList(list, str)) {
                free(str);
                printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
                return 1;
            }
        }
        else if (l->cmd->parm2Addressing2 != NULL) {
            sprintf(tempBuffer, "!%s!%d!%d",
                l->cmd->parm2Addressing2,
                l->lineNum,
                *address);
            str = strdup(tempBuffer);
            if (str == NULL || addLineToList(list, str)) {
                free(str);
                printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
                return 1;
            }
        }
        (*address)++;
    }else {
        if (l->vars != NULL && l->vars->varType == 0) { /* for number */
            char *numStr;
            var *temp = l->vars;
            while (temp != NULL && temp->digit != NULL) {
                binaryCode = atoi(temp->digit);

                numStr = (char *)malloc(MAX_NUM_LENGTH * sizeof(char));
                if (numStr == NULL) {
                    printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
                    return 1;
                }

                sprintf(numStr, "%d", binaryCode);
                if (addLineToList(list, numStr)) {
                    free(numStr);
                    printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
                    return 1;
                }
                temp = temp->next;
                (*address)++;
            }
        }
        else if (l->vars != NULL && l->vars->varType == 1) { /* for letter */
            char *letterStr;

            var *temp = l->vars;
            while (temp != NULL && temp->letter != '\0') {
                letterStr = (char *)malloc(2 * sizeof(char));
                if (letterStr == NULL) {
                    printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
                    return 1;
                }

                sprintf(letterStr, "%c", temp->letter);
                if (addLineToList(list, letterStr)) {
                    free(letterStr);
                    printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
                    return 1;
                }
                temp = temp->next;
                (*address)++;
            }
            if (addLineToList(list, "\0")) {
                printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
                return 1;
            }
            (*address)++;
        }
    }
    return 0;
}

int isCommand(const line *l) {
    return l != NULL && l->cmd != NULL;
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
        {"sub", 2, 2},
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

int getAddressingCode(const line *l, int *address) {
    int addressingCode1 = 0;
    int addressingCode2 = 0;
    int final;

    if (l->cmd->parm1Addressing0 != NULL) {
        addressingCode1 = 0;
        (*address)++;
    }
    else if (l->cmd->parm1Addressing1 != NULL) {
        addressingCode1 = 1;
        (*address)++;
    }
    else if (l->cmd->parm1Addressing2 != NULL) {
        addressingCode1 = 2;
        (*address)++;
    }
    else if (l->cmd->parm1Addressing3 != NULL) {
        addressingCode1 = 3;
    }


    if (l->cmd->parm2Addressing0 != NULL) {
        addressingCode2 = 0;
        (*address)++;
    }
    else if (l->cmd->parm2Addressing1 != NULL) {
        addressingCode2 = 1;
        (*address)++;
    }
    else if (l->cmd->parm2Addressing2 != NULL) {
        addressingCode2 = 2;
        (*address)++;
    }
    else if (l->cmd->parm2Addressing3 != NULL) {
        addressingCode2 = 3;
    }

    if (l->cmd->parm2Addressing0 == NULL && l->cmd->parm2Addressing1 == NULL && l->cmd->parm2Addressing2 == NULL && l->cmd->parm2Addressing3 == NULL) {
        addressingCode1 <<= 11;
    }else {
        addressingCode1 <<= 16;
        addressingCode2 <<= 11;
    }

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
    if (l->cmd->parm2Addressing2 == NULL && l->cmd->parm2Addressing1 == NULL && l->cmd->parm2Addressing2 == NULL && l->cmd->parm2Addressing3 == NULL) {
        registerCode1 <<= 8;
    }else {
        registerCode2 <<= 8;
        registerCode1 <<= 13;
    }



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
        return 1;
    }

    newNode->data = data;
    newNode->next = NULL;

    temp = list;
    if (temp->data == 0) {
        list->data = newNode->data;
	    list->next = NULL;
        free(newNode);
        return 0;
    }
    while (temp != NULL && temp->next != NULL) {
        temp = temp->next;
    }
    temp->next = newNode;
    return 0;
}