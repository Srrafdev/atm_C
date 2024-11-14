#include "header.h"
#include <ctype.h>
#include <stdio.h>

void clear_buffer() {
    int c;
    printf("(Enter)");
    while ((c = getchar()) != '\n' && c != EOF);
}

int isPhoneValid(int phone){
    if (phone >= 100000000 && phone <= 999999999) {
        return 1; 
    }
    return 0;
}

void Menuorexite(struct User u){
    int option;
    option = getIntInput("\nEnter 1 to go to the main menu and 0 to exit!\n",3);

    system("clear");
    if (option == 1){
        mainMenu(u);
    }else if (option == 0){
        exit(1);
    }else{
        printf("\nInsert a valid operation!\n");
        exit(0);
    }
}

void success(struct User u){
    int option;
    printf("\n✔ Success!\n\n");
    option = getIntInput("\nEnter 1 to go to the main menu and 0 to exit!\n",3);

    system("clear");
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

void getCharInput(const char* text, char* buffer, int size){ 
    while (1){
    printf("%s",text);
    if (fgets(buffer, size, stdin) != NULL) {
        int len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }
        int is = 0;
        for(int i = 0;i < len-1; i++){
            if(!isprint(buffer[i])){
                is = 1;
                break;
            }
        }
        if(is == 0 && len > 1){
            clear_buffer();
            return;
        }else{
            printf("\nchar no valid\n");
        }
        }else{
            printf("\nError reading input.\n");
            break;
        }
        clear_buffer();
    }
}

int getIntInput(const char* text, int size) {
    char buffer[size];
    long value;

    while (1) {
        printf("%s", text);

        if (fgets(buffer, size, stdin) != NULL) {
            int len = strlen(buffer);
            if (len > 0 && buffer[len - 1] == '\n') {
                buffer[len - 1] = '\0';
            }

            char* endptr;
            value = strtol(buffer, &endptr, 10);

            if (endptr == buffer) {
                printf("input is not digits.\n");
            } else if (*endptr != '\0') {
                printf("invalid input: %c\n", *endptr);
            }else{
                clear_buffer();
                return (int)value;
            }
        } else {
            printf("\nError reading input.\n");
            break;
        }
    }
    clear_buffer();
    return 0;
}

double getDoubleInput(const char* text, int size) {
     char buffer[size];
    double value;

    while (1) {
        printf("%s", text);

        if (fgets(buffer, size, stdin) != NULL) {
            int len = strlen(buffer);
            if (len > 0 && buffer[len - 1] == '\n') {
                buffer[len - 1] = '\0';
            }

            char* endptr;
            value = strtod(buffer, &endptr);

            if (endptr == buffer) {
                printf("input is not digits.\n");
            } else if (*endptr != '\0') {
                printf("invalid input: %c\n", *endptr);
            }else{
                clear_buffer();
                return (int)value;
            }
        } else {
            printf("\nError reading input.\n");
            break;
        }
    }
    clear_buffer();
    return 0.0;
}
