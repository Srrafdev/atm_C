#include "header.h"
#include <stdio.h>
#include <sqlite3.h>
#include <ctype.h>
#include <termios.h>



void clear_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}


void mainMenu(struct User u){
    int option;
    
    system("clear");
    printf("\n\n\t\t======= ATM =======\n\n");
    printf("\n\t\t-->> Feel free to choose one of the options below <<--\n");
    printf("\n\t\t[1]- Create a new account\n");
    printf("\n\t\t[2]- Update account information\n");
    printf("\n\t\t[3]- Check accounts\n");
    printf("\n\t\t[4]- Check list of owned account\n");
    printf("\n\t\t[5]- Make Transaction\n");
    printf("\n\t\t[6]- Remove existing account\n");
    printf("\n\t\t[7]- Transfer ownership\n");
    printf("\n\t\t[8]- Exit\n");
    scanf("%d", &option);

    switch (option)
    {
    case 1:
        createNewAcc(u);
       //CreateNewAccount(u);
        break;
    case 2:
        updateAcc(u);
        break;
    case 3:
        // student TODO : add your **Check the details of existing accounts** function
        // here
        break;
    case 4:
       // checkAllAccounts(u);
        break;
    case 5:
        // student TODO : add your **Make transaction** function
        // here
        break;
    case 6:
        // student TODO : add your **Remove existing account** function
        // here
        break;
    case 7:
        // student TODO : add your **Transfer owner** function
        // here
        break;
    case 8:
        exit(1);
        break;
    default:
        printf("Invalid operation!\n");
    }
};

void initMenu(struct User *u){
    START:

    int r = 0;
    int option;

    system("clear");
    printf("\n\n\t\t======= ATM =======\n");
    printf("\n\t\t-->> Feel free to login / register :\n");
    printf("\n\t\t[1]- login\n");
    printf("\n\t\t[2]- register\n");
    printf("\n\t\t[3]- exit\n");
    while (!r)
    {
        scanf("%d", &option);
        clear_buffer();
        switch (option){
        case 1:
            Login(u);
            r = 1;
            break;
        case 2:
            Registration(u);
            r = 1;
            break;
        case 3:
            exit(1);
            break;
        default:
            printf("Insert a valid operation!\n");
            goto START;
        }
    }
};

// login user by name and password
void Login(struct User *u){
    struct termios oflags, nflags;
    system("clear");
    START1:
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    char input[50] = "";

    while (fgets(u->name, 50,stdin) != NULL){
        u->name[strcspn(u->name, "\n")] = '\0';
        if (isStrValid(u->name) == 0){
            system("clear");
            printf("\nnot valid: %s", u->name);
            goto START1;
        }else{
            break;
        } 
    }
    clear_buffer();

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0){
        perror("tcsetattr");
        return exit(1);
    }

    START2:
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    while (fgets(u->password, 50,stdin) != NULL){
        u->password[strcspn(u->password, "\n")] = '\0';
        if (isStrValid(u->password) == 0){
            system("clear");
            printf("\nnot valid: %s", u->password);
            goto START2;
        } else{
            break;
        }
    }
    clear_buffer();

     // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0){
        perror("tcsetattr");
        return exit(1);
    }

    int err;
    sqlite3 *db;
    err = sqlite3_open("./data/database.db", &db);
    if (err != 0){
        sqlite3_close(db);
        printf("ERROR OPEN DB\n");
        return exit(0);
    }
    sqlite3_stmt* stm;
    char* query = sqlite3_mprintf("SELECT id FROM users WHERE name = %Q AND password = %Q", u->name, u->password);
    err = sqlite3_prepare_v2(db, query, -1, &stm, NULL);
    if (err != 0){
        sqlite3_close(db);
        printf("ERROR PREPARE DB\n");
        return exit(0);
    }
    int id = 0;
    while ((err = sqlite3_step(stm)) == SQLITE_ROW){
         id = sqlite3_column_int(stm, 0);
       // const unsigned char* name = sqlite3_column_text(stm,1); 
    }
    if(id == 0){
        printf("invalid data\n");
        goto START1;
    }
    u->id = id;
   sqlite3_finalize(stm);
   sqlite3_free(query);
  // sqlite3_close(db);
   mainMenu(*u);
}

