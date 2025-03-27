#include "headerFiles/inputAnalyzer.h"
#include "headerFiles/errorHandler.h"

/* Function prototypes */

/* This function removes leading spaces from a string until a non-space character is found.
 * line - the string to be processed
 * return - a pointer to the first non-space character in the string
 */
char *removeSpacesUntilChar(char *line);

/* This function gets a string and returns a pointer until the first non-letter character in the string.
 * str - the string to be processed
 * return - a pointer until the first non-letter character in the string
 */
char *getStringUntilNonLetterChar(const char *str);

/* This function removes leading spaces from a string until a non-letter character is found.
 * str - the string to be processed
 * return - a pointer to the first non-letter character in the string
 */
char *removeUntilNonLetter(char *str);

/* This function checks if a string is empty.
 * str - the string to be checked
 * return - 1 if the string is empty, 0 otherwise
 */
int isEndOfString(const char *str);

/* This function adds a digit to a var struct.
 * v - the var struct to be updated
 * digit - the digit to be added
 */
void addDigitToVar(var *v, char* digit);

/* This function adds a letter to a var struct.
 * v - the var struct to be updated
 * letter - the letter to be added
 */
void addLetterToVar(var *v, char letter);

/* This function gets a string and returns a pointer until the first non-quotation mark character in the string.
 * str - the string to be processed
 * return - a pointer until the first non-quotation mark character in the string
 */
char *getStringUntilNonQuotationMarks(const char *str);

