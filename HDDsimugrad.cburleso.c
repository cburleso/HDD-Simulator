// created by Connor Burleson on 11/29/2019

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#define MAX_TEST_LENGTH 16
#define LINELEN 256

#include "HDDsimugrad.cburleso.h"

// * function descriptions in HDDsimugrad.cburleso.h *

// add a request to the queue
int addRequest(IORequestNode **listP, int trackNum){
    IORequestNode *newRequest;

    newRequest = (IORequestNode *) malloc(sizeof(IORequestNode));
    newRequest->trackNum = trackNum;
    newRequest->next = *listP;
    newRequest->prev = NULL;

    if (*listP != NULL)
        (*listP)->prev = newRequest;

    *listP = newRequest;

    return 0;
}

// print entire request queue
void printRequestQueue(IORequestNode *list){
    IORequestNode *request;

    // if list is empty / NULL
    if (list == NULL){
        printf("[empty]\n");
    } else {
        // otherwise print entire list contents
        printf("[");
        request = list;
        while (request != NULL){
            printf("%d ", request->trackNum);
            request = request->next;
        }
        printf("]\n");
    }
}

// used to reverse d-list in test output
void reverseQueue(IORequestNode **listP){
    IORequestNode *temp = NULL;
    IORequestNode *curr = *listP;

    while(curr != NULL){
        temp = curr->prev;
        curr->prev = curr->next;
        curr->next = temp;
        curr = curr->prev;
    }

    if (temp != NULL)
        *listP = temp->prev;
}

// delete specific request node from the list
void deleteNode(IORequestNode **listP, IORequestNode *currRequest){
    // return nothing if both are null
    if (*listP == NULL || currRequest == NULL)
        return;

    // head node case
    if (*listP == currRequest)
        *listP = currRequest->next;

    // if node to be deleted is last node
    if (currRequest->next != NULL)
        currRequest->next->prev = currRequest->prev;

    // if node to be deleted is not first node
    if (currRequest->prev != NULL)
        currRequest->prev->next = currRequest->next;

    free(currRequest);
}

