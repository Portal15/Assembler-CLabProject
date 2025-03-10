#include "headerFiles/errorHandler.h"
#include "headerFiles/utils.h"
#include "headerFiles/second.h"
#include "headerFiles/dataTypes.h"

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
int DecimalCharArrayToInt(char *dec);

int SecondPass(binaryConvertedLines *list, objectTable *ob, externsAppearance *externs) {
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
        	objectTableCommands *command;

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

        	/* Search in commands */
        	command = ob->commands;
        	while (command && (address == -1)) {
        		if (strcmp(command->name, object) == 0) {
        			if(endLineNum == -1) {
        				address = command->line;
        			}
        			address <<= 3;
        			address |= R;
        		}
        		command = command->next;
        	}

        	/* Search in entries */
        	entry = ob->entries;
        	while (entry && (address == -1)) {
        		if (strcmp(entry->name, object) == 0) {
        			if(endLineNum == -1) {
        				address = entry->line;
        			}
        			address <<= 3;
        			address |= R;
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
						externs->name = object;
						externs = externs->next;
					}else {
						externsAppearance *newNode = (externsAppearance *)malloc(sizeof(externsAppearance));
						newNode->name = object;
						externs->next = newNode;
						externs = externs->next;
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
        				if(endLineNum == -1) {
        					address = data->line;
        				}
        				address <<= 3;
        				address |= R;
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
        	}else {/* If not found, print error messege */
        		printErrorMsg(LABEL_CALLED_BUT_WAS_NOT_DEFINED, lineNum);
        		error = 1;
        	}
        }else {
			current->data = IntToHex(DecimalCharArrayToInt(current->data));
			if(current->data == NULL) {
				printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
				error = 1;
			}		
		}
        current = current->next;
    }
    return error;
}


int DecimalCharArrayToInt(char *dec) {
    int result = 0;
    int negative = 0;

    if(*dec == '-') {
	dec++;
	negative = 1;	
    }
    while (*dec) {
        result = result * 10 + (*dec - '0');
        dec++;
    }
    if(negative) {
	result = result;
	result++;
    }

    return result;
}

char* IntToHex(int address) {
    int i;
    unsigned int unsignedAddress;

    if (address < 0) {
        unsignedAddress = (1U << 24) + address;
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