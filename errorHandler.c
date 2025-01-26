#include "errorHandler.h"
#include <stdio.h>


void printErrorMsg(const int errorType, const int lineNum){
    switch(errorType){
        case MEMORY_ALLOCATION_ERROR:
            if(lineNum != -1){
                fprintf(stderr, "Error: Memory allocation failed at line %d.\n", lineNum);
            } else {
                fprintf(stderr, "Error: Memory allocation failed.\n");
            }
            break;

        case FAILED_TO_OPEN_FILE_ERROR:
            fprintf(stderr, "Error: Failed to open file.\n");
            break;

        case FAILED_TO_CREATE_FILE_ERROR:
            fprintf(stderr, "Error: Failed to create file.\n");
            break;

        case INVALID_MACRO_ERROR:
            if(lineNum != -1){
                fprintf(stderr, "Error: Invalid macro at line %d.\n", lineNum);
            } else {
                fprintf(stderr, "Error: Invalid macro.\n");
            }
            break;

        case UNEXPECTED_TEXT_AFTER_MACRO_ERROR:
            if(lineNum != -1){
                fprintf(stderr, "Error: Unexpected text after macro at line %d.\n", lineNum);
            } else {
                fprintf(stderr, "Error: Unexpected text after macro.\n");
            }
            break;

        case UNEXPECTED_TEXT_AFTER_MACROEND_ERROR:
            if(lineNum != -1){
                fprintf(stderr, "Error: Unexpected text after macroend at line %d.\n", lineNum);
            } else {
                fprintf(stderr, "Error: Unexpected text after macroend.\n");
            }
            break;

        case ILLEGAL_COMMA_ERROR:
            fprintf(stderr, "Error: Illegal comma at line %d.\n", lineNum);
            break;
        case MISSING_COMMA_ERROR:
            fprintf(stderr, "Error: Missing comma at line %d.\n", lineNum);
            break;
        case MULTIPLE_COMMA_ERROR:
            fprintf(stderr, "Error: Multiple commas at line %d.\n", lineNum);
            break;
        case UNEXPECTED_TEXT_AFTER_END_OF_LINE_ERROR:
            fprintf(stderr, "Error: Unexpected text after end of line at line %d.\n", lineNum);
            break;
        case INVALID_PARAMETER_NOT_A_NUMBER_ERROR:
            fprintf(stderr, "Error: Invalid parameter, not a number at line %d.\n", lineNum);
            break;
        case INVALID_PARAMETER_NOT_A_REGISTER_ERROR:
            fprintf(stderr, "Error: Invalid parameter, not a register at line %d.\n", lineNum);
            break;
        case INVALID_LABEL_NAME_ERROR:
            fprintf(stderr, "Error: Invalid label name at line %d.\n", lineNum);
            break;
        case INVALID_COMMAND_NAME_ERROR:
            fprintf(stderr, "Error: Invalid command name at line %d.\n", lineNum);
            break;
        default:
            fprintf(stderr, "Error: Unknown error.\n");
            break;
    }


}