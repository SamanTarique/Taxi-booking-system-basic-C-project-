#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <cstdlib>
#include <algorithm>
using namespace std;

/* ======================== Auth ======================== */
enum class Role { Rider, Driver };

struct User {
    string username;
    string password;
    Role role;
};

vector<User> users = {
    {"rider1", "123", Role::Rider},
    {"driver1", "123", Role::Driver}
};

int findUserIndex(const string& username) {
    for (size_t i = 0; i < users.size(); ++i)
        if (users[i].username == username) return (int)i;
    return -1;
}

bool registerUser() {
    cout << "\n=== Register ===\n";
    string username, password;
    int roleOpt;

    cout << "Choose role (1=Rider, 2=Driver): ";
    if (!(cin >> roleOpt) || (roleOpt != 1 && roleOpt != 2)) {
        cout << "Invalid role.\n";
        cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false;
    }

    cout << "Username: ";
    cin >> username;
    if (findUserIndex(username) != -1) {
        cout << "Username already exists.\n";
        return false;
    }

    cout << "Password: ";
    cin >> password;

    users.push_back({username, password, roleOpt == 1 ? Role::Rider : Role::Driver});
    cout << "Registered successfully!\n";
    return true;
}

int loginUser() {
    cout << "\n=== Login ===\n";
    string username, password;
    cout << "Username: ";
    cin >> username;
    cout << "Password: ";
    cin >> password;

    int idx = findUserIndex(username);
    if (idx == -1 || users[idx].password != password) {
        cout << "Invalid credentials.\n";
        return -1;
    }
    cout << "Login successful. Welcome, " << username << "!\n";
    return idx;
}

