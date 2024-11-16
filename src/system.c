#include "header.h"
#include <sqlite3.h>

void createNewAcc(struct User u){
    struct Record r;
    int err = 0;

    system("clear");
date:
    printf("\t\t\t===== New record =====\n");

    printf("\nEnter today's date(mm/dd/yyyy):");
    r.deposit.month = 0;
    r.deposit.day = 0;
    r.deposit.year = 0;
    scanf("%d/%d/%d", &r.deposit.month, &r.deposit.day, &r.deposit.year);
    clear_buffer();
    if (isValidDate(r.deposit) != 0){
        printf("date not valid\n");
        goto date;
    }

    r.accountNbr = getIntInput("\nEnter the account number:",10);

country:
    getCharInput("\nEnter the country:",r.country,50);
    err = isAlphabit(r.country);
    if(err != 0){
        printf("\ncountry not valid %s\n",r.country);
        goto country;
    }
phone:    
    r.phone = getIntInput("\nEnter the phone number:",10);
    err = isPhoneValid(r.phone);
    if(err != 0){
        printf("\nphone not valid\n");
        goto phone;
    }

    r.amount = getDoubleInput("\nEnter amount to deposit: $",15);
   
type:
    getCharInput("\nChoose the type of account:\n\t-> saving\n\t-> current\n\t-> fixed01(for 1 year)\n\t-> fixed02(for 2 years)\n\t-> fixed03(for 3 years)\n\n\tEnter your choice:",r.accountType,10);
    err = isTypeAcountValid(r.accountType);
    if(err != 0){
        printf("\naccount type not valid %s\n",r.accountType);
        goto type;
    }

    sqlite3 *db = NULL;
    err = sqlite3_open("./data/database.db", &db);
    if (err != 0){
        sqlite3_close(db);
        printf("ERROR OPEN DB\n");
        return exit(0);
    }
    err = InsertRecord(db, r, u.id, u.name);
    if (err != 0){
        sqlite3_close(db);
        system("clear");
        printf("your data not valid\n");
        goto date;
    }

    sqlite3_close(db);
    success(u);
}

int InsertRecord(sqlite3 *db, struct Record r, int user_id, const char *userName){
    sqlite3_stmt *stmt;
    const char *sql = "INSERT INTO accounts (user_id, name_user, country, phone, accountType, accountNbr, amount, detposit) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?, ?);";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK){
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

    if (sqlite3_step(stmt) != SQLITE_DONE){
        printf("Number account alredy exec\n");
        return 1;
    }

    sqlite3_finalize(stmt);
}

int isValidDate(struct Date date){

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (date.year <= 0 || date.month <= 0 || date.day <= 0)
        return 1;

    if(date.year > 2050 || date.year <1900)
        return 1;

    if (date.month < 1 || date.month > 12)
        return 1;

    if (date.month == 2 && ((date.year % 4 == 0 && date.year % 100 != 0) || (date.year % 400 == 0)))
    {
        if (date.day < 1 || date.day > 29)
            return 1;
    }
    else
    {
        if (date.day < 1 || date.day > daysInMonth[date.month - 1])
            return 1;
    }

    return 0;
}
///////////////////////////////////////////////////////////////////////////////////
void updateAcc(struct User u) {
    int err = 0;
    sqlite3 *db = NULL;
    
    // Open the database
    err = sqlite3_open("./data/database.db", &db);
    if (err != 0) {
        printf("ERROR OPEN DB\n");
        sqlite3_close(db);
        return;
    }

    int accnb = getIntInput("\nWhat is the account number you want to change: ",10);

    err = checkAccountNumber(db, accnb, u.id);
    if (err != 1) {
        printf("\nAccount number not found.\n");
        sqlite3_close(db);
        return;
    }

    int nbr = getIntInput("\nWhich information do you want to update?\n1 -> phone number\n2 -> country\n",3);

   switch (nbr) {
    case 1: {
        system("clear");
        int newPhone = getIntInput("Enter the new phone number: ",10);

        err = UpdatePhone(db, newPhone, accnb);  
        if (err != 0) {
            printf("failed update phone number\n");
            sqlite3_close(db);
            return;
        }
        success(u);
        break;
    }
    case 2: {
        system("clear");
        char newCountry[50];
        getCharInput("Enter the new country: ",newCountry,50);
      
        err = UpdateCountry(db, newCountry, accnb); 
        if (err != 0) {
            printf("failed update country\n");
            sqlite3_close(db);
            return;
        }
        success(u);
        break;
    }
    default:
        printf("Please select 1 or 2!\n");
        sqlite3_close(db);
        return;
    }
    sqlite3_close(db);
}
// check number number is in my acounts
int checkAccountNumber(sqlite3 *db, int accountNbr, int userid) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT 1 FROM accounts WHERE accountNbr = ? AND user_id = ? LIMIT 1;";
    int exists = 0;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        return -1;
    }

    sqlite3_bind_int(stmt, 1, accountNbr);
    sqlite3_bind_int(stmt, 2, userid);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        exists = 1; 
    }

    sqlite3_finalize(stmt);

    return exists;
}


