//July 10, 2023. Finished on making the initial code, currently planning and polishing the main features.
//July 12, 2023. Improved the queue by adding the 'removing customers from waitlist' and 'viewing the waitlist' features.
//July 13, 2023. Added the 'view visitor history' feature which shows the previous customers that visited.

#include <iostream>
#include <queue>
using namespace std;

struct MenuItem {
    string name;
    double price;
    MenuItem* next;

    MenuItem(string n, double p) {
        name = n;
        price = p;
        next = nullptr;
    }
};

class Menu {
private:
    MenuItem* head;

public:
    Menu() {
        head = nullptr;
    }

    void addFood(string name, double price) {
        MenuItem* newItem = new MenuItem(name, price);

        if (head == nullptr) {
            head = newItem;
        } else {
            MenuItem* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newItem;
        }
    }

    void displayMenu() {
        MenuItem* temp = head;

        if (temp == nullptr) {
            cout << "Menu is empty." << endl;
        } else {
            cout << "Food Menu:" << endl;
            while (temp != nullptr) {
                cout << temp->name << " - $" << temp->price << endl;
                temp = temp->next;
            }
        }
    }
};

struct Seat {
    int seatNumber;
    bool isOccupied;
    Seat* next;

    Seat(int number) {
        seatNumber = number;
        isOccupied = false;
        next = nullptr;
    }
};

class SeatList {
private:
    Seat* head;
    int availableSeats;

public:
    SeatList() {
        head = nullptr;
        availableSeats = 0;
    }

    void addSeat(int seatNumber) {
        Seat* newSeat = new Seat(seatNumber);

        if (head == nullptr) {
            head = newSeat;
        } else {
            Seat* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newSeat;
        }

        availableSeats++;
    }

    void displayAvailableSeats() {
        Seat* temp = head;
        bool hasAvailableSeats = false;

        cout << "Available seats: ";
        while (temp != nullptr) {
            if (!temp->isOccupied) {
                cout << temp->seatNumber << " ";
                hasAvailableSeats = true;
            }
            temp = temp->next;
        }

        if (!hasAvailableSeats) {
            cout << "No available seats." << endl;
        } else {
            cout << endl;
        }
    }

    int getAvailableSeatCount() {
        return availableSeats;
    }

    bool occupySeat(int seatNumber) {
        Seat* temp = head;

        while (temp != nullptr) {
            if (temp->seatNumber == seatNumber && !temp->isOccupied) {
                temp->isOccupied = true;
                availableSeats--;
                return true;
            }
            temp = temp->next;
        }

        return false;
    }
};

class Canteen {
private:
    Menu menu;
    SeatList seatList;
    queue<string> waitingList;
    queue<string> visitedHistory;

public:
    Canteen() {
        seatList.addSeat(50);
    }

    void addFood(string name, double price) {
        menu.addFood(name, price);
        cout << "Food item added: " << name << " - $" << price << endl;
    }

    void displayMenu() {
        menu.displayMenu();
    }

    void addCustomerToWaitingList(string customerName) {
        waitingList.push(customerName);
        cout << customerName << " has been added to the waiting list." << endl;
    }

    void removeCustomerFromWaitingList() {
    if (!waitingList.empty()) {
        cout << "Waiting list:" << endl;
        displayWaitingList();

        string customerName;
        cout << "Enter the name of the customer to remove: ";
        cin.ignore();
        getline(cin, customerName);

        bool customerFound = false;
        queue<string> tempWaitingList;

        while (!waitingList.empty()) {
            string customer = waitingList.front();
            waitingList.pop();

            if (customer != customerName) {
                tempWaitingList.push(customer);
            } else {
                customerFound = true;
            }
        }

        waitingList = tempWaitingList;

        if (customerFound) {
            cout << customerName << " is removed from the waiting list." << endl;
            visitedHistory.push(customerName);
        } else {
            cout << "Customer not found in the waiting list. No customer removed." << endl;
        }
    } else {
        cout << "Waiting list is empty." << endl;
    }
}

