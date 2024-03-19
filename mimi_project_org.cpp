#include <iostream>
#include <vector>
#include <fstream>
#include <limits>
using namespace std;


class Account;

class User {
public:
    virtual ~User() {}
    virtual void displayInfo() const = 0;
};

class Customer : public User {
protected:
    vector<Account*>& accounts;

public:
    Customer(vector<Account*>& accs) : accounts(accs) {}
    ~Customer();
    void performTransaction();
    void displayInfo() const override;
};

class Staff : public User {
public:
    void addAmount(Account& account, double amount);
    void deductAmount(Account& account, double amount);
    void requestToOpenAccount(vector<Account*>& accounts);
    void displayInfo() const override;
    void deleteAccount(vector<Account*>& accounts);
};

class Admin : public User {
private:
    vector<Account*>& accounts;

public:
    Admin(vector<Account*>& accs) : accounts(accs) {}
    void viewAllAccounts() const;
    void readAllAccounts() const;
    void givePermissionToOpenAccount(Staff& staff);
    void displayInfo() const override;
};

class Account {
private:
    static int accountNumberGenerator;
    int accountNumber;

public:
    double balance;
    Customer* owner;

    Account(Customer* cust, double initialBalance) : accountNumber(++accountNumberGenerator), balance(initialBalance), owner(cust) {}

    int getAccountNumber() const { return accountNumber; }
    double getBalance() const { return balance; }

    friend void Staff::addAmount(Account& account, double amount);
    friend void Staff::deductAmount(Account& account, double amount);
    friend void Admin::viewAllAccounts() const;
    friend void Admin::readAllAccounts() const;

    void displayInfo() const;
};

int Account::accountNumberGenerator = 0;

void Customer::performTransaction() {
    if (accounts.empty()) {
        cout << "Customer has no accounts." << endl;
        return;
    }

    cout << "Select an account for the transaction:\n";
    for (size_t i = 0; i < accounts.size(); ++i) {
        cout << i + 1 << ". Account Number: " << accounts[i]->getAccountNumber() << ", Balance: " << accounts[i]->getBalance() << endl;
    }

    size_t selectedAccountIndex;
    cout << "Enter the number of the account: ";
    cin >> selectedAccountIndex;

    if (cin.fail() || selectedAccountIndex < 1 || selectedAccountIndex > accounts.size()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a valid account number." << endl;
        return;
    }

    // Adjust index to match the vector (0-based index)
    selectedAccountIndex--;

    double amount;
    cout << "Enter transaction amount: ";
    cin >> amount;

    if (cin.fail() || amount <= 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a valid positive amount." << endl;
        return;
    }
    else{
    int a;
    cout<<"1)deduction\n 2)addAmount"<<endl;
    cin>>a;
if(a==2){
    accounts[selectedAccountIndex]->balance += amount;
    cout << "Transaction successful. New balance: " << accounts[selectedAccountIndex]->getBalance() << endl;
}
else if(a==1)
{
     accounts[selectedAccountIndex]->balance -= amount;
    cout << "Transaction successful. New balance: " << accounts[selectedAccountIndex]->getBalance() << endl; 
}
}
}

void Customer::displayInfo() const {
    cout << "Customer\n";
}

void Staff::addAmount(Account& account, double amount) {
    account.balance += amount;
    cout << "Amount added successfully. New balance: " << account.balance << endl;
}

void Staff::deductAmount(Account& account, double amount) {
    if (account.balance >= amount) {
        account.balance -= amount;
        cout << "Amount deducted successfully. New balance: " << account.balance << endl;
    } else {
        cout << "Insufficient funds." << endl;
    }
}

void Staff::requestToOpenAccount(vector<Account*>& accounts) {
    Customer* newCustomer = new Customer(accounts);  // Pass the accounts vector
    Account* newAccount = new Account(newCustomer, 0.0);
    accounts.push_back(newAccount);

    cout << "Account opened successfully. Account number: " << newAccount->getAccountNumber() << endl;
}

