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


void CreateFiles(binaryConvertedLines *list, char* file, objectTable *ob, externsAppearance *externs) {
    FILE *obFile;
    int counter = 100;
    int invisLineCounter = 100;
    char obFileName[256];
    objectTableEntry *entry = ob->entries;
    objectTableEntry *tempEntry;
    objectTableExtern *ext = ob->externs;
    objectTableExtern *tempExt;
    externsAppearance *tempExtern;
    binaryConvertedLines *current = list;
    binaryConvertedLines *templine;
    objectTableData *data = ob->data;
    var *value = (data ? &(data->value) : NULL);
    int dataLines = 0;
    int totalLines = 0;
    int codeLines;

    /* Creating the name of the file with an ending of .ob */
    CreateFileName(file, obFileName, 0);

    /* Creating the .ob file */
    obFile = fopen(obFileName, "w");

    /* Return error if the file couldn't be created */
    if (obFile == NULL) {
        printErrorMsg(FAILED_TO_CREATE_FILE_ERROR, -1);
    
        /* Free dynamically allocated memory before returning */
        while (list) {
            templine = list;
            if(list->data) free(list->data);
            list = list->next;
            free(templine);
        }

        while (entry) {
            tempEntry = entry;
            if(entry->name) free(entry->name);
            entry = entry->next;
            free(tempEntry);
        }

        while (ext) {
            tempExt = ext;
            if(ext->name) free(ext->name);
            ext = ext->next;
            free(tempExt);
        }

	while (ob->data) {
	    objectTableData *tempData = ob->data;
    
	    if (tempData->name) free(tempData->name);

	    ob->data = ob->data->next;
	    free(tempData);
	}


        return;
    }

    /* Count data lines */
    while (data) {
	if(!data) break;
        value = data ? &(data->value) : NULL;
        while (value) {
	    if(!value) break;
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

    fprintf(obFile, "%6d %d\n", codeLines, dataLines);


    /* Handle extern file */
    if (externs) {
        CreateFileName(file, obFileName, 2);
        obFile = fopen(obFileName, "w");

        if (obFile == NULL) {
            printErrorMsg(FAILED_TO_CREATE_FILE_ERROR, -1);
            return;
        }

        while (externs && current) {
	    if(current->data[0] == '0' && current->data[1] == '0' && current->data[2] == '0' && current->data[3] == '0' && current->data[4] == '0' && current->data[5] == '1') {
        	fprintf(obFile, "%s %07d\n", externs->name, invisLineCounter);
        	free(externs->name);
        	tempExtern = externs;
        	externs = externs->next;
        	free(tempExtern);
	    }
	    invisLineCounter++;
	    current = current->next;
        }

        fclose(obFile);
    }


    /* Writing data */
    while (list) {
        fprintf(obFile, "%07d %s\n", counter, list->data);
        counter++;

        /* Free dynamically allocated data */
        free(list->data);
        templine = list;
        list = list->next;
        free(templine);
    }

    fclose(obFile);

    /* Handle entry file */
    if (entry) {
        CreateFileName(file, obFileName, 1);
        obFile = fopen(obFileName, "w");

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
    else if(fileType == 1) {
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