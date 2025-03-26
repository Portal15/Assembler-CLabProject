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
 * fileName - the buffer that will hold the copied file name with the desired ending
 * fileType - the integer that will decide which ending is going to be.
 * the function assumes that the file name is fixed (it has a .txt ending) since it wouldnt have been called otherwise.
 */


 void CreateFiles(binaryConvertedLines *list, char* file, objectTable *ob, externsAppearance *externsAppearances, int dataWithNoLabel) {
    FILE *fileName;
    int lineCounter = 100;
    int invisLineCounter = 100;
    char FileNameString[256];
    objectTableEntry *entry = ob->entries;
    binaryConvertedLines *current = list;
    binaryConvertedLines *lineCounterList = list;
    objectTableData *data = ob->data;
    var *value;
    int dataLines = dataWithNoLabel;

    int totalLines = 0;
    int codeLines;


    /* Handle extern file */
    if (externsAppearances->next) {
        CreateFileName(file, FileNameString, FILE_TYPE_EXTERN);
        fileName = fopen(FileNameString, "w");


		/* Return error if the file couldn't be created */
		if (fileName == NULL) {
			printErrorMsg(FAILED_TO_CREATE_FILE_ERROR, -1);
			return;
		}
    	printf("-V-     externs file successfully created\n");


		externsAppearances = externsAppearances->next;

        while (externsAppearances && current) {
	    if(current->data && strlen(current->data) >= 6 && current->data[0] == '0' && current->data[1] == '0' && current->data[2] == '0' && current->data[3] == '0' && current->data[4] == '0' && current->data[5] == '1') {
        	fprintf(fileName, "%s %07d\n", externsAppearances->name, invisLineCounter);
		if (externsAppearances->next != NULL) {
		    externsAppearances = externsAppearances->next;
		}
	    }
	    invisLineCounter++;
	    current = current->next;
        }

        fclose(fileName);
    }


    /* Creating the name of the file with an ending of .ob */
    if(list && list->data) {

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
    	while (lineCounterList && lineCounterList->data) {
        	totalLines++;
        	lineCounterList = lineCounterList->next;
    	}

    	codeLines = totalLines - dataLines;
	CreateFileName(file, FileNameString, FILE_TYPE_OBJECT);

	    /* Creating the .ob file */
	fileName = fopen(FileNameString, "w");



	/* Return error if the file couldn't be created */
	if (fileName == NULL) {
	    printErrorMsg(FAILED_TO_CREATE_FILE_ERROR, -1);
	    return;
	}
	printf("-V-     object file successfully created\n");

	if(codeLines || dataLines) {
	    fprintf(fileName, "%6d %d\n", codeLines, dataLines);
	}

	/* Writing data */
	while (list && list->data) {
	    fprintf(fileName, "%07d %s\n", lineCounter, list->data);
	    lineCounter++;
	    list = list->next;
	}

	fclose(fileName);
    }

    /* Handle entry file */
    if (entry) {
        CreateFileName(file, FileNameString, FILE_TYPE_ENTRY);
        fileName = fopen(FileNameString, "w");

    	/* Return error if the file couldn't be created */
		if (fileName == NULL) {
			printErrorMsg(FAILED_TO_CREATE_FILE_ERROR, -1);
			return;
		}
    	printf("-V-     entry file successfully created\n");



        while (entry) {
            fprintf(fileName, "%s %07d\n", entry->name, entry->address);
	    entry = entry->next;
        }

        fclose(fileName);
    }
}

  
void CreateFileName(char* file, char* fileName, int fileType) {
    char* dot;
   
    /* copying the file name into a buffer and changing the copies' ending to .ob */
    strcpy(fileName, file);
    dot = strrchr(fileName, '.');
    if(fileType == FILE_TYPE_OBJECT) {
	*++dot = 'o';
	*++dot = 'b';
	*++dot = '\0';
    }
    else if(fileType == FILE_TYPE_ENTRY) {
	*++dot = 'e';
	*++dot = 'n';
	*++dot = 't';
	*++dot = '\0';
    } else if (fileType == FILE_TYPE_AFTER_MACRO) {
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
  