#ifndef _LIB_RESEARCHER_H_
#define _LIB_RESEARCHER_H_

// Structs list of strings
typedef struct strNode {
    char *str;
    struct strNode *next;
} strNode_t;

typedef struct strList {
    strNode_t *head;
    strNode_t *tail;
} strList_t;

// Struct list of productions (conferences/periodics)
typedef struct productionNode {
    char *name;
    char *rating; // 2 + 1 char
    int year;
    struct productionNode *next;
} productionNode_t;

typedef struct productionList {
    int quantity;
    int *ratings; // Vector with 10 ratings: A1, A2, A3, A4, B1, B2, B3, B4, C, NC
    productionNode_t *head;
    productionNode_t *tail;
} productionList_t;

// Struct list of researchers
typedef struct researcherNode {
    char *name;
    int *confRatings;
    int *periodRatings;
    productionList_t *periodics;
    productionList_t *conferences;
    struct researcherNode *next;
} researcherNode_t;

typedef struct researcherList {
    int quantity;
    productionList_t *allPeriodics;
    productionList_t *allConferences;
    researcherNode_t *head;
    researcherNode_t *tail;
} researcherList_t;

// Return a pointer to an empty StringList
strList_t *createStrList();

// Create a new string and add in the List
strNode_t *createStr(strList_t *list, char *string);

void freeStrList(strList_t *list);

// Returns a pointer to an empty productionList
productionList_t *createProductionList();

// Create a new productionNode and add in the List
productionNode_t *createProduction(productionList_t *list, char *name, int year, char *rating);

// Returns a pointer to an empety ResearcherList
researcherList_t *createResearcherList();

// Create a new researcherNode and add in the List
researcherNode_t *createResearcher(researcherList_t *researchersList, char *name, int *confRatings, int *periodRatings, productionList_t *periodicsList, productionList_t *conferenceList);

// All Free Functions

#endif