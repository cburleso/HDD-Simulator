//
// Created by CHBADMIN on 11/20/2019.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dlist-simple.h"


// insert value at end

int insert(DListNode **listP, int value) {
    DListNode *currNode, *prevNode, *newNode;

    newNode = (DListNode *) malloc(sizeof(DListNode));
    newNode->value = value;
    newNode->next = *listP;
    newNode->prev = NULL;
    if (*listP != NULL)
        (*listP)->prev = newNode;
    *listP = newNode;
    return(0);
}

//=====================================================================

int printList(DListNode *list) {
    DListNode *node;
    if (list == NULL)
        printf("[empty]\n");
    else {
        printf("[ ");
        node = list;
        while (node != NULL) {
            printf("%d ", node->value);
            node = node->next;
        }
        printf("]\n");
    }

    return(0);
}

//=====================================================================
// find and return the first instance of the matching value; if it's
// not found, then return -1; if it is found, return 0;

int find(DListNode *list, int value) {
    DListNode *currNode;
    if (list == NULL)
        return(-1);

    currNode = list;
    while (currNode != NULL) {
        if (currNode->value == value)
            return(0);
        else
            currNode = currNode->next;
    }

    return(-1);
}

//=====================================================================
// delete ALL instances of the matching value
// if at least one instance was found, return 0; else return 1

int delete(DListNode **list, int value) {
    DListNode *currNode, *prevNode, *nextNode;
    int rtnval;

    if (*list == NULL)
        return(1);

    rtnval = 1;
    prevNode = NULL;
    currNode = *list;
    while (currNode != NULL) {
        nextNode = currNode->next;
        if (currNode->value == value) {
            // delete currNode
            rtnval = 0;
            if (prevNode != NULL) {
                prevNode->next = currNode->next;
                if (currNode->next != NULL)
                    currNode->next->prev = prevNode;
            } else {
                *list = currNode->next;
            }
            free(currNode);
        } else {
            prevNode = currNode;
        }
        currNode = nextNode;
    }

    return(rtnval);
}