// regester new user
void Registration(struct User *u){
    struct termios oflags, nflags;
    system("clear");
    START1:
    printf("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:");
    char input[50] = "";

    while (fgets(u->name, 50,stdin) != NULL){
        u->name[strcspn(u->name, "\n")] = '\0';
        if (isStrValid(u->name) == 0){
            system("clear");
            printf("\nnot valid: %s", u->name);
            goto START1;
        }else{
            break;
        } 
    }
    clear_buffer();

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0){
        perror("tcsetattr");
        return exit(1);
    }

    START2:
    printf("\n\n\n\n\n\t\t\t\tEnter the password to login:");
    while (fgets(u->password, 50,stdin) != NULL){
        u->password[strcspn(u->password, "\n")] = '\0';
        if (isStrValid(u->password) == 0){
            system("clear");
            printf("\nnot valid: %s", u->password);
            goto START2;
        } else{
            break;
        }
    }
    clear_buffer();

     // restore terminal
    if (tcsetattr(fileno(stdin), TCSANOW, &oflags) != 0){
        perror("tcsetattr");
        return exit(1);
    }

    int err;
    sqlite3 *db;
    err = sqlite3_open("./data/database.db", &db);
    if (err != 0){
        sqlite3_close(db);
        printf("ERROR OPEN DB\n");
        return exit(0);
    }

    char* query = sqlite3_mprintf("INSERT INTO users(name, password) VALUES('%q','%q')", u->name,u->password);
    err = sqlite3_exec(db, query,NULL,NULL,NULL);
    if (err != 0){
        sqlite3_close(db);
        system("clear");
        printf("NAME ALREDY EXEC\n");
        goto START1;
    }
    sqlite3_stmt* stm;
    char* query2 = sqlite3_mprintf("SELECT id FROM users WHERE name = %Q AND password = %Q", u->name, u->password);
    err = sqlite3_prepare_v2(db, query2, -1, &stm, NULL);
    if (err != 0){
        sqlite3_close(db);
        printf("ERROR PREPARE DB\n");
        return exit(0);
    }
    int id = 0;
    while ((err = sqlite3_step(stm)) == SQLITE_ROW){
         id = sqlite3_column_int(stm, 0);
       // const unsigned char* name = sqlite3_column_text(stm,1); 
    }
    u->id = id;
    sqlite3_close(db);
    sqlite3_free(query);
    mainMenu(*u);
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


int ExecQuery(const char *query, int numArgs, ...) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;

    rc = sqlite3_open("./data/database.db", &db);
    if (rc != 0) {
        printf("OPEN ERROR:\n");
        sqlite3_close(db);
        return rc;
    }

    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != 0) {
        printf("PREPARE ERROR:\n");
        sqlite3_close(db);
        return rc;
    }

    va_list args;
    va_start(args, numArgs);
    for (int i = 0; i < numArgs; i++) {
        const char *arg = va_arg(args, const char*);
        sqlite3_bind_text(stmt, i + 1, arg, -1, SQLITE_STATIC);
    }
    va_end(args);

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        printf("EXECUTION ERROR:\n");
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return rc == SQLITE_DONE ? 0 : rc;
}

int main() {
    struct User u;
    int err;

    const char *users = "CREATE TABLE IF NOT EXISTS users(id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL UNIQUE, password TEXT NOT NULL);";
    err = ExecQuery(users,0);
    if(err != 0){
        printf("ERROR CREAT TABLE users");
        return 0;
    }
    const char *accounts = "CREATE TABLE IF NOT EXISTS accounts("
                           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                           "user_id INTEGER NOT NULL,"
                           "name_user TEXT NOT NULL,"
                           "country TEXT NOT NULL,"
                           "phone TEXT NOT NULL,"
                           "accountType TEXT NOT NULL,"
                           "accountNbr INTEGER NOT NULL UNIQUE,"
                           "amount INTEGER NOT NULL,"
                           "detposit TEXT ,"
                           "withdraw TEXT,"
                           "FOREIGN KEY (name_user) REFERENCES users(name),"
                           "FOREIGN KEY (user_id) REFERENCES users(id));";

    err = ExecQuery(accounts,0);
    if(err != 0){
        printf("ERROR CREAT TABLE accounts");
        return 0;
    }


     initMenu(&u);
     mainMenu(u);
    return 0;
}