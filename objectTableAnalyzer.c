#include "errorHandler.h"
#include "lineAnalyzer.h"
#include "utils.h"
#include "dataTypes.h"

#include "errorHandler.h"
#include "lineAnalyzer.h"
#include "utils.h"
#include "dataTypes.h"

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
            newNode->name = l->label;
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
            newNode->name = l->label;
            newNode->value = *l->vars;
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
            newNode->name = l->label;
            newNode->value = *l->vars;
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
    } else if(l->vars != NULL && l->vars->varType == 2) {
        objectTableExtern *newNode = (objectTableExtern *)malloc(sizeof(objectTableExtern));
        if(newNode == NULL) {
            ot->error[0] = l->lineNum;
            ot->error[1] = MEMORY_ALLOCATION_ERROR;
            return;
        }
        newNode->name = l->vars->label;
        newNode->line = l->lineNum;
        newNode->next = NULL;

        if(ot->externs == NULL) {
            ot->externs = newNode;
        } else {
            objectTableExtern *temp = ot->externs;
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

    /* Handle entries */
    else if(l->vars != NULL && l->vars->varType == 3) {
        objectTableEntry *newNode = (objectTableEntry *)malloc(sizeof(objectTableEntry));
        if(newNode == NULL) {
            ot->error[0] = l->lineNum;
            ot->error[1] = MEMORY_ALLOCATION_ERROR;
            return;
        }
        newNode->name = l->vars->label;
        newNode->address = l->lineNum;
        newNode->next = NULL;

        if(ot->entries == NULL) {
            ot->entries = newNode;
        } else {
            objectTableEntry *temp = ot->entries;
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
}

objectTable* createObjectTable() {
    objectTable* ot = (objectTable*)malloc(sizeof(objectTable));
    if(ot != NULL) {
        ot->commands = NULL;
        ot->data = NULL;
        ot->externs = NULL;
        ot->entries = NULL;
    }
    return ot;
}