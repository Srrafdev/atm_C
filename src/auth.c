#include "header.h"
#include <ctype.h>
#include <stdio.h>

void clear_buffer() {
    int c;
    printf("(Enter)");
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
            printf("char no valid\n");
        }
        }else{
            printf("Error reading input.\n");
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
                return (int)value;
            }
        } else {
            printf("Error reading input.\n");
            break;
        }
    }
    return 0;
}

double getDoubleInput(const char* prompt) {
    char buffer[100];
    double value;

    while (1) {
        printf("%s", prompt);

        if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
            size_t len = strlen(buffer);
            if (len > 0 && buffer[len - 1] == '\n') {
                buffer[len - 1] = '\0';
            }

            // Convert to double
            char* endptr;
            //errno = 0;
            value = strtod(buffer, &endptr);

            // Check for errors
            // if (endptr != buffer && *endptr == '\0' && errno == 0 && value >= -DBL_MAX && value <= DBL_MAX) {
            //     return value;
            // } else {
            //     printf("Invalid double, please try again.\n");
            // }
        } else {
            printf("Error reading input.\n");
            break;
        }
    }
    return 0.0;
}
