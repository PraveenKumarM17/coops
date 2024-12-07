#include <iostream>
#include <vector>
#include <iomanip> // For setw
#include <string>
#include <ctime> // For date calculations

using namespace std;

// Class representing a Room
class Room {
public:
    int roomNumber;
    string roomType; // e.g., "Double", "Single", "Special"
    bool isAvailable;

    Room(int num, string type) : roomNumber(num), roomType(type), isAvailable(true) {}

    void displayRoomInfo() const {
        cout << "Room Number: " << roomNumber << ", Type: " << roomType << ", Availability: " << (isAvailable ? "Available" : "Occupied") << endl;
    }
};

// Class representing a Customer
class Customer {
public:
    int customerId;
    string name;
    string contact;
    string email;

    Customer(int id, string nm, string cont, string em) : customerId(id), name(nm), contact(cont), email(em) {}

    void displayCustomerInfo() const {
        cout << "Customer ID: " << customerId << ", Name: " << name << ", Contact: " << contact << ", Email: " << email << endl;
    }
};

// Class representing a Booking
class Booking {
public:
    int bookingId;
    int roomNumber;
    int customerId;
    string checkInDate;
    string checkOutDate;
    double totalAmount;

    Booking(int id, int rNumber, int cId, string inDate, string outDate, double amount = 0.0)
        : bookingId(id), roomNumber(rNumber), customerId(cId), checkInDate(inDate), checkOutDate(outDate), totalAmount(amount) {}

    void displayBookingInfo() const {
        cout << "Booking ID: " << bookingId << ", Room Number: " << roomNumber
             << ", Customer ID: " << customerId << ", Check-In Date: " << checkInDate
             << ", Check-Out Date: " << checkOutDate << ", Total Amount: $" << fixed << setprecision(2) << totalAmount << endl;
    }

    int calculateStayDays() const {
        struct tm inDate = {}, outDate = {};
        sscanf(checkInDate.c_str(), "%d-%d-%d", &inDate.tm_year, &inDate.tm_mon, &inDate.tm_mday);
        sscanf(checkOutDate.c_str(), "%d-%d-%d", &outDate.tm_year, &outDate.tm_mon, &outDate.tm_mday);
        inDate.tm_year -= 1900; // Adjust year for tm structure
        outDate.tm_year -= 1900;
        inDate.tm_mon -= 1; // Adjust month for tm structure
        outDate.tm_mon -= 1;

        time_t checkIn = mktime(&inDate);
        time_t checkOut = mktime(&outDate);
        double duration = difftime(checkOut, checkIn) / (60 * 60 * 24);
        return static_cast<int>(duration);
    }
};

// Hotel class to manage rooms, customers, and bookings
class Hotel {
private:
    vector<Room> rooms;
    vector<Customer> customers;
    vector<Booking> bookings;
    int nextBookingId = 1;

public:
    Hotel() {
        // Initialize rooms
        for (int i = 1; i <= 12; ++i) {
            rooms.push_back(Room(i, "Double"));
        }
        for (int i = 13; i <= 19; ++i) {
            rooms.push_back(Room(i, "Single"));
        }
        for (int i = 20; i <= 24; ++i) {
            rooms.push_back(Room(i, "Special"));
        }
    }

    void displayRooms() const {
        for (const auto& room : rooms) {
            room.displayRoomInfo();
        }
    }

    void addCustomer(int id, string name, string contact, string email) {
        // Check for duplicate customer ID
        for (const auto& customer : customers) {
            if (customer.customerId == id) {
                cout << "Customer ID " << id << " already exists." << endl;
                return;
            }
        }
        customers.push_back(Customer(id, name, contact, email));
        cout << "Customer added successfully!" << endl;
    }

    void displayCustomers() const {
        if (customers.empty()) {
            cout << "\nNo customers found.\n";
            return;
        }

        cout << "\n--- Customer List ---\n";
        cout << left << setw(15) << "Customer ID" 
             << left << setw(20) << "Name" 
             << left << setw(15) << "Contact" 
             << left << setw(25) << "Email" << endl;
        cout << string(75, '-') << endl; // Separator for better visualization

        for (const auto& customer : customers) {
            cout << left << setw(15) << customer.customerId
                 << left << setw(20) << customer.name
                 << left << setw(15) << customer.contact
                 << left << setw(25) << customer.email << endl;
        }
    }

