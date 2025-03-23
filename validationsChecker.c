#include "headerFiles/inputAnalyzer.h"
#include "headerFiles/utils.h"
#include "headerFiles/errorHandler.h"
#include "headerFiles/dataTypes.h"

/* This function checks if a string is a number.
 * str - the string to be checked
 * return - 1 if the string is a number, 0 otherwise
 */
int isNumber(const char *str);

/* This function checks if a string is a letter and \0.
 * str - the string to be checked
 * return - 1 if the string is a letter, 0 otherwise
 */
int isLetter(const char *str);

/* This function checks if a string is a register.
 * str - the string to be checked
 * return - 1 if the string is a register, 0 otherwise
 */
int isRegister(const char *str);

/* This function checks if there are any errors that found in the inputAnalyzer but not reported.
 * l - the line to be checked
 * return - 1 if there is an error, 0 otherwise
 */
int checkInputAnalyzerErrors(const line *l);

/* This function checks if the command is valid (like the parameters type and the name).
 * cmd - the command to be checked
 * return - 1 if there is an error, 0 otherwise
 */
int inCommandCheck(const command *cmd);

/* This function checks if the parameter type is allowed.
 * allowedTypes - the allowed types
 * type - the type to be checked
 * return - 1 if the type is allowed, 0 otherwise
 */
int checkIsAllowed(const int allowedTypes[], int type);

int checkInputValidation(line *l, char *forbiddenWords){
    if (checkInputAnalyzerErrors(l)){ /* The error was already act in the other file */
        return 0;
    }

    if(l->label != NULL && (isForbiddenWord(l->label, forbiddenWords) || !isalpha(l->label[0]))){
        printErrorMsg(INVALID_LABEL_NAME_ERROR, l->lineNum);
        return 0;
    }

    if (l->cmd != NULL) {
        if (inCommandCheck(l->cmd) == 0) {
            printErrorMsg(INVALID_COMMAND_ERROR, l->lineNum);
            return 0;
        }

        if(l->cmd->parm1Addressing0 != NULL && !isNumber(l->cmd->parm1Addressing0)){
            printErrorMsg(INVALID_PARAMETER_NOT_A_NUMBER_ERROR, l->lineNum);
            return 0;
        }

        if (l->cmd->parm1Addressing1 != NULL && isForbiddenWord(l->cmd->parm1Addressing1, forbiddenWords)) {
            printErrorMsg(PARAMETER_NAME_IS_NOT_ALLOWED, l->lineNum);
            return 0;
        }

        if (l->cmd->parm1Addressing2 != NULL && isForbiddenWord(l->cmd->parm1Addressing2, forbiddenWords)) {
            printErrorMsg(PARAMETER_NAME_IS_NOT_ALLOWED, l->lineNum);
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

        if (l->cmd->parm2Addressing1 != NULL && isForbiddenWord(l->cmd->parm2Addressing1, forbiddenWords)) {
            printErrorMsg(PARAMETER_NAME_IS_NOT_ALLOWED, l->lineNum);
            return 0;

        }

        if (l->cmd->parm2Addressing2 != NULL && isForbiddenWord(l->cmd->parm2Addressing2, forbiddenWords)) {
            printErrorMsg(PARAMETER_NAME_IS_NOT_ALLOWED, l->lineNum);
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
        if (l->vars->varType == 2 || l->vars->varType == 3) {
            if (temp.label != NULL && isForbiddenWord(temp.label, forbiddenWords)) {
                printErrorMsg(PARAMETER_NAME_IS_NOT_ALLOWED, l->lineNum);
                return 0;
            }
        }
    }

    return 1;

}

int inCommandCheck(const command *cmd) {
    struct commands {
        const char *name;
        int parm1AllowedTypes[4];
        int parm2TypeAllowedTypes[4];
    };
    /* -1 stands for nothing, -2 stands for NULL */
    struct commands commands[] = {
        {"mov", {0,1,3,-2}, {1,3,-2,-2}},
        {"cmp", {0,1,3, -2}, {0,1,3,-2}},
        {"add", {0,1,3, -2}, {1,3,-2,-2}},
        {"sub", {0,1,3,-2}, {1,3,-2,-2}},
        {"lea", {1,-2,-2,-2}, {1,3,-2,-2}},
        {"clr", {1,3,-2,-2}, {-1,-2,-2,-2}},
        {"not", {1,3,-2,-2}, {-1,-2,-2,-2}},
        {"inc", {1,3,-2,-2}, {-1,-2,-2,-2}},
        {"dec", {1,3,-2,-2}, {-1,-2,-2,-2}},
        {"jmp", {1,2,-2,-2}, {-1,-2,-2,-2}},
        {"bne", {1,2,-2,-2}, {-1,-2,-2,-2}},
        {"jsr", {1,2,-2,-2}, {-1,-2,-2,-2}},
        {"red", {1,3,-2,-2}, {-1,-2,-2,-2}},
        {"prn", {0,1,3,-2}, {-1,-2,-2,-2}},
        {"rts", {-1,-2,-2,-2}, {-1,-2,-2,-2}},
        {"stop", {-1,-2,-2,-2}, {-1,-2,-2,-2}}

    };
    int parm1Type = cmd->isFilled[1];
    int parm2Type = cmd->isFilled[2];
    int i;
    for (i = 0; i < 16; i++) {
        if (strcmp(cmd->name, commands[i].name) == 0 && checkIsAllowed(commands[i].parm1AllowedTypes, parm1Type)) {
            if (checkIsAllowed(commands[i].parm2TypeAllowedTypes, parm2Type))
                return 1;
        }
    }
    return 0;
}

int checkIsAllowed(const int allowedTypes[], const int type) {
    int i = 0;
    while (i < 4 && allowedTypes[i] != -2) {
        if (allowedTypes[i] == type) {
            return 1;
        }
        i++;
    }
    return 0;
}

int checkLabelErrors(const objectTable ot, const line *l) {
    if (l->label != NULL) {
        const objectTableCommands *temp = ot.commands;
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
        const objectTableData *temp1 = ot.data;
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

int checkObjectTableErrors(const objectTable ot) {
    if (ot.error[0] == MEMORY_ALLOCATION_ERROR) {
        printErrorMsg(MEMORY_ALLOCATION_ERROR, ot.error[1]);
        return 0;
    }
    return 1;
}

int checkInputAnalyzerErrors(const line *l) {
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
    if (*str == '.') return 0;

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