#ifndef DATATYPES_H
#define DATATYPES_H

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
    int isFilled[3];
} command;

typedef struct var {
    int varType; /* 0 - Number, 1 - Letter , 2 - extern , 3 - entry*/
    char* label;
    char* digit;
    char letter;
    struct var *next;
} var;

typedef struct line{
    int lineNum;
    char* label;
    command *cmd;
    var *vars;
    int error;
} line;

/* For object table */
typedef struct objectTableCommands {
    char *name;
    int line;
    int address;
    struct objectTableCommands *next;
} objectTableCommands;

typedef struct objectTableData {
    char *name;
    var value;
    int line;
    int address;
    struct objectTableData *next;
} objectTableData;

typedef struct objectTableExtern {
    char *name;
    int line;
    int address;
    struct objectTableExtern *next;
} objectTableExtern;

typedef struct objectTableEntry {
    char *name;
    int line;
    int address;
    struct objectTableEntry *next;
} objectTableEntry;

typedef struct objectTable {
    objectTableCommands *commands;
    objectTableData *data;
    objectTableExtern *externs;
    objectTableEntry *entries;
    int error [2];
} objectTable;

typedef struct binaryConvertedLines{
    char *data;
    struct binaryConvertedLines *next;
} binaryConvertedLines;

<<<<<<< Updated upstream
<<<<<<< Updated upstream
typedef struct externsAppearance{
    char *name;
    struct externsAppearance *next;
} externsAppearance;

#endif
=======
=======
>>>>>>> Stashed changes
#endif //DATATYPES_H
>>>>>>> Stashed changes
