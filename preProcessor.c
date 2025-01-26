#include "preProcessor.h"
#include "errorHandler.h"
#include "lineAnalyzer.h"
#include "utils.h"

/* This struct represents a mcro.
 * mcroName - the name of the mcro
 * mcroData - all the lines that are part of the mcro-v
 */
typedef struct mcro {
    char *mcroName;
    char *mcroData;
} mcro;

/* This struct represents a linked list of mcros.
 * mcro - a pointer to a mcro
 * next - a pointer to the next mcro in the list
 */
typedef struct linkedListOfMcros {
    mcro *mcro;
    struct linkedListOfMcros *next;
} linkedListOfMcros;

/* This function gets line and checks if it is the start of a macro.
 * If it is, it returns the name of the macro.
 * If it is not, it returns NULL.
 * line - the line to be checked
 * forbiddenWords - a string containing the forbidden words
 * createNewFile - a pointer to a variable that will be set to 0 if an error occurs
 * return - the name of the macro if it is the start of a macro, NULL otherwise
 */
char *isStartOfMcro(const char *line, char *forbiddenWords, int *createNewFile, int lineNum);

/* This function gets line and checks if it is the end of a macro.
 * If it is, it returns 1.
 * If it is not, it returns 0.
 * line - the line to be checked
 * createNewFile - a pointer to a variable that will be set to 0 if an error occurs
 * return - 1 if it is the end of a macro, 0 otherwise
 */
int isEndOfMcro(const char *line, int *createNewFile, int lineNum);

/* This function gets line and checks if it is a macro that already declared.
 * If it is, it returns the data of the macro.
 * If it is not, it returns NULL.
 * line - the line to be checked
 * mcros - a linked list of mcros
 * createNewFile - a pointer to a variable that will be set to 0 if an error occurs
 * return - the data of the macro if it is a macro, NULL otherwise
 */
char *isMcro(const char *line, linkedListOfMcros *mcros, int *createNewFile);

/* This function gets a linked list of mcros and returns a string containing the names of the mcros.
 * mcros - a linked list of mcros
 * return - a string containing the names of the mcros
 */
char *getMcrosName(linkedListOfMcros *mcros);

/* This function frees the memory allocated for the linked list of mcros.
 * mcros - a linked list of mcros
 */
void cleanupMcros(linkedListOfMcros *mcros);

