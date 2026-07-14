# Bank Management System (C++)

A console-based Bank Management System built in C++ using Object-Oriented
Programming (OOP) and File Handling. It lets you create, view, search,
update, and delete customer bank accounts, with all data saved permanently
to a text file (`accounts.txt`).

## Features

1. **Create New Bank Account** — auto or manual account number, name,
   phone, address, account type (Savings/Current), initial deposit.
2. **Display All Accounts** — lists every account on file.
3. **Search Account** — look up a single account by account number.
4. **Deposit Money** — add funds to an existing account.
5. **Withdraw Money** — remove funds, blocked if balance is insufficient.
6. **Check Account Balance**
7. **Update Customer Details** — name, phone, address, account type.
8. **Delete Account**
9. **Show Total Number of Accounts**
10. **Exit Program**

### Validation

- Account numbers must be unique.
- Deposit and withdrawal amounts must be greater than zero.
- Withdrawals cannot exceed the available balance.
- Non-numeric input is rejected and re-prompted instead of crashing the program.
- Clear success/error messages are shown for every operation.

## Project Structure

```
BankManagementSystem/
├── main.cpp          # Menu-driven console interface (entry point)
├── BankAccount.h      # BankAccount class declaration
├── BankAccount.cpp    # BankAccount class implementation
├── FileHandler.h       # File-handling (CRUD) class declaration
├── FileHandler.cpp     # File-handling (CRUD) class implementation
├── accounts.txt        # Permanent data storage (sample records included)
└── README.md            # This file
```

### Design overview

- **`BankAccount`** encapsulates a single customer's data (account number,
  name, phone, address, account type, balance) plus behavior such as
  `deposit()`, `withdraw()`, and `display()`. Data members are private,
  accessed only via getters/setters — a standard OOP encapsulation pattern.
- **`FileHandler`** owns all reading and writing of `accounts.txt` using
  `<fstream>`. It exposes static methods (`createAccount`, `getAllAccounts`,
  `searchAccount`, `updateAccount`, `deleteAccount`, etc.) so `main.cpp`
  never touches the file directly.
- **`main.cpp`** only handles the menu loop and user input/output, calling
  into `BankAccount` and `FileHandler` for everything else. This separation
  of concerns keeps the codebase modular and easy to extend.

### Storage format

Each account is stored as one line in `accounts.txt`, with fields
separated by `|`:

```
accountNumber|name|phone|address|accountType|balance
```

Example:

```
1001|John Doe|9876543210|123 Main St, Springfield|Savings|5000.00
```

Because updates/deletes require rewriting records, `FileHandler` reads
all accounts into memory, modifies the in-memory list, and rewrites the
whole file — a simple and reliable approach for a text-based store like
this.

## How to Compile & Run

### Requirements

- A C++17-capable compiler (e.g. g++ via MSYS2/MinGW on Windows, or g++/clang
  on Linux/macOS).

### Windows (MSYS2 / MinGW)

1. Open the **MSYS2 MinGW 64-bit** terminal.
2. Navigate to the project folder:
   ```bash
   cd /path/to/BankManagementSystem
   ```
3. Compile:
   ```bash
   g++ -std=c++17 -Wall -Wextra -o bank_system.exe main.cpp BankAccount.cpp FileHandler.cpp
   ```
4. Run:
   ```bash
   ./bank_system.exe
   ```

### Linux / macOS

```bash
cd BankManagementSystem
g++ -std=c++17 -Wall -Wextra -o bank_system main.cpp BankAccount.cpp FileHandler.cpp
./bank_system
```

The program will read from and write to `accounts.txt` in the same
directory it's run from. A few sample records are already included so
you can try "Display All Accounts" and "Search Account" immediately.

## Sample Menu

```
===============================
      BANK MANAGEMENT SYSTEM
===============================
1. Create Account
2. Display All Accounts
3. Search Account
4. Deposit Money
5. Withdraw Money
6. Check Balance
7. Update Account
8. Delete Account
9. Total Accounts
10. Exit
===============================
Enter your choice:
```

## Notes for Beginners

- Every menu action is implemented as its own function in `main.cpp`
  (e.g. `createAccount()`, `depositMoney()`) — read through them to see
  how each feature is wired together.
- `readInt()`, `readDouble()`, and `readLine()` in `main.cpp` are small
  helper functions that make user input robust against typos and bad
  input, so feel free to reuse that pattern in your own projects.
- Because account records are plain text, you can open `accounts.txt` in
  any text editor to inspect or manually adjust the data.
