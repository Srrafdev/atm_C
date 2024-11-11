#include "header.h"
#include <sqlite3.h>


void stayOrReturn(int notGood, void f(struct User u), struct User u)
{
    int option;
    if (notGood == 0)
    {
        system("clear");
        printf("\n✖ Record not found!!\n");
    invalid:
        printf("\nEnter 0 to try again, 1 to return to main menu and 2 to exit:");
        scanf("%d", &option);
        if (option == 0)
            f(u);
        else if (option == 1)
            mainMenu(u);
        else if (option == 2)
            exit(0);
        else
        {
            printf("Insert a valid operation!\n");
            goto invalid;
        }
    }
    else
    {
        printf("\nEnter 1 to go to the main menu and 0 to exit:");
        scanf("%d", &option);
    }
    if (option == 1)
    {
        system("clear");
        mainMenu(u);
    }
    else
    {
        system("clear");
        exit(1);
    }
}

void success(struct User u){
    int option;
    printf("\n✔ Success!\n\n");
invalid:
    printf("Enter 1 to go to the main menu and 0 to exit!\n");
    scanf("%d", &option);
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
        printf("Insert a valid operation!\n");
        goto invalid;
    }
}

void createNewAcc(struct User u){
    struct Record r;
    struct Record cr;
    char userName[50];

    system("clear");
noAccount:
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(mm/dd/yyyy):");
    r.deposit.month =0;
    r.deposit.day = 0;
    r.deposit.year =0;
     clear_buffer();
    scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
    if(isValidDate(r.deposit) != 0){
        system("clear");
        printf("date not valid\n");
        goto noAccount;
    }
   
    printf("\nEnter the account number:");

    scanf("%d", &r.accountNbr);
    printf("\nEnter the country:");
    scanf("%s", r.country);
    printf("\nEnter the phone number:");
    scanf("%d", &r.phone);
    printf("\nEnter amount to deposit: $");
    scanf("%lf", &r.amount);
    printf("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:");
    scanf("%s", r.accountType);
    clear_buffer();

    int err = 0;
    sqlite3 *db = NULL;
    err = sqlite3_open("./data/database.db", &db);
    if (err != 0){
        sqlite3_close(db);
        printf("ERROR OPEN DB\n");
        return exit(0);
    }
   err = InsertRecord(db,r,u.id,u.name);
   if(err != 0){
    sqlite3_close(db);
    system("clear");
    printf("Number account alredy exec\n");
    goto noAccount;
   }
  
    sqlite3_close(db);
    success(u);
}

int InsertRecord(sqlite3 *db, struct Record r, int user_id, const char *userName) {
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO accounts (user_id, name_user, country, phone, accountType, accountNbr, amount, detposit) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?, ?);";
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        printf("Failed to prepare statement\n");
        return 1;
    }

    sqlite3_bind_int(stmt, 1, user_id);                    
    sqlite3_bind_text(stmt, 2, userName, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, r.country, -1, SQLITE_STATIC); 
    sqlite3_bind_int(stmt, 4, r.phone);  
    sqlite3_bind_text(stmt, 5, r.accountType, -1, SQLITE_STATIC); 
    sqlite3_bind_int(stmt, 6, r.accountNbr);                
    sqlite3_bind_double(stmt, 7, r.amount);                
    
   
     //"MM/DD/YYYY\0"
    char dateStr[11];
    snprintf(dateStr, sizeof(dateStr), "%02d/%02d/%04d", r.deposit.month, r.deposit.day, r.deposit.year);
    sqlite3_bind_text(stmt, 8, dateStr, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Number account alredy exec\n");
        return 1;
    }

    sqlite3_finalize(stmt);
}

int isValidDate(struct Date date){

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (date.year <= 0 || date.month <= 0 || date.day <= 0)
        return 1;

    if (date.month < 1 || date.month > 12)
        return 1;

    if (date.month == 2 && ((date.year % 4 == 0 && date.year % 100 != 0) || (date.year % 400 == 0))){
        if (date.day < 1 || date.day > 29)
            return 1;
    }else{
        if (date.day < 1 || date.day > daysInMonth[date.month - 1])
            return 1;
    }

    return 0;
}

void updateAcc(struct User u){
    printf("\nwhat is the account number you want to change:");
    int accnb = 0;
    int r = 0;
    scanf("%d", &accnb);
    clear_buffer();

    // check acountNb is my acount
     while (!r){
        switch (accnb){
        case 1:
           
            r = 1;
            break;
        case 2:
          
            r = 1;
            break;
        default:
            printf("Insert a valid operation!\n");
        }
    }
}

int UpdateAccount(sqlite3 *db, struct Record r) {
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE accounts SET country = ?, phone = ? WHERE accountNbr = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        return 1;
    }

    sqlite3_bind_text(stmt, 1, r.country, -1, SQLITE_STATIC);      
    sqlite3_bind_int(stmt, 2, r.phone); 
    sqlite3_bind_int(stmt, 3, r.accountNbr);        
                           
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        return 1;
    }
    sqlite3_finalize(stmt);
}

