#include "headerFiles/errorHandler.h"
#include "headerFiles/utils.h"
#include "headerFiles/second.h"
#include "headerFiles/dataTypes.h"

/*
 * this function gets a character and it checks if it's an alphabetic letter.
 * ch - the char that we want to check.
 * the function returns 1 if ch is an alphabetic letter, and 0 otherwise (as an int).
 */
int IsAlpha(char ch);

/*
 * this function gets an address (that we are assuming that is 24 bits long) and converting it to hexadecimal.
 * address - the 24 bits long integer that we want to convert.
 * the function returns the hexadecimal number as a pointer to a char array (char*).
 */
char* IntToHex(int address);


int SecondPass(binaryConvertedLines *list, objectTable *ob, externsAppearance *externsAppearances) {
    binaryConvertedLines *current = list;
    char* temp;
    int error = 0;
    while (current) {
    	temp = current->data;
        if (current->data != NULL && current->data[0] == '!') {
	        char *label = current->data + 1;
        	int address = -1;
        	char *charLineNum;
        	int lineNum;
        	char *charEndLineNum;
        	int endLineNum = -1;
        	objectTableEntry *entry;
        	objectTableExtern *externs;
        	objectTableData *data;
        	objectTableCommands *command;
		int distance = 0;
		
		/* when a data of a node in list starts with !, its either !label!lineNum or !label!lineNum!EndLineNum
		so we want to get each one of them seperetly */
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


		
        	/* Search label in commands */
        	command = ob->commands;
        	while (command && (address == -1)) {
        		if (strcmp(command->name, label) == 0) {
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

        	/* Search label in entries if it wasn't found yet */
        	entry = ob->entries;
        	while (entry && (address == -1)) {
        		if (strcmp(entry->name, label) == 0) {
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

        	/* Search label in externs if it wasn't found yet 
		 each time it appears, we will save the labels' name in externsAppearances lined list 
		 since we are going on list in order, every label in externsAppearances wiil be there in order 
		 by their relative place to the other externs labels in list */
        	if (address == -1) {
        		externs = ob->externs;
        		while (externs && (address == -1)) {
        			if (strcmp(externs->name, label) == 0) {
        				if(endLineNum == -1) {
        					address = 0;
        				}
        				address |= E;
					externsAppearance *newNode = (externsAppearance *)malloc(sizeof(externsAppearance));
					newNode->name = strdup(label);
					externsAppearances->next = newNode;
					externsAppearances = externsAppearances->next;
					externsAppearances->next = NULL;
					
        			}
        			externs = externs->next;
        		}
        	}

        	/* Search label in data if it wasn't found yet */
        	if (address == -1) {
        		data = ob->data;
        		while (data && (address == -1)) {
        			if (strcmp(data->name, label) == 0) {
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

        	/* If label was found, replace it with address */
        	if (address != -1 || distance == 1) {
        		current->data = IntToHex(address);
        		if(current->data == NULL) {
        			printErrorMsg(MEMORY_ALLOCATION_ERROR, -1);
        			error = 1;
        		}
        	} else {/* If label wasn't found, print error messege */
        		printErrorMsg(LABEL_CALLED_BUT_WAS_NOT_DEFINED, lineNum);
        		error = 1;
        	}
        } else {
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

    /* converting address into its' unsigned form */
    if (address < 0) {
	unsignedAddress = (unsigned int)(address & 0xFFFFFF);
    } else {
        unsignedAddress = (unsigned int)address;
    }

    char *hexString = (char *)malloc((Size_Of_Address + 1) * sizeof(char)); /* the +1 is for the '\0'   */
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