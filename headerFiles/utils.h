#ifndef UTILS_H
#define UTILS_H

#include <string.h>
#include <stdlib.h>
/* This function copys the function strdup that not included in ansi
 * s - the word to duplicate
 * return - a duplicate of the string s
 */
char *strdup(const char *s);

/* This function gets line and checks if it is a forbidden word.
 * If it is, it returns 1.
 * If it is not, it returns 0.
 * word - the word to be checked
 * forbiddenWords - a string containing the forbidden words
 * return - 1 if it is a forbidden word, 0 otherwise
 */
int isForbiddenWord(char *word, char *forbiddenWords);

/*
 * This function counts the number of lines in a file.
 * fileName - the name of the file
 * return - the number of lines in the file
 */
int countLinesInFile(const char *fileName);

/* This function checks if a string is an allowed sentence (not a blank and not start with ;).
 * input - the string to be checked
 * return - 1 if the string is a sentence, 0 otherwise
 */
int isSentence(const char *input);


#endif
