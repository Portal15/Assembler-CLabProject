#include "headerFiles/errorHandler.h"
#include "headerFiles/utils.h"
#include "headerFiles/second.h"
#include "headerFiles/dataTypes.h"

int IsAlpha(char ch);

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

int DecimalCharArrayToInt(char *dec);
 */

int SecondPass(binaryConvertedLines *list, objectTable *ob, externsAppearance *externs) {
    binaryConvertedLines *current = list;
    char* temp;
    int error = 0;
    while (current) {
    	temp = current->data;
        if (current->data != NULL && current->data[0] == '!') {
	        char *object = current->data + 1;
        	int address = -1;
        	char *charLineNum;
        	int lineNum;
        	char *charEndLineNum;
        	int endLineNum = -1;
        	objectTableEntry *entry;
        	objectTableExtern *ext;
        	objectTableData *data;
        	objectTableCommands *command;
			int distance = 0;

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



        	/* Search in commands */
        	command = ob->commands;
        	while (command && (address == -1)) {
        		if (strcmp(command->name, object) == 0) {
        			address = command->address;
        			if(endLineNum != -1) {
					address += 1 - endLineNum;
					distance = 1;
        				address <<= 3;
        				address |= A;
				} else {
        				address <<= 3;
        				address |= R;
				}
        		}
        		command = command->next;
        	}

        	/* Search in entries */
        	entry = ob->entries;
        	while (entry && (address == -1)) {
        		if (strcmp(entry->name, object) == 0) {
        			address = entry->address;
        			if(endLineNum != -1) {
					address += 1 - endLineNum;
					distance = 1;
        				address <<= 3;
        				address |= A;
				} else {
        				address <<= 3;
        				address |= R;
				}
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
        				address |= E;
					if(!externs && !(externs->name)) {
						externs->name = strdup(object);
						externs = externs->next;
						externs->next = NULL;
					}else {
						externsAppearance *newNode = (externsAppearance *)malloc(sizeof(externsAppearance));
						newNode->name = strdup(object);
						externs->next = newNode;
						externs = externs->next;
						externs->next = NULL;
					}
        			}
        			ext = ext->next;
        		}
        	}

        	/* Search in data if not found in externs */
        	if (address == -1) {
        		data = ob->data;
        		while (data && (address == -1)) {
        			if (strcmp(data->name, object) == 0) {
        				address = data->address;
        				if(endLineNum != -1) {
						address += 1 - endLineNum;
						distance = 1;
        					address <<= 3;
        					address |= A;
					} else {
        					address <<= 3;
        					address |= R;
					}
        			}
        			data = data->next;
        		}
        	}

        	/* If found, replace with address */
        	if (address != -1 || distance == 1) {
        		current->data = IntToHex(address);
        		if(current->data == NULL) {
        			printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
        			error = 1;
        		}
        	}else {/* If not found, print error messege */
        		printErrorMsg(LABEL_CALLED_BUT_WAS_NOT_DEFINED, lineNum);
        		error = 1;
        	}
        }else {
			if(current->data != NULL && IsAlpha(current->data[0])) {
			    current->data = IntToHex((int) (current->data[0]));
			} else {
				if (current->data != NULL) {
					current->data = IntToHex(atoi(current->data));
					if(current->data == NULL) {
						printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
						error = 1;
					}
				}
			}
	}
        current = current->next;
    	if (temp != NULL && *temp != '\0') free(temp);
    }
    return error;
}

char* IntToHex(int address) {
    int i;
    unsigned int unsignedAddress;

    if (address < 0) {
	unsignedAddress = (unsigned int)(address & 0xFFFFFF);
    } else {
        unsignedAddress = (unsigned int)address;
    }

    char *hexString = (char *)malloc((Size_Of_Address + 1) * sizeof(char));
    if (hexString == NULL) {
        return NULL;
    }

    for (i = Size_Of_Address - 1; i >= 0; i--) {
        hexString[i] = HEX_DIGITS[unsignedAddress % Digits_In_Hexa];
        unsignedAddress /= Digits_In_Hexa;
    }
    hexString[Size_Of_Address] = '\0';

    return hexString;
}

int IsAlpha(char ch) {
    return ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'));
}