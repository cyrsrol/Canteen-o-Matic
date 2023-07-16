//July 10, 2023. Finished on making the initial code, currently planning and polishing the main features.
//July 12, 2023. Improved the queue by adding the 'viewing the waitlist' features.
//July 13, 2023. Added and improved the seating feature.
//July 14, 2023. Added a Food Menu and Waiting List per Stall.
//July 14, 2023. Added the visitor history feature.
//July 16, 2023. Added <ctime> to record when the customer joins the waiting list, and when they get served.
//July 16, 2023. Improved the UI using <iomanip>.
//July 16, 2023. Improved some of the features also apply some error handling.

//Will continue to improve the UI.

#include <iostream>
#include <queue>
#include <map>
#include <algorithm>
#include <iomanip>
#include <ctime>
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
    map<string, queue<MenuItem>> menuItems;

public:
    Menu() {
        addFood("Burger", 4.99, "Stall A");
        addFood("Pizza", 7.99, "Stall B");
        addFood("Salad", 5.99, "Stall A");
    }

    void addFood(const string& name, double price, const string& stall) {
        menuItems[stall].push(MenuItem(name, price));
    }

    void removeFood(const string& name, const string& stall) {
        auto it = menuItems.find(stall);
        if (it != menuItems.end()) {
            queue<MenuItem>& items = it->second;
            int itemCount = items.size();
            for (int i = 0; i < itemCount; i++) {
                MenuItem item = items.front();
                if (item.name == name) {
                    cout << "Food item removed: " << item.name << endl;
                } else {
                    items.push(item);
                }
                items.pop();
            }
        } else {
            cout << "Stall not found." << endl;
        }
    }

    void displayMenuPerStall() {
        if (menuItems.empty()) {
            cout << "Menu is empty." << endl;
        } else {
            vector<string> uniqueStalls;
            for (const auto& stall : menuItems) {
                uniqueStalls.push_back(stall.first);
            }
            sort(uniqueStalls.begin(), uniqueStalls.end());

            int maxItems = 0;
            for (const auto& stall : uniqueStalls) {
                maxItems = max(maxItems, static_cast<int>(menuItems[stall].size()));
            }

            int nameWidth = 12; // Width for displaying food names
            int priceWidth = 6; // Width for displaying food prices

            cout << "\t\t+";
            for (size_t i = 0; i < uniqueStalls.size(); ++i) {
                cout << "---------------------+";
            }
            cout << "" << endl;
            cout << "\t\t";
            for (const auto& stall : uniqueStalls) {
                cout << "| " << setw(nameWidth + priceWidth + 2) << left << stall;
            }
            cout << "|" << endl;

            cout << "\t\t|";
            for (size_t i = 0; i < uniqueStalls.size(); ++i) {
                cout << "---------------------|";
            }
            cout << "" << endl;
            for (int i = 0; i < maxItems; i++) {
                cout << "\t\t";
                for (const auto& stall : uniqueStalls) {
                    if (i < menuItems[stall].size()) {
                        MenuItem item = menuItems[stall].front();
                        menuItems[stall].pop();
                        cout << "| " << setw(nameWidth) << left << item.name << "$" << setw(priceWidth) << left << fixed << setprecision(2) << item.price << " ";
                        menuItems[stall].push(item);
                    } else {
                        cout << "| " << setw(nameWidth) << right << " " << setw(priceWidth + 2) << right << "";
                    }
                }
                cout << "|" << endl;
            }

            cout << "\t\t+";
            for (size_t i = 0; i < uniqueStalls.size(); ++i) {
                cout << "---------------------+";
            }
            cout << "" << endl;
        }
    }

    bool isStallValid(const string& stall) {
        return menuItems.count(stall) > 0;
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
        setMaxSeats(50);
    }

    ~SeatList() {
        while (head != nullptr) {
            Seat* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void setMaxSeats(int max) {
        maxSeats = max;

        while (head != nullptr) {
            Seat* temp = head;
            head = head->next;
            delete temp;
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
    map<string, queue<pair<time_t, string>>> waitingList;
    queue<string> visitorHistory;
    vector<string> stalls;

public:
    Canteen() {}

    void setMaxSeats(int max) {
        seatList.setMaxSeats(max);
    }

    void addStall(const string& stallName) {
        stalls.push_back(stallName);
    }

    void addFoodToStall(const string& name, double price, const string& stallName) {
        menu.addFood(name, price, stallName);
        cout << "Food item added: " << name << " - $" << price << " (Stall: " << stallName << ")" << endl;
    }

    void removeFoodFromStall(const string& name, const string& stallName) {
        menu.removeFood(name, stallName);
    }

    void displayMenuPerStall() {
        menu.displayMenuPerStall();
    }

    void addCustomerToWaitingList(const string& customerName, const string& stallName) {
        if (menu.isStallValid(stallName)) {
            time_t currentTime = time(nullptr);
            waitingList[stallName].push(make_pair(currentTime, customerName));
            cout << customerName << " has been added to the waiting list of " << stallName << "." << endl;
        } else {
            cout << "Invalid stall name. Please choose from the available stalls." << endl;
        }
    }

    void serveCustomer() {
        string stallName;
        cout << "Enter the stall name: ";
        cin.ignore();
        getline(cin, stallName);

        if (!waitingList.empty()) {
            auto it = waitingList.find(stallName);
            if (it != waitingList.end() && !it->second.empty()) {
                time_t servedTime = time(nullptr);
                time_t joinedTime = it->second.front().first;
                string customerName = it->second.front().second;
                it->second.pop();
                if (it->second.empty()) {
                    waitingList.erase(it);
                }
                cout << "Customer at " << stallName << ": " << customerName << endl;
                cout << "Joined at: " << ctime(&joinedTime);
                cout << "Served at: " << ctime(&servedTime);
                visitorHistory.push(customerName);
            } else {
                cout << "No customer to serve at " << stallName << "." << endl;
            }
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

        if (cin.fail()) {
            cout << "Invalid input. Please enter a valid number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        if (numSeats <= 0) {
            cout << "Invalid number of seats to occupy. Please enter a positive number." << endl;
            return;
        }

        seatList.occupySeats(numSeats);
    }

    void displayWaitingList() {
        if (waitingList.empty()) {
            cout << "Waiting list is empty." << endl;
        } else {
            cout << "--------------------- Waiting List ---------------------" << endl;
            for (const auto& entry : waitingList) {
                string stallName = entry.first;
                const queue<pair<time_t, string>>& customers = entry.second;
                cout << "Stall: " << stallName << endl;
                queue<pair<time_t, string>> temp = customers;
                int count = 1;
                while (!temp.empty()) {
                    time_t joinedTime = temp.front().first;
                    string customerName = temp.front().second;
                    struct tm* timeInfo = localtime(&joinedTime);
                    char timeStr[80];
                    strftime(timeStr, sizeof(timeStr), "%c", timeInfo);
                    cout << count << ". " << customerName << " (Joined at: " << timeStr << ")" << endl;
                    temp.pop();
                    count++;
                }
                cout << endl;
            }
            cout << "--------------------------------------------------------" << endl;
        }
    }

    void displayVisitorHistory() {
        if (visitorHistory.empty()) {
            cout << "Visitor history is empty." << endl;
        } else {
            cout << "Visitor history:" << endl;
            int count = 1;
            queue<string> temp = visitorHistory;
            while (!temp.empty()) {
                cout << count << ". " << temp.front() << endl;
                temp.pop();
                count++;
            }
        }
    }
};

void queueIn(Canteen& canteen) {
    canteen.addCustomerToWaitingList("polo1", "Stall A");
    canteen.addCustomerToWaitingList("polo2", "Stall A");
    canteen.addCustomerToWaitingList("mary1", "Stall B");
    canteen.addCustomerToWaitingList("mary2", "Stall B");
    string customerName, stallName;
    cout << "Enter stall name: ";
    cin.ignore();
    getline(cin, stallName);
    cout << "Enter customer name: ";
    getline(cin, customerName);
    canteen.addCustomerToWaitingList(customerName, stallName);
}

void serveCustomer(Canteen& canteen) {
    canteen.serveCustomer();
}

void addFoodToStall(Canteen& canteen) {
    canteen.displayMenuPerStall();
    string newFoodName, stallName;
    double newFoodPrice;

    cout << "Enter the food name to add: ";
    cin.ignore();
    getline(cin, newFoodName);

    if (newFoodName.length() > 12) {
        newFoodName = newFoodName.substr(0, 12);
    }

    cout << "Enter the food price: $";
    cin >> newFoodPrice;

    if (cin.fail()) {
        cout << "Invalid input. Please enter a valid number." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    cout << "Enter the stall name: ";
    cin.ignore();
    getline(cin, stallName);

    if (stallName.length() > 12) {
        stallName = stallName.substr(0, 12);
    }

    canteen.addFoodToStall(newFoodName, newFoodPrice, stallName);
}

void removeFoodFromStall(Canteen& canteen) {
    canteen.displayMenuPerStall();
    string removeFoodName, stallName;

    cout << "Enter the food name to remove: ";
    cin.ignore();
    getline(cin, removeFoodName);

    if (removeFoodName.length() > 12) {
        removeFoodName = removeFoodName.substr(0, 12);
    }

    cout << "Enter the stall name: ";
    getline(cin, stallName);

    if (stallName.length() > 12) {
        stallName = stallName.substr(0, 12);
    }

    canteen.removeFoodFromStall(removeFoodName, stallName);
}

void setMaxSeats(Canteen& canteen) {
    int maxSeats;
    cout << "Enter the maximum number of seats: ";
    cin >> maxSeats;

    if (cin.fail()) {
        cout << "Invalid input. Please enter a valid number." << endl;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    if (maxSeats <= 0) {
        cout << "Invalid number of seats. Please enter a positive number." << endl;
        return;
    }

    canteen.setMaxSeats(maxSeats);
    cout << "Maximum seats set to " << maxSeats << "." << endl;
}

void adminMenu(Canteen& canteen) {
    int choice;
    do {
        cout << "  .--.  _____  _  _  _____  ___  ___  _  _        .---.        .-._.-.  _____  _____  _  ____ \n";
        cout << " '  __)'  _  '| || |(_   _)| __)| __)| || |  __  '  _  '  __  |  _ _  ||  _  |(_   _)'_'|  __)\n";
        cout << " | |__ | (_) || .` |  | |  | _) | _) | .` | (__) | [_] | (__) | | v | || (_) |  | |  | || |__ \n";
        cout << " '.___)|_[ ]_||_||_|  |_|  |___)|___)|_||_|      '.___.'      |_|   |_||_[ ]_|  |_|  |_||____)\n";
        cout << "\n";
        cout << "----------- Canteen Management System -----------" << endl;
        cout << "--------- Admin Menu ---------" << endl;
        cout << "1. Serve Customer" << endl;
        cout << "2. Add Food to Stall" << endl;
        cout << "3. Remove Food from Stall" << endl;
        cout << "4. Add Seats" << endl;
        cout << "5. Display Visitor History" << endl;
        cout << "6. Logout" << endl;
        cout << "-----------------------------" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                serveCustomer(canteen);
                break;
            case 2:
                addFoodToStall(canteen);
                break;
            case 3:
                removeFoodFromStall(canteen);
                break;
            case 4:
                setMaxSeats(canteen);
                break;
            case 5:
                canteen.displayVisitorHistory();
                break;
            case 6:
                cout << "\t\tLogged out from admin menu." << endl;
                return;
            default:
                cout << "\t\tInvalid choice. Please try again." << endl;
        }

        cout << endl;
    } while (true);
}

void loginAsAdmin(Canteen& canteen, bool& isAdmin, const string& adminPassword) {
    string password;
    int invalidAttempts = 0;
    do {
        cout << "\t\tEnter admin password: ";
        cin.ignore();
        getline(cin, password);

        if (password == adminPassword) {
            isAdmin = true;
            cout << "\t\tLogged in as admin." << endl;
            adminMenu(canteen);
            break;
        } else {
            invalidAttempts++;
            cout << "\t\tIncorrect password. Login failed." << endl;
            if (invalidAttempts == 3) {
                cout << "\t\tExceeded maximum login attempts. Returning to the main menu." << endl;
                break;
            }
        }
    } while (true);
}

int main() {
    Canteen canteen;
    bool isAdmin = false;
    string adminPassword = "pass"; // Set the admin password here
    int choice;

    while (true) {
        cout << "\t\t .--.  _____  _  _  _____  ___  ___  _  _        .---.        .-._.-.  _____  _____  _  ____ \n";
        cout << "\t\t'  __)'  _  '| || |(_   _)| __)| __)| || |  __  '  _  '  __  |  _ _  ||  _  |(_   _)'_'|  __)\n";
        cout << "\t\t| |__ | (_) || .` |  | |  | _) | _) | .` | (__) | [_] | (__) | | v | || (_) |  | |  | || |__ \n";
        cout << "\t\t'.___)|_[ ]_||_||_|  |_|  |___)|___)|_||_|      '.___.'      |_|   |_||_[ ]_|  |_|  |_||____)\n";
        cout << "\n";
        cout << "\t\t-------------------------------- Canteen Management System ----------------------------------\n\n";
        cout << "\t\t\t\t\t\t     ";canteen.displayAvailableSeats();
        cout << "\n\t\t----------------------------------------- Food Menu ----------------------------------------\n";
        canteen.displayMenuPerStall();

        cout << "\t\t---------------------------------------------------------------------------------------------\n" << endl;
        cout << "\t\t\t\t\t\t 1. Queue In" << endl;
        cout << "\t\t\t\t\t\t 2. Occupy Seat(s)" << endl;
        cout << "\t\t\t\t\t\t 3. Display Waiting List" << endl;
        cout << "\t\t\t\t\t\t 4. Login as Admin" << endl;
        cout << "\t\t\t\t\t\t 5. Exit" << endl;

        cout << "\n\t\t---------------------------------------------------------------------------------------------" << endl;
        cout << "\t\t\t\t\t\tEnter your choice: ";
        cin >> choice;

        if (cin.fail()) {
            cout << "\t\tInvalid input. Please enter a valid number." << endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1:
                queueIn(canteen);
                break;
            case 2:
                canteen.occupySeats();
                break;
            case 3:
                canteen.displayWaitingList();
                break;
            case 4:
                loginAsAdmin(canteen, isAdmin, adminPassword);
                break;
            case 5:
                cout << "\t\tExiting the program." << endl;
                return 0;
            default:
                cout << "\t\tInvalid choice. Please try again." << endl;
                break;
        }

        cout << endl;
    }

    return 0;
}