char *preProcessor(const char *fileName, char *forbiddenWords) {
    linkedListOfMcros *mcros = NULL;
    char line[MAX_LINE_LENGTH];
    FILE *asFile;
    FILE *amFile;
    char *newFileName;
    char *mcroName;
    char *mcroData;
    char *dot;
    char *mcroNames;
    int createNewFile = 1;
    int lineNum = 1;


    asFile = fopen(fileName, "r+");
    if (asFile == NULL) {
        printErrorMsg(FAILED_TO_OPEN_FILE_ERROR, -1);
        return NULL;
    }
    newFileName = (char *)malloc(strlen(fileName) + 4);
    if (newFileName == NULL) {
        printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
        fclose(asFile);
        return NULL;
    }
    strcpy(newFileName, fileName);
    dot = strrchr(newFileName, '.');
    if (dot != NULL) {
        strcpy(dot, ".am");
    } else {
        strcat(newFileName, ".am");
    }

    amFile = fopen(newFileName, "w+");
    if (amFile == NULL) {
        printErrorMsg(FAILED_TO_CREATE_FILE_ERROR, -1);
        fclose(asFile);
        free(newFileName);
        return NULL;
    }
    while (fgets(line, MAX_LINE_LENGTH, asFile) != NULL) {
        mcroName = isStartOfMcro(line, forbiddenWords, &createNewFile, lineNum);
        if (mcroName != NULL) {
            int inMcro = 1;
            linkedListOfMcros *newMcro;

            mcroData = (char *)malloc(MAX_LINE_LENGTH);
            if (mcroData == NULL) {
                printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
                free(mcroName);
                cleanupMcros(mcros);
                fclose(amFile);
                remove(newFileName);
                free(newFileName);
                return NULL;
            }
            strcpy(mcroData, "");

            while (inMcro && fgets(line, MAX_LINE_LENGTH, asFile) != NULL) {
                lineNum++;
                if (isEndOfMcro(line, &createNewFile, lineNum)) {
                    inMcro = 0;
                } else {
                    char *newMcroData;
                    strcat(mcroData, line);
                    newMcroData = (char *)realloc(mcroData, strlen(mcroData) + MAX_LINE_LENGTH + 1);
                    if (newMcroData == NULL) {
                        printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
                        free(mcroData);
                        free(mcroName);
                        cleanupMcros(mcros);
                        fclose(amFile);
                        remove(newFileName);
                        free(newFileName);
                        return NULL;
                    }
                    mcroData = newMcroData;
                }
            }
            newMcro = (linkedListOfMcros *)malloc(sizeof(linkedListOfMcros));
            if (newMcro == NULL) {
                printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
                free(mcroData);
                free(mcroName);
                cleanupMcros(mcros);
                fclose(amFile);
                remove(newFileName);
                free(newFileName);
                return NULL;
;
            }
            newMcro->mcro = (mcro *)malloc(sizeof(mcro));
            if (newMcro->mcro == NULL) {
                printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
                free(mcroData);
                free(mcroName);
                free(newMcro);
                cleanupMcros(mcros);
                fclose(amFile);
                remove(newFileName);
                free(newFileName);
                return NULL;
;
            }
            if (mcroName[strlen(mcroName) - 1] == '\n') {
                mcroName[strlen(mcroName) - 1] = '\0';
            }
            newMcro->mcro->mcroName = mcroName;
            newMcro->mcro->mcroData = mcroData;
            newMcro->next = mcros;
            mcros = newMcro;
        } else {
            mcroData = isMcro(line, mcros, &createNewFile);
            if (mcroData != NULL) {
                fputs(mcroData, amFile);
            } else {
                fputs(line, amFile);
            }
        }
        lineNum++;
    }
    mcroNames = getMcrosName(mcros);
    cleanupMcros(mcros);
    fclose(amFile);
    if (createNewFile == 0)
        remove(newFileName);
    free(newFileName);
    return mcroNames;
}

char *isStartOfMcro(const char *line, char *forbiddenWords, int *createNewFile, int lineNum) {
    char *originalLineCopy;
    char *lineCopy;
    char *mcroName;
    char *start, *end;
    int isForbidden;

    originalLineCopy = (char *)malloc(strlen(line) + 1);
    if (originalLineCopy == NULL) {
        printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
        *createNewFile = 0;
        return NULL;
    }
    strcpy(originalLineCopy, line);
    lineCopy = originalLineCopy;

    while (isspace(*lineCopy)) {
        lineCopy++;
    }

    start = lineCopy;
    while (*lineCopy && !isspace(*lineCopy)) {
        lineCopy++;
    }
    *lineCopy = '\0';

    if (strcmp(start, "mcro") != 0) {
        free(originalLineCopy);
        return NULL;
    }

    lineCopy++;
    while (isspace(*lineCopy)) {
        lineCopy++;
    }

    start = lineCopy;
    while (*lineCopy && !isspace(*lineCopy) && *lineCopy != '\n') {
        lineCopy++;
    }
    end = lineCopy;
    *end = '\0';

    isForbidden = isForbiddenWord(start, forbiddenWords);
    if (isForbidden == 1) {
        *createNewFile = 0;
    }
    if (*start == '\0' || isForbiddenWord(start, forbiddenWords)) {
        printErrorMsg(INVALID_MACRO_ERROR, lineNum);
        free(originalLineCopy);
        *createNewFile = 0;
        return NULL;
    }

    lineCopy = end + 1;
    while (*lineCopy && isspace(*lineCopy)) {
        lineCopy++;
    }

    if (*lineCopy && *lineCopy != '\n') {
        printErrorMsg(UNEXPECTED_TEXT_AFTER_MACRO_ERROR, lineNum);
        free(originalLineCopy);
        *createNewFile = 0;
        return NULL;
    }

    mcroName = (char *)malloc(strlen(start) + 1);
    if (mcroName == NULL) {
        printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
        free(originalLineCopy);
        *createNewFile = 0;
        return NULL;
    }
    strcpy(mcroName, start);

    free(originalLineCopy);
    return mcroName;
}

