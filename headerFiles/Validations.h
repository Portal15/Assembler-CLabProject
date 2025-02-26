#ifndef LINE_VALIDATION_H
#define LINE_VALIDATION_H

#include "dataTypes.h"


int checkLineValidation(line *l, char *forbiddenWords, char* vars);

int checkLabelErrors(objectTable ot, line *l);

int checkObjectTableErrors(objectTable ot);


#endif // LINE_VALIDATION_H