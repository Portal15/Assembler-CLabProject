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


/*
 * this function gets a pointer to the start of 24 bits with the type of char and it is converting them into int.
 * binary - the pointer to the first char (bit).
 * the function return the int of the converted bits.
 */
int BinaryToInt(char *binary);

int SecondPass(binaryConvertedLines *list, objectTable *ob) {
    binaryConvertedLines *current = list;
    int error = 0;
    while (current) {
        if (current->data[0] == '!') {

            char *object = current->data + 1;
            int address = -1;
	    char *charLineNum;
	    int lineNum;
	    char *charEndLineNum;
	    int endLineNum = -1;
	    objectTableEntry *entry;
	    objectTableExtern *ext;
	    objectTableData *data;

	    current->data++;

	    while(current->data[0] != '!') {
		current->data++;
	    }

	    current->data[0] = '\0';
	    current->data++;
	    charLineNum = current->data;

	    while(current->data[0] != '!' && current->data[0] != '\0') {
		current->data++;
	    }

	    if(current->data[0] == '!') {
		current->data++;
		charEndLineNum = current->data;
	    	endLineNum = atoi(charEndLineNum);
	    }

	    lineNum = atoi(charLineNum);
            
	    /* In case the addressing method is relative addressing */
	    if(endLineNum != -1) {
		address = lineNum - endLineNum;
		if(address < 0) {
		    address = ~address;
		    address += 1;
		    address <<= 11;
		    address >>= 11;
		}
	    }

            /* Search in entries */
            entry = ob->entries;
            while (entry && (address == -1)) {
                if (strcmp(entry->name, object) == 0) {
		    if(endLineNum == -1) {
			address = entry->address;
		    }
		    address >>= 3;
		    address += R;
                }
                entry = entry->next;
            }
            
            /* Search in externs if not found in entries */
            if (address == -1) {
                ext = ob->externs;
                while (ext && (address == -1)) {
                    if (strcmp(ext->name, object) == 0) {
			if(endLineNum == -1) {
			    address = 0;
			}
			address += E;
                    }
                    ext = ext->next;
                }
            }
            
	    /* Search in data if not found in externs */
            if (address == -1) {
                data = ob->data;
                while (data && (address == -1)) {
                    if (strcmp(data->name, object) == 0) {
			if(endLineNum == -1) {
			    address = data->line;
			}
		    	address >>= 3;
		    	address += R;
                    }
                    data = data->next;
                }
            }

            /* If found, replace with address */
            if (address != -1) {
                current->data = IntToHex(address);
		if(current->data == NULL) {
		    printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
		    error = 1;
		}
            }

	    /* If not found, print error messege */
	    else {
		printErrorMsg(LABEL_CALLED_BUT_WAS_NOT_DEFINED, lineNum);
		error = 1;
	    }
        }
	else {
	    current->data = IntToHex(BinaryToInt(current->data));
	    printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
	}
        current = current->next;
    }
    return error;
}


int BinaryToInt(char *binary) {
    int result = 0;
    while (*binary) {
	result <<= 1;
        result += *binary - '0';
        binary++;
    }
    return result;
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
