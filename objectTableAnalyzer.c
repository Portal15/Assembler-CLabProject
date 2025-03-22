#include "headerFiles/errorHandler.h"
#include "headerFiles/utils.h"
#include "headerFiles/dataTypes.h"

var* varDupe(var *v);

void addLineToObjectTable(line *l, objectTable *ot) {
    if(l->label != NULL) {
        /* Handle commands */
        if(l->cmd != NULL) {
            objectTableCommands *newNode = (objectTableCommands *)malloc(sizeof(objectTableCommands));
            if(newNode == NULL) {
                ot->error[0] = l->lineNum;
                ot->error[1] = MEMORY_ALLOCATION_ERROR;
                return;
            }
            newNode->name = strdup(l->label);
            newNode->line = l->lineNum;
            newNode->next = NULL;

            if(ot->commands == NULL) {
                ot->commands = newNode;
            } else {
                objectTableCommands *temp = ot->commands;
                if (strcmp(temp->name, l->label) == 0) {
                    ot->error[0] = l->lineNum;
                    ot->error[1] = LABEL_NAME_APPEAR_TWICE_ERROR;
                }
                while(temp->next != NULL) {
                    temp = temp->next;
                    if (strcmp(temp->name, l->label) == 0) {
                        ot->error[0] = l->lineNum;
                        ot->error[1] = LABEL_NAME_APPEAR_TWICE_ERROR;
                    }
                }
                temp->next = newNode;
            }
        }

        /* Handle data (numbers) */
        if(l->vars != NULL && l->vars->varType == 0) {
            objectTableData *newNode = (objectTableData *)malloc(sizeof(objectTableData));
            if(newNode == NULL) {
                ot->error[0] = l->lineNum;
                ot->error[1] = MEMORY_ALLOCATION_ERROR;
                return;
            }
            newNode->name = strdup(l->label);
            newNode->value = varDupe(l->vars);
            newNode->line = l->lineNum;
            newNode->next = NULL;

            if(ot->data == NULL) {
                ot->data = newNode;
            } else {
                objectTableData *temp = ot->data;
                if (strcmp(temp->name, l->label) == 0) {
                    ot->error[0] = l->lineNum;
                    ot->error[1] = LABEL_NAME_APPEAR_TWICE_ERROR;
                }
                while(temp->next != NULL) {
                    temp = temp->next;
                    if (strcmp(temp->name, l->label) == 0) {
                        ot->error[0] = l->lineNum;
                        ot->error[1] = LABEL_NAME_APPEAR_TWICE_ERROR;
                    }
                }
                temp->next = newNode;
            }
        }

        /* Handle data (letters) */
        else if(l->vars != NULL && l->vars->varType == 1) {
            objectTableData *newNode = (objectTableData *)malloc(sizeof(objectTableData));
            if(newNode == NULL) {
                ot->error[0] = l->lineNum;
                ot->error[1] = MEMORY_ALLOCATION_ERROR;
                return;
            }
            newNode->name = strdup(l->label);
            newNode->value = varDupe(l->vars);
            newNode->line = l->lineNum;
            newNode->next = NULL;

            if(ot->data == NULL) {
                ot->data = newNode;
            } else {
                objectTableData *temp = ot->data;
                if (strcmp(temp->name, l->label) == 0) {
                    ot->error[0] = l->lineNum;
                    ot->error[1] = LABEL_NAME_APPEAR_TWICE_ERROR;                }
                while(temp->next != NULL) {
                    temp = temp->next;
                    if (strcmp(temp->name, l->label) == 0) {
                        ot->error[0] = l->lineNum;
                        ot->error[1] = LABEL_NAME_APPEAR_TWICE_ERROR;                    }
                }
                temp->next = newNode;
            }
        }

        /* Handle externals */
    }
    if(l->vars != NULL && l->vars->varType == 2) {
        objectTableExtern *newNode = (objectTableExtern *)malloc(sizeof(objectTableExtern));
        if(newNode == NULL) {
            ot->error[0] = l->lineNum;
            ot->error[1] = MEMORY_ALLOCATION_ERROR;
            return;
        }
        newNode->name = strdup(l->vars->label);
        newNode->line = l->lineNum;
        newNode->next = NULL;

        if(ot->externs == NULL) {
            ot->externs = newNode;
        } else {
            objectTableExtern *temp = ot->externs;
            if (l->label != NULL && strcmp(temp->name, l->label) == 0) {
                ot->error[0] = l->lineNum;
                ot->error[1] = LABEL_NAME_APPEAR_TWICE_ERROR;
            }
            while(temp->next != NULL) {
                temp = temp->next;
                if (strcmp(temp->name, l->label) == 0) {
                    ot->error[0] = l->lineNum;
                    ot->error[1] = LABEL_NAME_APPEAR_TWICE_ERROR;
                }
            }
            temp->next = newNode;
        }
    }

    /* Handle entries */
    if(l->vars != NULL && l->vars->varType == 3) {
        objectTableEntry *newNode = (objectTableEntry *)malloc(sizeof(objectTableEntry));
        if(newNode == NULL) {
            ot->error[0] = l->lineNum;
            ot->error[1] = MEMORY_ALLOCATION_ERROR;
            return;
        }
        newNode->name = strdup(l->vars->label);
        newNode->line = l->lineNum;
        newNode->next = NULL;

        if(ot->entries == NULL) {
            ot->entries = newNode;
        } else {
            objectTableEntry *temp = ot->entries;
            if (l->label != NULL && strcmp(temp->name, l->label) == 0) {
                ot->error[0] = l->lineNum;
                ot->error[1] = LABEL_NAME_APPEAR_TWICE_ERROR;
            }
            while(temp->next != NULL) {
                temp = temp->next;
                if (strcmp(temp->name, l->label) == 0) {
                    ot->error[0] = l->lineNum;
                    ot->error[1] = LABEL_NAME_APPEAR_TWICE_ERROR;
                }
            }
            temp->next = newNode;
        }
    }
}

