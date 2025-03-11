#ifndef CREATEFILES_H
#define CREATEFILES_H

#include "dataTypes.h"


/*
 * this function creates a the .ob, .ent and .ext files with the name of the given file and in the .ob file
 * it writes in it the number of lines of code and number of lines of data as a header, and the number
 * of line next to every line of code and data translated to machine language. in the .ent it writes 
 * every entry label next to its line, and in the .ext it writes every extern label next to every line it is being shown.
 * the function gets 3 parameters:
 * list - a linked list that containes the code after it was converted to machine language in hexa.
 * ob - the object table.
 * file - the name of the file that the function will create the .ob file based on
 * the function assumes that the file name is fixed since it wouldnt have been called otherwise.
 * the function assumes that the given file name isnt longer that 256 characters.
 */
void CreateFiles(binaryConvertedLines *list, char* file, objectTable *ob, externsAppearance *externs, int dataWithNoLabel);

void CreateFileName(char* file, char* obFile, int fileType);

#endif