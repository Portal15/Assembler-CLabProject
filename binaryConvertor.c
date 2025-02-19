#include "binaryConvertor.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define A 4 /* 100 */
#define R 2 /* 010 */
#define E 1 /* 001 */


int isCommand(line *l);

int getBineryCodeFromNumber(int *number);

int getOpcodeAndFunctCode(line *l);

int getAddressingCode(line *l);

int getRegistersCode(line *l);

/* */

int isCommand(line *l){
    return l->cmd != NULL;
}


int getOpcodeAndFunctCode(line *l){
    struct commandsOpcode{
        char *name;
        int opcode;
        int funct;
    };
    struct commandsOpcode commandsOpcodes[] = {
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
    int opcode = -1, funct = -1;
    int final;
    while(commandsOpcodes[i].name != NULL){
        if(strcmp(commandsOpcodes[i].name, l->cmd->name) == 0){
            opcode = commandsOpcodes[i].opcode;
            funct = commandsOpcodes[i].funct;
        }
        i++;
    }
    opcode <<= 18;
    funct <<= 3;
    final = opcode | funct;
    return final;
}

int getAddressingCode(line *l){
    int final;
    int addressingCode1 = 0, addressingCode2 = 0;
    if(l->cmd->parm1Addressing0 != NULL){
        addressingCode1 = 0;
    }else if(l->cmd->parm1Addressing1 != NULL){
        addressingCode1 = 1;
    }else if(l->cmd->parm1Addressing2 != NULL){
        addressingCode1 = 2;
    }else if(l->cmd->parm1Addressing3 != NULL){
        addressingCode1 = 3;
    }
    addressingCode1 <<= 16;
    if(l->cmd->parm2Addressing0 != NULL){
        addressingCode2 = 0;
    }else if(l->cmd->parm2Addressing1 != NULL){
        addressingCode2 = 1;
    }else if(l->cmd->parm2Addressing2 != NULL){
        addressingCode2 = 2;
    }else if(l->cmd->parm2Addressing3 != NULL){
        addressingCode2 = 3;
    }
    addressingCode2 <<= 11;
    final = addressingCode1 | addressingCode2;
    return final;
}

int getRegistersCode(line *l){
    int final;
    int registerCode1 = 0, registerCode2 = 0;
    if(l->cmd->parm1Addressing3 != NULL){
        registerCode1 = atoi(l->cmd->parm1Addressing3);
    }
    if(l->cmd->parm2Addressing3 != NULL){
        registerCode2 = atoi(l->cmd->parm2Addressing3);
    }
    registerCode1 <<= 13;
    registerCode2 <<= 8;
    final = registerCode1 | registerCode2;
    return final;

}

int getBinaryCodeFromNumber(int *number){
    int binaryCode = 0;
    int i = 0;
    while(*number != 0){
        binaryCode += (*number % 2) * (10^i);
        *number = *number / 2;
        i++;
    }
    return binaryCode;
}

int binaryConvertor(line *l, int invisibleLineNum, binaryConvertedLines *list){
    int binaryCode1 = 0;
    if(isCommand(l)){
        binaryCode1 = getOpcodeAndFunctCode(l);
        binaryCode1 |= getAddressingCode(l);
        binaryCode1 |= getRegistersCode(l);
        binaryCode1 |= A;
    }
    return binaryCode1;
}