objectTable* createObjectTable() {
    objectTable* ot = (objectTable*)malloc(sizeof(objectTable));
    if(ot != NULL) {
        ot->commands = NULL;
        ot->data = NULL;
        ot->externs = NULL;
        ot->entries = NULL;
        ot->error[0] = 0;
        ot->error[1] = 0;
    }
    return ot;
}

void fillAddresses(objectTable* ot, const int* invisibleLineNum, int lineNum) {
    objectTableCommands* command = ot->commands;
    objectTableData* data = ot->data;
    objectTableExtern* ext = ot->externs;
    objectTableEntry* entry = ot->entries;
    while (entry != NULL) {
        if (entry->line == lineNum) {
            entry->address = *invisibleLineNum;
        }
        entry = entry->next;
    }
    while (ext != NULL) {
        if (ext->line == lineNum) {
            ext->address = *invisibleLineNum;
        }
        ext = ext->next;
    }
    while (data != NULL) {
        if (data->line == lineNum) {
            data->address = *invisibleLineNum;
        }
        data = data->next;
    }
    while (command != NULL) {
        if (command->line == lineNum) {
            command->address = *invisibleLineNum;
        }
        command = command->next;
    }
}

int checkForDoubles(objectTable* ot) {
    objectTableEntry* entry = ot->entries;
    objectTableExtern* ext = ot->externs;
    objectTableData* data = ot->data;
    objectTableCommands* command = ot->commands;
    while (entry != NULL) {
        while (data != NULL) {
            if (data->name != NULL && entry->name != NULL && strcmp(entry->name, data->name) == 0) {
                entry->address = data->address;
            }
            data = data->next;
        }
        data = ot->data;
        while (command != NULL) {
            if (command->name != NULL && entry->name != NULL && strcmp(entry->name, command->name) == 0) {
                entry->address = command->address;
            }
            command = command->next;
        }
        command = ot->commands;

        entry = entry->next;
    }
    while (ext != NULL) {
        while (data != NULL) {
            if (data->name != NULL && ext->name != NULL && strcmp(ext->name, data->name) == 0) {
                printErrorMsg(LABEL_NAME_APPEAR_TWICE_ERROR, data->line);
                return 1;
            }
            data = data->next;
        }
        data = ot->data;
        while (command != NULL) {
            if (command->name != NULL && ext->name != NULL && strcmp(ext->name, command->name) == 0) {
                printErrorMsg(LABEL_NAME_APPEAR_TWICE_ERROR, command->line);
                return 1;
            }
            command = command->next;
        }
        command = ot->commands;

        ext = ext->next;
    }
    return 0;

}

var* varDupe(var *v) {
    if (v == NULL) return NULL;

    var* newVar = (var*)malloc(sizeof(var));
    if(newVar != NULL) {
        newVar->varType = v->varType;
        newVar->label = strdup(v->label);
        newVar->digit = strdup(v->digit);
        newVar->letter = v->letter;
        newVar->next = varDupe(v->next);
    }
    return newVar;
}
