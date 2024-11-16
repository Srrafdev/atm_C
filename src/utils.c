#include "header.h"
#include <ctype.h>
#include <stdio.h>

void clear_buffer() {
    int c;
    printf("(Enter)");
    while ((c = getchar()) != '\n' && c != EOF);
}

int isPhoneValid(int phone){
    if (phone < 100000000 || phone > 1000000000) {
        return 1; 
    }
    return 0;
}

int isAlphabit(const char* text){
     for(int i = 0; i < strlen(text); i++){
        if(isalpha(text[i]) == 0){
            return 1;
        }
    }
    return 0;
}

int isTypeAcountValid(const char *type){
    if(strcmp(type,"saving") != 0 && strcmp(type,"current") != 0 &&
       strcmp(type,"fixed01") != 0 && strcmp(type,"fixed02") != 0 &&
       strcmp(type,"fixed03") != 0){
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
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
        }else{
            printf("\nError reading input.\n");
            break;
        }
        clear_buffer();
    }
}

int getIntInput(const char* text, int size) {
    char buffer[size];

    while (1) {
        printf("%s", text);

        if (fgets(buffer, size, stdin) != NULL) {
            int len = strlen(buffer);
            if (len > 0 && buffer[len - 1] == '\n') {
                buffer[len - 1] = '\0';
            }
            int is = 0;
            for(int i = 0;i < len-1;i++){
                if(isdigit(buffer[i])== 0){
                    is = 1;
                    break;
                }
            }
            if(is == 0 && len > 1){
                clear_buffer();
                return atoi(buffer);
            }else{
                printf("input is not digits.\n");
            }
           
        //    int c;
        //    while ((c = getchar()) != '\n' && c != EOF);
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
                return value;
            }
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
        } else {
            printf("\nError reading input.\n");
            break;
        }
    }
    clear_buffer();
    return 0.0;
}
