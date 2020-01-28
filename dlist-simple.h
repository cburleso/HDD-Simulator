//
// Created by CHBADMIN on 11/20/2019.
//

#ifndef ASSIGNMENT6_DLIST_SIMPLE_H
#define ASSIGNMENT6_DLIST_SIMPLE_H
typedef struct DListNodeStruct {
    int value;
    struct DListNodeStruct *next;
    struct DListNodeStruct *prev;
} DListNode;

int insert(DListNode **list, int value);
int find(DListNode *list, int value);
int delete(DListNode **list, int value);
int printList(DListNode *list);

#endif //ASSIGNMENT6_DLIST_SIMPLE_H
