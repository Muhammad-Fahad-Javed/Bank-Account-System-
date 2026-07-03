#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <iomanip>
using namespace std;

class BankAccount {
protected:
    int accountNumber;
    string holderName;
    double balance;
    vector<string> transactions;

public:
    BankAccount() : accountNumber(0), holderName(""), balance(0.0) {}

    BankAccount(int accNo, string name, double initialBalance) {
        accountNumber = accNo;
        holderName = name;
        balance = initialBalance;
        if (initialBalance > 0) {
            transactions.push_back("Account opened with balance: " + to_string(initialBalance));
        }
    }

    virtual ~BankAccount() {}

    int getAccountNumber() const { return accountNumber; }
    string getHolderName() const { return holderName; }
    double getBalance() const { return balance; }

    void deposit(double amount) {
        if (amount <= 0) {
            cout << "Invalid deposit amount." << endl;
            return;
        }
        balance += amount;
        transactions.push_back("Deposited: " + to_string(amount));
        cout << "Deposit successful. New balance: " << balance << endl;
    }

    virtual void withdraw(double amount) {
        if (amount <= 0) {
            cout << "Invalid withdrawal amount." << endl;
            return;
        }
        if (amount > balance) {
            cout << "Insufficient balance." << endl;
            return;
        }
        balance -= amount;
        transactions.push_back("Withdrew: " + to_string(amount));
        cout << "Withdrawal successful. New balance: " << balance << endl;
    }

    virtual double calculateInterest() {
        return 0.0;
    }

    virtual void displayType() {
        cout << "General Account";
    }

    void displayAccount() {
        cout << "Account Number: " << accountNumber << endl;
        cout << "Holder Name: " << holderName << endl;
        cout << "Balance: " << balance << endl;
        cout << "Type: ";
        displayType();
        cout << endl;
    }

    void showTransactions() {
        if (transactions.empty()) {
            cout << "No transactions yet." << endl;
            return;
        }
        cout << "Transaction History for account " << accountNumber << ":" << endl;
        for (size_t i = 0; i < transactions.size(); ++i) {
            cout << "  " << transactions[i] << endl;
        }
    }

    void addTransaction(string desc) {
        transactions.push_back(desc);
    }
};

class SavingsAccount : public BankAccount {
private:
    double interestRate;

public:
    SavingsAccount(int accNo, string name, double initialBalance, double rate)
        : BankAccount(accNo, name, initialBalance) {
        interestRate = rate;
    }

    double calculateInterest() override {
        return balance * (interestRate / 100.0);
    }

    void displayType() override {
        cout << "Savings Account (Interest Rate: " << interestRate << "%)";
    }
};

class CurrentAccount : public BankAccount {
private:
    double overdraftLimit;

public:
    CurrentAccount(int accNo, string name, double initialBalance, double limit)
        : BankAccount(accNo, name, initialBalance) {
        overdraftLimit = limit;
    }

    void withdraw(double amount) override {
        if (amount <= 0) {
            cout << "Invalid withdrawal amount." << endl;
            return;
        }
        if (amount > balance + overdraftLimit) {
            cout << "Withdrawal exceeds overdraft limit." << endl;
            return;
        }
        balance -= amount;
        transactions.push_back("Withdrew: " + to_string(amount) + " (overdraft allowed)");
        cout << "Withdrawal successful. New balance: " << balance << endl;
    }

    void displayType() override {
        cout << "Current Account (Overdraft Limit: " << overdraftLimit << ")";
    }
};

vector<BankAccount*> accounts;

int findAccountIndex(int accNo) {
    for (size_t i = 0; i < accounts.size(); ++i) {
        if (accounts[i]->getAccountNumber() == accNo)
            return i;
    }
    return -1;
}

bool accountExists(int accNo) {
    return findAccountIndex(accNo) != -1;
}

void createSavingsAccount() {
    int accNo;
    string name;
    double balance, rate;

    cout << "Enter account number: ";
    cin >> accNo;
    if (accountExists(accNo)) {
        cout << "Account number already exists." << endl;
        return;
    }
    cout << "Enter holder name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter initial balance: ";
    cin >> balance;
    if (balance < 0) {
        cout << "Balance cannot be negative." << endl;
        return;
    }
    cout << "Enter interest rate (in %): ";
    cin >> rate;
    if (rate < 0) {
        cout << "Interest rate cannot be negative." << endl;
        return;
    }

    accounts.push_back(new SavingsAccount(accNo, name, balance, rate));
    cout << "Savings account created successfully." << endl;
}

