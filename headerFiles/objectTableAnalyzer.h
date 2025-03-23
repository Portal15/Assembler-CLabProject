#ifndef OBJECTTABLEANALYZER_H
#define OBJECTTABLEANALYZER_H

#include "dataTypes.h"

void addLineToObjectTable(line *l, objectTable *ot);

objectTable* createObjectTable();

void fillAddresses(objectTable* ot, int* invisibleLineNum, int lineNum);

int checkForDoubles(objectTable* ot);

void freeVar(var *v);

#endif