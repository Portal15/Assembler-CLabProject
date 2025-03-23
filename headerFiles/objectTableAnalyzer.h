#ifndef OBJECTTABLEANALYZER_H
#define OBJECTTABLEANALYZER_H

#include "dataTypes.h"

/* This function adds the data in the line to the object table if there are any labels or externs/entries
 * l - the line to be added
 * ot - the object table that will get the new stuff
 */
void addLineToObjectTable(const line *l, objectTable *ot);

/* This function creates an object to the struct.
 * return - pointer to the struct
 */
objectTable* createObjectTable();

/*
 * This function fills the objects that doesn't have address, only a lineNum.
 * ot - the object table
 * address - the address to be added
 * lineNum - the line number of the object
 */
void fillAddresses(const objectTable* ot, const int* address, int lineNum);

/* This function check for entry/extern and command/data and fill the address with the entry/extern to be the address of the command/data
 * ot - the object table
 * return - 0 if it goes good and 0 otherwise.
 */
int checkForDoubles(const objectTable* ot);

#endif