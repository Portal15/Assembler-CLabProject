#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <stdio.h>

/* General Errors */
#define MEMORY_ALLOCATION_ERROR 1
#define FAILED_TO_OPEN_FILE_ERROR 2
#define FAILED_TO_CREATE_FILE_ERROR 3

/* PreProcessor Errors */
#define INVALID_MACRO_ERROR 10
#define UNEXPECTED_TEXT_AFTER_MACRO_ERROR 11
#define UNEXPECTED_TEXT_AFTER_MACROEND_ERROR 12

/* LineAnalyzer Errors */
#define ILLEGAL_COMMA_ERROR 20
#define MISSING_COMMA_ERROR 21
#define MULTIPLE_COMMA_ERROR 22
#define UNEXPECTED_TEXT_AFTER_END_OF_LINE_ERROR 23
#define INVALID_PARAMETER_NOT_A_NUMBER_ERROR 24
#define INVALID_PARAMETER_NOT_A_REGISTER_ERROR 25
#define INVALID_LABEL_NAME_ERROR 26
#define INVALID_COMMAND_NAME_ERROR 27
#define INVALID_PARAMETER_NOT_A_STRING_ERROR 28

/* Object Table Errors */
#define LABEL_NAME_APPEAR_TWICE_ERROR 30


/* This function prints an error message to stderr.
 * errorType - the type of the error
 * lineNum - the line number of the error
 */
void printErrorMsg(const int errorType, const int lineNum);

#endif // ERROR_HANDLER_H