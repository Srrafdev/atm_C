#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

struct Date
{
    int month, day, year;
};

// all fields for each record of an account
struct Record
{
    int id;
    int userId;
    char name[50];
    char country[50];
    int phone;
    char accountType[10];
    int accountNbr;
    double amount;
    struct Date deposit;
    struct Date withdraw;
};

struct User
{
    int id;
    char name[50];
    char password[50];
};

// authentication functions
void loginMenu(char a[50], char pass[50]);
//void registerMenu(char a[50], char pass[50]);
const char *getPassword(struct User u);

// system function
void createNewAcc(struct User u);
void mainMenu(struct User u);
//void checkAllAccounts(struct User u);

// my func
void Login(struct User *u);
void Registration(struct User *u);
int isStrValid(const char *str);
void clear_buffer();
int isStrValid(const char *str);
int ExecQuery(const char *query, int numArgs, ...);
//void CreateNewAccount(struct User u);
int isValidDate(struct Date date);
int InsertRecord(sqlite3 *db, struct Record r, int user_id, const char *userName);
// update account
void updateAcc(struct User u);
int UpdateAccount(sqlite3 *db, struct Record r);
