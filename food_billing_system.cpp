#include <iostream>
#include <vector>
#include <iomanip>  // For formatting output
#include <string>

using namespace std;

// Class representing an individual food item
class FoodItem {
public:
    int itemId;
    string name;
    double priceInINR;

    FoodItem(int id, string itemName, double itemPriceInINR)
        : itemId(id), name(itemName), priceInINR(itemPriceInINR) {}

    void displayItem() const {
        cout << left << setw(4) << itemId << ". " << setw(20) << name
             << "Rs." << right << setw(8) << fixed << setprecision(2) << priceInINR << endl;
    }
};

// Class representing a shopping cart
class Cart {
private:
    vector<pair<FoodItem, int>> items;  // Pair of FoodItem and quantity

public:
    void addItem(const FoodItem& item, int quantity) {
        for (auto& entry : items) {
            if (entry.first.itemId == item.itemId) {
                entry.second += quantity;
                return;
            }
        }
        items.push_back(make_pair(item, quantity));
    }

    void removeItem(int itemId, int quantity) {
        for (auto it = items.begin(); it != items.end(); ++it) {
            if (it->first.itemId == itemId) {
                if (it->second > quantity) {
                    it->second -= quantity;
                } else {
                    items.erase(it);
                }
                return;
            }
        }
        cout << "Item not found in the cart.\n";
    }

    void viewCart() const {
        if (items.empty()) {
            cout << "Your cart is empty.\n";
            return;
        }

        double total = 0;
        cout << "\n---------------------------------------------\n";
        cout << "              YOUR BILL            \n";
        cout << "---------------------------------------------\n";
        cout << left << setw(4) << "ID" << setw(20) << "Item" << setw(8) << "Qty"
             << right << setw(10) << "Price (Rs.)" << endl;
        cout << "---------------------------------------------\n";

        for (const auto& entry : items) {
            const FoodItem& item = entry.first;
            int quantity = entry.second;
            double priceInINR = item.priceInINR * quantity;
            total += priceInINR;
            cout << left << setw(4) << item.itemId << setw(20) << item.name
                 << setw(8) << quantity << right << setw(8) << fixed << setprecision(2) << priceInINR << endl;
        }

        cout << "---------------------------------------------\n";
        cout << left << setw(32) << "Total" << right << "Rs." << fixed << setprecision(2) << total << endl;
        cout << "---------------------------------------------\n";
    }
};

// Main function
int main() {
    // Creating 10 Indian food items
    vector<FoodItem> items = {
        FoodItem(1, "Biryani", 200.0),
        FoodItem(2, "Butter Chicken", 300.0),
        FoodItem(3, "Paneer Tikka", 180.0),
        FoodItem(4, "Masala Dosa", 150.0),
        FoodItem(5, "Chole Bhature", 250.0),
        FoodItem(6, "Tandoori Chicken", 350.0),
        FoodItem(7, "Pani Puri", 100.0),
        FoodItem(8, "Samosa", 50.0),
        FoodItem(9, "Aloo Paratha", 120.0),
        FoodItem(10, "Rogan Josh", 400.0)
    };

    // Displaying the available food items
    cout << "-----------------------------------\n";
    cout << "          Menu of Food Items      \n";
    cout << "-----------------------------------\n";
    cout << left << setw(4) << "ID" << setw(20) << "Item" << "Price (Rs.)" << endl;
    cout << "-----------------------------------\n";
    for (const auto& item : items) {
        item.displayItem();
    }
    cout << "-----------------------------------\n";

    Cart cart;
    int choice, quantity;

    while (true) {
        cout << "\n-----------------------------------\n";
        cout << "              MAIN MENU            \n";
        cout << "-----------------------------------\n";
        cout << "1. Add item to cart\n";
        cout << "2. Remove item from cart\n";
        cout << "3. View cart\n";
        cout << "4. Exit\n";
        cout << "-----------------------------------\n";
        cout << "Choose an option (1-4): ";
        cin >> choice;

        switch (choice) {
            case 1: {
                cout << "Enter the item ID to add : ";
                cin >> choice;
                if (choice < 1 || choice > 10) {
                    cout << "Invalid item ID. Please try again.\n";
                    break;
                }
                cout << "Enter the quantity to add: ";
                cin >> quantity;
                if (quantity <= 0) {
                    cout << "Invalid quantity. Please enter a positive number.\n";
                    break;
                }
                cart.addItem(items[choice - 1], quantity);
                cout << "Item added to the cart.\n";
                break;
            }
            case 2: {
                cout << "Enter the item ID to remove from the cart (1-10): ";
                cin >> choice;
                if (choice < 1 || choice > 10) {
                    cout << "Invalid item ID. Please try again.\n";
                    break;
                }
                cout << "Enter the quantity to remove: ";
                cin >> quantity;
                if (quantity <= 0) {
                    cout << "Invalid quantity. Please enter a positive number.\n";
                    break;
                }
                cart.removeItem(choice, quantity);
                cout << "Item removed from the cart.\n";
                break;
            }
            case 3:
                cart.viewCart();
                break;
            case 4:
                cout << "Thank you for using the food billing system. Goodbye!\n";
                return 0;
            default:
                cout << "Invalid choice. Please enter a number between 1 and 4.\n";
                break;
        }
    }
}