// service next request from the queue
int serviceNextRequest(IORequestNode **listP, int *headPosition, int *headDirection, SchedulerType schedType, int *displacement){
    IORequestNode *currRequest, *prevRequest, *tempRequest, *iterator, *newIterator;
    int trackVal = 0;
    int lastTrackVal = 0;

    // return -1 if list is empty
    if (*listP == NULL)
        return -1;

    // check to see if request is null / last
    if (currRequest == NULL)
        return -1;
    else {
        // initialize current and previous requests
        currRequest = *listP;
        prevRequest = currRequest->prev;


        // FCFS scheduler type
        if (schedType == SCHEDULER_FCFS) {
            // if first request
            if (prevRequest == NULL) {
                // track value becomes track num of current request
                trackVal = currRequest->trackNum;
                *listP = currRequest->next;   // update head of queue

                // get displacement
                *displacement = abs(trackVal - *headPosition);
                *headDirection = 1;   // head direction always remains 1
                *headPosition = trackVal;  // reset head position to the track value
                return trackVal;
            }

            // set temporary request to current request / node
            tempRequest = currRequest;

            // return next request in queue
            trackVal = tempRequest->trackNum;     // return value becomes next track
            lastTrackVal = prevRequest->trackNum;  // get prev. value from last node

            // update head to point to next node
            *listP = currRequest->next;

            // update head values and direction
            if (trackVal - *headPosition < 0)
                *headDirection = -1;
            else
                *headDirection = 1;

            *headPosition = trackVal;
            *displacement = abs(lastTrackVal - trackVal);  // abs. value of total change in head position
        }

        // SSTF scheduler type
        else if (schedType == SCHEDULER_SSTF) {
            int prevHeadPosition = *headPosition;

            *displacement = *headPosition;
            iterator = currRequest;
            // element nearest to head position
            long nearest = abs(iterator->trackNum - *headPosition);
            int find = iterator->trackNum;

            while (iterator != NULL) {
                if (abs(iterator->trackNum - *headPosition) < nearest) {
                    nearest = abs(iterator->trackNum - *headPosition);
                    find = iterator->trackNum;
                }

                iterator = iterator->next;
            }

            while (currRequest != NULL) {
                if (currRequest->trackNum == find) {
                    deleteNode(listP, currRequest);
                    break;
                }
                currRequest = currRequest->next;
            }

            // update head position based on return track
            if (*headPosition < find)
                *headDirection = 1;
            else
                *headDirection = -1;

            *headPosition = find;
            trackVal = find;
            *displacement = abs(prevHeadPosition - *headPosition);
        }

        // SCAN scheduler type
        else if (schedType == SCHEDULER_SCAN) {
            int prevHeadPosition = *headPosition;

            *displacement = *headPosition;
            iterator = currRequest;
            newIterator = currRequest;
            long nearest = INT_MAX;  // max int
            int find = NULL;

            if(*headDirection == 1) {
                if (iterator->trackNum > *headPosition) {
                    nearest = abs(iterator->trackNum - *headPosition);
                    find = iterator->trackNum;
                }

                while (iterator != NULL) {
                    if (abs(iterator->trackNum - *headPosition) < nearest) {
                        if (iterator->trackNum > *headPosition) {
                            nearest = abs(iterator->trackNum - *headPosition);
                            find = iterator->trackNum;
                        }
                    }
                    iterator = iterator->next;
                }
            } else {
                if (iterator->trackNum <= *headPosition) {
                    nearest = abs(iterator->trackNum - *headPosition);
                    find = iterator->trackNum;
                }

                while (iterator != NULL) {
                    if (abs(iterator->trackNum - *headPosition) < nearest) {
                        if (iterator->trackNum)
                            nearest = abs(iterator->trackNum - *headPosition);
                        find = iterator->trackNum;
                    }
                    iterator = iterator->next;
                }
            }

            if (nearest == INT_MAX || find == NULL){
                if(*headDirection == 1)
                    *headDirection = -1;
                else
                    *headDirection = 1;

                if(*headDirection == -1) {
                    if (newIterator->trackNum > *headPosition) {
                        nearest = abs(newIterator->trackNum - *headPosition);
                        find = newIterator->trackNum;
                    }

                    while (newIterator != NULL) {
                        if (abs(newIterator->trackNum - *headPosition) < nearest) {
                            if (newIterator->trackNum)
                                nearest = abs(newIterator->trackNum - *headPosition);
                            find = newIterator->trackNum;
                        }
                        newIterator = newIterator->next;
                    }
                } else {
                    if (newIterator->trackNum <= *headPosition) {
                        nearest = abs(newIterator->trackNum - *headPosition);
                        find = newIterator->trackNum;
                    }

                    while (newIterator != NULL) {
                        if (abs(newIterator->trackNum - *headPosition) < nearest) {
                            if (newIterator->trackNum)
                                nearest = abs(newIterator->trackNum - *headPosition);
                            find = newIterator->trackNum;
                        }

                        newIterator = newIterator->next;
                    }
                }
            }

            while (currRequest != NULL) {
                if (currRequest->trackNum == find) {
                    deleteNode(listP, currRequest);
                    break;
                }
                currRequest = currRequest->next;
            }

            *headPosition = find;
            trackVal = find;
            *displacement = abs(prevHeadPosition - *headPosition);
        }
    }
    return trackVal; // return value of track
}

// ------------------------------------------- TESTING FUNCTIONS -------------------------------------------------------

int testOne(SchedulerType schedType) {
    IORequestNode *requestQueue;;
    int i, trackNum, headPosition, headDirection, displacement;
    int displacementTotal;
    int requests[4];
    int numRequests;

    requests[0] = 5;
    requests[1] = 3;
    requests[2] = 7;
    requests[3] = 2;
    numRequests = 4;

    requestQueue = NULL;

    headPosition = 0;
    headDirection = 1;

    for (i=0; i<numRequests; ++i)
        addRequest(&requestQueue, requests[i]);

    printRequestQueue(requestQueue);

    // reverse queue for expected output
    reverseQueue(&requestQueue);

    printf("head position = %d; head direction = %d\n", headPosition, headDirection);

    if (schedType == SCHEDULER_SSTF)
        printf("SSTF: order of these should be 2 - 3 - 5 - 7\n");
    else if (schedType == SCHEDULER_FCFS)
        printf("FCFS: order of these should be 5 - 3 - 7 - 2\n");
    else if (schedType == SCHEDULER_SCAN)
        printf("SCAN: order of these should be 2 - 3 - 5 - 7\n");

    displacementTotal = 0;

    trackNum = serviceNextRequest(&requestQueue, &headPosition, &headDirection, schedType, &displacement);

    while (trackNum >= 0) {
        displacementTotal = displacementTotal + displacement;

        printf("next request: %d; displacement = %d; pos = %d  dir = %d\n", trackNum, displacement, headPosition, headDirection);

        trackNum = serviceNextRequest(&requestQueue, &headPosition, &headDirection, schedType, &displacement);
    }

    if (schedType == SCHEDULER_SSTF)
        printf("total displacement should be 7; value is %d\n", displacementTotal);
    else if (schedType == SCHEDULER_FCFS)
        printf("total displacement should be 16; value is %d\n", displacementTotal);
    else if (schedType == SCHEDULER_SCAN)
        printf("total displacement should be 7; value is %d\n", displacementTotal);

    return(0);
}

