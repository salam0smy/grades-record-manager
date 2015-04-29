//
//  main.c
//  GradesRead
//
//  Created by Salam alyahya on 2014-12-28.
//  Copyright (c) 2014 Salam alyahya. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE 1000

void loadGrades(char*, long*, float*, char**, int*);
void printList(long *ids, float *grades, char **names, int count);
void swapIds(long *ids, int i, int j);
void swapGrades(float *grades, int i, int j);
void swapNames(char **names, int i, int j);
void sortByIds(long *ids, float *grades, char **names, int count);
void sortByGrade(long *ids, float *grades, char **names, int count, int increase);
void sortByName(long *ids, float *grades, char **names, int count);
float findAverage(float *grades, int count);
int idExists(long newID, long *ids, int size);
void addNewEntery(long *ids, float *grades, char **names, int *count);
void saveToFile(char* fileName, long *ids, float *grades, char **names, int count);
void printMenu(char* fileName);
int getUserMenuChoise();
void printGrade(long id, float grade, char* name, int index);
int findGradeById(long *ids, int size);

int main(void) {
    // declare the variables and perralell arrays
    char fileName[] = "grades.txt";
    long ids[SIZE];// array of id number
    float grades[SIZE]; // array of grades
    char *names[SIZE]; // array of names
    int count = 0;// arrays' current size
    
    // load the arrays from data from the file name
    printf("\nLoading Data. . .\n");
    loadGrades(fileName, ids, grades, names, &count);
    printf("\nFinished loading\n");
    
    int input = 0;
    do{
        // print the menu and get user input
        printMenu(fileName);
        input = getUserMenuChoise();
        switch (input) {
            case 1:
                printf("\n\tAdd a Grade\n");
                addNewEntery(ids, grades, names, &count);
                printf("Grade added.\n");
                printGrade(ids[count-1], grades[count-1], names[count-1], count-1);
                break;
            case 2:
            {
                printf("\n\tFind a Grade\n");
                int index = findGradeById(ids, count);
                if (index > 0) {
                    printGrade(ids[index], grades[index], names[index], index);
                }
                break;
            }
            case 3:{
                printf("\n\tSort by ID\n");
                sortByIds(ids, grades, names, count);
                printList(ids, grades, names, count);
                break;
                }
            case 4:{
                printf("\n\tSort by Grade (low to high)\n");
                sortByGrade(ids, grades, names, count, 1);
                printList(ids, grades, names, count);
                break;
            }
            case 5:{
                printf("\n\tSort by Grade (high to low)\n");
                sortByGrade(ids, grades, names, count, 0);
                printList(ids, grades, names, count);
                break;
            }
            case 6:{
                printf("\n\tSort by Name\n");
                sortByName(ids, grades, names, count);
                printList(ids, grades, names, count);
                break;
            }
            case 7:{
                printf("\n\tFind the Average\n");
                float avg = findAverage(grades, count);
                printList(ids, grades, names, count);
                printf("\n\t average: %f", avg);
                break;
            }
            case 8:{
                printf("Saving to file %s . . .\n", fileName);
                saveToFile(fileName, ids, grades, names, count);
                printf("Grades saved to file.\n");
                break;
            }
            case 9:
                    printf("\n\tExiting\n");
                break;
                
            default:
                break;
        }
        
        printf("\nPress enter to continue...");
        getchar();
        getchar();
    }while (input !=9);

    return 0;
}

// function that reads the file, and parse the data into the arrays
void loadGrades(char *fileName, long *ids, float *grades, char **names, int* count){
    FILE *mFile = fopen(fileName, "r");
    
    if (!mFile) {
        printf("File doesn't exists\n");
        return;
    }
    
    char line[256];
    int i = 0; // line number
    while (fgets(line, sizeof(line), mFile)) {
        names[i] = (char*) malloc( sizeof(char)*50);
        sscanf(line, "%lu %a %49s", &ids[i], &grades[i], names[i]);// get data from the line
        i++;// next line
    }
    *count = i; // number of elements is the number of lines
    fclose(mFile);
}

// prints the whole list of grades
void printList(long *ids, float *grades, char **names, int count){
    printf("\n");
    for (int i=0; i<count; i++) {
        printGrade(ids[i], grades[i], names[i], i);
    }
}

// prints a formatted grade
void printGrade(long id, float grade, char* name, int index){
    printf("\n%i- %lu\t%f\t%s", index, id, grade, name);
}

// helper function to swap elements
void swapIds(long *ids, int i, int j){
    long temp = ids[i];
    ids[i] = ids[j];
    ids[j] = temp;
}

// helper function to swap elements
void swapGrades(float *grades, int i, int j){
    float temp = grades[i];
    grades[i] = grades[j];
    grades[j] = temp;
}

