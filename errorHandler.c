#include "headerFiles/errorHandler.h"
#include <stdio.h>


void printErrorMsg(const int errorType, int lineNum){
    lineNum -= 100;
    switch(errorType){
        case MEMORY_ALLOCATION_ERROR:
            if(lineNum != -1){
                printf("-!-     Error: Memory allocation failed at line %d.\n", lineNum);
                fprintf(stderr, "Error: Memory allocation failed at line %d.\n", lineNum);
            } else {
                printf("-!-     Error: Memory allocation failed.\n");
                fprintf(stderr, "Error: Memory allocation failed.\n");
            }
            break;

        case FAILED_TO_OPEN_FILE_ERROR:
            printf("-!-     Error: Failed to open file.\n");
            fprintf(stderr, "Error: Failed to open file.\n");
            break;

        case FAILED_TO_CREATE_FILE_ERROR:
            printf("-!-     Error: Failed to create file.\n");
            fprintf(stderr, "Error: Failed to create file.\n");
            break;

        case INVALID_MACRO_ERROR:
            if(lineNum != -1){
                printf("-!-     Error: Invalid macro at line %d.\n", lineNum);
                fprintf(stderr, "Error: Invalid macro at line %d.\n", lineNum);
            } else {
                printf("-!-     Error: Invalid macro");
                fprintf(stderr, "Error: Invalid macro.\n");
            }
            break;

        case UNEXPECTED_TEXT_AFTER_MACRO_ERROR:
            if(lineNum != -1){
                printf("-!-     Error: Unexpected text after macro at line %d.\n", lineNum);
                fprintf(stderr, "Error: Unexpected text after macro at line %d.\n", lineNum);
            } else {
                printf("-!-     Error: Unexpected text after macro.\n");
                fprintf(stderr, "Error: Unexpected text after macro.\n");
            }
            break;

        case UNEXPECTED_TEXT_AFTER_MACROEND_ERROR:
            if(lineNum != -1){
                printf("-!-     Error: Unexpected text after macroend at line %d.\n", lineNum);
                fprintf(stderr, "Error: Unexpected text after macroend at line %d.\n", lineNum);
            } else {
                printf("-!-     Error: Unexpected text after macroend.\n");
                fprintf(stderr, "Error: Unexpected text after macroend.\n");
            }
            break;

        case ILLEGAL_COMMA_ERROR:
            printf("-!-     Error: Illegal comma at line %d.\n", lineNum);
            fprintf(stderr, "Error: Illegal comma at line %d.\n", lineNum);
            break;
        case MISSING_COMMA_ERROR:
            printf("-!-     Error: Missing comma at line %d.\n", lineNum);
            fprintf(stderr, "Error: Missing comma at line %d.\n", lineNum);
            break;
        case MULTIPLE_COMMA_ERROR:
            printf("-!-     Error: Multiple comma at line %d.\n", lineNum);
            fprintf(stderr, "Error: Multiple commas at line %d.\n", lineNum);
            break;
        case UNEXPECTED_TEXT_AFTER_END_OF_LINE_ERROR:
            printf("-!-     Error: Unexpected text after end of line at line %d.\n", lineNum);
            fprintf(stderr, "Error: Unexpected text after end of line at line %d.\n", lineNum);
            break;
        case INVALID_PARAMETER_NOT_A_NUMBER_ERROR:
            printf("-!-     Error: Invalid parameter, not a number at line %d.\n", lineNum);
            fprintf(stderr, "Error: Invalid parameter, not a number at line %d.\n", lineNum);
            break;
        case INVALID_PARAMETER_NOT_A_REGISTER_ERROR:
            printf("-!-     Error: Invalid parameter, not a register at line %d.\n", lineNum);
            fprintf(stderr, "Error: Invalid parameter, not a register at line %d.\n", lineNum);
            break;
        case INVALID_LABEL_NAME_ERROR:
            printf("-!-     Error: Invalid label name at line %d.\n", lineNum);
            fprintf(stderr, "Error: Invalid label name at line %d.\n", lineNum);
            break;
        case INVALID_COMMAND_NAME_ERROR:
            printf("-!-     Error: Invalid command name at line %d.\n", lineNum);
            fprintf(stderr, "Error: Invalid command name at line %d.\n", lineNum);
            break;
        case INVALID_PARAMETER_NOT_A_STRING_ERROR:
            printf("-!-     Error: Invalid parameter, not a string at line %d.\n", lineNum);
            fprintf(stderr, "Error: Invalid parameter, not a string at line %d.\n", lineNum);
            break;
        case LABEL_NAME_APPEAR_TWICE_ERROR:
            printf("-!-     Error: Label name appears twice at line %d.\n", lineNum);
            fprintf(stderr, "Error: Label name appears twice at line %d.\n", lineNum);
            break;
	    case LABEL_CALLED_BUT_WAS_NOT_DEFINED:
	        printf("-!-     Error: Label called but not defined at line %d.\n", lineNum);
            fprintf(stderr, "Error: Label was called but wasn't defined at line %d.\n", lineNum);
            break;
        case EXTERN_LABEL_DECLARED:
            printf("-!-     Error: External label declared at line %d.\n", lineNum);
            fprintf(stderr, "Error: External label declared again at line %d.\n", lineNum);
        case INVALID_COMMAND_ERROR:
            printf("-!-     Error: Invalid command at line %d.\n", lineNum);
            fprintf(stderr, "Error: Invalid command at line %d.\n", lineNum);
            break;
        case PARAMETER_NAME_IS_NOT_ALLOWED:
            printf("-!-     Error: Parameter name is not allowed at line %d.\n", lineNum);
            fprintf(stderr, "Error: Parameter name is not allowed at line %d.\n", lineNum);
            break;
        case EMPTY_LABEL_ERROR:
            printf("-!-     Error: Empty label at line %d.\n", lineNum);
            fprintf(stderr, "Error: Empty label at line %d.\n", lineNum);
            break;
        case LINE_IS_TOO_LONG_ERROR:
            printf("-!-     Error: Line is too long at line %d.\n", lineNum);
            fprintf(stderr, "Error: Line is too long at line %d.\n", lineNum);
            break;
        default:
            printf("-!-     Error: Unknown error\n");
            fprintf(stderr, "Error: Unknown error.\n");
            break;
    }


}