// --------------------------------------------------------------------------------------------------------------------

int testTwo(SchedulerType schedType) {
    IORequestNode *requestQueue;
    int i, trackNum, headPosition, headDirection, displacement;
    int displacementTotal;
    int requests[10];
    int numRequests;

    requests[0] = 50;
    requests[1] = 57;
    requests[2] = 47;
    requests[3] = 39;
    requests[4] = 65;
    requests[5] = 10;
    requests[6] = 150;
    requests[7] = 15;
    requests[8] = 160;
    requests[9] = 1;
    numRequests = 10;

    headPosition = 48;
    headDirection = 1;

    requestQueue = NULL;

    for (i=0; i<numRequests; ++i)
        addRequest(&requestQueue, requests[i]);

    printRequestQueue(requestQueue);

    // reverse queue for expected output
    reverseQueue(&requestQueue);
    printf("head position = %d; head direction = %d\n", headPosition, headDirection);

    if (schedType == SCHEDULER_SSTF)
        printf("SSTF: order of these should be 47 - 50 - 57 - 65 - 39 - 10 - 1 - 150 - 160\n");
    else if (schedType == SCHEDULER_FCFS)
        printf("FCFS: order of these should be 50 - 57 - 47 - 39 - 65 - 10 - 150 - 15 - 160 - 1\n");
    else if (schedType == SCHEDULER_SCAN)
        printf("SCAN: order of these should be 50 - 57 - 65 - 150 - 160 - 47 - 39 - 15 - 10 - 1\n");

    displacementTotal = 0;

    trackNum = serviceNextRequest(&requestQueue, &headPosition, &headDirection, schedType, &displacement);

    while (trackNum >= 0) {
        displacementTotal = displacementTotal + displacement;
        printf("next request: %d; displacement = %d; pos = %d  dir = %d\n", trackNum, displacement, headPosition, headDirection);
        trackNum = serviceNextRequest(&requestQueue, &headPosition, &headDirection, schedType, &displacement);
    }

    if (schedType == SCHEDULER_SSTF)
        printf("total displacement should be 242; value is %d\n", displacementTotal);
    else if (schedType == SCHEDULER_FCFS)
        printf("total displacement should be 687; value is %d\n", displacementTotal);
    else if (schedType == SCHEDULER_SCAN)
        printf("total displacement should be 271; value is %d\n", displacementTotal);

    return(0);
}

// --------------------------------------------------------------------------------------------------------------------

int testThree(SchedulerType schedType) {
    IORequestNode *requestQueue;
    int i, trackNum, headPosition, headDirection, displacement;
    int displacementTotal;
    int requests[10];
    int numRequests;

    requests[0] = 1;
    requests[1] = 125;
    requests[2] = 50;
    requests[3] = 1;
    requests[4] = 125;
    requests[5] = 50;
    requests[6] = 1;
    requests[7] = 125;
    requests[8] = 50;
    requests[9] = 1;
    numRequests = 10;

    headPosition = 100;
    headDirection = -1;

    requestQueue = NULL;

    for (i=0; i<numRequests; ++i)
        addRequest(&requestQueue, requests[i]);

    printRequestQueue(requestQueue);
    printf("head position = %d; head direction = %d\n", headPosition, headDirection);

    if (schedType == SCHEDULER_SSTF)
        printf("SSTF: order of these should be 125 - 125 - 125 - 50 - 50 - 50 - 1 - 1 - 1 - 1\n");
    else if (schedType == SCHEDULER_FCFS)
        printf("FCFS: order of these should be 1 - 50 - 125 - 1 - 50 - 125 - 1 - 50 - 125 - 1\n");
    else if (schedType == SCHEDULER_SCAN)
        printf("SCAN: order of these should be 50 - 50 - 50 - 1 - 1 - 1 - 1 - 125 - 125 - 125 - 125\n");

    displacementTotal = 0;

    trackNum = serviceNextRequest(&requestQueue, &headPosition, &headDirection, schedType, &displacement);

    while (trackNum >= 0) {
        displacementTotal = displacementTotal + displacement;

        printf("next request: %d; displacement = %d; pos = %d  dir = %d\n", trackNum, displacement, headPosition, headDirection);

        trackNum = serviceNextRequest(&requestQueue, &headPosition, &headDirection, schedType, &displacement);
    }

    if (schedType == SCHEDULER_SSTF)
        printf("total displacement should be 149; value is %d\n", displacementTotal);
    else if (schedType == SCHEDULER_FCFS)
        printf("total displacement should be 843; value is %d\n", displacementTotal);
    else if (schedType == SCHEDULER_SCAN)
        printf("total displacement should be 223; value is %d\n", displacementTotal);

    return(0);
}

