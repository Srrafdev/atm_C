#include "header.h"
#include <ctype.h>
#include <stdio.h>

void clear_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int isStrValid(const char *str){
    int i;
    if(strlen(str) > 49){
        return 0;
    }
    for (i = 0; i < strlen(str); i++){
        if (isalpha(str[i]) == 0){
            return 0;
        }
    }
    return 1;
}

int isPhoneValid(int phone){
    if (phone >= 100000000 && phone <= 999999999) {
        return 1; 
    }
    return 0;
}

int isCountryValid(char *country){
    int length = strlen(country);

    if (length == 0 || length > 49) {
        return 1;
    }
    for (int i = 0; i < strlen(country); i++){
        if (isalpha(country[i]) == 0){
            return 1;
        }
    }
    return 0;
}

void Menuorexite(struct User u){
    int option;
    printf("\nEnter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    clear_buffer();
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("\nInsert a valid operation!\n");
        exit(0);
    }
}

void success(struct User u){
    int option;
    printf("\n✔ Success!\n\n");
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
    system("clear");
    clear_buffer();
    if (option == 1)
    {
        mainMenu(u);
    }
    else if (option == 0)
    {
        exit(1);
    }
    else
    {
        printf("\nInsert a valid operation!\n");
        exit(0);
    }
}

void getInput(char* buffer, int bufferSize) { 
    if (fgets(buffer, bufferSize, stdin) != NULL) {
        
        int len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
    }
    clear_buffer();
}

