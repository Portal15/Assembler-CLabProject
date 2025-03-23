#ifndef INPUT_ANALYZER_H
#define INPUT_ANALYZER_H


#include "dataTypes.h"
#define MAX_SIZE_OF_STRING 100

/* This function is the first pass in the project, it takes a line (inputed line) and it returns a line struct.
 * The function will analyze the inputed line and will fill the line struct with the data that it found.
 * inputedLine - the line that the function will analyze
 * lineNum - the number of the line
 * ot - the object table
 * return - the line struct that the function filled
 */
line analyzeInput(char *inputedLine, int lineNum);

/* This function frees the memory allocated for a command struct.
 * cmd - the command struct to be freed
 */
void freeCommand(command *cmd);

/* This function frees the memory allocated for a var struct.
 * v - the var struct to be freed
 */
void freeVar(var *v);


#endif