int isEndOfMcro(const char *line, int *createNewFile, int lineNum) {
    char *originalLineCopy;
    char *lineCopy;
    char *token;
    int result = 0;

    originalLineCopy = (char *)malloc(strlen(line) + 1);
    if (originalLineCopy == NULL) {
        printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
        *createNewFile = 0;
        return 0;
    }
    strcpy(originalLineCopy, line);
    lineCopy = originalLineCopy;

    while (isspace(*lineCopy)) {
        lineCopy++;
    }
    token = strtok(lineCopy, " ");
    if (token == NULL) {
        free(originalLineCopy);
        return 0;
    }

    token = token[strlen(token) - 1] == '\n' ? strtok(token, "\n") : token;
    if (strcmp(token, "mcroend") != 0) {
        free(originalLineCopy);
        return 0;
    }
    token = strtok(NULL, " ");
    if (token != NULL) {
        printErrorMsg(UNEXPECTED_TEXT_AFTER_MACROEND_ERROR, lineNum);
        *createNewFile = 0;
        result = 0;
    } else {
        result = 1;
    }

    free(originalLineCopy);
    return result;
}

char *isMcro(const char *line, linkedListOfMcros *mcros, int *createNewFile) {
    char *originalLineCopy;
    char *lineCopy;
    char *token;
    linkedListOfMcros *current;
    char *result = NULL;

    originalLineCopy = (char *)malloc(strlen(line) + 1);
    if (originalLineCopy == NULL) {
        printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
        *createNewFile = 0;
        return NULL;
    }
    strcpy(originalLineCopy, line);
    lineCopy = originalLineCopy;

    while (isspace(*lineCopy)) {
        lineCopy++;
    }
    token = strtok(lineCopy, " ");
    if (token == NULL) {
        free(originalLineCopy);
        return NULL;
    }
    current = mcros;
    while (current != NULL && result == NULL) {
        if (token[strlen(token) - 1] == '\n') {
            char *newToken = strtok(token, "\n");
            if (newToken != NULL && strcmp(newToken, current->mcro->mcroName) == 0) {
                result = current->mcro->mcroData;
            }
        } else if (strcmp(token, current->mcro->mcroName) == 0) {
            result = current->mcro->mcroData;
        }
        current = current->next;
    }
    token = strtok(NULL, " ");
    if (token != NULL){
        free(originalLineCopy);
        return NULL;
    }

    free(originalLineCopy);
    return result;
}

char *getMcrosName(linkedListOfMcros *mcros) {
    int totalLength = 0;
    linkedListOfMcros *current = mcros;
    char *result, *ptr;

    while (current != NULL) {
        totalLength += strlen(current->mcro->mcroName) + 1; // +1 for '\n'
        current = current->next;
    }

    result = (char *)malloc(totalLength + 1); // +1 for the null terminator
    if (result == NULL) {
        printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
        return NULL;
    }

    current = mcros;
    ptr = result;
    while (current != NULL) {
        strcpy(ptr, current->mcro->mcroName);
        ptr += strlen(current->mcro->mcroName);
        *ptr = '\n';
        ptr++;
        current = current->next;
    }
    *ptr = '\0';

    return result;
}

void cleanupMcros(linkedListOfMcros *mcros) {
    linkedListOfMcros *current = mcros;
    while (current != NULL) {
        linkedListOfMcros *next = current->next;
        free(current->mcro->mcroName);
        free(current->mcro->mcroData);
        free(current->mcro);
        free(current);
        current = next;
    }
}