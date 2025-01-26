#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lineAnalyzer.h"
#include "lineValidation.h"
#include "errorHandler.h"
#include "preProcessor.h"

int main(void) {
    // char *preForbiddenWords = "jmp\nmov";
    // char *forbiddenWords;
    // forbiddenWords = malloc(strlen(preForbiddenWords) + 1);
    //
    // strcpy(forbiddenWords, preForbiddenWords);
    // char *mcros = preProcessor("bla.as", forbiddenWords);
    // printf("%s\n", mcros);
    // free(mcros);
    // free(forbiddenWords);

    char* input = malloc(sizeof(char) * 100);
    fgets(input, 100, stdin);
    line l = analyzeLine(input, 1);
    int isValid = checkValidation(&l, "jmp\nmov", "bla");
    free(input);
    free(l.cmd);
    free(l.label);
    free(l.vars);
    return 0;
}
