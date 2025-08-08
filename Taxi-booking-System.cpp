#include <iostream>
#include <string>
#include <cstdlib>
using namespace std;

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

int getDistance(string from, string to) {
    int fromDist = -1, toDist = -1;
    for (int i = 0; i < numLocations; i++) {
        if (locations[i].name == from) fromDist = locations[i].distanceFromOrigin;
        if (locations[i].name == to) toDist = locations[i].distanceFromOrigin;
    }
    if (fromDist == -1 || toDist == -1) return -1;
    return abs(toDist - fromDist);
}

struct Booking {
    int taxiID;
    int fare;
};

Booking bookedTaxis[100];
int totalBookings = 0;

int main() {
    int choice;
    int taxiIDCounter = 100;
    int totalEarned = 0;

    while (true) {
        cout << "\n--- Taxi Booking System ---\n";
        cout << "1. Book a Taxi\n";
        cout << "2. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;

        if (choice == 2) {
            cout << "\n--- Booking Summary ---\n";
            cout << "Total Taxis Booked: " << totalBookings << endl;

            for (int i = 0; i < totalBookings; i++) {
                cout << "Taxi ID: " << bookedTaxis[i].taxiID
                     << " | Earned: Rs. " << bookedTaxis[i].fare << endl;
            }

            cout << "Exiting program..\n";
            break;
        } else if (choice == 1) {
            cout << "\nAvailable Locations:\n";
            for (int i = 0; i < numLocations; i++) {
                cout << i + 1 << ". " << locations[i].name << endl;
            }

            int pickupNum, dropNum;
            cout << "\nEnter Pickup Location Number: ";
            cin >> pickupNum;
            cout << "Enter Drop Location Number: ";
            cin >> dropNum;

            if (pickupNum < 1 || pickupNum > numLocations || dropNum < 1 || dropNum > numLocations) {
                cout << "Invalid location numbers!\n";
                continue;
            }

            string pickup = locations[pickupNum - 1].name;
            string drop = locations[dropNum - 1].name;

            int distance = getDistance(pickup, drop);
            int calculatedFare = distance * 100;
            int driverFare = calculatedFare - 200;

            cout << "\nTaxi available!\n";
            cout << "Pickup: " << pickup << "\n";
            cout << "Drop: " << drop << "\n";
            cout << "Distance: " << distance << " km\n";
            cout << "Calculated Fare: Rs. " << calculatedFare << "\n";
            cout << "Driver says: I will charge Rs. " << driverFare
                 << " from " << pickup << " to " << drop << endl;

            int userOffer;
            cout << "You: I can pay Rs. ";
            cin >> userOffer;

            if (userOffer >= driverFare) {
                cout << "Driver: Okay, accepted. Booking confirmed.\n";
                bookedTaxis[totalBookings].taxiID = taxiIDCounter++;
                bookedTaxis[totalBookings].fare = userOffer;
                totalEarned += userOffer;
                totalBookings++;
            } else {
                cout << "Driver rejected your offer.\n";
                char confirm;
                cout << "Do you still want to proceed with the booking? (y/n): ";
                cin >> confirm;

                if (confirm == 'y' || confirm == 'Y') {
                    cout << "Booking Confirmed! Safe journey.\n";
                    bookedTaxis[totalBookings].taxiID = taxiIDCounter++;
                    bookedTaxis[totalBookings].fare = userOffer;
                    totalEarned += userOffer;
                    totalBookings++;
                } else {
                    cout << "Booking cancelled by you.\n";
                }
            }

        } else {
            cout << "Invalid choice!\n";
        }
    }

    return 0;
}