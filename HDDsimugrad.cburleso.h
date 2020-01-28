// created by Connor Burleson on 11/29/2019

#ifndef ASSIGNMENT6_HDDSIMUGRAD_CBURLESO_H
#define ASSIGNMENT6_HDDSIMUGRAD_CBURLESO_H

#define NUM_TRACKS 200

typedef enum {
    SCHEDULER_FCFS,
    SCHEDULER_SSTF,
    SCHEDULER_SCAN
} SchedulerType;

typedef struct IORequestNodeStruct {
    int trackNum;
    struct IORequestNodeStruct *prev;
    struct IORequestNodeStruct *next;
} IORequestNode;

// This adds a new request to the request queue. It should return zero unless something really bad happens.
int addRequest(IORequestNode **listP, int trackNum);

// Description from assignment :
// This selects (and deletes from the queue) the next request from the queue that should be serviced. The value of
// schedType determines which request is chosen for service from the request queue. The variables headPosition and
// headDirection are updated after the request is serviced. displacement will have the number of tracks that the head
// moved to service the next request. If the request queue is empty, this function returns -1; otherwise it returns the
// next track that will be serviced.
// If headDirection is 1, then then head is moving towards higher-numbered tracks; if it is -1, then the head is moving
// towards lower-numbered tracks.
int serviceNextRequest(IORequestNode **listP, int *headPosition, int *headDirection,
        SchedulerType schedType, int *displacement);

// This will print out the values in the request queue.
void printRequestQueue(IORequestNode *list);

// delete a node from the queue
void deleteNode(IORequestNode **listP, IORequestNode *currRequest);

// reverse the queue
void reverseQueue(IORequestNode **listP);

// testing functions for HDD sim
int testOne(SchedulerType schedType);
int testTwo(SchedulerType schedType);
int testThree(SchedulerType schedType);

#endif //ASSIGNMENT6_HDDSIMUGRAD_CBURLESO_H