// update country
int UpdateCountry(sqlite3 *db, const char *country, int accountNbr) {
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE accounts SET country = ? WHERE accountNbr = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        return 1; 
    }
    sqlite3_bind_text(stmt, 1, country, -1, SQLITE_STATIC); 
    sqlite3_bind_int(stmt, 2, accountNbr);                  

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return 1;  
    }

    sqlite3_finalize(stmt);
    return 0; 
}

// update phone number
int UpdatePhone(sqlite3 *db, int phone, int accountNbr) {
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE accounts SET phone = ? WHERE accountNbr = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK) {
        return 1; 
    }
    sqlite3_bind_int(stmt, 1, phone); 
    sqlite3_bind_int(stmt, 2, accountNbr);                  

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        sqlite3_finalize(stmt);
        return 1;  
    }

    sqlite3_finalize(stmt);
    return 0; 
}

//////////////////////////////////////////////////////////////////////////////////
void CheckAccounts(struct User u){
    int err = 0;
    sqlite3 *db = NULL;
    
    err = sqlite3_open("./data/database.db", &db);
    if (err != 0) {
        printf("ERROR OPEN DB\n");
        sqlite3_close(db);
       return;
    }

    int accnb = getIntInput("\nEnter account number: ",10);

     system("clear");
    // Check if the account number exists
    err = checkAccountNumber(db, accnb, u.id);
    if (err != 1) {
        system("clear");
        printf("\nAccount number not found.\n");
        sqlite3_close(db);
       return;
    }

    err = GetAccountInfo(db,accnb, 1);
    if (err != 0){
        printf("can't get data acount\n");
    }

    sqlite3_close(db);
   return;
}
// get data acount by acount number
int GetAccountInfo(sqlite3 *db, int accountNbr, int showStatus) {
    sqlite3_stmt *stmt;
    const char *sql = "SELECT country, phone, accountType, amount, detposit "
                      "FROM accounts WHERE accountNbr = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Failed to prepare statement\n");
        return 1;
    }

    sqlite3_bind_int(stmt, 1, accountNbr);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char *country = sqlite3_column_text(stmt, 0);
        int phone = sqlite3_column_int(stmt, 1);
        const unsigned char *accountType = sqlite3_column_text(stmt, 2);
        double amount = sqlite3_column_double(stmt, 3);
        const unsigned char *deposit = sqlite3_column_text(stmt, 4);

        printf("Account Number:%d\n", accountNbr);
        printf("Country:%s\n", country);
        printf("Phone number:%d\n", phone);
        printf("Type Of Account:%s\n", accountType);
        printf("Amount deposited:$%lf\n", amount);
        printf("Deposit Date:%s\n", deposit);

        if(showStatus == 1){
            checkStatus(deposit,accountType,amount);
        }
    } else {
        printf("Account number %d not found.\n", accountNbr);
    }

    sqlite3_finalize(stmt);
    return 0;
}

