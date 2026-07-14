/*
 * BankAccount.h
 * -------------
 * Declares the BankAccount class which represents a single customer
 * bank account. This class demonstrates OOP concepts such as
 * encapsulation (private data members with public getters/setters),
 * constructors, and member functions.
 */

#ifndef BANK_ACCOUNT_H
#define BANK_ACCOUNT_H

#include <string>

class BankAccount {
private:
    int accountNumber;       // Unique account number
    std::string name;        // Customer name
    std::string phone;       // Customer phone number
    std::string address;     // Customer address
    std::string accountType; // "Savings" or "Current"
    double balance;          // Current account balance

public:
    // ---------- Constructors ----------
    BankAccount(); // Default constructor
    BankAccount(int accNo, const std::string& name, const std::string& phone,
                const std::string& address, const std::string& accountType,
                double balance);

    // ---------- Getters (encapsulation: read access) ----------
    int getAccountNumber() const;
    std::string getName() const;
    std::string getPhone() const;
    std::string getAddress() const;
    std::string getAccountType() const;
    double getBalance() const;

    // ---------- Setters (encapsulation: controlled write access) ----------
    void setName(const std::string& name);
    void setPhone(const std::string& phone);
    void setAddress(const std::string& address);
    void setAccountType(const std::string& accountType);
    void setBalance(double balance);

    // ---------- Core banking operations ----------
    void deposit(double amount);   // Adds amount to balance
    bool withdraw(double amount);  // Subtracts amount if sufficient funds available

    // ---------- Utility ----------
    void display() const; // Pretty-print account details to the console

    // Converts the account object into a single delimited line for file storage
    std::string toFileString() const;

    // Reconstructs a BankAccount object from a delimited line read from file
    static BankAccount fromFileString(const std::string& line);
};

#endif // BANK_ACCOUNT_H
