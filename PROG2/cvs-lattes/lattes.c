#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> 
#include <getopt.h>
#include <ctype.h>
#include <dirent.h>
#include <sys/types.h>
#include "libResearcher.h"
#include "libStrDistance.h"

#define STRSIZE 2048

// ./lattes -d lattes-cvs -c qualis-conf.txt -p qualis-periodicos.txt 

// Recieves a file and copies it into a string
char *fileToBuffer(char *file_name)
{
    FILE *file;
    char *buffer;
    int tam;
    
    file = fopen(file_name, "r");
    if (! file){
        printf("Failed to open %s\n", file_name);
        exit(1);
    }

    if (file){
        fseek(file, 0, SEEK_END);
        tam = ftell(file);
        fseek(file, 0, SEEK_SET);
        buffer = malloc ((sizeof(char) * (tam + 1)));
        if (buffer)
            fread(buffer, 1, tam, file);
    }
    fclose(file);

    buffer[tam] = '\0';

    return buffer;
}

// Support function for readDir
void readSubdir(char *dirPath, char *subDirPath, strList_t *xmlList)
{
    DIR *dirStream;
    struct dirent *dirEntry;
    char *currentEntry;
    char *auxDirPath = malloc(sizeof(char) * STRSIZE);

    // subDirPath = lattes-cvs/arquitetura

    dirStream = opendir(subDirPath);
    if (! dirStream){
        printf("Failed to open %s\n", subDirPath);
        exit(1);
    }

    for (;;)
    {
        dirEntry = readdir(dirStream);
        if (! dirEntry)
            break;

        currentEntry = dirEntry->d_name; 
        if ((strcmp(dirEntry->d_name, ".") != 0) && (strcmp(dirEntry->d_name, "..") != 0)){
            auxDirPath = strcpy(auxDirPath, subDirPath);
            auxDirPath = strcat(auxDirPath, "/");
            auxDirPath = strcat(auxDirPath, currentEntry);
            createStr(xmlList, auxDirPath);
        }
    }
    free(auxDirPath);
    (void) closedir(dirStream);
}

// Recieves a directory path and inserts all the names of the files in the string List
void readDir(char *dirPath, strList_t *xmlList)
{
    DIR *dirStream;
    struct dirent *dirEntry;
    char *currentEntry;
    char *subDirPath = malloc(sizeof(char) * STRSIZE);

    dirStream = opendir(dirPath);
    if (! dirStream){
        printf("Failed to open %s\n", dirPath);
        exit(1);
    }

    for (;;)
    {
        dirEntry = readdir(dirStream);
        if (! dirEntry)
            break;

        currentEntry = dirEntry->d_name;
        if ((strcmp(currentEntry, ".") != 0) && (strcmp(currentEntry, "..") != 0)){
            if (dirEntry->d_type == DT_DIR){
                subDirPath = strcpy(subDirPath, dirPath);
                subDirPath = strcat(subDirPath, "/");
                subDirPath = strcat(subDirPath, currentEntry);
                readSubdir(dirPath, subDirPath, xmlList);
            }
        }
    }
    free(subDirPath);
    (void) closedir(dirStream);
}

// Fin the researcher name in the xml
char *findNameXml(char *buffer)
{
    char *ptrBuffer = strdup(buffer);
    char *nameRet = malloc(sizeof(char) * STRSIZE);

    while ( (ptrBuffer = strstr(ptrBuffer, "DADOS-GERAIS")) ){
        if ( (ptrBuffer = strstr(ptrBuffer, "NOME-COMPLETO")) ){
            nameRet = strtok(ptrBuffer, "\"");
            nameRet = strtok(NULL, "\"");
            nameRet = strtok(nameRet, "\"");
            nameRet = strtok(nameRet, "\"");
        }
    }
    free(ptrBuffer);
    return nameRet;
}

// Recieve a string and return the content in uppercase
void setUppercase(char *strIn, char *converted)
{
    int i;
    for(i=0; i < strlen(strIn); i++) {
        converted[i] = toupper(strIn[i]);
    }
    converted[i] = '\0';
}

// Search for the rating of a periodic/conference
char *searchRating(char *qualis, char *name)
{
    char *ptrQualis = strdup(qualis);
    char *upperName = malloc(sizeof(char) * (strlen(name) + 1));
    char upperAux[STRSIZE];
    char *aux;
    int distance;

    setUppercase(name, upperName);
    while ( (ptrQualis = strstr(ptrQualis, "\n")) ){
        aux = strdup(ptrQualis); 
        // setUppercase(aux, upperAux);
        // printf("%s\n", upperAux);
        // distance = levenshtein(aux, name);
        // printf("%d\n", distance);
        if (distance <= (strlen(name)/3)){

        }

        ptrQualis += (strlen(name) + 3);
        free(aux);
    }

    free(ptrQualis);
    return "A1";
}