line analyzeInput(char *inputedLine, int lineNum) {
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
        newLine.error = MEMORY_ALLOCATION_ERROR;
        return newLine;
    }
    /* Initialize command structure */
    memset(cmd, 0, sizeof(command));
    cmd->isFilled[0] = -1;
    cmd->isFilled[1] = -1;
    cmd->isFilled[2] = -1;

    /* Step 1: Remove leading spaces */
    inputedLine = removeSpacesUntilChar(inputedLine);

    /* Step 2: Check if line is empty */
    if (isEndOfString(inputedLine)) {
        freeCommand(cmd);
        return newLine;
    }

    /* Step 3: Extract first parameter */
    currentParm = getStringUntilNonLetterChar(inputedLine);
    if (currentParm == NULL) {
        newLine.error = MEMORY_ALLOCATION_ERROR;
        freeCommand(cmd);
        return newLine;
    }
    inputedLine = removeUntilNonLetter(inputedLine);

    /* Step 4: Check for label */
    if (currentParm[strlen(currentParm) - 1] == ':') {
        currentParm[strlen(currentParm) - 1] = '\0'; /* Step 4.1: Remove ':' */
        label = strdup(currentParm); /* Step 4.2: Set label */

        if (label == NULL) {
            free(currentParm);
            freeCommand(cmd);
            newLine.error = MEMORY_ALLOCATION_ERROR;
            return newLine;
        }
        free(currentParm);

        inputedLine = removeSpacesUntilChar(inputedLine); /* Step 4.3: Remove spaces after label */
        if (isEndOfString(inputedLine)) { /* Step 4.4: Check if line is empty */
            newLine.label = label;
            freeCommand(cmd);
            newLine.error = EMPTY_LABEL_ERROR;
            return newLine;
        }

        currentParm = getStringUntilNonLetterChar(inputedLine); /* Step 4.5: Extract command name */
        if (currentParm == NULL) {
            newLine.error = MEMORY_ALLOCATION_ERROR;
            free(label);
            freeCommand(cmd);
            return newLine;
        }
        inputedLine = removeUntilNonLetter(inputedLine);
    }
    newLine.label = label;

    /* Step 5: Skip assembler directives */
    if (currentParm[0] == '.') {
        inputedLine = removeSpacesUntilChar(inputedLine);
        if (isEndOfString(inputedLine)) {
            free(currentParm);
            freeCommand(cmd);
            return newLine;
        }
        if (strcmp(currentParm, ".extern") == 0) {
            vars = (var *)malloc(sizeof(var));
            if (vars == NULL) {
                newLine.error = MEMORY_ALLOCATION_ERROR;
                free(currentParm);
                freeCommand(cmd);
                free(label);
                return newLine;
            }
            memset(vars, 0, sizeof(var));

            free(currentParm);
            currentParm = getStringUntilNonLetterChar(inputedLine);
            if (currentParm == NULL) {
                newLine.error = MEMORY_ALLOCATION_ERROR;
                free(label);
                freeCommand(cmd);
                freeVar(vars);
                return newLine;
            }
            inputedLine = removeUntilNonLetter(inputedLine);
            inputedLine = removeSpacesUntilChar(inputedLine);
            if (!isEndOfString(inputedLine)) {
                newLine.error = UNEXPECTED_TEXT_AFTER_END_OF_LINE_ERROR;
                free(currentParm);
                free(label);
                freeCommand(cmd);
                freeVar(vars);
                return newLine;
            }
            vars->label = strdup(currentParm);
            if (vars->label == NULL) {
                newLine.error = MEMORY_ALLOCATION_ERROR;
                free(currentParm);
                free(label);
                freeCommand(cmd);
                freeVar(vars);
                return newLine;
            }
            vars->varType = 2;
            newLine.vars = vars;
            free(currentParm);
            freeCommand(cmd);
            return newLine;
        }
        if (strcmp(currentParm, ".entry") == 0) {
            vars = (var *)malloc(sizeof(var));
            if (vars == NULL) {
                newLine.error = MEMORY_ALLOCATION_ERROR;
                free(currentParm);
                freeCommand(cmd);
                freeVar(vars);
                free(label);
                return newLine;
            }
            memset(vars, 0, sizeof(var));

            free(currentParm);
            currentParm = getStringUntilNonLetterChar(inputedLine);
            if (currentParm == NULL) {
                newLine.error = MEMORY_ALLOCATION_ERROR;
                freeCommand(cmd);
                free(label);
                freeVar(vars);
                return newLine;
            }
            inputedLine = removeUntilNonLetter(inputedLine);
            inputedLine = removeSpacesUntilChar(inputedLine);
            if (!isEndOfString(inputedLine)) {
                newLine.error = UNEXPECTED_TEXT_AFTER_END_OF_LINE_ERROR;
                free(currentParm);
                free(label);
                freeCommand(cmd);
                freeVar(vars);
                return newLine;
            }
            vars->label = strdup(currentParm);
            if (vars->label == NULL) {
                newLine.error = MEMORY_ALLOCATION_ERROR;
                free(currentParm);
                free(label);
                freeCommand(cmd);
                freeVar(vars);
                return newLine;
            }
            vars->varType = 3;
            newLine.vars = vars;
            free(currentParm);
            freeCommand(cmd);
            return newLine;
        }
        if (strcmp(currentParm, ".data") == 0) {
            vars = (var *)malloc(sizeof(var));
            if (vars == NULL) {
                newLine.error = MEMORY_ALLOCATION_ERROR;
                free(currentParm);
                freeCommand(cmd);
                free(label);
                freeVar(vars);
                return newLine;
            }
            memset(vars, 0, sizeof(var));

            free(currentParm);
            currentParm = getStringUntilNonLetterChar(inputedLine);
            if (currentParm == NULL) {
                newLine.error = MEMORY_ALLOCATION_ERROR;
                free(label);
                freeCommand(cmd);
                freeVar(vars);
                return newLine;
            }
            inputedLine = removeUntilNonLetter(inputedLine);

            vars->varType = 0;
            vars->digit = strdup(currentParm);
            if (vars->digit == NULL) {
                newLine.error = MEMORY_ALLOCATION_ERROR;
                free(currentParm);
                free(label);
                freeCommand(cmd);
                freeVar(vars);
                return newLine;
            }
            free(currentParm);
            inputedLine = removeSpacesUntilChar(inputedLine);

            while (!isEndOfString(inputedLine)) {
                if (inputedLine[0] != ',') {
                    newLine.error = MISSING_COMMA_ERROR;
                    free(label);
                    freeCommand(cmd);
                    freeVar(vars);
                    return newLine;
                }
                inputedLine++;
                inputedLine = removeSpacesUntilChar(inputedLine);
                if (inputedLine[0] == ',') {
                    newLine.error = MULTIPLE_COMMA_ERROR;
                    free(label);
                    freeCommand(cmd);
                    freeVar(vars);
                    return newLine;
                }
                if (isEndOfString(inputedLine)) {
                    newLine.error = UNEXPECTED_TEXT_AFTER_END_OF_LINE_ERROR;
                    free(label);
                    freeCommand(cmd);
                    freeVar(vars);
                    return newLine;
                }
                currentParm = getStringUntilNonLetterChar(inputedLine);
                if (currentParm == NULL) {
                    newLine.error = MEMORY_ALLOCATION_ERROR;
                    free(label);
                    freeCommand(cmd);
                    freeVar(vars);
                    return newLine;
                }
                inputedLine = removeUntilNonLetter(inputedLine);

                addDigitToVar(vars, currentParm);
                inputedLine = removeSpacesUntilChar(inputedLine);
            }

            newLine.vars = vars;
            freeCommand(cmd);
            return newLine;
        }
        if (strcmp(currentParm, ".string") == 0) {
            vars = (var *)malloc(sizeof(var));
            if (vars == NULL) {
                newLine.error = MEMORY_ALLOCATION_ERROR;
                free(currentParm);
                freeCommand(cmd);
                free(label);
                freeVar(vars);
                return newLine;
            }
            memset(vars, 0, sizeof(var));
            vars->varType = 1;

            free(currentParm);
            currentParm = getStringUntilNonQuotationMarks(inputedLine);
            if (currentParm == NULL) {
                newLine.error = MEMORY_ALLOCATION_ERROR;
                freeCommand(cmd);
                free(label);
                freeVar(vars);
                return newLine;
            }
            inputedLine += strlen(currentParm);

            if (currentParm[0] != '"') {
                newLine.error = INVALID_PARAMETER_NOT_A_STRING_ERROR;
                free(currentParm);
                freeCommand(cmd);
                free(label);
                freeVar(vars);
                return newLine;
            }

            vars->letter = currentParm[1]; /* First character after opening quote */
            char *strPtr = currentParm + 2; /* Start from second character */

            while (*strPtr != '"' && *strPtr != '\0') {
                addLetterToVar(vars, *strPtr);
                strPtr++;
            }

            if (*strPtr != '"') {
                newLine.error = INVALID_PARAMETER_NOT_A_STRING_ERROR;
                free(currentParm);
                freeCommand(cmd);
                free(label);
                freeVar(vars);
                return newLine;
            }

            addLetterToVar(vars, '\0');
            free(currentParm);

            inputedLine = removeSpacesUntilChar(inputedLine);
            if (!isEndOfString(inputedLine)) {
                newLine.error = UNEXPECTED_TEXT_AFTER_END_OF_LINE_ERROR;
                freeCommand(cmd);
                free(label);
                freeVar(vars);
                return newLine;
            }
            newLine.vars = vars;
            freeCommand(cmd);
            return newLine;
        }
        free(currentParm);
        freeCommand(cmd);
        free(label);
        freeVar(vars);
        newLine.error = INVALID_COMMAND_NAME_ERROR;
        return newLine;
    }

    /* Step 6: Set command name */
    cmd->name = strdup(currentParm);
    if (cmd->name == NULL) {
        newLine.error = MEMORY_ALLOCATION_ERROR;
        free(currentParm);
        free(label);
        freeCommand(cmd);
        freeVar(vars);
        return newLine;
    }
    free(currentParm);
    currentParm = NULL;

    cmd->isFilled[0] = 1;

    /* Step 7: Remove landing spaces */
    inputedLine = removeSpacesUntilChar(inputedLine);

    if (isEndOfString(inputedLine)) { /* Step 8:  Check if line is empty */
        newLine.label = label;
        newLine.cmd = cmd;
        return newLine;
    }

    if (inputedLine[0] == ',') { /* Step 9: Check for illegal comma */
        newLine.error = ILLEGAL_COMMA_ERROR;
        free(label);
        freeCommand(cmd);
        freeVar(vars);
        return newLine;
    }

    currentParm = getStringUntilNonLetterChar(inputedLine); /* Step 10: Extract first parameter */

    if (currentParm == NULL) {
        newLine.error = MEMORY_ALLOCATION_ERROR;
        free(label);
        freeCommand(cmd);
        freeVar(vars);
        return newLine;
    }
    inputedLine = removeUntilNonLetter(inputedLine);

    /* Step 11: Process first parameter addressing */
    if (currentParm[0] == '#') {
        cmd->parm1Addressing0 = strdup(currentParm + 1);
        if (cmd->parm1Addressing0 == NULL) {
            newLine.error = MEMORY_ALLOCATION_ERROR;
            free(currentParm);
            free(label);
            freeCommand(cmd);
            freeVar(vars);
            return newLine;
        }
        cmd->isFilled[1] = 0;
    } else if (currentParm[0] == '&') {
        cmd->parm1Addressing2 = strdup(currentParm + 1);
        if (cmd->parm1Addressing2 == NULL) {
            newLine.error = MEMORY_ALLOCATION_ERROR;
            free(currentParm);
            free(label);
            freeCommand(cmd);
            freeVar(vars);
            return newLine;
        }
        cmd->isFilled[1] = 2;
    } else if (currentParm[0] == 'r') {
        cmd->parm1Addressing3 = strdup(currentParm + 1);
        if (cmd->parm1Addressing3 == NULL) {
            newLine.error = MEMORY_ALLOCATION_ERROR;
            free(currentParm);
            free(label);
            freeCommand(cmd);
            freeVar(vars);
            return newLine;
        }
        cmd->isFilled[1] = 3;
    } else {
        cmd->parm1Addressing1 = strdup(currentParm);
        if (cmd->parm1Addressing1 == NULL) {
            newLine.error = MEMORY_ALLOCATION_ERROR;
            free(currentParm);
            free(label);
            freeCommand(cmd);
            freeVar(vars);
            return newLine;
        }
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
        newLine.error = MISSING_COMMA_ERROR;
        free(label);
        freeCommand(cmd);
        freeVar(vars);
        return newLine;
    }
    inputedLine++;

    /* Step 15: Remove landing spaces */
    inputedLine = removeSpacesUntilChar(inputedLine);

    /* Step 16: Check if line is empty */
    if (isEndOfString(inputedLine)) {
        newLine.error = UNEXPECTED_TEXT_AFTER_END_OF_LINE_ERROR;
        free(label);
        freeCommand(cmd);
        freeVar(vars);
        return newLine;
    }

    /* Step 17: Check for multiple comma */
    if (inputedLine[0] == ',') {
        newLine.error = MULTIPLE_COMMA_ERROR;
        free(label);
        freeCommand(cmd);
        freeVar(vars);
        return newLine;
    }

    /* Step 18: Extract second parameter */
    currentParm = getStringUntilNonLetterChar(inputedLine);
    if (currentParm == NULL) {
        newLine.error = MEMORY_ALLOCATION_ERROR;
        free(label);
        freeCommand(cmd);
        freeVar(vars);
        return newLine;
    }
    inputedLine = removeUntilNonLetter(inputedLine);

    /* Step 19: Process second parameter addressing */
    if (currentParm[0] == '#') {
        cmd->parm2Addressing0 = strdup(currentParm + 1);
        if (cmd->parm2Addressing0 == NULL) {
            newLine.error = MEMORY_ALLOCATION_ERROR;
            free(currentParm);
            free(label);
            freeCommand(cmd);
            freeVar(vars);
            return newLine;
        }
        cmd->isFilled[2] = 0;
    } else if (currentParm[0] == '&') {
        cmd->parm2Addressing2 = strdup(currentParm + 1);
        if (cmd->parm2Addressing2 == NULL) {
            newLine.error = MEMORY_ALLOCATION_ERROR;
            free(currentParm);
            free(label);
            freeCommand(cmd);
            freeVar(vars);
            return newLine;
        }
        cmd->isFilled[2] = 2;
    } else if (currentParm[0] == 'r') {
        cmd->parm2Addressing3 = strdup(currentParm + 1);
        if (cmd->parm2Addressing3 == NULL) {
            newLine.error = MEMORY_ALLOCATION_ERROR;
            free(currentParm);
            free(label);
            freeCommand(cmd);
            freeVar(vars);
            return newLine;
        }
        cmd->isFilled[2] = 3;
    } else {
        cmd->parm2Addressing1 = strdup(currentParm);
        if (cmd->parm2Addressing1 == NULL) {
            newLine.error = MEMORY_ALLOCATION_ERROR;
            free(currentParm);
            free(label);
            freeCommand(cmd);
            freeVar(vars);
            return newLine;
        }
        cmd->isFilled[2] = 1;
    }

    free(currentParm);
    currentParm = NULL;

    /* Step 20: Remove landing spaces */
    inputedLine = removeSpacesUntilChar(inputedLine);

    /* Step 21: Check if line is empty */
    if (!isEndOfString(inputedLine)) {
        newLine.error = UNEXPECTED_TEXT_AFTER_END_OF_LINE_ERROR;
    }

    newLine.label = label;
    newLine.cmd = cmd;
    newLine.vars = vars;
    return newLine;
}