    void displayWaitingList() {
        if (!waitingList.empty()) {
            queue<string> tempWaitingList = waitingList;
            int count = 1;
            while (!tempWaitingList.empty()) {
                cout << count << ". " << tempWaitingList.front() << endl;
                tempWaitingList.pop();
                count++;
            }
        } else {
            cout << "Waiting list is empty." << endl;
        }
    }

    void displayVisitedHistory() {
        if (!visitedHistory.empty()) {
            cout << "Visited history:" << endl;
            queue<string> tempVisitedHistory = visitedHistory;
            int count = 1;
            while (!tempVisitedHistory.empty()) {
                cout << count << ". " << tempVisitedHistory.front() << endl;
                tempVisitedHistory.pop();
                count++;
            }
        } else {
            cout << "Visited history is empty." << endl;
        }
    }

    void serveNextCustomer() {
        if (!waitingList.empty()) {
            string customerName = waitingList.front();
            waitingList.pop();
            visitedHistory.push(customerName);
            cout << "Next customer: " << customerName << endl;
        } else {
            cout << "Waiting list is empty." << endl;
        }
    }

    void displayAvailableSeats() {
        seatList.displayAvailableSeats();
    }

    int getAvailableSeatCount() {
        return seatList.getAvailableSeatCount();
    }

    bool occupySeat(int seatNumber) {
        if (seatList.occupySeat(seatNumber)) {
            cout << "Seat " << seatNumber << " has been occupied." << endl;
            return true;
        } else {
            cout << "Seat " << seatNumber << " is already occupied or does not exist." << endl;
            return false;
        }
    }
};

int main() {
    Canteen canteen;

    int choice;
    string foodName, customerName;
    double foodPrice;
    int seatNumber;

    bool isAdmin = false;

    while (true) {
        cout << "----------- Canteen Management System -----------" << endl;
        canteen.displayMenu();
        canteen.displayAvailableSeats();
        cout << "1. Add Customer to Waiting List" << endl;
        cout << "2. Remove Customer from Waiting List" << endl;
        cout << "3. View Waiting List" << endl;
        cout << "4. View Visited History" << endl;
        cout << "5. Serve Next Customer" << endl;
        cout << "6. Occupy Seat" << endl;

        if (isAdmin) {
            cout << "7. Add Food" << endl;
            cout << "8. Logout" << endl;
        } else {
            cout << "7. Login as Admin" << endl;
            cout << "8. Exit" << endl;
        }

        cout << "------------------------------------------------" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "Enter customer name: ";
                cin.ignore();
                getline(cin, customerName);
                canteen.addCustomerToWaitingList(customerName);
                break;
            case 2:
                canteen.removeCustomerFromWaitingList();
                break;
            case 3:
                canteen.displayWaitingList();
                break;
            case 4:
                canteen.displayVisitedHistory();
                break;
            case 5:
                canteen.serveNextCustomer();
                break;
            case 6:
                cout << "Enter seat number: ";
                cin >> seatNumber;
                canteen.occupySeat(seatNumber);
                break;
            case 7:
                if (isAdmin) {
                    isAdmin = false;
                } else {
                    string adminPassword;
                    cout << "Enter admin password: ";
                    cin.ignore();
                    getline(cin, adminPassword);

                    // Add password verification logic here
                    if (adminPassword == "pass") {
                        isAdmin = true;
                    } else {
                        cout << "Incorrect password. Login failed." << endl;
                    }
                }
                break;
            case 8:
                cout << "Exiting the program..." << endl;
                return 0;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }

        if (isAdmin && choice == 7) {
            cout << "You are logged in as admin." << endl;
            cout << endl;
            canteen.displayMenu();
            cout << "----------- Admin Options -----------" << endl;
            cout << "1. Add Food" << endl;
            cout << "2. Logout" << endl;
            cout << "-------------------------------------" << endl;
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {
                case 1:
                    cout << "Enter food name: ";
                    cin.ignore();
                    getline(cin, foodName);
                    cout << "Enter food price: ";
                    cin >> foodPrice;
                    canteen.addFood(foodName, foodPrice);
                    break;
                case 2:
                    isAdmin = false;
                    break;
                default:
                    cout << "Invalid choice. Please try again." << endl;
            }
        }

        cout << endl;
    }

    return 0;
}
