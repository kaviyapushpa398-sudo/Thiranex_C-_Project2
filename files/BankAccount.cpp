/*
 * BankAccount.cpp
 * ----------------
 * Implements the member functions declared in BankAccount.h
 */

#include "BankAccount.h"
#include <iostream>
#include <sstream>
#include <iomanip>

// ---------- Constructors ----------

// Default constructor: initializes an empty/invalid account
BankAccount::BankAccount()
    : accountNumber(0), name(""), phone(""), address(""),
      accountType("Savings"), balance(0.0) {
}

// Parameterized constructor: used when creating a brand-new account
BankAccount::BankAccount(int accNo, const std::string& name, const std::string& phone,
                          const std::string& address, const std::string& accountType,
                          double balance)
    : accountNumber(accNo), name(name), phone(phone), address(address),
      accountType(accountType), balance(balance) {
}

// ---------- Getters ----------

int BankAccount::getAccountNumber() const { return accountNumber; }
std::string BankAccount::getName() const { return name; }
std::string BankAccount::getPhone() const { return phone; }
std::string BankAccount::getAddress() const { return address; }
std::string BankAccount::getAccountType() const { return accountType; }
double BankAccount::getBalance() const { return balance; }

// ---------- Setters ----------

void BankAccount::setName(const std::string& newName) { name = newName; }
void BankAccount::setPhone(const std::string& newPhone) { phone = newPhone; }
void BankAccount::setAddress(const std::string& newAddress) { address = newAddress; }
void BankAccount::setAccountType(const std::string& newAccountType) { accountType = newAccountType; }
void BankAccount::setBalance(double newBalance) { balance = newBalance; }

// ---------- Core banking operations ----------

// Adds the given amount to the balance. Caller is expected to validate amount > 0.
void BankAccount::deposit(double amount) {
    balance += amount;
}

// Subtracts the given amount from the balance only if sufficient funds exist.
// Returns true on success, false if funds are insufficient.
bool BankAccount::withdraw(double amount) {
    if (amount > balance) {
        return false; // Insufficient balance
    }
    balance -= amount;
    return true;
}

// ---------- Utility ----------

// Prints a neatly formatted summary of the account to the console.
void BankAccount::display() const {
    std::cout << "-----------------------------------------\n";
    std::cout << "Account Number : " << accountNumber << "\n";
    std::cout << "Name           : " << name << "\n";
    std::cout << "Phone          : " << phone << "\n";
    std::cout << "Address        : " << address << "\n";
    std::cout << "Account Type   : " << accountType << "\n";
    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Balance        : Rs. " << balance << "\n";
    std::cout << "-----------------------------------------\n";
}

// Serializes the account into a single "|" delimited line for file storage.
// Format: accountNumber|name|phone|address|accountType|balance
std::string BankAccount::toFileString() const {
    std::ostringstream oss;
    oss << accountNumber << "|" << name << "|" << phone << "|"
        << address << "|" << accountType << "|" << balance;
    return oss.str();
}

// Parses a "|" delimited line (as produced by toFileString) back into a BankAccount.
BankAccount BankAccount::fromFileString(const std::string& line) {
    std::stringstream ss(line);
    std::string token;
    std::string fields[6];
    int index = 0;

    // Split the line on '|' into up to 6 fields
    while (std::getline(ss, token, '|') && index < 6) {
        fields[index++] = token;
    }

    int accNo = 0;
    double bal = 0.0;
    try {
        accNo = std::stoi(fields[0]);
        bal = std::stod(fields[5]);
    } catch (...) {
        // If parsing fails, default values (0) remain; caller should validate.
        accNo = 0;
        bal = 0.0;
    }

    return BankAccount(accNo, fields[1], fields[2], fields[3], fields[4], bal);
}
