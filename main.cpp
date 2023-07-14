//July 10, 2023. Finished on making the initial code, currently planning and polishing the main features.
//July 12, 2023. Improved the queue by adding the 'removing customers from waitlist' and 'viewing the waitlist' features.
//July 13, 2023. Added and improved the seating feature.
//July 14, 2023. Added a Food Menu and Waiting List per Stall.

// will improve the UI
// will add the visitor history feature.

#include <iostream>
#include <queue>
#include <map>
#include <algorithm>
using namespace std;

struct MenuItem {
    string name;
    double price;

    MenuItem(string n = "", double p = 0.0) {
        name = n;
        price = p;
    }
};

class Menu {
private:
    map<string, vector<MenuItem>> menuItems;

public:
    Menu() {
        // Adding default menu items
        addFood("Burger", 4.99, "Stall A");
        addFood("Pizza", 7.99, "Stall B");
        addFood("Salad", 5.99, "Stall A");
    }

    void addFood(string name, double price, string stall) {
        menuItems[stall].push_back(MenuItem(name, price));
    }

    void removeFood(string name, string stall) {
        auto it = menuItems.find(stall);
        if (it != menuItems.end()) {
            vector<MenuItem>& items = it->second;
            items.erase(remove_if(items.begin(), items.end(), [&](const MenuItem& item) {
                return item.name == name;
            }), items.end());
            cout << "Food item removed: " << name << endl;
        } else {
            cout << "Stall not found." << endl;
        }
    }

    void displayMenuPerStall() {
        if (menuItems.empty()) {
            cout << "Menu is empty." << endl;
        } else {
            cout << "Food Menu per Stall:" << endl;
            for (const auto& stall : menuItems) {
                cout << "Stall: " << stall.first << endl;
                const vector<MenuItem>& items = stall.second;
                for (const auto& item : items) {
                    cout << item.name << " - $" << item.price << endl;
                }
                cout << endl;
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
    vector<string> stalls;

public:
    Canteen() {}

    void setMaxSeats(int max) {
        seatList.setMaxSeats(max);
    }

    void addStall(string stallName) {
        stalls.push_back(stallName);
    }

    void addFoodToStall(string name, double price, string stallName) {
        menu.addFood(name, price, stallName);
        cout << "Food item added: " << name << " - $" << price << " (Stall: " << stallName << ")" << endl;
    }

    void removeFoodFromStall(string name, string stallName) {
        menu.removeFood(name, stallName);
    }

    void displayMenuPerStall() {
        menu.displayMenuPerStall();
    }

    void addCustomerToWaitingList(string customerName) {
        waitingList.push(customerName);
        cout << customerName << " has been added to the waiting list." << endl;
    }

    void serveNextCustomer() {
        if (!waitingList.empty()) {
            cout << "Next customer: " << waitingList.front() << endl;
            waitingList.pop();
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

    void occupySeats() {
        int numSeats;
        cout << "Enter the number of seats to occupy: ";
        cin >> numSeats;

        seatList.occupySeats(numSeats);
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
};

int main() {
    Canteen canteen;
    bool isAdmin = false;
    int choice;

    while (true) {
        cout << "----------- Canteen Management System -----------" << endl;
        canteen.displayMenuPerStall();
        canteen.displayAvailableSeats();
        cout << "1. Add Customer" << endl;
        cout << "2. Occupy Seats" << endl;
        cout << "3. Display Waiting List" << endl;

        if (isAdmin) {
            cout << "4. Add Food to Stall" << endl;
            cout << "5. Remove Food from Stall" << endl;
            cout << "6. Set Maximum Seats" << endl;
            cout << "7. Logout" << endl;
        } else {
            cout << "4. Login as Admin" << endl;
            cout << "5. Exit" << endl;
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
                canteen.occupySeats();
                break;
            case 3:
                canteen.displayWaitingList();
                break;
            case 4: {
                if (isAdmin) {
                    string foodName, stallName;
                    double foodPrice;
                    cout << "Enter food name: ";
                    cin.ignore();
                    getline(cin, foodName);
                    cout << "Enter food price: $";
                    cin >> foodPrice;
                    cout << "Enter stall name: ";
                    cin.ignore();
                    getline(cin, stallName);
                    canteen.addFoodToStall(foodName, foodPrice, stallName);
                } else {
                    string adminPassword;
                    cout << "Enter admin password: ";
                    cin.ignore();
                    getline(cin, adminPassword);

                    // Add password verification logic here
                    if (adminPassword == "pass") {
                        isAdmin = true;
                        cout << "Logged in as admin." << endl;
                    } else {
                        cout << "Incorrect password. Login failed." << endl;
                    }
                }
                break;
            }
            case 5:
                if (isAdmin) {
                    canteen.displayMenuPerStall();
                    string foodName, stallName;
                    cout << "Enter the food name to remove: ";
                    cin.ignore();
                    getline(cin, foodName);
                    cout << "Enter the stall name: ";
                    getline(cin, stallName);
                    canteen.removeFoodFromStall(foodName, stallName);
                } else {
                    cout << "Exiting the program..." << endl;
                    return 0;
                }
                break;
            case 6:
                if (isAdmin) {
                    int maxSeats;
                    cout << "Enter the maximum number of seats: ";
                    cin >> maxSeats;
                    canteen.setMaxSeats(maxSeats);
                    cout << "Maximum seats set to " << maxSeats << "." << endl;
                } else {
                    cout << "Invalid choice. Please try again." << endl;
                }
                break;
            case 7:
                if (isAdmin) {
                    isAdmin = false;
                    cout << "Logged out." << endl;
                } else {
                    cout << "Invalid choice. Please try again." << endl;
                }
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }

        cout << endl;
    }

    return 0;
}

