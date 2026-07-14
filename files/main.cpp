/*
 * main.cpp
 * --------
 * Entry point for the Bank Management System.
 * Displays a menu-driven console interface and delegates all business
 * logic to BankAccount (data + operations) and FileHandler (permanent
 * storage). This keeps main.cpp focused purely on user interaction.
 */

#include <iostream>
#include <limits>
#include <iomanip>
#include "BankAccount.h"
#include "FileHandler.h"

using namespace std;

// ------------------------------------------------------------------
// Helper: safely reads an integer from the user, re-prompting on
// invalid (non-numeric) input so the program never crashes or loops
// infinitely on bad input.
// ------------------------------------------------------------------
int readInt(const string& prompt) {
    int value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear(); // Reset error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard bad input
            cout << "Invalid input. Please enter a whole number.\n";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard trailing newline
            return value;
        }
    }
}

// ------------------------------------------------------------------
// Helper: safely reads a positive double (used for money amounts).
// Re-prompts if the value is not a valid number.
// ------------------------------------------------------------------
double readDouble(const string& prompt) {
    double value;
    while (true) {
        cout << prompt;
        cin >> value;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a valid amount.\n";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return value;
        }
    }
}

// ------------------------------------------------------------------
// Helper: reads a full line of text (used for names/addresses that
// may contain spaces). Strips any leading blank line left in the
// buffer from a previous cin >> read.
// ------------------------------------------------------------------
string readLine(const string& prompt) {
    string value;
    cout << prompt;
    getline(cin, value);
    return value;
}

// ------------------------------------------------------------------
// Helper: repeatedly prompts until the user enters "Savings" or
// "Current" (case-insensitive), returning a normalized capitalised
// string.
// ------------------------------------------------------------------
string readAccountType() {
    string type;
    while (true) {
        type = readLine("Account Type (Savings/Current): ");
        // Normalize to lowercase for comparison
        string lower = type;
        for (auto& c : lower) c = tolower(c);

        if (lower == "savings") return "Savings";
        if (lower == "current") return "Current";

        cout << "Invalid account type. Please type 'Savings' or 'Current'.\n";
    }
}

// ------------------------------------------------------------------
// Menu option 1: Create a new account
// ------------------------------------------------------------------
void createAccount() {
    cout << "\n--- Create New Bank Account ---\n";

    int accNo;
    cout << "Enter account number (or 0 to auto-generate): ";
    accNo = readInt("");

    if (accNo == 0) {
        accNo = FileHandler::generateAccountNumber();
        cout << "Auto-generated Account Number: " << accNo << "\n";
    } else if (FileHandler::accountExists(accNo)) {
        cout << "Error: Account number " << accNo << " already exists. "
             << "Please try again with a unique number.\n";
        return;
    }

    string name = readLine("Customer Name: ");
    string phone = readLine("Phone Number: ");
    string address = readLine("Address: ");
    string accType = readAccountType();

    double initialDeposit = readDouble("Initial Deposit: Rs. ");
    if (initialDeposit <= 0) {
        cout << "Error: Initial deposit must be greater than zero. Account not created.\n";
        return;
    }

    BankAccount newAccount(accNo, name, phone, address, accType, initialDeposit);

    if (FileHandler::createAccount(newAccount)) {
        cout << "\nAccount created successfully!\n";
        newAccount.display();
    } else {
        cout << "Error: Failed to create account (duplicate account number).\n";
    }
}

// ------------------------------------------------------------------
// Menu option 2: Display all accounts
// ------------------------------------------------------------------
void displayAllAccounts() {
    cout << "\n--- All Bank Accounts ---\n";
    vector<BankAccount> accounts = FileHandler::getAllAccounts();

    if (accounts.empty()) {
        cout << "No accounts found. Please create an account first.\n";
        return;
    }

    for (const auto& acc : accounts) {
        acc.display();
    }
    cout << "Total accounts: " << accounts.size() << "\n";
}

// ------------------------------------------------------------------
// Menu option 3: Search account by account number
// ------------------------------------------------------------------
void searchAccount() {
    cout << "\n--- Search Account ---\n";
    int accNo = readInt("Enter account number to search: ");

    BankAccount result;
    if (FileHandler::searchAccount(accNo, result)) {
        cout << "Account found:\n";
        result.display();
    } else {
        cout << "No account found with account number " << accNo << ".\n";
    }
}

// ------------------------------------------------------------------
// Menu option 4: Deposit money
// ------------------------------------------------------------------
void depositMoney() {
    cout << "\n--- Deposit Money ---\n";
    int accNo = readInt("Enter account number: ");

    BankAccount acc;
    if (!FileHandler::searchAccount(accNo, acc)) {
        cout << "No account found with account number " << accNo << ".\n";
        return;
    }

    double amount = readDouble("Enter amount to deposit: Rs. ");
    if (amount <= 0) {
        cout << "Error: Deposit amount must be greater than zero.\n";
        return;
    }

    acc.deposit(amount);
    if (FileHandler::updateAccount(acc)) {
        cout << "Deposit successful! New balance: Rs. " << fixed << setprecision(2)
             << acc.getBalance() << "\n";
    } else {
        cout << "Error: Could not update account record.\n";
    }
}

