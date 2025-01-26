#include "lineAnalyzer.h"
#include "utils.h"
#include "errorHandler.h"

int isNumber(const char *str);
int isRegister(const char *str);

int checkValidation(line *l, char *forbiddenWords, char* vars){
    if (l->cmd->error != 0){ /* The error was already act in the other file */
        return 0;
    }

    if(l->label != NULL && isForbiddenWord(l->label, forbiddenWords)){
        printErrorMsg(INVALID_LABEL_NAME_ERROR, l->lineNum);
        return 0;
    }

    /* TODO: FIX THIS IF */
    if(isForbiddenWord(l->cmd->name, forbiddenWords)){
        printErrorMsg(INVALID_COMMAND_NAME_ERROR, l->lineNum);
        return 0;
    }

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

    return 1;

}

int isNumber(const char *str){
    int dots = 0;
    int flag = 1;

    if (*str == '#') {
        str++;
    }

    if (*str == '-') {
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

int isRegister(const char *str) {
    return str[0] >= '0' && str[0] <= '7' && str[1] == '\0';
}