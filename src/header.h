#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

struct Date{
    int month, day, year;
};

// all fields for each record of an account
struct Record{
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

struct User{
    int id;
    char name[50];
    char password[50];
};

// utils func
int isPhoneValid(int phone);
int isTypeAcountValid(const char *type);
void Menuorexite(struct User u);
void success(struct User u);
void getCharInput(const char* text, char* buffer, int size);
int getIntInput(const char* prompt, int size);
double getDoubleInput(const char* text, int size);
int isAlphabit(const char* text);
void clear_buffer();

// system function
void createNewAcc(struct User u);
void mainMenu(struct User u);
//void checkAllAccounts(struct User u);

// signup and login
void Login(struct User *u);
void Registration(struct User *u);
//void CreateNewAccount(struct User u);
int isValidDate(struct Date date);
int InsertRecord(sqlite3 *db, struct Record r, int user_id, const char *userName);
// update account
void updateAcc(struct User u);
int checkAccountNumber(sqlite3 *db, int accountNbr, int userid);
int UpdateCountry(sqlite3 *db, const char *country, int accountNbr);
int UpdatePhone(sqlite3 *db, int phone, int accountNbr);
// acount info
void CheckAccounts(struct User u);
int GetAccountInfo(sqlite3 *db, int accountNbr, int showStatus);
void checkStatus(const char *deposit, const char *accountType, double amount);
// all acounts info
void checkAllAccounts(struct User u);
// make transction
void MakeTransaction(struct User u);
int WithdrawDeposit(sqlite3 *db,int accountNbr,double amount, int pluMin);
void getAccountType(sqlite3 *db,int accNbr, char *type);
// remove account
void RemoveAccount(struct User u);
int Remove(sqlite3 *db,int accountNbr);
//transfer Owner to anther user
void TransferOwner(struct User u);
int GetIdbyName(sqlite3 *db, const char *nameUser);
int Transform(sqlite3 *db,int accountNbr,int id, const char *user);