/* ===================== Validation ===================== */
int readInt(const string& prompt) {
    while (true) {
        cout << prompt;
        int v;
        if (cin >> v) return v;
        cout << "Make sure your input is integer\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

int readIntInRange(const string& prompt, int minVal, int maxVal) {
    while (true) {
        cout << prompt;
        int v;
        if (!(cin >> v)) {
            cout << "Make sure your input is integer\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }
        if (v < minVal || v > maxVal) {
            cout << "Invalid location numbers! Choose between " << minVal << " and " << maxVal << ".\n";
            continue;
        }
        return v;
    }
}

/* ===================== Domain Data ==================== */
struct Location {
    string name;
    int distanceFromOrigin;
};

Location locations[] = {
    {"Gulshan", 0},
    {"Saddar", 10},
    {"Nazimabad", 18},
    {"Airport", 20}
};
int numLocations = sizeof(locations) / sizeof(locations[0]);

int getDistance(const string& from, const string& to) {
    int fromDist = -1, toDist = -1;
    for (int i = 0; i < numLocations; i++) {
        if (locations[i].name == from) fromDist = locations[i].distanceFromOrigin;
        if (locations[i].name == to)   toDist   = locations[i].distanceFromOrigin;
    }
    if (fromDist == -1 || toDist == -1) return -1;
    return abs(toDist - fromDist);
}

/* ===================== Drivers ======================== */
struct Driver {
    int id;
    string name;
    string imagePath;  // e.g., "driver1.jpg"
    bool available;
    string area;       // reserved for future location-based matching
};

vector<Driver> drivers = {
    {1, "Ahan",  "driver1.jpg", true, "Gulshan"},
    {2, "Sara",  "driver2.jpg", true, "Saddar"},
    {3, "Imran", "driver3.jpg", true, "Nazimabad"},
    {4, "Mina",  "driver4.jpg", true, "Airport"}
};

int assignFirstAvailableDriver() {
    for (size_t i = 0; i < drivers.size(); ++i)
        if (drivers[i].available) return (int)i;
    return -1;
}

/* ===================== Booking ======================== */
enum class VehicleType { Bike, Car };
string vehicleToString(VehicleType v) { return v == VehicleType::Bike ? "Bike" : "Car"; }

enum class BookingStatus { Confirmed, Cancelled, Pending };
string to_string(BookingStatus s) {
    switch (s) {
        case BookingStatus::Confirmed: return "Confirmed";
        case BookingStatus::Cancelled: return "Cancelled";
        case BookingStatus::Pending:   return "Pending";
    }
    return "Unknown";
}

struct Booking {
    int taxiID;
    int fare;
    string customerName;
    string pickup;
    string drop;
    BookingStatus status;
    int driverIndex;       // -1 if not assigned
    VehicleType vehicle;   // Bike or Car
};

Booking bookedTaxis[100];
int totalBookings = 0;
int taxiIDCounter = 100;

/* ===================== Summary ======================== */
void printBookingSummary(const Booking& b) {
    cout << "\n================= BOOKING SUMMARY =================\n";
    cout << "Taxi ID        : " << b.taxiID << "\n";
    cout << "Customer       : " << b.customerName << "\n";
    cout << "Pickup         : " << b.pickup << "\n";
    cout << "Drop           : " << b.drop << "\n";
    cout << "Vehicle        : " << vehicleToString(b.vehicle) << "\n";
    cout << "Fare (agreed)  : Rs. " << b.fare << "\n";
    cout << "Status         : " << to_string(b.status) << "\n";
    if (b.driverIndex >= 0) {
        const Driver& d = drivers[b.driverIndex];
        cout << "Driver Name    : " << d.name << "\n";
        cout << "Driver Image   : " << d.imagePath << "\n";
    } else {
        cout << "Driver         : (none)\n";
    }
    cout << "===================================================\n";
}

long long computeTotalEarnedConfirmed() {
    long long sum = 0;
    for (int i = 0; i < totalBookings; ++i)
        if (bookedTaxis[i].status == BookingStatus::Confirmed) sum += bookedTaxis[i].fare;
    return sum;
}

/* =================== Rider Session ==================== */
void riderSession(const string& currentRiderUsername) {
    while (true) {
        cout << "\n--- Rider Menu (logged in as: " << currentRiderUsername << ") ---\n";
        cout << "1. Book a Taxi\n";
        cout << "2. Booking Summary & Logout\n";
        int choice = readInt("Enter your choice: ");

        if (choice == 2) {
            cout << "\n--- Booking Summary (All) ---\n";
            cout << "Total Taxis Booked: " << totalBookings << "\n";
            for (int i = 0; i < totalBookings; i++) {
                cout << "\n#" << (i+1) << " ---------------------------------\n";
                printBookingSummary(bookedTaxis[i]);
            }
            cout << "Total Earned (Confirmed only): Rs. " << computeTotalEarnedConfirmed() << "\n";
            cout << "Logging out...\n";
            break;
        } else if (choice == 1) {
            // Show locations
            cout << "\nAvailable Locations:\n";
            for (int i = 0; i < numLocations; i++)
                cout << i + 1 << ". " << locations[i].name << "\n";

            int pickupNum = readIntInRange("\nEnter Pickup Location Number: ", 1, numLocations);
            int dropNum   = readIntInRange("Enter Drop Location Number: ", 1, numLocations);

            string pickup = locations[pickupNum - 1].name;
            string drop   = locations[dropNum   - 1].name;

            // Assign driver first
            int dIdx = assignFirstAvailableDriver();
            if (dIdx == -1) {
                cout << "Sorry, no drivers available right now.\n";
                continue;
            }

            int distance  = getDistance(pickup, drop);
            if (distance < 0) {
                cout << "Invalid locations!\n";
                continue;
            }
            int baseFare  = distance * 100;

            // Vehicle selection
            VehicleType vehicle = (baseFare < 400) ? VehicleType::Bike : VehicleType::Car;
            cout << "\nVehicle Assigned: " << vehicleToString(vehicle) << "\n";

            int finalFare = 0;
            Booking b{};
            b.taxiID       = taxiIDCounter++;
            b.customerName = currentRiderUsername; // use logged-in username
            b.pickup       = pickup;
            b.drop         = drop;
            b.vehicle      = vehicle;
            b.driverIndex  = -1; // set on confirm
            b.status       = BookingStatus::Pending;

            if (vehicle == VehicleType::Bike) {
                // Fixed, non-negotiable
                cout << "Note: This is a bike ride. Fare is fixed and non-negotiable.\n";
                cout << "Pickup: " << pickup << "\n";
                cout << "Drop: "   << drop   << "\n";
                cout << "Distance: " << distance << " km\n";
                cout << "Fare: Rs. " << baseFare << " (fixed)\n";

                char proceed;
                cout << "Proceed with booking? (y/n): ";
                cin >> proceed;

                if (proceed == 'y' || proceed == 'Y') {
                    finalFare      = baseFare;
                    b.fare         = finalFare;
                    b.status       = BookingStatus::Confirmed;
                    b.driverIndex  = dIdx;
                    drivers[dIdx].available = false; // occupy driver
                    bookedTaxis[totalBookings++] = b;
                    printBookingSummary(b);
                } else {
                    b.fare   = 0;
                    b.status = BookingStatus::Cancelled;
                    bookedTaxis[totalBookings++] = b;
                    printBookingSummary(b);
                }
            } else {
                // Car: minimum fare 400, then negotiate
                int shownFare  = max(baseFare, 400);
                int driverFare = shownFare - 200;

                cout << "Pickup: " << pickup << "\n";
                cout << "Drop: "   << drop   << "\n";
                cout << "Distance: " << distance << " km\n";
                cout << "Calculated Fare (min 400): Rs. " << shownFare << "\n";
                cout << "Driver says: I will charge Rs. " << driverFare
                     << " from " << pickup << " to " << drop << "\n";

                int userOffer = readInt("You: I can pay Rs. ");
                b.fare = userOffer;

                if (userOffer >= driverFare) {
                    cout << "Driver: Okay, accepted. Booking confirmed.\n";
                    b.status      = BookingStatus::Confirmed;
                    b.driverIndex = dIdx;
                    drivers[dIdx].available = false;
                    bookedTaxis[totalBookings++] = b;
                    printBookingSummary(b);
                } else {
                    cout << "Driver rejected your offer.\n";
                    char confirm;
                    cout << "Do you still want to proceed with the booking? (y/n): ";
                    cin >> confirm;

                    if (confirm == 'y' || confirm == 'Y') {
                        cout << "Booking Confirmed! Safe journey.\n";
                        b.status      = BookingStatus::Confirmed;
                        b.driverIndex = dIdx;
                        drivers[dIdx].available = false;
                        bookedTaxis[totalBookings++] = b;
                        printBookingSummary(b);
                    } else {
                        cout << "Booking cancelled by you.\n";
                        b.status = BookingStatus::Cancelled;
                        bookedTaxis[totalBookings++] = b;
                        printBookingSummary(b);
                    }
                }
            }
        } else {
            cout << "Invalid choice!\n";
        }
    }
}

/* =================== Driver Session =================== */
void driverSession(const string& currentDriverUsername) {
    while (true) {
        cout << "\n--- Driver Menu (logged in as: " << currentDriverUsername << ") ---\n";
        cout << "1. View total bookings & earnings\n";
        cout << "2. Logout\n";
        int choice = readInt("Enter your choice: ");

        if (choice == 2) {
            cout << "Logging out...\n";
            break;
        } else if (choice == 1) {
            long long earnings = computeTotalEarnedConfirmed();
            cout << "Total bookings so far: " << totalBookings << "\n";
            cout << "Total earned (confirmed): Rs. " << earnings << "\n";
            cout << "(Placeholder) Future: accept rides, toggle availability, etc.\n";
        } else {
            cout << "Invalid choice!\n";
        }
    }
}

/* ======================== Main ======================== */
int main() {
    while (true) {
        cout << "\n=== Welcome ===\n";
        cout << "1. Register\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        int choice = readInt("Enter your choice: ");

        if (choice == 3) {
            // Optional final global summary before exit
            cout << "\n--- Booking Summary (All) ---\n";
            cout << "Total Taxis Booked: " << totalBookings << "\n";
            for (int i = 0; i < totalBookings; i++) {
                cout << "\n#" << (i+1) << " ---------------------------------\n";
                printBookingSummary(bookedTaxis[i]);
            }
            cout << "Total Earned (Confirmed only): Rs. " << computeTotalEarnedConfirmed() << "\n";
            cout << "Exiting program..\n";
            break;
        } else if (choice == 1) {
            registerUser();
        } else if (choice == 2) {
            int idx = loginUser();
            if (idx == -1) continue;

            if (users[idx].role == Role::Rider) riderSession(users[idx].username);
            else                                 driverSession(users[idx].username);
        } else {
            cout << "Invalid choice!\n";
        }
    }
    return 0;
}
