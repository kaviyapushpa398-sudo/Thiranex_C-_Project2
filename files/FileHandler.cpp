/*
 * FileHandler.cpp
 * ----------------
 * Implements all permanent-storage (file handling) operations declared
 * in FileHandler.h using the <fstream> library.
 */

#include "FileHandler.h"
#include <fstream>
#include <iostream>

// Definition of the static filename constant
const std::string FileHandler::FILENAME = "accounts.txt";

// ---------------------------------------------------------------------
// Reads every line of accounts.txt and converts it into BankAccount
// objects. If the file does not exist yet, an empty vector is returned
// (ifstream simply fails to open, which we treat as "no records yet").
// ---------------------------------------------------------------------
std::vector<BankAccount> FileHandler::getAllAccounts() {
    std::vector<BankAccount> accounts;
    std::ifstream inFile(FILENAME);

    if (!inFile.is_open()) {
        return accounts; // No file yet -> no accounts
    }

    std::string line;
    while (std::getline(inFile, line)) {
        if (!line.empty()) {
            accounts.push_back(BankAccount::fromFileString(line));
        }
    }

    inFile.close();
    return accounts;
}

// ---------------------------------------------------------------------
// Completely rewrites accounts.txt using the provided vector. This is
// the simplest reliable way to support update/delete with plain text
// files, since text files do not support in-place record removal.
// ---------------------------------------------------------------------
void FileHandler::rewriteFile(const std::vector<BankAccount>& accounts) {
    // std::ios::trunc erases the existing file content before writing
    std::ofstream outFile(FILENAME, std::ios::trunc);

    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open " << FILENAME << " for writing.\n";
        return;
    }

    for (const auto& acc : accounts) {
        outFile << acc.toFileString() << "\n";
    }

    outFile.close();
}

// ---------------------------------------------------------------------
// Checks whether an account number already exists in the file.
// ---------------------------------------------------------------------
bool FileHandler::accountExists(int accountNumber) {
    std::vector<BankAccount> accounts = getAllAccounts();
    for (const auto& acc : accounts) {
        if (acc.getAccountNumber() == accountNumber) {
            return true;
        }
    }
    return false;
}

// ---------------------------------------------------------------------
// Auto-generates the next account number by finding the current
// maximum account number in the file and adding 1. Starts at 1001
// if no accounts exist yet.
// ---------------------------------------------------------------------
int FileHandler::generateAccountNumber() {
    std::vector<BankAccount> accounts = getAllAccounts();
    int maxAccNo = 1000; // Base number so the first account becomes 1001

    for (const auto& acc : accounts) {
        if (acc.getAccountNumber() > maxAccNo) {
            maxAccNo = acc.getAccountNumber();
        }
    }
    return maxAccNo + 1;
}

// ---------------------------------------------------------------------
// Appends a new account to the end of the file (fast: no need to
// rewrite the whole file for a pure insert).
// ---------------------------------------------------------------------
bool FileHandler::createAccount(const BankAccount& account) {
    if (accountExists(account.getAccountNumber())) {
        return false; // Duplicate account number not allowed
    }

    // std::ios::app opens the file in append mode, creating it if needed
    std::ofstream outFile(FILENAME, std::ios::app);
    if (!outFile.is_open()) {
        std::cerr << "Error: Could not open " << FILENAME << " for writing.\n";
        return false;
    }

    outFile << account.toFileString() << "\n";
    outFile.close();
    return true;
}

// ---------------------------------------------------------------------
// Linearly searches all accounts for a matching account number.
// ---------------------------------------------------------------------
bool FileHandler::searchAccount(int accountNumber, BankAccount& result) {
    std::vector<BankAccount> accounts = getAllAccounts();
    for (const auto& acc : accounts) {
        if (acc.getAccountNumber() == accountNumber) {
            result = acc;
            return true;
        }
    }
    return false;
}

// ---------------------------------------------------------------------
// Finds the account matching updatedAccount's account number and
// replaces it with the new data, then rewrites the file.
// ---------------------------------------------------------------------
bool FileHandler::updateAccount(const BankAccount& updatedAccount) {
    std::vector<BankAccount> accounts = getAllAccounts();
    bool found = false;

    for (auto& acc : accounts) {
        if (acc.getAccountNumber() == updatedAccount.getAccountNumber()) {
            acc = updatedAccount;
            found = true;
            break;
        }
    }

    if (found) {
        rewriteFile(accounts);
    }
    return found;
}

// ---------------------------------------------------------------------
// Removes the account matching the given account number by rebuilding
// the account list without it, then rewriting the file.
// ---------------------------------------------------------------------
bool FileHandler::deleteAccount(int accountNumber) {
    std::vector<BankAccount> accounts = getAllAccounts();
    std::vector<BankAccount> remaining;
    bool found = false;

    for (const auto& acc : accounts) {
        if (acc.getAccountNumber() == accountNumber) {
            found = true; // Skip adding this one -> effectively deletes it
        } else {
            remaining.push_back(acc);
        }
    }

    if (found) {
        rewriteFile(remaining);
    }
    return found;
}

// ---------------------------------------------------------------------
// Returns the total number of stored accounts.
// ---------------------------------------------------------------------
int FileHandler::totalAccounts() {
    return static_cast<int>(getAllAccounts().size());
}