    void bookRoom(int roomNumber, int customerId, string checkInDate, string checkOutDate) {
        // Check if the room exists and is available
        for (auto& room : rooms) {
            if (room.roomNumber == roomNumber) {
                if (room.isAvailable) {
                    // Check if the customer exists
                    for (const auto& customer : customers) {
                        if (customer.customerId == customerId) {
                            bookings.push_back(Booking(nextBookingId++, roomNumber, customerId, checkInDate, checkOutDate));
                            room.isAvailable = false;
                            cout << "Booking successful for Room Number " << roomNumber << " by Customer ID " << customerId << endl;
                            return;
                        }
                    }
                    cout << "Customer ID " << customerId << " not found." << endl;
                    return;
                } else {
                    cout << "Room Number " << roomNumber << " is not available." << endl;
                    return;
                }
            }
        }
        cout << "Room Number " << roomNumber << " not found." << endl;
    }

    void checkOut(int bookingId) {
        for (auto it = bookings.begin(); it != bookings.end(); ++it) {
            if (it->bookingId == bookingId) {
                int roomNumber = it->roomNumber;
                for (auto& room : rooms) {
                    if (room.roomNumber == roomNumber) {
                        room.isAvailable = true;
                        break;
                    }
                }

                // Calculate the stay duration and total bill
                int stayDays = it->calculateStayDays();
                double ratePerDay = roomNumber <= 12 ? 100.0 : (roomNumber <= 19 ? 75.0 : 150.0);
                it->totalAmount = stayDays * ratePerDay;

                cout << "\n--- Check-Out ---\n";
                cout << "Booking ID: " << bookingId << " has been checked out." << endl;
                cout << "Total bill for the stay: $" << it->totalAmount << endl;
                bookings.erase(it);
                return;
            }
        }
        cout << "Booking ID " << bookingId << " not found." << endl;
    }

    void displayBookings() const {
        if (bookings.empty()) {
            cout << "\nNo bookings found.\n";
            return;
        }

        cout << "\n--- Booking List ---\n";
        cout << left << setw(15) << "Booking ID"
             << left << setw(15) << "Room Number"
             << left << setw(15) << "Customer ID"
             << left << setw(15) << "Check-In Date"
             << left << setw(15) << "Check-Out Date"
             << left << setw(15) << "Total Amount" << endl;
        cout << string(80, '-') << endl; // Separator for better visualization

        for (const auto& booking : bookings) {
            booking.displayBookingInfo();
        }
    }
};

// Main function
int main() {
    Hotel hotel;
    int choice;

    while (true) {
        cout << "\n--- Hotel Management System ---\n";
        cout << "1. Display Rooms\n";
        cout << "2. Add Customer\n";
        cout << "3. Display Customers\n";
        cout << "4. Book Room\n";
        cout << "5. Check-Out (Calculate Bill)\n";
        cout << "6. Display Bookings\n";
        cout << "7. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << "\nAvailable Rooms:\n";
                hotel.displayRooms();
                break;
            case 2: {
                int id;
                string name, contact, email;
                cout << "\n--- Add Customer ---\n";
                cout << "Enter Customer ID: ";
                cin >> id;
                cin.ignore(); // Ignore the newline character left by previous input
                cout << "Enter Name: ";
                getline(cin, name);
                cout << "Enter Contact: ";
                getline(cin, contact);
                cout << "Enter Email: ";
                getline(cin, email);

                hotel.addCustomer(id, name, contact, email);
                break;
            }
            case 3:
                hotel.displayCustomers();
                break;
            case 4: {
                int roomNumber, customerId;
                string checkInDate, checkOutDate;
                cout << "\n--- Book Room ---\n";
                cout << "Enter Room Number: ";
                cin >> roomNumber;
                cout << "Enter Customer ID: ";
                cin >> customerId;
                cout << "Enter Check-In Date (YYYY-MM-DD): ";
                cin >> checkInDate;
                cout << "Enter Check-Out Date (YYYY-MM-DD): ";
                cin >> checkOutDate;

                hotel.bookRoom(roomNumber, customerId, checkInDate, checkOutDate);
                break;
            }
            case 5: {
                int bookingId;
                cout << "\n--- Check-Out ---\n";
                cout << "Enter Booking ID to check out: ";
                cin >> bookingId;

                hotel.checkOut(bookingId);
                break;
            }
            case 6:
                hotel.displayBookings();
                break;
            case 7:
                cout << "Exiting the program.\n";
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
}
