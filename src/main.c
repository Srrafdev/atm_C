#include "header.h"
#include <stdio.h>
#include <sqlite3.h>

void mainMenu(struct User u)
{
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
       // createNewAcc(u);
        break;
    case 2:
        // student TODO : add your **Update account information** function
        // here
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

void initMenu(struct User *u)
{
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
        switch (option)
        {
        case 1:
           // loginMenu(u->name, u->password);
            // if (strcmp(u->password, getPassword(*u)) == 0)
            // {
            //     printf("\n\nPassword Match!");
            // }
            // else
            // {
            //     printf("\nWrong password!! or User Name\n");
            //     exit(1);
            // }
            r = 1;
            break;
        case 2:
            // student TODO : add your **Registration** function
            // here
            r = 1;
            break;
        case 3:
            exit(1);
            break;
        default:
            printf("Insert a valid operation!\n");
        }
    }
};


// int OpenDB(sqlite3 **db){
//     int err;

//     err = sqlite3_open("./data/database.db", db);
//     if (err != 0){
//         printf("ERROR OPEN DATABASE");
//         sqlite3_close(*db);
//         return 1;
//     } 
    
//   return 0;
// }
// int createDB(sqlite3 *db){
//     int err;
//     char *query = "CREATE TABLE IF NOT EXICSTS users("
//                   "id INTEGER PRIMARY KEY AUTOINCREMENT,"
//                   "name TEXT NOT NULL,"
//                   "password TEXT NOT NULL);";

//     err = sqlite3_exec(db, query,NULL,NULL,NULL);
//     if (err != 0){
//         printf("ERROR EXEC TO DB");
//         sqlite3_close(db);
//         return err;
//     }

//     return 0;
// }
  // err = sqlite3_exec(db, query,NULL,NULL,NULL);
    // if (err != 0){
    //     printf("ERROR EXEC DATABASE");
    //     sqlite3_close(db);
    //     return 1;
    // }

    // sqlite3_close(db);
    // return 0;

// int OpenDB(sqlite3 **db) {
//     int err = sqlite3_open("./data/database.db", db);
//     if (err != SQLITE_OK) {
//         printf("ERROR OPEN DATABASE: %s\n", sqlite3_errmsg(*db));
//         sqlite3_close(*db);
//         return 1;
//     }
//     return 0;
// }

// int ExecSQL(sqlite3 *db, const char *query) {
//     char *msgErr = NULL;
//     int err = sqlite3_exec(db, query, 0, 0, &msgErr);
//     if (err != SQLITE_OK) {
//         printf("ERROR EXEC QUERY: %s\n", msgErr);
//         sqlite3_free(msgErr);
//         return 1;
//     }
//     return 0;
// }

int ExecQuery(const char *query, int numArgs, ...) {
    sqlite3 *db;
    sqlite3_stmt *stmt;
    int rc;

    rc = sqlite3_open("./data/database.db", &db);
    if (rc != 0) {
        fprintf(stderr, "OPEN ERROR: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return rc;
    }

    rc = sqlite3_prepare_v2(db, query, -1, &stmt, NULL);
    if (rc != 0) {
        fprintf(stderr, "PREPARE ERROR: %s\n", sqlite3_errmsg(db));
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
        fprintf(stderr, "EXECUTION ERROR: %s\n", sqlite3_errmsg(db));
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
        printf("ERROR CREAT TABLE USER");
        return 0;
    }
    const char *accounts = "CREATE TABLE IF NOT EXISTS accounts(id INTEGER PRIMARY KEY AUTOINCREMENT,);";


    // initMenu(&u);
    // mainMenu(u);
    return 0;
}