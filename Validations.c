#include "lineAnalyzer.h"
#include "utils.h"
#include "errorHandler.h"
#include "dataTypes.h"

int isNumber(const char *str);
int isLetter(const char *str);
int isRegister(const char *str);
int checkLineAnalyzerErrors(line *l);

int checkLineValidation(line *l, char *forbiddenWords){
    if (checkLineAnalyzerErrors(l)){ /* The error was already act in the other file */
        return 0;
    }

    if(l->label != NULL && isForbiddenWord(l->label, forbiddenWords)){
        printErrorMsg(INVALID_LABEL_NAME_ERROR, l->lineNum);
        return 0;
    }

    /* TODO: FIX THIS IF */
    if (l->cmd != NULL) {
        // if(l->cmd->name != NULL && isForbiddenWord(l->cmd->name, forbiddenWords)){
        //     printErrorMsg(INVALID_COMMAND_NAME_ERROR, l->lineNum);
        //     return 0;
        // }

        if(l->cmd->parm1Addressing0 != NULL && !isNumber(l->cmd->parm1Addressing0)){
            printErrorMsg(INVALID_PARAMETER_NOT_A_NUMBER_ERROR, l->lineNum);
            return 0;
        }

        if(l->cmd->parm1Addressing3 != NULL && !isRegister(l->cmd->parm1Addressing3)){
            printErrorMsg(INVALID_PARAMETER_NOT_A_REGISTER_ERROR, l->lineNum);
            return 0;
        }
        if(l->cmd->parm2Addressing0 != NULL && !isNumber(l->cmd->parm2Addressing0)){
            printErrorMsg(INVALID_PARAMETER_NOT_A_NUMBER_ERROR, l->lineNum);
            return 0;
        }

        if(l->cmd->parm2Addressing3 != NULL && !isRegister(l->cmd->parm2Addressing3)){
            printErrorMsg(INVALID_PARAMETER_NOT_A_REGISTER_ERROR, l->lineNum);
            return 0;
        }
    }

    if (l->vars != NULL) {
        var temp = *l->vars;
        if (l->vars->varType == 0) {
            while (temp.next != NULL) {
                if (temp.digit != NULL && !isNumber(temp.digit)) {
                    printErrorMsg(INVALID_PARAMETER_NOT_A_NUMBER_ERROR, l->lineNum);
                    return 0;
                }
                temp = *temp.next;
            }
            if (temp.digit != NULL && !isNumber(temp.digit)) {
                printErrorMsg(INVALID_PARAMETER_NOT_A_NUMBER_ERROR, l->lineNum);
                return 0;
            }
        }
        if (l->vars->varType == 1) {
            while (temp.next != NULL) {
                if (temp.digit != NULL && !isLetter(temp.digit)) {
                    printErrorMsg(INVALID_PARAMETER_NOT_A_NUMBER_ERROR, l->lineNum);
                    return 0;
                }
                temp = *temp.next;
            }
            if (temp.digit != NULL && !isNumber(temp.digit)) {
                printErrorMsg(INVALID_PARAMETER_NOT_A_NUMBER_ERROR, l->lineNum);
                return 0;
            }
        }
    }

    return 1;

}

int checkLabelErrors(objectTable ot, line *l) {
    if (l->label != NULL) {
        objectTableCommands *temp = ot.commands;
        if (temp != NULL && strcmp(temp->name, l->label) == 0) {
            printErrorMsg(LABEL_NAME_APPEAR_TWICE_ERROR, l->lineNum);
            return 0;
        }
        while(temp != NULL && temp->next != NULL) {
            temp = temp->next;
            if (strcmp(temp->name, l->label) == 0) {
                printErrorMsg(LABEL_NAME_APPEAR_TWICE_ERROR, l->lineNum);
                return 0;
            }
        }
        /* Second Time */
        objectTableData *temp1 = ot.data;
        if (temp1 != NULL && strcmp(temp1->name, l->label) == 0) {
            printErrorMsg(LABEL_NAME_APPEAR_TWICE_ERROR, l->lineNum);
            return 0;
        }
        while(temp1 != NULL && temp1->next != NULL) {
            temp1 = temp1->next;
            if (strcmp(temp1->name, l->label) == 0) {
                printErrorMsg(LABEL_NAME_APPEAR_TWICE_ERROR, l->lineNum);
                return 0;
            }
        }
    }
    return 1;
}

int checkObjectTableErrors(objectTable ot) {
    if (ot.error[0] == MEMORY_ALLOCATION_ERROR) {
        printErrorMsg(MEMORY_ALLOCATION_ERROR, ot.error[1]);
        return 0;
    }
    return 1;
}

int checkLineAnalyzerErrors(line *l) {
    int error;
    for (error = ILLEGAL_COMMA_ERROR; error <= INVALID_PARAMETER_NOT_A_STRING_ERROR; error++) {
        if (l->error == error) {
            printErrorMsg(error, l->lineNum);
            return 1;
        }
    }
    if (l->error == MEMORY_ALLOCATION_ERROR) {
        printErrorMsg(MEMORY_ALLOCATION_ERROR, l->lineNum);
    }
    return 0;
}

int isNumber(const char *str){
    int dots = 0;
    int flag = 1;

    if (*str == '#') {
        str++;
    }

    if (*str == '-' || *str == '+') {
        str++;
    }

    if (*str == '\n') return 0;

    while (*str != '\0' && *str != '\n' && flag != 0) {
        if (*str == '.' && dots >= 1) {
            flag = 0;
        } else if (*str == '.') {
            dots++;
        } else if (!isdigit(*str)) {
            flag = 0;
        }
        str++;
    }
    return flag;
}

int isLetter(const char *str) {
    return isalpha(*str) && str[1] == '\0';
}

int isRegister(const char *str) {
    return str[0] >= '0' && str[0] <= '7' && str[1] == '\0';
}