void checkStatus(const char *deposit, const char *accountType, double amount) {
    double interestRate = 0.0;
    int termYears = 0;  
    double interestAmount = 0.0;

    if (strcmp(accountType, "current") == 0) {
        printf("\nYou will not get interests because the account is of type current\n");
        return;
    } else if (strcmp(accountType, "saving") == 0) {
        interestRate = 0.07;  
        interestAmount = amount * interestRate / 12;  
     
        char day[3];
        strncpy(day,deposit,2);
        printf("\n\nYou will get $%.2f as interest on day %s of every month.\n", interestAmount, day);  
        return;
    } else if (strcmp(accountType, "fixed01") == 0) {
        interestRate = 0.04;  // 4% for 1-year fixed
        termYears = 1;
    } else if (strcmp(accountType, "fixed02") == 0) {
        interestRate = 0.05;  // 5% for 2-year fixed
        termYears = 2;
    } else if (strcmp(accountType, "fixed03") == 0) {
        interestRate = 0.08;  // 8% for 3-year fixed
        termYears = 3;
    } else {
        printf("\nUnknown account type\n");
        return;
    }

    interestAmount = amount * interestRate * termYears;

   
    printf("\nFor a %d year, you will get $%.2f as interest in total.\n", termYears, interestAmount);
}

//////////////////////////////////////////////////////////////////////////////////
void checkAllAccounts(struct User u){
    int err = 0;
    sqlite3 *db;
    sqlite3_stmt *stmt;
    
    
    err = sqlite3_open("./data/database.db", &db);
    if (err != 0) {
        printf("ERROR OPEN DB\n");
        sqlite3_close(db);
        Menuorexite(u);
    }
    const char *sql = "SELECT accountNbr FROM accounts WHERE user_id = ?;";

    if(sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK){
        printf("Failed to prepare statement\n");
        return ;
    }

    sqlite3_bind_int(stmt, 1,u.id);
    system("clear");
    while(sqlite3_step(stmt) == SQLITE_ROW){
        int accnbr = sqlite3_column_int(stmt, 0);
        printf("\n===============================\n");
        GetAccountInfo(db,accnbr,0);
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

//////////////////////////////////////////////////////////////////////////////////
void MakeTransaction(struct User u){
    int err = 0;
    sqlite3 *db = NULL;
    
    err = sqlite3_open("./data/database.db", &db);
    if (err != 0) {
        printf("ERROR OPEN DB\n");
        sqlite3_close(db);
       return;
    }

    int accnb = getIntInput("\nEnter account number: ",10);

    // Check if the account number exists
    err = checkAccountNumber(db, accnb, u.id);
    if (err != 1) {
        system("clear");
        printf("\nAccount number not found.\n");
        sqlite3_close(db);
        return;
    }

    //check acount type is not fixed
     char *type;
    getAccountType(db,accnb,type);
    
    if(strcmp(type,"saving") != 0 && strcmp(type,"current") != 0){
        system("clear");
        printf("\nyou cant make transection in account type %s.\n",type);
        sqlite3_close(db);
        return;
    }

    int nbr = getIntInput("\nWhich information do you want to update?\n1 -> withdraw\n2 -> deposit\n",3);

    switch (nbr) {
    case 1: {
        double amount = getDoubleInput("Enter the amount you want to withdrow: $",15);
        
        if(amount > 0){
         err = WithdrawDeposit(db,accnb,amount,0); 
         if (err != 0){
            printf("amount not valid");
            return;
         }
        }
       
        success(u);
        break;
    }
    case 2: {
        double amount = getDoubleInput("Enter the amount you want to deposit: $",15);

        if(amount > 0){
           err = WithdrawDeposit(db,accnb,amount,1);
            if (err != 0){
            printf("amount not valid");
            return;
         }
        }
        success(u);
        break;
    }
    default:
        printf("Please select 1 or 2!\n");
        sqlite3_close(db);
        return;
    }
    sqlite3_close(db);
}

void getAccountType(sqlite3 *db,int accNbr, char *type){
      sqlite3_stmt *stmt;
    const char *sql = "SELECT accountType FROM accounts WHERE accountNbr = ?;";

    // Prepare the SQL statement
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Failed to prepare statement");
        return;
    }

    sqlite3_bind_int(stmt, 1, accNbr);
   
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        const unsigned char *accountType = sqlite3_column_text(stmt, 0);
        if (accountType) {
            strncpy(type, (const char *)accountType, 10 - 1);
        } 
    } else {
        printf("No account found for accountNbr %d.\n", accNbr);
    }

    sqlite3_finalize(stmt);
}

