#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libResearcher.h"

#define STRSIZE 1024

// Recieves the string with the rating and updates the list
void updateListRating(productionList_t *list, char *rating)
{
    if (strcmp(rating, "A1") == 0) // Mudar para switch case depois, se possivel
        list->ratings[0]++;
    else if (strcmp(rating, "A2") == 0)
        list->ratings[1]++;
    else if (strcmp(rating, "A3") == 0)
        list->ratings[2]++;
    else if (strcmp(rating, "A4") == 0)
        list->ratings[3]++;
    else if (strcmp(rating, "B1") == 0)
        list->ratings[4]++;
    else if (strcmp(rating, "B2") == 0)
        list->ratings[5]++;
    else if (strcmp(rating, "B3") == 0)
        list->ratings[6]++;
    else if (strcmp(rating, "B4") == 0)
        list->ratings[7]++;
    else if (strcmp(rating, "C") == 0)
        list->ratings[8]++;
    else if (strcmp(rating, "NC") == 0)
        list->ratings[9]++;
}

// Return a pointer to an empty StringList
strList_t *createStrList()
{
    strList_t *list = malloc(sizeof(strList_t));
    if (! list){
        perror("Failed to alloc memory to the strList\n");
        return NULL;
    }

    list->head = NULL;
    list->tail = NULL;

    return list;
}

// Create a new string and add in the List
strNode_t *createStr(strList_t *strList, char *string)
{
    // Alloc memory for the new Node
    strNode_t *retNode = malloc(sizeof(strNode_t));
    if (! retNode){
        perror("Failed to alloc memory to strNode\n");
        return NULL;
    }
    retNode->str = strdup(string);
    retNode->next = NULL;

    // Verify if the list is empty
    if (strList->head == NULL)
        strList->head = retNode;
    else // not empty
        strList->tail->next = retNode;
    strList->tail = retNode;    

    return retNode;
}

// Returns a pointer to an empty productionList
productionList_t *createProductionList()
{
    productionList_t *list = malloc(sizeof(productionList_t));
    if (! list){
        perror("Failed to alloc memory to the productionList\n");
        return NULL;
    }
    list->ratings = calloc(10, sizeof(int));
    if (! list->ratings){
        perror("Failed to alloc memory to the productionList\n");
        return NULL;
    }

    list->head = NULL;
    list->tail = NULL;
    list->quantity = 0;

    return list;
}

// Create a new productionNode and add in the List and return the head of the list
productionNode_t *createProduction(productionList_t *productionsList, char *name, int year, char *rating)
{
    productionNode_t *retNode = malloc(sizeof(productionNode_t));
    if (! retNode){
        perror("Failed to alloc memory to productionNode\n");
        return NULL;
    }
    retNode->name = strdup(name);
    if (! retNode->name){
        perror("Failed to alloc memory to name productioNode\n");
        return NULL;
    }
    retNode->rating = malloc(sizeof(char) * 3);
    if (! retNode->rating){
        perror("Failed to alloc memory to the rating of productionNode\n");
        return NULL;
    }
    strcpy(retNode->rating, rating);

    retNode->year = year;
    retNode->next = NULL;

    // Add the Node into the list
    if (productionsList->head == NULL)
        productionsList->head = retNode;
    else
        productionsList->tail->next = retNode;
    productionsList->tail = retNode;

    updateListRating(productionsList, retNode->rating);
    productionsList->quantity++;

    return retNode;
}

// Returns a pointer to an empety ResearcherList
researcherList_t *createResearcherList()
{
    researcherList_t *list = malloc(sizeof(researcherList_t));
    if (! list){
        perror("Failed to alloc memory to the researcherList\n");
        return NULL;
    }
    list->allPeriodics = createProductionList();
    list->allConferences = createProductionList();
    list->head = NULL;
    list->tail = NULL;
    list->quantity = 0;

    return list;
}

// Create a new researcherNode and add in the List
researcherNode_t *createResearcher(researcherList_t *researchersList, char *name, int *confRatings, int *periodRatings, productionList_t *periodicsList, productionList_t *conferenceList)
{
    researcherNode_t *retNode = malloc(sizeof(researcherNode_t));
    if (! retNode){
        perror("Failed to alloc memory to the researcherNode\n");
        return NULL;
    }
    retNode->name = malloc(sizeof(char) * STRSIZE);
    if (! retNode->name){
        perror("Failed to alloc memory to the name of researcherNode\n");
        return NULL;
    }
    strcpy(retNode->name, name);

    retNode->confRatings = confRatings;
    retNode->periodRatings = periodRatings;
    retNode->conferences = conferenceList;
    retNode->periodics = periodicsList;

    // Include the Node in list
    if (researchersList->head == NULL)
        researchersList->head = retNode;
    else
        researchersList->tail->next = retNode;
    researchersList->tail = retNode;

    // Include periodicsList into allPeriodics
    if (researchersList->allPeriodics->head == NULL) // allPeriodics is empty
        researchersList->allPeriodics->head = periodicsList->head;
    else // allPeriodics is NOT empty
        researchersList->allPeriodics->tail->next = periodicsList->head;
    researchersList->allPeriodics->tail = periodicsList->tail;

    // Include conferenceList into allConferences
    if (researchersList->allConferences->head == NULL)
        researchersList->allConferences->head = conferenceList->head;
    else
        researchersList->allConferences->tail->next = conferenceList->head;
    researchersList->allConferences->tail = conferenceList->tail;

    researchersList->quantity++;

    return retNode;
}