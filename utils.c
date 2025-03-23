#include "headerFiles/utils.h"
#include "headerFiles/errorHandler.h"
#include "headerFiles/dataTypes.h"

char *strdup(const char *s) {
    size_t len;
    char *copy;

    if (s == NULL) {
        return NULL;
    }
    len = strlen(s) + 1;
    copy = malloc(len);
    if (copy != NULL) {
        memcpy(copy, s, len);
    }
    return copy;
}

int isForbiddenWord(char *word, char *forbiddenWords) {
    char *forbiddenWordsCopy;
    char *token;

    forbiddenWordsCopy = strdup(forbiddenWords);
    if (forbiddenWordsCopy == NULL) {
        printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
        return 0;
    }

    token = strtok(forbiddenWordsCopy, "\n\0");
    while (token != NULL) {
        if (strcmp(word, token) == 0) {
            free(forbiddenWordsCopy);
            return 1;
        }
        token = strtok(NULL, "\n\0");
    }

    free(forbiddenWordsCopy);
    return 0;
}

int countLinesInFile(const char *fileName) {
    FILE *file = fopen(fileName, "r");
    if (file == NULL) {
        perror("Failed to open file");
        return -1;
    }

    int lines = 0;
    char ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n') {
            lines++;
        }
    }

    fclose(file);
    return lines;
}

int isSentence(const char *input) {
    if (input == NULL) {
        return 0;
    }
    if (!*input) {
        return 0;
    }
    if (*input == ';') {
        return 0;
    }

    while (*input != '\0') {
        if (*input != ' ' || *input != '\t' || *input != '\n') {
            return 1;
        }
        input++;
    }

    return 0;
}

