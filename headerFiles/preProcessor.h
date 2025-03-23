#ifndef PRE_PROCESSOR_H
#define PRE_PROCESSOR_H

#include "headerFiles/errorHandler.h"
#include "headerFiles/inputAnalyzer.h"
#include "headerFiles/utils.h"
#define MAX_LINE_LENGTH 81

/* This function is the main function of the preprocessor.
 * It reads the file line by line and processes the macros.
 * fileName - the name of the file to be processed (also will be the name of the new file with .am extension)
 * forbiddenWords - a string containing the forbidden words (jmp, mov, etc.)
 * return - the name of the mcros that found in the file
 */
char* preProcessor(const char *fileName, char *forbiddenWords);

#endif // PRE_PROCESSOR_H