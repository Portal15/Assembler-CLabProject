#ifndef LINE_ANALYZER_H
#define LINE_ANALYZER_H

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_SIZE_OF_STRING 100


typedef struct command {
    char *name;
    /* for parm1 */
    char* parm1Addressing0; /* Number Value */
    char* parm1Addressing1; /* Variable Name */
    char* parm1Addressing2; /* Variable Name */
    char* parm1Addressing3; /* Register Number */
    /* for parm2 */
    char* parm2Addressing0; /* Number Value */
    char* parm2Addressing1; /* Variable Name */
    char* parm2Addressing2; /* Variable Name */
    char* parm2Addressing3; /* Register Number */
    /* for error handling */
    int error;
    int isFilled[3];
} command;

typedef struct var {
    int digit;
    char* letter;
    struct var *next;
} var;

typedef struct line{
    int lineNum;
    char* label;
    command *cmd;
    var *vars;
} line;

line analyzeLine(char *inputedLine, int lineNum);

#endif // LINE_ANALYZER_H