// helper function to swap elements
void swapNames(char **names, int i, int j){
    char* temp = names[i];
    names[i] = names[j];
    names[j] = temp;
}

// selection sort by id number
void sortByIds(long *ids, float *grades, char **names, int count){
    for (int i=0; i<count-1; i++) {
        int min;
        min = i;
        for (int j=i+1; j<count; j++) {
            if (ids[j] < ids[min]) {
                min = j;
            }
        }
        swapIds(ids, i, min);
        swapGrades(grades, i, min);
        swapNames(names, i, min);
    }
}

// selection sort by grade, increasing or decreasing
void sortByGrade(long *ids, float *grades, char **names, int count, int increase){
    for (int i=0; i<count-1; i++) {
        int min;
        min = i;
        for (int j=i+1; j<count; j++) {
            if (increase && grades[j] < grades[min]) {
                min = j;
            }
            if (!increase && grades[j] > grades[min]) {
                min = j;
            }
        }
        swapIds(ids, i, min);
        swapGrades(grades, i, min);
        swapNames(names, i, min);
    }
}

// selection sort alpabatically
void sortByName(long *ids, float *grades, char **names, int count){
    for (int i=0; i<count-1; i++) {
        int min;
        min = i;
        for (int j=i+1; j<count; j++) {
            if (strcmp(names[j], names[min]) < 0) {
                min = j;
            }
            
        }
        swapIds(ids, i, min);
        swapGrades(grades, i, min);
        swapNames(names, i, min);
    }
}

// calculate the average of the grades
float findAverage(float *grades, int count){
    float val = 0;
    for (int i=0; i<count; i++) {
        val+=grades[i];// add all grades
    }
    return val/count;// devide by the count
}

// findout if an id number exists, if it does it returns its index, if not return -1
int idExists(long newID, long *ids, int size){
    
    for (int i=0; i<size; i++) {
        if (newID == ids[i]) {
            return i;// return the index if it was found
        }
    }
    
    return -1;// faild to find a match
}


void addNewEntery(long *ids, float *grades, char **names, int *count){
    
    // get pointers to new elements in the arrays
    long *newID = &ids[*count];
    float *newGrade = &grades[*count];
    
    names[*count] = (char*) malloc( sizeof(char)*50);
    char *newName = names[*count];
    
    // get the new values, and validate if nessecery
    printf("Enter the following:\n");
    printf("ID number: ");
    scanf("%lu", newID);
    while (idExists(*newID, ids, *count)>-1) {
        printf("ID number already exists, please enter a new ID.\n");
        printf("ID number: ");
        scanf("%lu", newID);
    }
    printf("Grade: ");
    scanf("%f", newGrade);
    
    printf("Name: ");
    scanf("%s", newName);
    
    
    (*count)+=1;// increment elements counter
}

// write the data arrays to the file
void saveToFile(char* fileName, long *ids, float *grades, char **names, int count){
    FILE *mFile = fopen(fileName, "w+");
    
    for (int i=0; i<count; i++) {
        fprintf(mFile, "%lu %f %s\n", ids[i], grades[i], names[i]);
    }
    fclose(mFile);
    
}

// prints the program's menu
void printMenu(char* fileName){
    printf("\n\n---\t---\t---\t---\t---\n\n");
    printf("Handling file '%s' of grades, Ids, and names.\n\n", fileName);
    printf("Choose from the following: \n");
    printf("1. Add a grade\n");
    printf("2. Find a grade\n");
    printf("3. Sort by ID\n");
    printf("4. Sort by Grade (low to high)\n");
    printf("5. Sort by Grade (high to low)\n");
    printf("6. Sort by Name\n");
    printf("7. Find the average\n");
    printf("8. Save to file\n");
    printf("9. Exit\n\n");
}

// returns the user menu choise, and validate that input is in rage of menu
// it reads the first char input and convert to int
// only the first char is taken into account, the rest are ignored
int getUserMenuChoise(){
    int input = 0;
    char inputS[2];
    printf("Enter your choise: ");
    scanf("%s", inputS);
    input = inputS[0] - '0';
    while (input<1 || input >9) {
        printf("Please enter a valid number.\n");
        printf("Enter your choise: ");
        scanf("%s", inputS);
        input = (int) (inputS[0] - '0');
    }
    return input;
}

int findGradeById(long *ids, int size){
    long findID;
    int index = -1;
    printf("Enter ID number: ");
    scanf("%lu", &findID);
    index = idExists(findID, ids, size);
    if (index>0) {
        printf("Grade found\n");
    }
    else{
        printf("Grade doesn't exists, check ID.\n");
    }
    return index;
}