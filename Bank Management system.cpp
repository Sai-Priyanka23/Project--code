#include <iostream>
#include <string>
using namespace std;

class Account {
private:
    int accountNumber;
    string accountHolder;
    double balance;
    string* transactions; // Pointer to dynamically allocated array for transaction history
    int transactionCount;
    int transactionCapacity; // Capacity of the transactions array

    void resizeTransactions() {
        transactionCapacity *= 2;
        string* newTransactions = new string[transactionCapacity];
        for (int i = 0; i < transactionCount; ++i) {
            newTransactions[i] = transactions[i];
        }
        delete[] transactions; // Delete old transaction array
        transactions = newTransactions;
    }

public:
    Account() : accountNumber(0), accountHolder(""), balance(0.0), transactionCount(0), transactionCapacity(10) {
        transactions = new string[transactionCapacity];
    }

    Account(int accNum, string accHolder, double bal)
        : accountNumber(accNum), accountHolder(accHolder), balance(bal), transactionCount(0), transactionCapacity(10) {
        transactions = new string[transactionCapacity];
    }

    // Copy Constructor
    Account(const Account& other)
        : accountNumber(other.accountNumber), accountHolder(other.accountHolder), balance(other.balance),
          transactionCount(other.transactionCount), transactionCapacity(other.transactionCapacity) {
        transactions = new string[transactionCapacity];
        for (int i = 0; i < transactionCount; ++i) {
            transactions[i] = other.transactions[i];
        }
    }

    // Copy Assignment Operator
    Account& operator=(const Account& other) {
        if (this != &other) { // Prevent self-assignment
            delete[] transactions; // Free the existing array

            accountNumber = other.accountNumber;
            accountHolder = other.accountHolder;
            balance = other.balance;
            transactionCount = other.transactionCount;
            transactionCapacity = other.transactionCapacity;
            
            transactions = new string[transactionCapacity];
            for (int i = 0; i < transactionCount; ++i) {
                transactions[i] = other.transactions[i];
            }
        }
        return *this;
    }

    // Destructor
    ~Account() {
        delete[] transactions;
    }

    void deposit(double amount) {
        balance += amount;
        if (transactionCount == transactionCapacity) {
            resizeTransactions();
        }
        transactions[transactionCount++] = "Deposited: " + to_string(amount);
        cout << "Deposited: " << amount << ", New Balance: " << balance << endl;
    }

    void withdraw(double amount) {
        if (amount <= balance) {
            balance -= amount;
            if (transactionCount == transactionCapacity) {
                resizeTransactions();
            }
            transactions[transactionCount++] = "Withdrawn: " + to_string(amount);
            cout << "Withdrawn: " << amount << ", New Balance: " << balance << endl;
        } else {
            cout << "Insufficient balance!" << endl;
        }
    }

    void transfer(Account& toAccount, double amount) {
        if (amount <= 0) {
            cout << "Transfer amount must be positive!" << endl;
            return;
        }
        if (amount <= balance) {
            withdraw(amount);
            toAccount.deposit(amount);
            if (transactionCount == transactionCapacity) {
                resizeTransactions();
            }
            transactions[transactionCount++] = "Transferred: " + to_string(amount) + " to Account " + to_string(toAccount.getAccountNumber());
        } else {
            cout << "Insufficient balance for transfer!" << endl;
        }
    }

    void display() const {
        cout << "Account Number: " << accountNumber
             << ", Account Holder: " << accountHolder
             << ", Balance: " << balance << endl;
    }

    void displayTransactions() const {
        cout << "Transaction history for Account " << accountNumber << ":" << endl;
        for (int i = 0; i < transactionCount; ++i) {
            cout << transactions[i] << endl;
        }
    }

    void editAccountHolder(const string& newHolder) {
        accountHolder = newHolder;
        cout << "Account holder name updated to: " << accountHolder << endl;
    }

    int getAccountNumber() const {
        return accountNumber;
    }

    string getAccountHolder() const {
        return accountHolder;
    }

    double getBalance() const {
        return balance;
    }
};

class Bank {
private:
    Account* accounts; // Pointer to dynamically allocated array for accounts
    int accountCount;
    int accountCapacity; // Capacity of the accounts array

    void resizeAccounts() {
        accountCapacity *= 2;
        Account* newAccounts = new Account[accountCapacity];
        for (int i = 0; i < accountCount; ++i) {
            newAccounts[i] = accounts[i];
        }
        delete[] accounts;
        accounts = newAccounts;
    }

public:
    Bank() : accountCount(0), accountCapacity(10) {
        accounts = new Account[accountCapacity];
    }

