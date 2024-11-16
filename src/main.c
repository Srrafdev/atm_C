#include "header.h"
#include <stdio.h>
#include <sqlite3.h>
#include <ctype.h>
#include <termios.h>

void mainMenu(struct User u){
    system("clear");
    START:
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

    int option = getIntInput(":",3);
    switch (option)
    {
    case 1:
        createNewAcc(u);
        Menuorexite(u);
        break;
    case 2:
        updateAcc(u);
        Menuorexite(u);
        break;
    case 3:
        CheckAccounts(u);
        Menuorexite(u);
        break;
    case 4:
        checkAllAccounts(u);
        Menuorexite(u);
        break;
    case 5:
        MakeTransaction(u);
        Menuorexite(u);
        break;
    case 6:
        RemoveAccount(u);
        Menuorexite(u);
        break;
    case 7:
        TransferOwner(u);
        Menuorexite(u);
        break;
    case 8:
        exit(1);
        break;
    default:
        system("clear");
        printf("Invalid operation!\n");
        goto START;
    }
};

void initMenu(struct User *u){
    system("clear");
    START:
    printf("\n\n\t\t======= ATM =======\n");
    printf("\n\t\t-->> Feel free to login / register :\n");
    printf("\n\t\t[1]- login\n");
    printf("\n\t\t[2]- register\n");
    printf("\n\t\t[3]- exit\n");
   
       int option = getIntInput(":",3);
        switch (option){
        case 1:
            Login(u);
            break;
        case 2:
            Registration(u);
            break;
        case 3:
            exit(1);
            break;
        default:
            system("clear");
            printf("Insert a valid operation!\n");
            goto START;
        }
}

// login user by name and password
void Login(struct User *u){
    struct termios oflags, nflags;
    system("clear");
    START:
    getCharInput("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:",u->name,50);
  
    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;

    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0){
        perror("tcsetattr");
        return exit(1);
    }

    getCharInput("\n\n\n\n\n\t\t\t\tEnter the password to login:",u->password,50);

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
    }
    if(id == 0){
        system("clear");
        printf("user name or password not corect\n");
        goto START;
    }
    u->id = id;
   sqlite3_finalize(stm);
   sqlite3_free(query);
   sqlite3_close(db);
   mainMenu(*u);
}

// regester new user
void Registration(struct User *u){
    struct termios oflags, nflags;
    system("clear");
    START:

    getCharInput("\n\n\n\t\t\t\t   Bank Management System\n\t\t\t\t\t User Login:",u->name,50);

    // disabling echo
    tcgetattr(fileno(stdin), &oflags);
    nflags = oflags;
    nflags.c_lflag &= ~ECHO;
    nflags.c_lflag |= ECHONL;
    if (tcsetattr(fileno(stdin), TCSANOW, &nflags) != 0){
        perror("tcsetattr");
        return exit(1);
    }

    getCharInput("\n\n\n\n\n\t\t\t\tEnter the password to login:",u->password,50);

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
        printf("this user already exists\n");
        goto START;
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
    }
    u->id = id;
    sqlite3_close(db);
    sqlite3_free(query);
    mainMenu(*u);
}

int main() {
    struct User u;
    int err;
    sqlite3 *db;
    err = sqlite3_open("./data/database.db", &db);
    if (err != 0) {
        printf("ERROR OPEN DB\n");
        sqlite3_close(db);
       return 0;
    }

    const char *users = "CREATE TABLE IF NOT EXISTS users(id INTEGER PRIMARY KEY AUTOINCREMENT, name TEXT NOT NULL UNIQUE, password TEXT NOT NULL);";
    err = sqlite3_exec(db,users,0,0,0);
    if (err != 0){
        printf("ERROR EXEC user\n");
        sqlite3_close(db);
        return 0;
    }
    const char *accounts = "CREATE TABLE IF NOT EXISTS accounts("
                           "id INTEGER PRIMARY KEY AUTOINCREMENT,"
                           "user_id INTEGER NOT NULL,"
                           "name_user TEXT NOT NULL,"
                           "country TEXT NOT NULL,"
                           "phone INTEGER NOT NULL,"
                           "accountType TEXT NOT NULL,"
                           "accountNbr INTEGER NOT NULL UNIQUE CHECK (amount BETWEEN 0 AND 9999999999),"
                           "amount INTEGER NOT NULL CHECK (amount BETWEEN 0 AND 9999999999),"
                           "detposit TEXT ,"
                           "withdraw TEXT,"
                           "FOREIGN KEY (name_user) REFERENCES users(name),"
                           "FOREIGN KEY (user_id) REFERENCES users(id));";

    err = sqlite3_exec(db,accounts,0,0,0);
    if (err != 0){
        printf("ERROR EXEC accounts\n");
        sqlite3_close(db);
        return 0;
    }

     initMenu(&u);
     mainMenu(u);
    return 0;
}