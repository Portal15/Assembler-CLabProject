#ifndef VALIDATIONSCHECKER_H

#define VALIDATIONSCHECKER_H

#include "dataTypes.h"

/* This function checks if the line has any errors that didnt found in the inputAnalyzer.
 * l - the line to be checked
 * return - 1 if there is an error, 0 otherwise
 */
int checkInputValidation(line *l, char *forbiddenWords);

/* This function checks if the label in the line is valid.
 * ot - the object table
 * l - the line to be checked
 * return - 1 if there is an error, 0 otherwise
 */
int checkLabelErrors(const objectTable ot, const line *l);

/* This function checks if the object table has any errors.
 * ot - the object table to be checked
 * return - 1 if there is an error, 0 otherwise
 */
int checkObjectTableErrors(const objectTable ot);


#endif