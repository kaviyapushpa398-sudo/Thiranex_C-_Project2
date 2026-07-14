/*
 * FileHandler.h
 * -------------
 * Declares the FileHandler class, which is responsible for ALL
 * permanent storage operations (create, read, update, delete) on the
 * accounts.txt file. Keeping file-handling logic separate from the
 * BankAccount class and from main.cpp follows the Single Responsibility
 * Principle and keeps the project modular.
 *
 * All methods are static because FileHandler does not need to maintain
 * any state of its own between calls -- it simply operates on the file
 * every time it is invoked.
 */

#ifndef FILE_HANDLER_H
#define FILE_HANDLER_H

#include <string>
#include <vector>
#include "BankAccount.h"

class FileHandler {
public:
    // Name of the file used for permanent storage of account records
    static const std::string FILENAME;

    // Returns true if an account with the given account number already exists
    static bool accountExists(int accountNumber);

    // Generates the next available account number (current max + 1, starting at 1001)
    static int generateAccountNumber();

    // Appends a new account record to the file. Returns false if the account
    // number already exists.
    static bool createAccount(const BankAccount& account);

    // Reads and returns every account record currently stored in the file
    static std::vector<BankAccount> getAllAccounts();

    // Searches for an account by account number. If found, fills 'result'
    // and returns true; otherwise returns false.
    static bool searchAccount(int accountNumber, BankAccount& result);

    // Updates an existing account record (matched by account number) with
    // new data. Returns true if the account was found and updated.
    static bool updateAccount(const BankAccount& updatedAccount);

    // Deletes the account record matching the given account number.
    // Returns true if the account was found and deleted.
    static bool deleteAccount(int accountNumber);

    // Returns the total number of accounts currently stored
    static int totalAccounts();

private:
    // Rewrites the entire file using the given list of accounts.
    // Used internally by updateAccount() and deleteAccount().
    static void rewriteFile(const std::vector<BankAccount>& accounts);
};

#endif // FILE_HANDLER_H
