#include "errorHandler.h"
#include "utils.h"
#include "second.h"
#include "dataTypes.h"

/*
 * this function gets an address (that we are assuming that is 24 bits long) and converting it to hexadecimal.
 * address - the 24 bits long integer that we want to convert.
 * the function returns the hexadecimal number as a pointer to a char array (char*).
 */
char* IntToHex(int address);

int SecondPass(ConvertedLines *list, objectTable *ob) {
    ConvertedLines *current = list;
    int error = 0;
    while (current) {
        if (current->data[0] == '!') {
            char *object = current->data + 1;
            int address = -1;
	    char *charLineNum;
	    int lineNum;
	    objectTableEntry *entry;
	    objectTableExtern *ext;
	    objectTableData *data;
	    while(current->data[0] != '!') {
		current->data++;
	    }
	    current->data[0] = '\0';
	    charLineNum = current->data + 1;
	    lineNum = atoi(charLineNum);
            
            /* Search in entries */
            entry = ob->entries;
            while (entry && (address == -1)) {
                if (strcmp(entry->name, object) == 0) {
                    address = entry->address;
                }
                entry = entry->next;
            }
            
            /* Search in externs if not found in entries */
            if (address == -1) {
                ext = ob->externs;
                while (ext && (address == -1)) {
                    if (strcmp(ext->name, object) == 0) {
                        address = 1;
                    }
                    ext = ext->next;
                }
            }
            
	    /* Search in data if not found in externs */
            if (address == -1) {
                data = ob->data;
                while (data && (address == -1)) {
                    if (strcmp(data->name, object) == 0) {
                        address = data->line;
                    }
                    data = data->next;
                }
            }

            /* If found, replace with address */
            if (address != -1) {
                free(current->data);
                current->data = IntToHex(address);
		if(current->data == NULL) {
		    printErrorMsg(MEMORY_ALLOCATION_ERROR, lineNum);
		    error = 1;
		}
            }
	    /* If not found, print error messege */
	    else {
		printErrorMsg(LABEL_CALLED_BUT_WAS_NOT_DEFINED, lineNum);
		error = 1;
	    }
        }
        current = current->next;
    }
    return error;
}


char* IntToHex(int address) {
    int i;
    char *hexString = (char *)malloc((Size_Of_Address + 1) * sizeof(char));
    if (hexString != NULL) {
        for (i = (Size_Of_Address - 1); i >= 0; i--) {
            hexString[i] = HEX_DIGITS[address % Digits_In_Hexa];
            address >>= Bits_In_An_Hexa_Digit;
        }
        hexString[Size_Of_Address] = '\0';
    }
    return hexString;
}