void findConferencesToList(char *buffer, productionList_t *confList, char *qualis)
{
    char *ptrBuffer = strdup(buffer);
    char *eventYear, *eventName, *eventRating;
    int eventYearInt;

    // Get event data
    while ( (ptrBuffer = strstr(ptrBuffer, "<TRABALHO-EM-EVENTOS")) ) {
        // Get year
        if ( (ptrBuffer = strstr(ptrBuffer, "ANO-DO-TRABALHO=")) ){
            eventYear = strdup(ptrBuffer);  // Tentar de novo colocar isso em uma função -> dificil porque embola com os ponteiros
            eventYear = strtok(eventYear, "\"");
            eventYear = strtok(NULL, "\"");
            eventYear = strtok(eventYear, "\"");
            eventYear = strtok(eventYear, "\"");
            eventYearInt = atoi(eventYear);
        }
        // Get event name
        if ( (ptrBuffer = strstr(ptrBuffer, "NOME-DO-EVENTO=")) ){
            eventName = strdup(ptrBuffer);
            eventName = strtok(eventName, "\"");
            eventName = strtok(NULL, "\"");
            eventName = strtok(eventName, "\"");
            eventName = strtok(eventName, "\"");
        }

        // eventRating = searchRating(qualis, eventName);
        char *eventRating = "A1";

        createProduction(confList, eventName, eventYearInt, eventRating);
    }

    free(ptrBuffer);
}

void findPeriodicsToList(char *buffer, productionList_t *periodicList, char *qualis) // event
{
    char *ptrBuffer = strdup(buffer);
    char *eventYear, *eventName, *eventRating;
    int eventYearInt;

    // Get event data
    while ( (ptrBuffer = strstr(ptrBuffer, "<DADOS-BASICOS-DO-ARTIGO")) ) {
        // Get year
        if ( (ptrBuffer = strstr(ptrBuffer, "ANO-DO-ARTIGO=")) ){
            eventYear = strdup(ptrBuffer);  // Tentar de novo colocar isso em uma função -> dificil porque embola com os ponteiros
            eventYear = strtok(eventYear, "\"");
            eventYear = strtok(NULL, "\"");
            eventYear = strtok(eventYear, "\"");
            eventYear = strtok(eventYear, "\"");
            eventYearInt = atoi(eventYear);
        }
        // Get event name
        if ( (ptrBuffer = strstr(ptrBuffer, "TITULO-DO-PERIODICO-OU-REVISTA=")) ){
            eventName = strdup(ptrBuffer);
            eventName = strtok(eventName, "\"");
            eventName = strtok(NULL, "\"");
            eventName = strtok(eventName, "\"");
            eventName = strtok(eventName, "\"");
        }

        // Get event rating
        // eventRating = searchRating(qualis, eventName);
        char *eventRating = "A1";

        createProduction(periodicList, eventName, eventYearInt, eventRating);
    }

    free(ptrBuffer);
}

void printResearcherList(researcherList_t *rList)
{
    int i;

    printf("\n");
    printf("Quantidade de pesquisadores: %d\n", rList->quantity);

    researcherNode_t *currentResearcher = rList->head;

    while (currentResearcher != NULL){ // Vai passando por todos pesquisadores
        printf("-----------------------------------------------------------------------\n");
        printf("Nome do pesquisador: %s\n", currentResearcher->name);

        printf("Estratos de conferencias: ");
        for (i = 0; i < 10; i++)
            printf("%d ", currentResearcher->confRatings[i]);
        printf("\n");
        printf("Estratos de periodicos: ");
        for (i = 0; i < 10; i++)
            printf("%d ", currentResearcher->periodRatings[i]);
        printf("\n");

        currentResearcher = currentResearcher->next;
    }

    printf("\n");
    printf("Imprimindo TODOS periodicos.\n");

    productionNode_t *currentPeriodic = rList->allPeriodics->head;
    while (currentPeriodic != NULL) {
        printf("----------\n");
        printf("Nome do periodico: %s\n", currentPeriodic->name);
        printf("Ano do periodico: %d\n", currentPeriodic->year);
        printf("Rating do periodico: %s\n", currentPeriodic->rating);
        currentPeriodic = currentPeriodic->next;
    }
}

int main(int argc, char **argv)
{
    int option;
    char *bufferConf, *bufferPeriod, *dirPath;
    strList_t *xmlList = createStrList();

    printf("Tratando entradas...\n");

    dirPath = malloc(sizeof(char) * STRSIZE);
    if (! dirPath)
        return 1;

    while ((option = getopt(argc, argv, "d:c:p:")) != -1){
        switch (option)
        {
        case 'd' :
            dirPath = optarg;
            break;
        
        case 'c' :
            bufferConf = fileToBuffer(optarg);
            break;

        case 'p' :
            bufferPeriod = fileToBuffer(optarg);
            break;

        default:
            printf("Sem opção de chamada.\n");
            exit(1);
            break;
        }
    }

    // Verify if all the arguments are being called
    if (argc < 6){
        printf("Chamada incorreta.\n");
        return 1;
    }

    // Inserts all file names inside the directory into the xmlList
    readDir(dirPath, xmlList);

    strNode_t *current = xmlList->head;
    char *currentXml; 

    // Researcher Variables
    researcherList_t *allResearchersList = createResearcherList();
    char *rName;
    productionList_t *rConferencesList;
    productionList_t *rPeriodicsList;

    printf("Procurando por dados nos arquivos xml...\n");
    while (current != NULL){ // Read until the end of the list
        currentXml = fileToBuffer(current->str);

        // Each loop is a diferent xml file
        // Each loop creates a new list of conferences and a new list of periodics

        rConferencesList = createProductionList();
        rPeriodicsList = createProductionList();

        // Look for data to insert on reseacherNode
        rName = findNameXml(currentXml);
        findConferencesToList(currentXml, rConferencesList, bufferConf);
        findPeriodicsToList(currentXml, rPeriodicsList, bufferPeriod);

        // Insert the data into the allResearchersList
        createResearcher(allResearchersList, rName, rConferencesList->ratings, rPeriodicsList->ratings, rPeriodicsList, rConferencesList);

        current = current->next;
    }

    printResearcherList(allResearchersList);
}