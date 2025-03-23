#ifndef BINARYCONVERTOR_H
#define BINARYCONVERTOR_H

#include "dataTypes.h"

/* This function converts a line to binary code and adds it to the list.
 * l - the line to be converted
 * address - the address of the line
 * list - the list of the converted lines
 * return - 1 if there is an error, 0 otherwise
 */
int binaryConvertor(const line *l, int *address, binaryConvertedLines *list);

#endif
