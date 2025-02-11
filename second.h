#ifndef SECOND_H
#define SECOND_H

#include "dataTypes.h"

#define HEX_DIGITS "0123456789ABCDEF"
#define Size_Of_Address 6
#define Digits_In_Hexa 16
#define Bits_In_An_Hexa_Digit 4

/* A place-holder for the linked list of the converted lines */
typedef struct ConvertedLines{
    char *data;
    struct ConvertedLines *next;
} ConvertedLines;


/*
 * this function gets a pointer to the converted lines, and a pointer to the object table.
 * the function will go over the converted lined and it will check for lines with data that starts with !.
 * for every line that starts with ! that the function finds, it will search the string without the ! in the entries, externs and data in
 * the object table and if it finds a match it in the entries, it will copy the entry's address, convert it into hexadecimal and will
 * replace the data of the line to the hexadecimal, if it finds a match it in the externs, it will replace the data of the 
 * line to 1 in hexadecimal, and if it finds a match it in the entries, it will copy the entry's address, convert it into hexadecimal and will
 * replace the data of the line to the hexadecimal.
 * list - the linked list of the converted lines.
 * ob - the object table.
 */
int SecondPass(ConvertedLines *list, objectTable *ob);

#endif