void Staff::displayInfo() const {
    cout << "Staff\n";
}
void Staff::deleteAccount(vector<Account*>& accounts) {
    int accountNumber;
    
    cout << "Enter the account number to delete: ";
    cin >> accountNumber;

    for (auto it = accounts.begin(); it != accounts.end(); ++it) {
        if ((*it)->getAccountNumber() == accountNumber) {
            cout << "Deleting Account " << accountNumber << endl;
            delete *it;
            accounts.erase(it);
            cout << "Account deleted successfully.\n";
            return;
        }
    }
    
    cout << "Account not found.\n";
}
void Admin::viewAllAccounts() const {
    ifstream inFile("all_accounts.txt");

    if (!inFile.is_open()) {
        cout << "Error opening file: all_accounts.txt\n";
        return;
    }

    if (accounts.empty()) {
        cout << "No accounts available." << endl;
        inFile.close();
        return;
    }

    cout << "All Accounts:\n";
    for (const auto& acc : accounts) {
        acc->displayInfo();
    }

    cout << "Accounts from file:\n";
    while (!inFile.eof()) {
        int accountNumber;
        double balance;
        inFile >> accountNumber >> balance;

        if (inFile.fail()) {
            break;
        }

        cout << "Account Number: " << accountNumber << ", Balance: " << balance << endl;
    }

    inFile.close();
}

void Admin::readAllAccounts() const {
    ofstream outFile("all_accounts.txt");

    if (accounts.empty()) {
        cout << "No accounts available." << endl;
        outFile.close();
        return;
    }

    for (const auto& acc : accounts) {
        outFile << "Account Number: " << acc->getAccountNumber() << ", Balance: " << acc->getBalance() << endl;
    }

    outFile.close();
    cout << "All accounts information written to all_accounts.txt" << endl;
}

void Admin::givePermissionToOpenAccount(Staff& staff) {
    staff.requestToOpenAccount(accounts);
}

void Admin::displayInfo() const {
    cout << "Admin\n";
}

void Account::displayInfo() const {
    cout << "Account Number: " << accountNumber << ", Balance: " << balance << endl;
}

Customer::~Customer() {
    for (auto& account : accounts) {
        delete account;
    }
    accounts.clear();
}



int main() {
    vector<Account*> accounts;
    Customer customer(accounts); // Pass the accounts vector to the Customer constructor
    Account* account = new Account(&customer, 1000.0);
    accounts.push_back(account);

    Staff staff;
    Admin admin(accounts);
    admin.givePermissionToOpenAccount(staff);

    int choice;

    do {
        cout << "1. Customer Menu\n";
        cout << "2. Staff Menu\n";
        cout << "3. Admin Menu\n";
        cout << "4. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
              
                cout << "Customer Menu\n";
                customer.performTransaction();
                break;

            case 2:
              
                cout << "Staff Menu\n";
                cout << "1. Add Amount\n";
                cout << "2. Deduct Amount\n";
                cout << "3. Request to Open Account\n";
                cout << "4. Delete Account\n";
cout << "5. Back to Main Menu\n";

                cout << "Enter your choice: ";
                cin >> choice;

                switch (choice) {
                    case 1:
                      
                        cout << "Add Amount\n";
                        staff.addAmount(*account, 500.0);
                        break;

                    case 2:
                       
                        cout << "Deduct Amount\n";
                        staff.deductAmount(*account, 200.0);
                        break;

                    case 3:
                     
                        cout << "Request to Open Account\n";
                        staff.requestToOpenAccount(accounts);
                        break;

                    case 5:
                        break;
                        case 4:
    cout << "Delete Account\n";
     staff.deleteAccount(accounts);
    break;

                    default:
                      
                        cout << "Invalid choice. Please try again.\n";
                        break;
                }
                break;

            case 3:
              
                cout << "Admin Menu\n";
                cout << "1. View All Accounts\n";
                cout << "2. Read All Accounts\n";
                cout << "3. Give Permission to Open Account\n";
                cout << "4. Back to Main Menu\n";
                cout << "Enter your choice: ";
                cin >> choice;

                switch (choice) {
                    case 1:
                     
                        cout << "View All Accounts\n";
                        admin.viewAllAccounts();
                        break;

                    case 2:
                      
                        cout << "Read All Accounts\n";
                        admin.readAllAccounts();
                        break;

                    case 3:
                      
                        cout << "Give Permission to Open Account\n";
                        admin.givePermissionToOpenAccount(staff);
                        break;

                    case 4:
                        break;

                    default:
                      
                        cout << "Invalid choice. Please try again.\n";
                        break;
                }
                break;

            case 4:
                break;

            default:
             
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 4);


    return 0;
}