int WithdrawDeposit(sqlite3 *db,int accountNbr,double amount, int pluMin){
    sqlite3_stmt *stmt;
    const char *sql = "";
    if(pluMin == 1){
        sql = "UPDATE accounts SET amount = amount + ? WHERE accountNbr = ?;";
    }else if (pluMin == 0){
        sql = "UPDATE accounts SET amount = amount - ? WHERE accountNbr = ?;";
    }else{
        return 1;
    }

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK){
        printf("\nFailed to prepare statement\n");
        return 1;
    }
    sqlite3_bind_double(stmt, 1, amount);
    sqlite3_bind_int(stmt, 2,accountNbr);

    if(sqlite3_step(stmt) != SQLITE_DONE){
        printf("\nnot valid\n");
        return 1;
    }
    sqlite3_finalize(stmt);
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////
void RemoveAccount(struct User u){
     int err = 0;
    sqlite3 *db = NULL;
    
    err = sqlite3_open("./data/database.db", &db);
    if (err != 0) {
        printf("ERROR OPEN DB\n");
        sqlite3_close(db);
       return;
    }

    int accnb = getIntInput("\nEnter account number: ",10);

     system("clear");
    // Check if the account number exists
    err = checkAccountNumber(db, accnb, u.id);
    if (err != 1) {
        system("clear");
        printf("\nAccount number not found.\n");
        sqlite3_close(db);
       return;
    }
    err = Remove(db,accnb);
    if(err){
        system("clear");
        printf("\nfeald remove account.\n");
        sqlite3_close(db);
        return;
    }
    sqlite3_close(db);
    success(u);
}

int Remove(sqlite3 *db,int accountNbr){
    sqlite3_stmt *stmt;
    const char *sql = "DELETE FROM accounts WHERE accountNbr = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK){
        printf("\nFailed to prepare statement\n");
        return 1;
    }
    sqlite3_bind_int(stmt,1,accountNbr);

    if(sqlite3_step(stmt) != SQLITE_DONE){
        printf("\nnot valid\n");
        return 1;
    }
    sqlite3_finalize(stmt);
    return 0;
}

//////////////////////////////////////////////////////////////////////////////////
void TransferOwner(struct User u){
      int err = 0;
    sqlite3 *db = NULL;
    
    err = sqlite3_open("./data/database.db", &db);
    if (err != 0) {
        printf("ERROR OPEN DB\n");
        sqlite3_close(db);
       return;
    }

    int accnb = getIntInput("\nEnter account number you want to transfere ownership:",10);
    
    // Check if the account number exists
    err = checkAccountNumber(db, accnb, u.id);
    if (err != 1) {
        printf("\nAccount number not found\n");
        sqlite3_close(db);
        return;
    }
    err = GetAccountInfo(db,accnb,0);
    if (err != 0){
        printf("\ncan't get account info\n");
        sqlite3_close(db);
        return;
    }
     
    char nameuser[50];
    getCharInput("\nwhich user you transfer ownership to (user name):",nameuser,50);

    int id = GetIdbyName(db,nameuser);
    if(id == 0){
        printf("\nuser not found\n");
        sqlite3_close(db);
        return;
    }
    err = Transform(db,accnb,id,nameuser);
    if (err != 0){
        printf("\nuser name not valid\n");
        sqlite3_close(db);
        return;
    }
    sqlite3_close(db);
    system("clear");
    success(u);
}

int GetIdbyName(sqlite3 *db, const char *nameUser){
    sqlite3_stmt *stmt;
    const char *sql = "SELECT id FROM  users WHERE name = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Failed to prepare statement\n");
        return 0;
    }
    sqlite3_bind_text(stmt,1,nameUser,-1,SQLITE_STATIC);
    
    int id = 0;
    if(sqlite3_step(stmt)== SQLITE_ROW){
        id = sqlite3_column_int(stmt,0);
    }else{
        printf("Account number %s not found.\n", nameUser);
    }
    sqlite3_finalize(stmt);
    return id;
}

int Transform(sqlite3 *db,int accountNbr,int id, const char *user){
    sqlite3_stmt *stmt;
    const char *sql = "UPDATE accounts SET user_id = ?,name_user = ? WHERE accountNbr = ?;";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, 0) != SQLITE_OK){
        printf("\nFailed to prepare statement\n");
        return 1;
    }
    sqlite3_bind_int(stmt,1,id);
    sqlite3_bind_text(stmt,2,user,-1,SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3,accountNbr);

    if(sqlite3_step(stmt) != SQLITE_DONE){
        printf("\nnot valid\n");
        return 1;
    }
    sqlite3_finalize(stmt);
    return 0;
}