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

void SecondPass(ConvertedLines *list, objectTable *ob) {
    ConvertedLines *current = list;
    while (current) {
        if (current->data[0] == '!') {
            char *object = current->data + 1;
            int address = -1;
            
            /* Search in entries */
            objectTableEntry *entry = ob->entries;
            while (entry) {
                if (strcmp(entry->name, object) == 0) {
                    address = entry->address;
                    break;
                }
                entry = entry->next;
            }
            
            /* Search in externs if not found in entries */
            if (address == -1) {
                objectTableExtern *ext = ob->externs;
                while (ext) {
                    if (strcmp(ext->name, object) == 0) {
                        address = 1;
                        break;
                    }
                    ext = ext->next;
                }
            }
            
            /* If found, replace with address */
            if (address != -1) {
                free(current->data);
                current->data = IntToHex(address);
            }
	    /* TODO: error messege (how to find line???)*/
	    else {
		
	    }
        }
        current = current->next;
    }
}


char* IntToHex(int address) {
    int i;
    char *hexString = (char *)malloc(7 * sizeof(char));
    if (hexString != NULL) {
        for (i = 5; i >= 0; i--) {
            hexString[i] = HEX_DIGITS[address % 16];
            address >>= 4;
        }
        hexString[6] = '\0';
    }
    return hexString;
}