char *removeSpacesUntilChar(char *line) {
    while (isspace((unsigned char)*line)) {
        line++;
    }
    return line;
}

char *getStringUntilNonLetterChar(const char *str) {
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

char *getStringUntilNonQuotationMarks(const char *str) {
    char *string = malloc(sizeof(char) * MAX_SIZE_OF_STRING);
    char *start;

    if (string == NULL) {
        printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
        return NULL;
    }

    /* Skip initial whitespace to find the quote */
    while (isspace((unsigned char)*str)) {
        str++;
    }

    if (*str != '"') {
        *string = '"'; /* Add opening quote for error handling */
        string[1] = '\0';
        return string;
    }

    *string = '"';
    start = string + 1;
    str++; /* Move past the opening quote */

    while (*str != '\0' && *str != '"' && *str != '\n') {
        *start = *str;
        start++;
        str++;
    }

    *start = '"';
    start++;
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
    if (cmd != NULL) {
        if (cmd->name != NULL) free(cmd->name);
        if (cmd->parm1Addressing0 != NULL) free(cmd->parm1Addressing0);
        if (cmd->parm1Addressing1 != NULL) free(cmd->parm1Addressing1);
        if (cmd->parm1Addressing2 != NULL) free(cmd->parm1Addressing2);
        if (cmd->parm1Addressing3 != NULL) free(cmd->parm1Addressing3);
        if (cmd->parm2Addressing0 != NULL) free(cmd->parm2Addressing0);
        if (cmd->parm2Addressing1 != NULL) free(cmd->parm2Addressing1);
        if (cmd->parm2Addressing2 != NULL) free(cmd->parm2Addressing2);
        if (cmd->parm2Addressing3 != NULL) free(cmd->parm2Addressing3);
        free(cmd);
        cmd = NULL;
    }
}

void freeVar(var *v) {
    var *temp;
    while (v != NULL) {
        temp = v;
        v = v->next;
        if (temp->digit != NULL) free(temp->digit);
        if (temp->label != NULL) free(temp->label);
        free(temp);
    }
}

void addDigitToVar(var *v, char* digit) {
    while (v->next != NULL) {
        v = v->next;
    }
    v->next = (var *)malloc(sizeof(var));
    if (v->next == NULL) {
        printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
        return;
    }
    memset(v->next, 0, sizeof(var));
    v->next->digit = digit;
    v->next->next = NULL;
}

void addLetterToVar(var *v, const char letter) {
    if (v == NULL) return;

    while (v->next != NULL) {
        v = v->next;
    }
    v->next = (var *)malloc(sizeof(var));
    if (v->next == NULL) {
        printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
        return;
    }
    memset(v->next, 0, sizeof(var));
    v->next->letter = letter;
    v->next->next = NULL;
}