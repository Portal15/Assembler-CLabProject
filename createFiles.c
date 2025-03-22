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

void CreateFiles(binaryConvertedLines *list, char* file, objectTable *ob, externsAppearance *externs, int dataWithNoLabel) {
    FILE *obFile;
    int counter = 100;
    int invisLineCounter = 100;
    char obFileName[256];
    objectTableEntry *entry = ob->entries;
    objectTableExtern *ext = ob->externs;
    binaryConvertedLines *current = list;
    binaryConvertedLines *templine;
    objectTableData *data = ob->data;
    var *value;
    int dataLines = dataWithNoLabel;

    objectTableEntry *entryFree = entry;
    objectTableExtern *extFree = ext;
    objectTable *obFree = ob;
    externsAppearance *externsFree = externs;
    binaryConvertedLines *listFree = list;

    int totalLines = 0;
    int codeLines;

    /* Count data lines */
    while (data) {
        value = data->value;
        while (value) {
            dataLines++;
            value = value->next;
        }
        data = data->next;
    }

    /* Count total lines */
    templine = current;
    while (templine) {
        totalLines++;
        templine = templine->next;
    }

    codeLines = totalLines - dataLines;


    /* Handle extern file */
    if (externs->next) {
        CreateFileName(file, obFileName, 2);
        obFile = fopen(obFileName, "w");

	/* Return error if the file couldn't be created */
	if (obFile == NULL) {
	    printErrorMsg(FAILED_TO_CREATE_FILE_ERROR, -1);
	    return;
	}

	externs = externs->next;

    while (externs && current) {
	    if(current->data && strlen(current->data) >= 6 && current->data[0] == '0' && current->data[1] == '0' && current->data[2] == '0' && current->data[3] == '0' && current->data[4] == '0' && current->data[5] == '1') {
        	fprintf(obFile, "%s %07d\n", externs->name, invisLineCounter);
	    	if (externs->next != NULL)
				externs = externs->next;
	    }
	    invisLineCounter++;
	    current = current->next;
    }

        fclose(obFile);
    }


    /* Creating the name of the file with an ending of .ob */
    CreateFileName(file, obFileName, 0);

    /* Creating the .ob file */
    obFile = fopen(obFileName, "w");


    /* Return error if the file couldn't be created */
    if (obFile == NULL) {
        printErrorMsg(FAILED_TO_CREATE_FILE_ERROR, -1);
	return;
    }

    fprintf(obFile, "%6d %d\n", codeLines, dataLines);

    /* Writing data */
    while (list) {
        fprintf(obFile, "%07d %s\n", counter, list->data);
        counter++;
	list = list->next;
    }

    fclose(obFile);

    /* Handle entry file */
    if (entry) {
        CreateFileName(file, obFileName, 1);
        obFile = fopen(obFileName, "w");

        /* Return error if the file couldn't be created */
	if (obFile == NULL) {
	    printErrorMsg(FAILED_TO_CREATE_FILE_ERROR, -1);
	    return;
	}

        while (entry) {
            fprintf(obFile, "%s %07d\n", entry->name, entry->address);
	    entry = entry->next;
        }

        fclose(obFile);
    }
}

  
void CreateFileName(char* file, char* obFile, int fileType) {
    char* dot;
   
    /* copying the file name into a buffer and changing the copies' ending to .ob */
    strcpy(obFile, file);
    dot = strrchr(obFile, '.');
    if(fileType == 0) {
		*++dot = 'o';
		*++dot = 'b';
		*++dot = '\0';
    }
    else if(fileType == 1) {
		*++dot = 'e';
		*++dot = 'n';
		*++dot = 't';
		*++dot = '\0';
    } else if (fileType == 3) {
		*++dot = 'a';
    	*++dot = 'm';
    	*++dot = '\0';
    }else {
    	*++dot = 'e';
    	*++dot = 'x';
    	*++dot = 't';
    	*++dot = '\0';
    }
}

