#include "headerFiles/createFiles.h"
#include "headerFiles/errorHandler.h"
#include "headerFiles/utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*
 * this function gets a file name and a buffer and its copying the file name into the buffer
 * with an ending of .ob or .ent or .ext
 * the function gets 3 parameters:
 * file - the file name that we want to copy
 * obFile - the buffer that will hold the copied file name with the desired ending
 * fileType - the integer that will decide which ending is going to be.
 * the function assumes that the file name is fixed (it has a .txt ending) since it wouldnt have been called otherwise.
 */
void CreateFileName(char* file, char* obFile, int fileType);


void CreateFiles(binaryConvertedLines *list, char* file, objectTable *ob)
{
    FILE *obFile;
    int counter = 100;
    char obFileName[256];
    objectTableEntry *entry = ob->entries;
    objectTableEntry *tempEntry;
    objectTableExtern *ext = ob->externs;
    objectTableExtern *tempExt;
    binaryConvertedLines *current = list;
    binaryConvertedLines *temp;

    /* creating the name of the file with an ending of .ob */
    CreateFileName(file, obFileName, 0);

    /* creating the .ob file */
    obFile = fopen(obFileName, "w");

    /* returning error if the file couldnt be created */
    if (obFile == NULL) {
	printErrorMsg(FAILED_TO_CREATE_FILE_ERROR, -1);
        return;
    }

    /* writing in the file the number of line next to every line of code and data translated to machine language */
    while (current) {
        fprintf(obFile, "%07d %s\n", counter, current->data);
        counter++;
        
        /* free dynamically allocated data */
        free(current->data);
        temp = current;
        current = current->next;
        free(temp);
    }

    fclose(obFile);

    if (entry) {
        CreateFileName(file, obFileName, 1);

        /* creating the .ent file */
        obFile = fopen(obFileName, "w");

        /* returning error if the file couldnt be created */
        if (obFile == NULL) {
	    printErrorMsg(FAILED_TO_CREATE_FILE_ERROR, -1);
            return;
        }

        while (entry) {
            fprintf(obFile, "%s %07d\n", entry->name, entry->address);
            free(entry->name);
            tempEntry = entry;
            entry = entry->next;
            free(tempEntry);
        }

        fclose(obFile);
    }

    if (ext) {
        CreateFileName(file, obFileName, 2);

        /* creating the .ext file */
        obFile = fopen(obFileName, "w");

        /* returning error if the file couldnt be created */
        if (obFile == NULL) {
	    printErrorMsg(FAILED_TO_CREATE_FILE_ERROR, -1);
            return;
        }

        while (ext) {
            fprintf(obFile, "%s %07d\n", ext->name, ext->line);
            free(ext->name);
            tempExt = ext;
            ext = ext->next;
            free(tempExt);
        }

        fclose(obFile);
    }
}

void CreateFileName(char* file, char* obFile, int fileType)
{
    char* dot;
   
    /* copying the file name into a buffer and changing the copies' ending to .ob */
    strcpy(obFile, file);
    dot = strrchr(obFile, '.');
    if(fileType == 0) {
	*++dot = 'o';
	*++dot = 'b';
	*++dot = '\0';
    }
    if(fileType == 1) {
	*++dot = 'e';
	*++dot = 'n';
	*++dot = 't';
	*++dot = '\0';
    } else {
	*++dot = 'e';
	*++dot = 'x';
	*++dot = 't';
	*++dot = '\0';
    }
}