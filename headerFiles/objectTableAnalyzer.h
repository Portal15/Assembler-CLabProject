//
// Created by Yonatan Ben Yehuda on 27/01/2025.
//

#ifndef OBJECTTABLEANALYZER_H
#define OBJECTTABLEANALYZER_H

#include "dataTypes.h"

void addLineToObjectTable(line *l, objectTable *ot);

objectTable* createObjectTable();

void fillInvisibleLineNum(objectTable* ot, int* invisibleLineNum, int lineNum);

int checkForDoubles(objectTable* ot);


#endif //OBJECTTABLEANALYZER_H
