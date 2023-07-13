//July 10, 2023. Finished on making the initial code, currently planning and polishing the main features.
//July 12, 2023. Improved the queue by adding the 'removing customers from waitlist' and 'viewing the waitlist' features.
//July 13, 2023. Added and improved the seating feature.

// Will add the visitor history feature.

#include <iostream>
#include <queue>
using namespace std;

struct MenuItem {
    string name;
    double price;
    MenuItem* next;

    MenuItem(string n = "", double p = 0.0) {
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
        // Adding default menu items
        addFood("Burger", 4.99);
        addFood("Pizza", 7.99);
        addFood("Salad", 5.99);
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

    void removeFood(string name) {
        if (head == nullptr) {
            cout << "Menu is empty." << endl;
            return;
        }

        MenuItem* temp = head;
        MenuItem* prev = nullptr;

        while (temp != nullptr) {
            if (temp->name == name) {
                if (prev == nullptr) {
                    // Removing the first node
                    head = temp->next;
                } else {
                    prev->next = temp->next;
                }

                delete temp;
                cout << "Food item removed: " << name << endl;
                return;
            }

            prev = temp;
            temp = temp->next;
        }

        cout << "Food item not found." << endl;
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
    int maxSeats;

public:
    SeatList() {
        head = nullptr;
        availableSeats = 0;
        maxSeats = 0;
        setMaxSeats(50); // Setting default maximum seats to 50
    }

    void setMaxSeats(int max) {
        maxSeats = max;

        if (head != nullptr) {
            // Clear existing seats
            Seat* temp = head;
            while (temp != nullptr) {
                Seat* nextSeat = temp->next;
                delete temp;
                temp = nextSeat;
            }
            head = nullptr;
        }

        for (int i = 1; i <= maxSeats; i++) {
            addSeat(i);
        }
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
        cout << "Available seats: " << availableSeats << endl;
    }

    int getAvailableSeatCount() {
        return availableSeats;
    }

    bool occupySeats(int numSeats) {
        if (numSeats <= 0 || numSeats > availableSeats) {
            cout << "Invalid number of seats to occupy." << endl;
            return false;
        }

        int occupiedCount = 0;
        Seat* temp = head;

        while (temp != nullptr && occupiedCount < numSeats) {
            if (!temp->isOccupied) {
                temp->isOccupied = true;
                occupiedCount++;
                availableSeats--;
            }
            temp = temp->next;
        }

        if (occupiedCount == numSeats) {
            cout << "Successfully occupied " << occupiedCount << " seat(s)." << endl;
            return true;
        } else {
            cout << "Insufficient available seats." << endl;
            return false;
        }
    }
};

class Canteen {
private:
    Menu menu;
    SeatList seatList;
    queue<string> waitingList;
    bool isAdmin;

public:
    Canteen() {
        isAdmin = false;
    }

    void setMaxSeats(int max) {
        seatList.setMaxSeats(max);
    }

    void addFood(string name, double price) {
        menu.addFood(name, price);
        cout << "Food item added: " << name << " - $" << price << endl;
    }

    void removeFood(string name) {
        menu.removeFood(name);
    }

    void displayMenu() {
        menu.displayMenu();
    }

    void addCustomerToWaitingList(string customerName) {
        waitingList.push(customerName);
        cout << customerName << " has been added to the waiting list." << endl;
    }
    void removeCustomerFromWaitingList() {
    if (waitingList.empty()) {
        cout << "Waiting list is empty." << endl;
    } else {
        displayWaitingList();

        cout << "Enter the name of the customer to remove: ";
        string customerName;
        cin.ignore();
        getline(cin, customerName);

        bool found = false;
        queue<string> temp;

        while (!waitingList.empty()) {
            string frontCustomer = waitingList.front();
            waitingList.pop();

            if (frontCustomer == customerName) {
                found = true;
                cout << "Removed customer: " << frontCustomer << " from the waiting list." << endl;
                break;
            } else {
                temp.push(frontCustomer);
            }
        }

        while (!temp.empty()) {
            waitingList.push(temp.front());
            temp.pop();
        }

        if (!found) {
            cout << "Customer not found in the waiting list." << endl;
        }
    }
}

    void displayWaitingList() {
        if (waitingList.empty()) {
            cout << "Waiting list is empty." << endl;
        } else {
            cout << "Waiting list:" << endl;
            int count = 1;
            queue<string> temp = waitingList;
            while (!temp.empty()) {
                cout << count << ". " << temp.front() << endl;
                temp.pop();
                count++;
            }
        }
    }

    void occupySeats() {
        int numSeats;
        cout << "Enter the number of seats to occupy: ";
        cin >> numSeats;

        seatList.occupySeats(numSeats);
    }

    bool isAdminUser() {
        return isAdmin;
    }

    void loginAdmin() {
        string adminPassword;
        cout << "Enter admin password: ";
        cin.ignore();
        getline(cin, adminPassword);

        // Add password verification logic here
        if (adminPassword == "admin") {
            isAdmin = true;
            cout << "Logged in as admin." << endl;
        } else {
            cout << "Incorrect password. Login failed." << endl;
        }
    }

    void logoutAdmin() {
        isAdmin = false;
        cout << "Logged out from admin." << endl;
    }

    void displayAvailableSeats() {
        seatList.displayAvailableSeats();
    }
};

int main() {
    Canteen canteen;
    int choice;

    while (true) {
        cout << "----------- Canteen Management System -----------" << endl;
        canteen.displayMenu();
        canteen.displayAvailableSeats();
        cout << "1. Add Customer" << endl;
        cout << "2. Remove Customer from Waiting List" << endl;
        cout << "3. Display Waiting List" << endl;
        cout << "4. Occupy Seats" << endl;

        if (canteen.isAdminUser()) {
            cout << "5. Add Food" << endl;
            cout << "6. Logout from Admin" << endl;
        } else {
            cout << "5. Login as Admin" << endl;
            cout << "6. Exit" << endl;
        }

        cout << "------------------------------------------------" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1: {
                string customerName;
                cout << "Enter customer name: ";
                cin.ignore();
                getline(cin, customerName);
                canteen.addCustomerToWaitingList(customerName);
                break;
            }
            case 2:
                canteen.removeCustomerFromWaitingList();
                break;
            case 3:
                canteen.displayWaitingList();
                break;
            case 4:
                canteen.occupySeats();
                break;
            case 5:
                if (canteen.isAdminUser()) {
                    string foodName;
                    double foodPrice;
                    cout << "Enter food name: ";
                    cin.ignore();
                    getline(cin, foodName);
                    cout << "Enter food price: $";
                    cin >> foodPrice;
                    canteen.addFood(foodName, foodPrice);
                } else {
                    canteen.loginAdmin();
                }
                break;
            case 6:
                if (canteen.isAdminUser()) {
                    canteen.logoutAdmin();
                } else {
                    cout << "Exiting the program..." << endl;
                    return 0;
                }
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }

        cout << endl;
    }

    return 0;
}