void createCurrentAccount() {
    int accNo;
    string name;
    double balance, limit;

    cout << "Enter account number: ";
    cin >> accNo;
    if (accountExists(accNo)) {
        cout << "Account number already exists." << endl;
        return;
    }
    cout << "Enter holder name: ";
    cin.ignore();
    getline(cin, name);
    cout << "Enter initial balance: ";
    cin >> balance;
    if (balance < 0) {
        cout << "Balance cannot be negative." << endl;
        return;
    }
    cout << "Enter overdraft limit: ";
    cin >> limit;
    if (limit < 0) {
        cout << "Overdraft limit cannot be negative." << endl;
        return;
    }

    accounts.push_back(new CurrentAccount(accNo, name, balance, limit));
    cout << "Current account created successfully." << endl;
}

void depositMoney() {
    int accNo;
    double amount;
    cout << "Enter account number: ";
    cin >> accNo;
    int idx = findAccountIndex(accNo);
    if (idx == -1) {
        cout << "Account not found." << endl;
        return;
    }
    cout << "Enter amount to deposit: ";
    cin >> amount;
    accounts[idx]->deposit(amount);
}

void withdrawMoney() {
    int accNo;
    double amount;
    cout << "Enter account number: ";
    cin >> accNo;
    int idx = findAccountIndex(accNo);
    if (idx == -1) {
        cout << "Account not found." << endl;
        return;
    }
    cout << "Enter amount to withdraw: ";
    cin >> amount;
    accounts[idx]->withdraw(amount);
}

void displayAccountDetails() {
    int accNo;
    cout << "Enter account number: ";
    cin >> accNo;
    int idx = findAccountIndex(accNo);
    if (idx == -1) {
        cout << "Account not found." << endl;
        return;
    }
    accounts[idx]->displayAccount();
}

void displayAllAccounts() {
    if (accounts.empty()) {
        cout << "No accounts to display." << endl;
        return;
    }
    cout << "\n--- All Accounts ---" << endl;
    for (size_t i = 0; i < accounts.size(); ++i) {
        accounts[i]->displayAccount();
        cout << "------------------------" << endl;
    }
}

void searchAccount() {
    int accNo;
    cout << "Enter account number: ";
    cin >> accNo;
    int idx = findAccountIndex(accNo);
    if (idx == -1) {
        cout << "Account not found." << endl;
        return;
    }
    accounts[idx]->displayAccount();
}

void showTransactionHistory() {
    int accNo;
    cout << "Enter account number: ";
    cin >> accNo;
    int idx = findAccountIndex(accNo);
    if (idx == -1) {
        cout << "Account not found." << endl;
        return;
    }
    accounts[idx]->showTransactions();
}

void calculateInterest() {
    int accNo;
    cout << "Enter account number: ";
    cin >> accNo;
    int idx = findAccountIndex(accNo);
    if (idx == -1) {
        cout << "Account not found." << endl;
        return;
    }
    double interest = accounts[idx]->calculateInterest();
    if (interest == 0.0) {
        cout << "This account does not earn interest (or interest rate is 0)." << endl;
    }
    else {
        cout << "Interest for this account: " << interest << endl;
    }
}

void deleteAccount() {
    int accNo;
    cout << "Enter account number to delete: ";
    cin >> accNo;
    int idx = findAccountIndex(accNo);
    if (idx == -1) {
        cout << "Account not found." << endl;
        return;
    }
    delete accounts[idx];
    accounts.erase(accounts.begin() + idx);
    cout << "Account deleted successfully." << endl;
}

void cleanup() {
    for (size_t i = 0; i < accounts.size(); ++i) {
        delete accounts[i];
    }
    accounts.clear();
}

int main() {
    int choice;
    do {
        cout << "\n--- Bank Account System ---" << endl;
        cout << "1. Create Savings Account" << endl;
        cout << "2. Create Current Account" << endl;
        cout << "3. Deposit Money" << endl;
        cout << "4. Withdraw Money" << endl;
        cout << "5. Display Account Details" << endl;
        cout << "6. Display All Accounts" << endl;
        cout << "7. Search Account by Number" << endl;
        cout << "8. Show Transaction History" << endl;
        cout << "9. Calculate Interest (Savings)" << endl;
        cout << "10. Delete Account" << endl;
        cout << "0. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
        case 1: createSavingsAccount(); break;
        case 2: createCurrentAccount(); break;
        case 3: depositMoney(); break;
        case 4: withdrawMoney(); break;
        case 5: displayAccountDetails(); break;
        case 6: displayAllAccounts(); break;
        case 7: searchAccount(); break;
        case 8: showTransactionHistory(); break;
        case 9: calculateInterest(); break;
        case 10: deleteAccount(); break;
        case 0: cout << "Exiting..." << endl; break;
        default: cout << "Invalid choice. Try again." << endl;
        }
    } while (choice != 0);

    cleanup();
    return 0;
}