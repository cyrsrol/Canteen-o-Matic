//July 10, 2023. Finished on making the base code, currently planning and polishing the main features.
//July 12, 2023. Improved the queue by adding the 'viewing the waitlist' features.
//July 13, 2023. Fixed the seating feature.
//July 14, 2023. Added <map> and <algorithm> to display and sort Food Menu and Waiting List per Stall.
//July 14, 2023. Added the visitor history feature.
//July 16, 2023. Added <ctime> to record when the customer joins the waiting list, and when they get served.
//July 16, 2023. Improved the UI using <iomanip>.
//July 16, 2023. Improved and changed some of the features and apply some error handling.
//July 17, 2023. Improved the UI and added <windows.h> to apply color to text.

#include <iostream>
#include <queue>
#include <map>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <windows.h>

using namespace std;

HANDLE h = GetStdHandle(STD_OUTPUT_HANDLE);

void header() {
    SetConsoleTextAttribute(h, 11);
    cout << "\t\t _____  _____  _   _  _____  _____  _____  _   _           __   __  _____  _____  __  _____ \n";
    cout << "\t\t|     ||  _  || | | ||     ||     ||     || | | |  _____  |  |_|  ||  _  ||     ||  ||     |\n";
    cout << "\t\t|   __|| |_| ||  || ||_   _||  ___||  ___||  || | |  _  | |       || |_| ||_   _||  ||   __|\n";
    cout << "\t\t|  |   |     ||   ' |  | |  | |___ | |___ |   ' | | | | | |       ||     |  | |  |  ||  |   \n";
    cout << "\t\t|  |__ |  _  ||  .  |  | |  |  ___||  ___||  .  | | |_| | | ||_|| ||  _  |  | |  |  ||  |__ \n";
    cout << "\t\t|     || | | || | ' |  | |  | |___ | |___ | |   | |_____| | |   | || | | |  | |  |  ||     |\n";
    cout << "\t\t|_____||_| |_||_| |_|  |_|  |_____||_____||_| |_|         |_|   |_||_| |_|  |_|  |__||_____|\n\n";
    SetConsoleTextAttribute(h, 14);
    cout << "\t\t-------------------------------- Canteen Management System ----------------------------------\n\n";
    SetConsoleTextAttribute(h, 15);
}
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
    Menu() {   // "/*" Can be removed to set default stalls
        /*
        addFood("Burger", 50, "Mcdo");
        addFood("Fries", 35, "Mcdo");
        addFood("Chicken Joy", 90, "Jabe");
        addFood("CokeFloat", 40, "Jabe");
        addFood("Chicken", 90, "KFC");
        addFood("Inihaw", 99, "Mang Inasal");
        */
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
                    system("cls");
                    SetConsoleTextAttribute(h, 10);
                    cout << "\t\tFood item removed: " << item.name;
                    SetConsoleTextAttribute(h, 15);
                } else {
                    system("cls");
                    SetConsoleTextAttribute(h, 12);
                    cout << "\t\tFood name not found.";
                    SetConsoleTextAttribute(h, 15);
                    items.push(item);
                }
                items.pop();
            }
        } else {
            SetConsoleTextAttribute(h, 12);
            system("cls");
            cout << "\t\tStall not found.";
            SetConsoleTextAttribute(h, 15);
        }
    }

    void displayMenuPerStall() {
        if (menuItems.empty()) {
            SetConsoleTextAttribute(h, 12);
            cout << "\t\tMenu is empty.\n";
            SetConsoleTextAttribute(h, 15);
        } else {
            int count = 0;
            vector<string> uniqueStalls;
            for (const auto& stall : menuItems) {
                uniqueStalls.push_back(stall.first);
            }
            sort(uniqueStalls.begin(), uniqueStalls.end());

            int maxItems = 0;
            for (const auto& stall : uniqueStalls) {
                maxItems = max(maxItems, static_cast<int>(menuItems[stall].size()));
            }

            int nameWidth = 12;
            int priceWidth = 6;
            SetConsoleTextAttribute(h, 11);
            cout << "\t\t  +";
            for (size_t i = 0; i < uniqueStalls.size(); ++i) {

                cout << "---------------------+";
            }
            cout << "\n";
            cout << "\t\t  ";
            for (const auto& stall : uniqueStalls) {
                cout << "| " << setw(nameWidth + priceWidth + 2) << left << stall;
            }
            cout << "|\n";

            cout << "\t\t  |";
            for (size_t i = 0; i < uniqueStalls.size(); ++i) {
                cout << "---------------------|";
            }
            cout << "\n";
            for (int i = 0; i < maxItems; i++) {
                cout << "\t\t  ";
                for (const auto& stall : uniqueStalls) {
                    if (i < menuItems[stall].size()) {
                        MenuItem item = menuItems[stall].front();
                        menuItems[stall].pop();
                        cout << "| " << setw(nameWidth) << left << item.name << "P" << setw(priceWidth) << left << fixed << setprecision(2) << item.price << " ";
                        menuItems[stall].push(item);
                    } else {
                        cout << "| " << setw(nameWidth) << right << " " << setw(priceWidth + 2) << right << "";
                    }
                }
                cout << "|\n";
            }

            cout << "\t\t  +";
            for (size_t i = 0; i < uniqueStalls.size(); ++i) {
                cout << "---------------------+";
            }
            cout << "\n";
            SetConsoleTextAttribute(h, 15);
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
        SetConsoleTextAttribute(h, 11);
        cout << "Available seats: " << availableSeats << "\n";
        SetConsoleTextAttribute(h, 15);
    }

    int getAvailableSeatCount() {
        return availableSeats;
    }

    bool occupySeats(int numSeats) {
        if (numSeats <= 0 || numSeats > availableSeats) {
            system("cls");
            SetConsoleTextAttribute(h, 12);
            cout << "\t\tInvalid number of seats to occupy.";
            SetConsoleTextAttribute(h, 15);
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
            system("cls");
            SetConsoleTextAttribute(h, 10);
            cout << "\t\tSuccessfully occupied " << occupiedCount << " seat(s).";
            SetConsoleTextAttribute(h, 15);
            return true;
        } else {
            system("cls");
            SetConsoleTextAttribute(h, 12);
            cout << "\t\tInsufficient available seats.";
            SetConsoleTextAttribute(h, 15);
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
        system("cls");
        SetConsoleTextAttribute(h, 10);
        cout << "\t\tFood item added: " << name << " - P" << price << " (Stall: " << stallName << ")";
        SetConsoleTextAttribute(h, 15);
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
            system("cls");
            SetConsoleTextAttribute(h, 10);
            cout << "\t\t" << customerName << " has been added to the waiting list of " << stallName << ".";
            SetConsoleTextAttribute(h, 15);
        } else {
            system("cls");
            SetConsoleTextAttribute(h, 12);
            cout << "\t\tInvalid stall name. Please choose from the available stalls.";
            SetConsoleTextAttribute(h, 15);
        }
    }

    void serveCustomer() {
        if (!waitingList.empty()) {
            system("cls");
            SetConsoleTextAttribute(h, 11);
            cout << "\t\t\t\t\t _______  _______  ______    __   __  _______ \n";
            cout << "\t\t\t\t\t|       ||       ||    _ |  |  | |  ||       |\n";
            cout << "\t\t\t\t\t|  _____||    ___||   | ||  |  |_|  ||    ___|\n";
            cout << "\t\t\t\t\t| |_____ |   |___ |   |_||_ |       ||   |___ \n";
            cout << "\t\t\t\t\t|_____  ||    ___||    __  ||       ||    ___|\n";
            cout << "\t\t\t\t\t _____| ||   |___ |   |  | | |     | |   |___ \n";
            cout << "\t\t\t\t\t|_______||_______||___|  |_|  |___|  |_______|\n";
            cout << "\n\t\t---------------------------------------------------------------------------------------------\n";
            displayWaitingList();
            SetConsoleTextAttribute(h, 14);
            string stallName;
            SetConsoleTextAttribute(h, 14);
            cout << "\t\t\t\tEnter the stall name: ";
            SetConsoleTextAttribute(h, 15);
            cin.ignore();
            getline(cin, stallName);
            auto it = waitingList.find(stallName);
            if (it != waitingList.end() && !it->second.empty()) {
                time_t servedTime = time(nullptr);
                time_t joinedTime = it->second.front().first;
                string customerName = it->second.front().second;
                it->second.pop();
                if (it->second.empty()) {
                    waitingList.erase(it);
                }
                system("cls");
                SetConsoleTextAttribute(h, 11);
                cout << "\t\t\t\t    _______  _______  ______    __   __  _______  ______  \n";
                cout << "\t\t\t\t   |       ||       ||    _ |  |  | |  ||       ||      | \n";
                cout << "\t\t\t\t   |  _____||    ___||   | ||  |  |_|  ||    ___||  _    |\n";
                cout << "\t\t\t\t   | |_____ |   |___ |   |_||_ |       ||   |___ | | |   |\n";
                cout << "\t\t\t\t   |_____  ||    ___||    __  ||       ||    ___|| |_|   |\n";
                cout << "\t\t\t\t    _____| ||   |___ |   |  | | |     | |   |___ |       |\n";
                cout << "\t\t\t\t   |_______||_______||___|  |_|  |___|  |_______||______| \n";
                cout << "\n\t\t---------------------------------------------------------------------------------------------\n";
                SetConsoleTextAttribute(h, 14);
                cout << "\t\t\t\t   Customer at " << stallName << ": " << customerName << "\n";
                SetConsoleTextAttribute(h, 10);
                cout << "\t\t\t\t   Joined at: " << ctime(&joinedTime);
                SetConsoleTextAttribute(h, 12);
                cout << "\t\t\t\t   Served at: " << ctime(&servedTime);
                SetConsoleTextAttribute(h, 15);
                visitorHistory.push(customerName);
                SetConsoleTextAttribute(h, 11);
                cout << "\t\t---------------------------------------------------------------------------------------------\n";
                cout << "\t\t\t\t   ";
                SetConsoleTextAttribute(h, 15);
                system("pause");
                system("cls");
            } else {
                system("cls");
                SetConsoleTextAttribute(h, 12);
                cout << "\t\tInvalid stall name. Please choose from the available stalls..";
                SetConsoleTextAttribute(h, 15);
            }
        } else {
            system("cls");
            SetConsoleTextAttribute(h, 12);
            cout << "\t\tWaiting list is empty.";
            SetConsoleTextAttribute(h, 15);
        }
    }

    void displayAvailableSeats() {
        seatList.displayAvailableSeats();
    }

    int getAvailableSeatCount() {
        return seatList.getAvailableSeatCount();
    }

    void occupySeats() {
        system("cls");
        SetConsoleTextAttribute(h, 11);
        cout << "\t\t _______  _______  _______  __   __  _______  __   __    _______  _______  _______  _______ \n";
        cout << "\t\t|       ||       ||       ||  | |  ||       ||  | |  |  |       ||       ||   _   ||       |\n";
        cout << "\t\t|   _   ||       ||       ||  | |  ||    _  ||  |_|  |  |  _____||    ___||  |_|  ||_     _|\n";
        cout << "\t\t|  | |  ||       ||       ||  |_|  ||   |_| ||       |  | |_____ |   |___ |       |  |   |  \n";
        cout << "\t\t|  |_|  ||      _||      _||       ||    ___||_     _|  |_____  ||    ___||       |  |   |  \n";
        cout << "\t\t|       ||     |_ |     |_ |       ||   |      |   |     _____| ||   |___ |   _   |  |   |  \n";
        cout << "\t\t|_______||_______||_______||_______||___|      |___|    |_______||_______||__| |__|  |___|  \n";
        cout << "\n\t\t---------------------------------------------------------------------------------------------\n";
        SetConsoleTextAttribute(h, 14);
        int numSeats;
        cout << "\t\t\t\t\tEnter the number of seats to occupy: ";
        SetConsoleTextAttribute(h, 15);
        cin >> numSeats;

        if (cin.fail()) {
            system("cls");
            SetConsoleTextAttribute(h, 12);
            cout << "\t\tInvalid input. Please enter a valid number.";
            SetConsoleTextAttribute(h, 15);
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        if (numSeats <= 0) {
            system("cls");
            SetConsoleTextAttribute(h, 12);
            cout << "\t\tInvalid number of seats to occupy. Please enter a positive number.";
            SetConsoleTextAttribute(h, 15);
            return;
        }

        seatList.occupySeats(numSeats);
    }

    void displayWaitingList() {
        if (waitingList.empty()) {
            system("cls");
            SetConsoleTextAttribute(h, 12);
            cout << "\t\tWaiting list is empty.";
            SetConsoleTextAttribute(h, 15);
        } else {
            for (const auto& entry : waitingList) {
                string stallName = entry.first;
                const queue<pair<time_t, string>>& customers = entry.second;
                SetConsoleTextAttribute(h, 14);
                cout << "\t\t" << stallName << "\n";
                queue<pair<time_t, string>> temp = customers;
                int count = 1;
                while (!temp.empty()) {
                    time_t joinedTime = temp.front().first;
                    string customerName = temp.front().second;
                    struct tm* timeInfo = localtime(&joinedTime);
                    char timeStr[80];
                    strftime(timeStr, sizeof(timeStr), "%c", timeInfo);
                    SetConsoleTextAttribute(h, 10);
                    cout << "\t\t" << count << ". " << customerName << " (Joined at: " << timeStr << ")\n";
                    temp.pop();
                    count++;
                }
                cout << "\n";
            }
            SetConsoleTextAttribute(h, 11);
            cout << "\n\t\t---------------------------------------------------------------------------------------------\n";
            SetConsoleTextAttribute(h, 15);
            cout << "\t\t";

        }
    }

    void displayVisitorHistory() {
        if (visitorHistory.empty()) {
            system("cls");
            SetConsoleTextAttribute(h, 12);
            cout << "\t\tVisitor history is empty.";
            SetConsoleTextAttribute(h, 15);
        } else {
            system("cls");
            SetConsoleTextAttribute(h, 11);
            cout << "\t\t\t\t __   __  ___   _______  _______  _______  ______    __   __ \n";
            cout << "\t\t\t\t|  | |  ||   | |       ||       ||       ||    _ |  |  | |  |\n";
            cout << "\t\t\t\t|  |_|  ||   | |  _____||_     _||   _   ||   | ||  |  |_|  |\n";
            cout << "\t\t\t\t|       ||   | | |_____   |   |  |  | |  ||   |_||_ |       |\n";
            cout << "\t\t\t\t|       ||   | |_____  |  |   |  |  |_|  ||    __  ||_     _|\n";
            cout << "\t\t\t\t|   _   ||   |  _____| |  |   |  |       ||   |  | |  |   |  \n";
            cout << "\t\t\t\t|__| |__||___| |_______|  |___|  |_______||___|  |_|  |___|  \n";
            cout << "\n\t\t---------------------------------------------------------------------------------------------\n";
            SetConsoleTextAttribute(h, 14);
            int count = 1;
            queue<string> temp = visitorHistory;
            while (!temp.empty()) {
                cout << "\t\t\t\t" << count << ". " << temp.front() << "\n";
                temp.pop();
                count++;
            }
            SetConsoleTextAttribute(h, 11);
            cout << "\t\t---------------------------------------------------------------------------------------------\n";
            SetConsoleTextAttribute(h, 15);
            cout << "\t\t\t\t";
            system("pause");
            system("cls");
        }
    }
};

void queueIn(Canteen& canteen) {
    string customerName, stallName;
    system("cls");
    SetConsoleTextAttribute(h, 11);
    cout << "\t\t\t\t _______  __   __  _______  __   __  _______    ___   __    _ \n";
    cout << "\t\t\t\t|       ||  | |  ||       ||  | |  ||       |  |   | |  |  | |\n";
    cout << "\t\t\t\t|   _   ||  | |  ||    ___||  | |  ||    ___|  |   | |   |_| |\n";
    cout << "\t\t\t\t|  | |  ||  |_|  ||   |___ |  |_|  ||   |___   |   | |       |\n";
    cout << "\t\t\t\t|  |_|  ||       ||    ___||       ||    ___|  |   | |  _    |\n";
    cout << "\t\t\t\t|      | |       ||   |___ |       ||   |___   |   | | | |   |\n";
    cout << "\t\t\t\t|____||_||_______||_______||_______||_______|  |___| |_|  |__|\n";
    cout << "\n\t\t---------------------------------------------------------------------------------------------\n";
    SetConsoleTextAttribute(h, 14);
    cout << "\n\t\t----------------------------------------- Food Menu -----------------------------------------\n";
    canteen.displayMenuPerStall();
    SetConsoleTextAttribute(h, 11);
    cout << "\n\t\t---------------------------------------------------------------------------------------------\n";
    SetConsoleTextAttribute(h, 14);
    cout << "\t\t\t\t\t\tEnter stall name: ";
    SetConsoleTextAttribute(h, 15);
    cin.ignore();
    getline(cin, stallName);
    SetConsoleTextAttribute(h, 14);
    cout << "\t\t\t\t\t\tEnter customer name: ";
    SetConsoleTextAttribute(h, 15);
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

    system("cls");
    SetConsoleTextAttribute(h, 10);
    cout << "\t\t\t\t _______  ______   ______     _______  _______  _______  ______  \n";
    cout << "\t\t\t\t|   _   ||      | |      |   |       ||       ||       ||      | \n";
    cout << "\t\t\t\t|  |_|  ||  _    ||  _    |  |    ___||   _   ||   _   ||  _    |\n";
    cout << "\t\t\t\t|       || | |   || | |   |  |   |___ |  | |  ||  | |  || | |   |\n";
    cout << "\t\t\t\t|       || |_|   || |_|   |  |    ___||  |_|  ||  |_|  || |_|   |\n";
    cout << "\t\t\t\t|   _   ||       ||       |  |   |    |       ||       ||       |\n";
    cout << "\t\t\t\t|__| |__||______| |______|   |___|    |_______||_______||______| \n";
    SetConsoleTextAttribute(h, 11);
    cout << "\n\t\t---------------------------------------------------------------------------------------------\n";
    SetConsoleTextAttribute(h, 14);
    cout << "\t\t\t\t\t\tEnter the food name to add: ";
    SetConsoleTextAttribute(h, 15);
    cin.ignore();
    getline(cin, newFoodName);

    if (newFoodName.length() > 12) {
        newFoodName = newFoodName.substr(0, 12);
    }
    SetConsoleTextAttribute(h, 14);
    cout << "\t\t\t\t\t\tEnter the food price: P";
    SetConsoleTextAttribute(h, 15);
    cin >> newFoodPrice;

    if (cin.fail()) {
        system("cls");
        SetConsoleTextAttribute(h, 12);
        cout << "\t\tInvalid input. Please enter a valid number.";
        SetConsoleTextAttribute(h, 15);
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }
    SetConsoleTextAttribute(h, 14);
    cout << "\t\t\t\t\t\tEnter the stall name: ";
    SetConsoleTextAttribute(h, 15);
    cin.ignore();
    getline(cin, stallName);

    if (stallName.length() > 12) {
        stallName = stallName.substr(0, 12);
    }

    canteen.addFoodToStall(newFoodName, newFoodPrice, stallName);
}

void removeFoodFromStall(Canteen& canteen) {
    system("cls");
    SetConsoleTextAttribute(h, 12);
    cout << "\t\t ______    _______  __   __  _______  __   __  _______    _______  _______  _______  ______  \n";
    cout << "\t\t|    _ |  |       ||  |_|  ||       ||  | |  ||       |  |       ||       ||       ||      | \n";
    cout << "\t\t|   | ||  |    ___||       ||   _   ||  |_|  ||    ___|  |    ___||   _   ||   _   ||  _    |\n";
    cout << "\t\t|   |_||_ |   |___ |       ||  | |  ||       ||   |___   |   |___ |  | |  ||  | |  || | |   |\n";
    cout << "\t\t|    __  ||    ___||       ||  |_|  ||       ||    ___|  |    ___||  |_|  ||  |_|  || |_|   |\n";
    cout << "\t\t|   |  | ||   |___ | ||_|| ||       | |     | |   |___   |   |    |       ||       ||       |\n";
    cout << "\t\t|___|  |_||_______||_|   |_||_______|  |___|  |_______|  |___|    |_______||_______||______| \n";
    SetConsoleTextAttribute(h, 11);
    cout << "\n\t\t---------------------------------------------------------------------------------------------\n";
    canteen.displayMenuPerStall();
    string removeFoodName, stallName;
    SetConsoleTextAttribute(h, 11);
    cout << "\n\t\t---------------------------------------------------------------------------------------------\n";
    SetConsoleTextAttribute(h, 14);
    cout << "\t\t\t\t\t\tEnter the food name to remove: ";
    SetConsoleTextAttribute(h, 15);
    cin.ignore();
    getline(cin, removeFoodName);

    if (removeFoodName.length() > 12) {
        removeFoodName = removeFoodName.substr(0, 12);
    }
    SetConsoleTextAttribute(h, 14);
    cout << "\t\t\t\t\t\tEnter the stall name: ";
    SetConsoleTextAttribute(h, 15);
    getline(cin, stallName);

    if (stallName.length() > 12) {
        stallName = stallName.substr(0, 12);
    }

    canteen.removeFoodFromStall(removeFoodName, stallName);
}

void setMaxSeats(Canteen& canteen) {
    int maxSeats;
    system("cls");
    SetConsoleTextAttribute(h, 11);
    cout << "\t\t\t   _______  ______   ______     _______  _______  _______  _______  _______ \n";
    cout << "\t\t\t  |   _   ||      | |      |   |       ||       ||   _   ||       ||       |\n";
    cout << "\t\t\t  |  |_|  ||  _    ||  _    |  |  _____||    ___||  |_|  ||_     _||  _____|\n";
    cout << "\t\t\t  |       || | |   || | |   |  | |_____ |   |___ |       |  |   |  | |_____ \n";
    cout << "\t\t\t  |       || |_|   || |_|   |  |_____  ||    ___||       |  |   |  |_____  |\n";
    cout << "\t\t\t  |   _   ||       ||       |   _____| ||   |___ |   _   |  |   |   _____| |\n";
    cout << "\t\t\t  |__| |__||______| |______|   |_______||_______||__| |__|  |___|  |_______|\n";
    cout << "\n\t\t---------------------------------------------------------------------------------------------\n";
    SetConsoleTextAttribute(h, 14);
    cout << "\t\t\t\t\tEnter the number of seat(s) to be added: ";
    SetConsoleTextAttribute(h, 15);
    cin >> maxSeats;

    if (cin.fail()) {
        system("cls");
        SetConsoleTextAttribute(h, 12);
        cout << "\t\tInvalid input. Please enter a valid number.";
        SetConsoleTextAttribute(h, 15);
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return;
    }

    if (maxSeats <= 0) {
        system("cls");
        SetConsoleTextAttribute(h, 12);
        cout << "\t\tInvalid number of seat(s). Please enter a positive number.";
    SetConsoleTextAttribute(h, 15);
        return;
    }

    canteen.setMaxSeats(maxSeats);
    system("cls");
    SetConsoleTextAttribute(h, 10);
    cout << "\t\tAdded " << maxSeats << " Seat(s).";
    SetConsoleTextAttribute(h, 15);
}

void adminMenu(Canteen& canteen) {
    int choice;
    do {
        header();
        SetConsoleTextAttribute(h, 11);
        cout << "\n\t\t--------------------------------------- Admin Menu -----------------------------------------\n\n";
        cout << "\t\t\t\t\t\t1. Serve Customer\n";
        cout << "\t\t\t\t\t\t2. Add Food to Stall\n";
        cout << "\t\t\t\t\t\t3. Remove Food from Stall\n";
        cout << "\t\t\t\t\t\t4. Add Seats\n";
        cout << "\t\t\t\t\t\t5. Display Visitor History\n";
        cout << "\t\t\t\t\t\t6. Logout\n";
        cout << "\n\t\t---------------------------------------------------------------------------------------------\n";
        SetConsoleTextAttribute(h, 14);
        cout << "\t\t\t\t\t\tEnter your choice: ";
        SetConsoleTextAttribute(h, 15);
        cin >> choice;

        if (cin.fail()) {
            SetConsoleTextAttribute(h, 12);
            system("cls");
            cout << "\t\tInvalid input. Please enter a valid number." << endl;
            SetConsoleTextAttribute(h, 15);
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

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
                system("cls");
                SetConsoleTextAttribute(h, 10);
                cout << "\t\tLogged out from admin menu.";
                SetConsoleTextAttribute(h, 15);
                return;
            default:
                system("cls");
                SetConsoleTextAttribute(h, 12);
                cout << "\t\tInvalid choice. Please try again.";
                SetConsoleTextAttribute(h, 15);
        }

        cout << "\n";
    } while (true);
}

void loginAsAdmin(Canteen& canteen, bool& isAdmin, const string& adminPassword) {
    string password;
    int invalidAttempts = 0;
    do {
        SetConsoleTextAttribute(h, 11);
        cout << "\t\t    _______  ______   __   __  ___   __    _    ___      _______  _______  ___   __    _ \n";
        cout << "\t\t   |   _   ||      | |  |_|  ||   | |  |  | |  |   |    |       ||       ||   | |  |  | |\n";
        cout << "\t\t   |  |_|  ||  _    ||       ||   | |   |_| |  |   |    |   _   ||    ___||   | |   |_| |\n";
        cout << "\t\t   |       || | |   ||       ||   | |       |  |   |    |  | |  ||   | __ |   | |       |\n";
        cout << "\t\t   |       || |_|   ||       ||   | |  _    |  |   |___ |  |_|  ||   ||  ||   | |  _    |\n";
        cout << "\t\t   |   _   ||       || ||_|| ||   | | | |   |  |       ||       ||   |_| ||   | | | |   |\n";
        cout << "\t\t   |__| |__||______| |_|   |_||___| |_|  |__|  |_______||_______||_______||___| |_|  |__|\n";
        cout << "\n\t\t---------------------------------------------------------------------------------------------\n";
        SetConsoleTextAttribute(h, 14);
        cout << "\t\t\t\t\t\tEnter admin password: ";
        SetConsoleTextAttribute(h, 15);
        cin.ignore();
        getline(cin, password);

        if (password == adminPassword) {
            isAdmin = true;
            SetConsoleTextAttribute(h, 10);
            system("cls");
            cout << "\t\tLogged in as admin.\n";
            SetConsoleTextAttribute(h, 15);
            adminMenu(canteen);
            break;
        } else {
            invalidAttempts++;
            system("cls");
            SetConsoleTextAttribute(h, 12);
            cout << "\t\tIncorrect password. Login failed.\n";
            SetConsoleTextAttribute(h, 15);
            if (invalidAttempts == 3) {
                system("cls");
                SetConsoleTextAttribute(h, 12);
                cout << "\t\tExceeded maximum login attempts. Returning to the main menu.";
                SetConsoleTextAttribute(h, 15);
                break;
            }
        }
    } while (true);
}

int main() {
    Canteen canteen;
    bool isAdmin = false;
    string adminPassword = "pass";
    int choice;

    while (true) {
        header();
        cout << "\t\t\t\t\t\t     ";
        canteen.displayAvailableSeats();
        SetConsoleTextAttribute(h, 14);
        cout << "\n\t\t----------------------------------------- Food Menu -----------------------------------------\n";
        SetConsoleTextAttribute(h, 15);
        canteen.displayMenuPerStall();
        SetConsoleTextAttribute(h, 14);
        cout << "\t\t---------------------------------------------------------------------------------------------\n";
        SetConsoleTextAttribute(h, 11);
        cout << "\t\t---------------------------------------------------------------------------------------------\n";
        cout << "\t\t\t\t\t\t[1]. Queue In\n";
        cout << "\t\t\t\t\t\t[2]. Occupy Seat(s)\n";
        cout << "\t\t\t\t\t\t[3]. Display Waiting List\n";
        cout << "\t\t\t\t\t\t[4]. Login as Admin\n";
        cout << "\t\t\t\t\t\t[5]. Exit\n";

        cout << "\t\t---------------------------------------------------------------------------------------------\n";
        SetConsoleTextAttribute(h, 14);
        cout << "\t\t\t\t\t\tEnter your choice: ";
        SetConsoleTextAttribute(h, 15);
        cin >> choice;

        if (cin.fail()) {
            SetConsoleTextAttribute(h, 12);
            system("cls");
            cout << "\t\tInvalid input. Please enter a valid number." << endl;
            SetConsoleTextAttribute(h, 15);
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
                system("cls");
                SetConsoleTextAttribute(h, 11);
                cout << "\t\t\t     _     _  _______  ___   _______    ___      ___   _______  _______ \n";
                cout << "\t\t\t    | | _ | ||   _   ||   | |       |  |   |    |   | |       ||       |\n";
                cout << "\t\t\t    | || || ||  |_|  ||   | |_     _|  |   |    |   | |  _____||_     _|\n";
                cout << "\t\t\t    |       ||       ||   |   |   |    |   |    |   | | |_____   |   |  \n";
                cout << "\t\t\t    |       ||       ||   |   |   |    |   |___ |   | |_____  |  |   |  \n";
                cout << "\t\t\t    |   _   ||   _   ||   |   |   |    |       ||   |  _____| |  |   |  \n";
                cout << "\t\t\t    |__| |__||__| |__||___|   |___|    |_______||___| |_______|  |___|  \n";
                cout << "\n\t\t---------------------------------------------------------------------------------------------\n";
                SetConsoleTextAttribute(h, 14);
                canteen.displayWaitingList();
                cout << "\n\t\t";
                system("pause");
                system("cls");
                break;
            case 4:
                system("cls");
                loginAsAdmin(canteen, isAdmin, adminPassword);
                break;
            case 5:
                SetConsoleTextAttribute(h, 12);
                cout << "\t\tExiting the program." << endl;
                SetConsoleTextAttribute(h, 15);
                cout << "\t\t";
                system("pause");
                system("cls");
                return 0;
            default:
                system("cls");
                SetConsoleTextAttribute(h, 12);
                cout << "\t\tInvalid choice. Please try again." << endl;
                SetConsoleTextAttribute(h, 15);
                break;
        }
        cout << endl;
    }
    return 0;
}