    void addAccount(const Account& account) {
        if (accountCount == accountCapacity) {
            resizeAccounts();
        }
        accounts[accountCount++] = account;
    }

    Account* findAccount(int accNum) {
        for (int i = 0; i < accountCount; ++i) {
            if (accounts[i].getAccountNumber() == accNum) {
                return &accounts[i];
            }
        }
        return nullptr;
    }

    void removeAccount(int accNum) {
        int index = -1;
        for (int i = 0; i < accountCount; ++i) {
            if (accounts[i].getAccountNumber() == accNum) {
                index = i;
                break;
            }
        }
        if (index != -1) {
            for (int i = index; i < accountCount - 1; ++i) {
                accounts[i] = accounts[i + 1];
            }
            accountCount--;
            cout << "Account " << accNum << " closed." << endl;
        } else {
            cout << "Account not found!" << endl;
        }
    }

    void displayAccounts() const {
        for (int i = 0; i < accountCount; ++i) {
            accounts[i].display();
        }
    }

    ~Bank() {
        delete[] accounts;
    }
};

int main() {
    Bank bank;
    int choice, accountNumber, toAccountNumber;
    string accountHolder;
    double initialBalance, amount;
	cout << "\n1. Add Account\n2. Deposit\n3. Withdraw\n4. Transfer Money\n5. Display All Accounts\n";
	cout<<"6. Check Account Details\n7. Edit Account Information\n8. Close Account\n";
	cout<<"9. Generate Account Statement\n10. Exit\n";
        
    while (true) {
    	cout<<"Choose an option: ";
        cin >> choice;

        switch (choice) {
        case 1:
            cout << "Enter Account Number: ";
            cin >> accountNumber;
            cout << "Enter Account Holder Name: ";
            cin.ignore();
            getline(cin, accountHolder);
            cout << "Enter Initial Balance: ";
            cin >> initialBalance;
            bank.addAccount(Account(accountNumber, accountHolder, initialBalance));
            break;

        case 2:
            cout << "Enter Account Number: ";
            cin >> accountNumber;
            if (Account* acc = bank.findAccount(accountNumber)) {
                cout << "Enter Amount to Deposit: ";
                cin >> amount;
                acc->deposit(amount);
            } else {
                cout << "Account not found!" << endl;
            }
            break;

        case 3:
            cout << "Enter Account Number: ";
            cin >> accountNumber;
            if (Account* acc = bank.findAccount(accountNumber)) {
                cout << "Enter Amount to Withdraw: ";
                cin >> amount;
                acc->withdraw(amount);
            } else {
                cout << "Account not found!" << endl;
            }
            break;

        case 4:
            cout << "Enter Your Account Number: ";
            cin >> accountNumber;
            if (Account* fromAccount = bank.findAccount(accountNumber)) {
                cout << "Enter Recipient Account Number: ";
                cin >> toAccountNumber;
                if (Account* toAccount = bank.findAccount(toAccountNumber)) {
                    cout << "Enter Amount to Transfer: ";
                    cin >> amount;
                    fromAccount->transfer(*toAccount, amount);
                } else {
                    cout << "Recipient account not found!" << endl;
                }
            } else {
                cout << "Your account not found!" << endl;
            }
            break;

        case 5:
            bank.displayAccounts();
            break;

        case 6:
            cout << "Enter Account Number: ";
            cin >> accountNumber;
            if (Account* acc = bank.findAccount(accountNumber)) {
                acc->display();
            } else {
                cout << "Account not found!" << endl;
            }
            break;

        case 7:
            cout << "Enter Account Number: ";
            cin >> accountNumber;
            if (Account* acc = bank.findAccount(accountNumber)) {
                cout << "Enter New Account Holder Name: ";
                cin.ignore();
                getline(cin, accountHolder);
                acc->editAccountHolder(accountHolder);
            } else {
                cout << "Account not found!" << endl;
            }
            break;

        case 8:
            cout << "Enter Account Number: ";
            cin >> accountNumber;
            bank.removeAccount(accountNumber);
            break;

        case 9:
            cout << "Enter Account Number: ";
            cin >> accountNumber;
            if (Account* acc = bank.findAccount(accountNumber)) {
                acc->displayTransactions();
            } else {
                cout << "Account not found!" << endl;
            }
            break;

        case 10:
            cout << "Exiting..." << endl;
            return 0;

        default:
            cout << "Invalid choice! Try again." << endl;
        }
    }

    return 0;
}