// ------------------------------------------ END TESTING FUNCTIONS ----------------------------------------------------

// Experiment Functions

//int getInputs(long *seed, SchedulerType *schedType) {
//    char line[LINELEN];
//    int i, seedOK, schedTypeOK, numvals;
//
//    seedOK = 0;
//    schedTypeOK = 0;
//
//    while ( ! seedOK ) {
//        printf("seed (integer value): ");
//        fgets(line, LINELEN, stdin);
//        line[strlen(line)-1] = '\0'; // chop off the newline char
//        for (i=0; i<strlen(line); ++i) // convert to lower-case
//            line[i] = tolower(line[i]);
//        if (strlen(line) > 0) {
//            numvals = sscanf(line, "%ld", seed);
//            if (numvals != 1) {
//                printf("seed must be an integer value\n");
//            } else {
//                seedOK = 1;
//            }
//        }
//    }
//
//    while ( ! schedTypeOK ) {
//        printf("scheduler (FCFS, SCAN, or SSTF): ");
//        fgets(line, LINELEN, stdin);
//        line[strlen(line)-1] = '\0'; // chop off the newline char
//        for (i=0; i<strlen(line); ++i) // convert to lower-case
//            line[i] = tolower(line[i]);
//        if (strlen(line) > 0) {
//            if ( !strcmp(line, "fcfs") ) {
//                schedTypeOK = 1;
//                *schedType = SCHEDULER_FCFS;
//            } else if ( !strcmp(line, "sstf") ) {
//                schedTypeOK = 1;
//                *schedType = SCHEDULER_SSTF;
//            } else if ( !strcmp(line, "scan") ) {
//                schedTypeOK = 1;
//                *schedType = SCHEDULER_SCAN;
//            } else {
//                printf("scheduler must be 'FCFS' or 'SCAN' or 'SSTF'\n");
//            }
//        }
//    }
//
//    return(0);
//}
//
//int testRandomRequests(
//        SchedulerType schedType,
//        int numRequests,
//        int maxRequest,
//        long seed) {
//    IORequestNode *requestQueue;
//    int i, trackNum;
//    double r, meandpr;
//    int displacementTotal;
//    int headPosition, headDirection, displacement;
//
//    srand48(seed);
//
//    requestQueue = NULL;
//    displacementTotal = 0;
//
//    for (i=0; i<numRequests; ++i) {
//        r = drand48();
//        trackNum = (int) maxRequest * r;
//        addRequest(&requestQueue, trackNum);
//    }
//
//    printRequestQueue(requestQueue);
//
//    headPosition = maxRequest / 2;
//    headDirection = 1;
//
//    printf("[");
//    trackNum = serviceNextRequest(&requestQueue, &headPosition,
//                                  &headDirection, schedType, &displacement);
//    while (trackNum >= 0) {
//        printf(" %d", trackNum);
//        displacementTotal = displacementTotal + displacement;
////  printf("next request: %d; displacement = %d; pos = %d  dir = %d\n",
////         trackNum, displacement, headPosition, headDirection);
//        trackNum = serviceNextRequest(&requestQueue, &headPosition,
//                                      &headDirection, schedType, &displacement);
//    }
//    printf(" ]\n");
//
//    meandpr = (double) displacementTotal / numRequests;
//    printf("%d requests; mean displacement per request = %.2f\n",
//           numRequests, meandpr);
//}

// ---------------------------------------- END EXPERIMENT FUNCTIONS ---------------------------------------------------


int main() {
    SchedulerType schedType;
    long seed;
    int numTrackVals, maxTrackNum;

    numTrackVals = 25;
    maxTrackNum = 200;

    schedType = SCHEDULER_FCFS;
    //schedType = SCHEDULER_SSTF;
    //schedType = SCHEDULER_SCAN;

    // test HDD algorithms using scheduler type

    testOne(schedType);
    printf("\n");
    testTwo(schedType);
    printf("\n");
    testThree(schedType);

    // * USED FOR EXPERIMENTS *

//    printf("\n");
//    printf("random tests\n");
//
//    getInputs(&seed, &schedType);
//
//    printf("seed is %ld; ", seed);
//
//    if (schedType == SCHEDULER_FCFS)
//        printf("scheduler is FCFS\n");
//    else if (schedType == SCHEDULER_SCAN)
//        printf("scheduler is SCAN\n");
//    else if (schedType == SCHEDULER_SSTF)
//        printf("scheduler is SSTF\n");
//    else
//        printf("scheduler is ????\n");
//
//    testRandomRequests(schedType, numRandomVals, maxTrackNum, seed);
}