// ------------------------------------------------------------------
// Menu option 5: Withdraw money
// ------------------------------------------------------------------
void withdrawMoney() {
    cout << "\n--- Withdraw Money ---\n";
    int accNo = readInt("Enter account number: ");

    BankAccount acc;
    if (!FileHandler::searchAccount(accNo, acc)) {
        cout << "No account found with account number " << accNo << ".\n";
        return;
    }

    double amount = readDouble("Enter amount to withdraw: Rs. ");
    if (amount <= 0) {
        cout << "Error: Withdrawal amount must be greater than zero.\n";
        return;
    }

    if (acc.withdraw(amount)) {
        FileHandler::updateAccount(acc);
        cout << "Withdrawal successful! New balance: Rs. " << fixed << setprecision(2)
             << acc.getBalance() << "\n";
    } else {
        cout << "Error: Insufficient balance. Available balance: Rs. "
             << fixed << setprecision(2) << acc.getBalance() << "\n";
    }
}

// ------------------------------------------------------------------
// Menu option 6: Check balance
// ------------------------------------------------------------------
void checkBalance() {
    cout << "\n--- Check Balance ---\n";
    int accNo = readInt("Enter account number: ");

    BankAccount acc;
    if (FileHandler::searchAccount(accNo, acc)) {
        cout << "Account Number " << accNo << " Balance: Rs. "
             << fixed << setprecision(2) << acc.getBalance() << "\n";
    } else {
        cout << "No account found with account number " << accNo << ".\n";
    }
}

// ------------------------------------------------------------------
// Menu option 7: Update customer details
// ------------------------------------------------------------------
void updateAccountDetails() {
    cout << "\n--- Update Account Details ---\n";
    int accNo = readInt("Enter account number to update: ");

    BankAccount acc;
    if (!FileHandler::searchAccount(accNo, acc)) {
        cout << "No account found with account number " << accNo << ".\n";
        return;
    }

    cout << "Current details:\n";
    acc.display();

    cout << "\nLeave a field blank and press Enter to keep it unchanged.\n";

    string name = readLine("New Name [" + acc.getName() + "]: ");
    if (!name.empty()) acc.setName(name);

    string phone = readLine("New Phone [" + acc.getPhone() + "]: ");
    if (!phone.empty()) acc.setPhone(phone);

    string address = readLine("New Address [" + acc.getAddress() + "]: ");
    if (!address.empty()) acc.setAddress(address);

    string type = readLine("New Account Type (Savings/Current) [" + acc.getAccountType() + "]: ");
    if (!type.empty()) {
        string lower = type;
        for (auto& c : lower) c = tolower(c);
        if (lower == "savings") acc.setAccountType("Savings");
        else if (lower == "current") acc.setAccountType("Current");
        else cout << "Invalid account type entered; keeping previous value.\n";
    }

    if (FileHandler::updateAccount(acc)) {
        cout << "\nAccount updated successfully!\n";
        acc.display();
    } else {
        cout << "Error: Could not update account.\n";
    }
}

// ------------------------------------------------------------------
// Menu option 8: Delete an account
// ------------------------------------------------------------------
void deleteAccount() {
    cout << "\n--- Delete Account ---\n";
    int accNo = readInt("Enter account number to delete: ");

    BankAccount acc;
    if (!FileHandler::searchAccount(accNo, acc)) {
        cout << "No account found with account number " << accNo << ".\n";
        return;
    }

    cout << "Account to be deleted:\n";
    acc.display();

    string confirm = readLine("Are you sure you want to delete this account? (y/n): ");
    if (confirm == "y" || confirm == "Y") {
        if (FileHandler::deleteAccount(accNo)) {
            cout << "Account deleted successfully.\n";
        } else {
            cout << "Error: Could not delete account.\n";
        }
    } else {
        cout << "Deletion cancelled.\n";
    }
}

// ------------------------------------------------------------------
// Menu option 9: Show total number of accounts
// ------------------------------------------------------------------
void showTotalAccounts() {
    cout << "\n--- Total Accounts ---\n";
    cout << "Total number of accounts: " << FileHandler::totalAccounts() << "\n";
}

// ------------------------------------------------------------------
// Prints the main menu
// ------------------------------------------------------------------
void printMenu() {
    cout << "\n===============================\n";
    cout << "      BANK MANAGEMENT SYSTEM\n";
    cout << "===============================\n";
    cout << "1. Create Account\n";
    cout << "2. Display All Accounts\n";
    cout << "3. Search Account\n";
    cout << "4. Deposit Money\n";
    cout << "5. Withdraw Money\n";
    cout << "6. Check Balance\n";
    cout << "7. Update Account\n";
    cout << "8. Delete Account\n";
    cout << "9. Total Accounts\n";
    cout << "10. Exit\n";
    cout << "===============================\n";
}

// ------------------------------------------------------------------
// Main program loop
// ------------------------------------------------------------------
int main() {
    cout << "Welcome to the Bank Management System!\n";

    bool running = true;
    while (running) {
        printMenu();
        int choice = readInt("Enter your choice: ");

        switch (choice) {
            case 1: createAccount(); break;
            case 2: displayAllAccounts(); break;
            case 3: searchAccount(); break;
            case 4: depositMoney(); break;
            case 5: withdrawMoney(); break;
            case 6: checkBalance(); break;
            case 7: updateAccountDetails(); break;
            case 8: deleteAccount(); break;
            case 9: showTotalAccounts(); break;
            case 10:
                cout << "\nThank you for using the Bank Management System. Goodbye!\n";
                running = false;
                break;
            default:
                cout << "Invalid choice. Please enter a number between 1 and 10.\n";
        }
    }

    return 0;
}
