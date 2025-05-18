#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <iomanip>
using namespace std;

const string EMAIL_REGEX = "(^[a-zA-Z0-9._%+-]+@(gmail|yahoo)\\.com$)";
const string PASSWORD_REGEX = "^[a-zA-Z0-9@#$%^&+=]{6,}$";
const string FIRSTNAME_REGEX = "^([A-Z][a-z]{1,49})( [A-Z][a-z]{1,49})?$";
const string LASTNAME_REGEX = "^[A-Z][a-z]{1,49}(['-][A-Z][a-z]{1,49})?( (Jr\\.|Sr\\.|II|III|IV|V))?$";
const string CONTACT_REGEX = "^(09|\\+639)\\d{9}$";

class InputValidator
{
private:
    InputValidator() {}
    InputValidator(const InputValidator &) = delete;
    InputValidator &operator=(const InputValidator &) = delete;

public:
    static InputValidator &get()
    {
        static InputValidator instance;
        return instance;
    }

    bool isValidInput(const string &input, const regex &compiledPattern)
    {
        return regex_match(input, compiledPattern);
    }
    string getValidatedInput(const string &prompt, const string &pattern, const string &errorMessage)
    {
        string input;
        while (true)
        {
            cout << prompt;
            getline(cin, input);
            regex compiledPattern(pattern);
            if (isValidInput(input, compiledPattern))
            {
                return input;
            }
            else
            {
                cout << errorMessage << endl;
            }
        }
    }
};

class Guest
{
private:
    string email, password, firstName, lastName, fullName, contactNumber;

public:
    Guest() = default;
    Guest(string _email, string _password, string _firstName, string _lastName, string _fullName, string _contactNumber) : email(_email), password(_password), firstName(_firstName), lastName(_lastName), fullName(_fullName), contactNumber(_contactNumber) {}

    string getEmail() const { return email; }
    string getPassword() const { return password; }
    string getFirstName() const { return firstName; }
    string getLastName() const { return lastName; }
    string getFullName() const { return fullName; }
    string getContactNumber() const { return contactNumber; }

    void setPassword(const string &p) { password = p; }
    void setFirstName(const string &fn)
    {
        firstName = fn;
        fullName = fn + " " + lastName;
    }
    void setLastName(const string &ln)
    {
        lastName = ln;
        fullName = firstName + " " + ln;
    }
    void setContactNumber(const string &cn) { contactNumber = cn; }
};

class Booking
{
private:
    string bookingID, guestEmail, guestName, roomNumber, checkInDate, checkOutDate, paymentMethod; // Added paymentMethod
    double totalPrice;

public:
    Booking(string bookingID, string guestEmail, string guestName, string roomNumber, string checkInDate, string checkOutDate, double totalPrice, string paymentMethod)
        : bookingID(bookingID), guestEmail(guestEmail), guestName(guestName), roomNumber(roomNumber), checkInDate(checkInDate), checkOutDate(checkOutDate), totalPrice(totalPrice), paymentMethod(paymentMethod) {}

    string getBookingID() const { return bookingID; }
    string getGuestEmail() const { return guestEmail; }
    string getGuestName() const { return guestName; }
    string getRoomNumber() const { return roomNumber; }
    string getCheckInDate() const { return checkInDate; }
    string getCheckOutDate() const { return checkOutDate; }
    double getTotalPrice() const { return totalPrice; }
    string getPaymentMethod() const { return paymentMethod; } // Getter for paymentMethod

    void setRoomNumber(const string &roomNum) { roomNumber = roomNum; }
    void setCheckInDate(const string &checkIn) { checkInDate = checkIn; }
    void setCheckOutDate(const string &checkOut) { checkOutDate = checkOut; }
    void setTotalPrice(const double &price) { totalPrice = price; }

    void displayBookingDetailsAdmin() const
    {
        cout << "Booking ID: " << bookingID << endl;
        cout << "Guest Email: " << guestEmail << endl;
        cout << "Guest Name: " << guestName << endl;
        cout << "Room Number: " << roomNumber << endl;
        cout << "Check-In Date: " << checkInDate << endl;
        cout << "Check-Out Date: " << checkOutDate << endl;
        cout << "Total Price: $" << fixed << setprecision(2) << totalPrice << endl;
        cout << "Payment Method: " << paymentMethod << endl; // Display paymentMethod
    }

    void displayBookingDetailsGuest() const
    {
        cout << "Booking ID: " << bookingID << endl;
        cout << "Room Number: " << roomNumber << endl;
        cout << "Check-In Date: " << checkInDate << endl;
        cout << "Check-Out Date: " << checkOutDate << endl;
        cout << "Total Price: $" << fixed << setprecision(2) << totalPrice << endl;
        cout << "Payment Method: " << paymentMethod << endl; // Display paymentMethod
    }
};

class GuestManager
{
private:
    Guest loggedInGuest;

    const string USER_FILE;

    bool emailExistsInFile(const string &email)
    {
        fstream file;

        file.open(USER_FILE, ios::in); // read
        if (file.is_open())
        {
            string line;
            while (getline(file, line))
            {
                if (line.substr(0, email.size()) == email && line[email.size()] == '|')
                {
                    file.close();
                    return true;
                }
            }
        } // else throw (File can't be open.
        return false;
    }

    void addGuestToFile(const Guest &guest)
    {
        fstream file;

        string line;
        vector<string> lines;

        file.open(USER_FILE, ios::app); // append
        if (file.is_open())
        {
            file << guest.getEmail() << "|" << guest.getPassword() << "|" << guest.getFirstName() << "|" << guest.getLastName() << "|" << guest.getFullName() << "|" << guest.getContactNumber();
            file.close();
        } // else throw (File can't be open.
    }

    bool loadGuest(const string &email, const string &password)
    {
        fstream file;
        vector<string> tokens;

        file.open(USER_FILE, ios::in);
        if (file.is_open())
        {
            string line;
            while (getline(file, line))
            {
                tokens.clear();
                stringstream ss(line);
                string token;
                while (getline(ss, token, '|'))
                {
                    tokens.push_back(token);
                }
                if (tokens.size() == 6 && tokens[0] == email && tokens[1] == password)
                {
                    loggedInGuest = Guest(tokens[0], tokens[1], tokens[2], tokens[3], tokens[4], tokens[5]);
                    file.close();
                    return true;
                }
            }
            file.close();
        } // else throw (File can't be open.
        return false; // email/password not found
    }

public:
    GuestManager() : USER_FILE("hotel_users.txt") {}

    void signUp()
    {
        cout << string(50, '-') << " Sign Up Page " << string(50, '-') << "\n";
        string email = InputValidator::get().getValidatedInput("Enter your email: ", EMAIL_REGEX, "Invalid email format.");
        if (emailExistsInFile(email))
        {
            cout << "That email is already registered. Try another.\n";
            return;
        }
        string password = InputValidator::get().getValidatedInput("Enter your password: ", PASSWORD_REGEX, "Password must be at least 6 characters long and can include letters, numbers, and special characters.");
        string firstName = InputValidator::get().getValidatedInput("Enter your first name: ", FIRSTNAME_REGEX, "First name can only contain letters and spaces.");
        string lastName = InputValidator::get().getValidatedInput("Enter your last name: ", LASTNAME_REGEX, "Last name can only contain letters and spaces.");
        string contactNumber = InputValidator::get().getValidatedInput("Enter your contact number: ", CONTACT_REGEX, "Contact number must start with '09' or '+639' and be 11 digits long.");
        string fullName = firstName + " " + lastName;
        Guest newGuest(email, password, firstName, lastName, fullName, contactNumber);
        addGuestToFile(newGuest);
        cout << "Account created successfully! Welcome, " << newGuest.getFullName() << "!\n";
    }

    Guest signIn()
    {
        cout << string(50, '-') << " Sign In Page " << string(50, '-') << "\n";
        string email = InputValidator::get().getValidatedInput("Enter your email: ", EMAIL_REGEX, "Invalid email format.");
        string password = InputValidator::get().getValidatedInput("Enter your password: ", PASSWORD_REGEX, "Password must be at least 6 characters long and can include letters, numbers, and special characters.");
        if (!loadGuest(email, password))
        {
            cout << "Email not found or password is incorrect.\n";
            return Guest();
        }
        cout << "Welcome, " << loggedInGuest.getFullName() << "!\n";
        return loggedInGuest;
    }

    void forgotPassword()
    {
        cout << string(50, '-') << " Forgot Password " << string(50, '-') << "\n";
        string email = InputValidator::get().getValidatedInput("Enter your email: ", EMAIL_REGEX, "Invalid email format.");
        string password = InputValidator::get().getValidatedInput("Enter your password: ", PASSWORD_REGEX, "Password must be at least 6 characters long and can include letters, numbers, and special characters.");
        if (!loadGuest(email, password))
        {
            cout << "Email not found or password is incorrect.\n";
            return;
        }
        cout << "\n";
        fstream file;
        string line;
        vector<string> lines;
        file.open(USER_FILE, ios::in);
        if (file.is_open())
        {
            while (getline(file, line))
            {
                stringstream ss(line);
                string token;
                getline(ss, token, '|');
                if (token == email)
                {
                    string temp;
                    vector<string> tokens;
                    string newPassword = InputValidator::get().getValidatedInput("Enter your new password: ", PASSWORD_REGEX, "Password must be at least 6 characters long and can include letters, numbers, and special characters.");
                    while (getline(ss, temp, '|'))
                        tokens.push_back(temp);
                    if (tokens.size() == 5)
                        line = email + '|' + newPassword + '|' + tokens[1] + '|' + tokens[2] + '|' + tokens[3] + '|' + tokens[4];
                }
                lines.push_back(line);
            }
            file.close();
        } // else throw (File can't be open.)

        file.open(USER_FILE, ios::out);
        if (file.is_open())
        {
            for (const auto &l : lines)
            {
                file << l << "\n";
            }
            file.close();
        }
        cout << "Password updated successfully.\n";
    }

    void editGuestInfo(vector<Booking> &bookingsList)
    {
        // for Admin
        // fstream file;
        // string temp;
        // file.open(USER_FILE, ios::in); // read
        // if (file.is_open())
        // {
        //     getline(file, temp);
        //     if (temp.empty())
        //     {
        //         cout << "No users found.\n";
        //         file.close();
        //         return;
        //     }
        //     file.close();
        // } // else throw (File can't be open.
        cout << string(50, '-') << " Personal Info " << string(50, '-') << "\n";

        cout << "Name: " << loggedInGuest.getFullName() << "\n"
             << "Phone: " << loggedInGuest.getContactNumber() << "\n"
             << "Email: " << loggedInGuest.getEmail() << "\n"
             << "Password: " << loggedInGuest.getPassword() << "\n\n";

        cout << "Editing Info...\n"
             << "Note: If you want to continue with that info press ENTER.\n";
        string firstName = InputValidator::get().getValidatedInput("Enter your first name: ", "^$|^([A-Z][a-z]{1,49})( [A-Z][a-z]{1,49})?$", "First name can only contain letters and spaces.");
        if (firstName.empty())
        {
            firstName = loggedInGuest.getFirstName();
        }
        string lastName = InputValidator::get().getValidatedInput("Enter your last name: ", "^$|^[A-Z][a-z]{1,49}(['-][A-Z][a-z]{1,49})?( (Jr\\.|Sr\\.|II|III|IV|V))?$", "Last name can only be contain letters and spaces.");
        if (lastName.empty())
        {
            lastName = loggedInGuest.getLastName();
        }
        string fullName = firstName + " " + lastName;
        string contactNumber = InputValidator::get().getValidatedInput("Enter your contact number: ", "^$|^(09|\\+639)\\d{9}$", "Contact number must start with '09' or '+639' and be 11 digits long.");
        if (contactNumber.empty())
        {
            contactNumber = loggedInGuest.getContactNumber();
        }
        string password = InputValidator::get().getValidatedInput("Enter your password: ", "^$|^[a-zA-Z0-9@#$%^&+=]{6,}$", "Password must be at least 6 characters long and can include letters, numbers, and special characters.");
        if (password.empty())
        {
            password = loggedInGuest.getPassword();
        }

        cout << "\nName: " << fullName << "\n"
             << "Phone: " << contactNumber << "\n"
             << "Email: " << loggedInGuest.getEmail() << "\n"
             << "Password: " << password << "\n";

        string choice = InputValidator::get().getValidatedInput("Confirm these changes [Y/N]? ", "^[YyNn]$", "Please enter [Y/N].");
        if (choice == "N" || choice == "n")
        {
            cout << "Changes discarded.\n";
            return;
        }

        loggedInGuest = Guest(loggedInGuest.getEmail(), password, firstName, lastName, fullName, contactNumber);

        fstream file;
        vector<string> lines;
        string line;
        file.open(USER_FILE, ios::in);
        if (file.is_open())
        {
            while (getline(file, line))
            {
                stringstream ss(line);
                string email;
                getline(ss, email, '|');
                if (email == loggedInGuest.getEmail())
                {
                    line = loggedInGuest.getEmail() + "|" + loggedInGuest.getPassword() + "|" + loggedInGuest.getFirstName() + "|" + loggedInGuest.getLastName() + "|" + loggedInGuest.getFullName() + "|" + loggedInGuest.getContactNumber();
                }
                lines.push_back(line);
            }
            file.close();
        } // else throw (File can't be open)

        file.open(USER_FILE, ios::out);
        if (file.is_open())
        {
            for (const auto &l : lines)
            {
                file << l << "\n";
            }
            file.close();
        } // else throw (File can't be open)

        for (auto &booking : bookingsList)
        {
            if (booking.getGuestEmail() == loggedInGuest.getEmail())
            {
                booking = Booking(booking.getBookingID(), loggedInGuest.getEmail(), loggedInGuest.getFullName(), booking.getRoomNumber(), booking.getCheckInDate(), booking.getCheckOutDate(), booking.getTotalPrice(), booking.getPaymentMethod());
            }
        }
        cout << "Your info has been updated successfully!\n";
    }

    void deleteGuest()
    {
        string email = InputValidator::get().getValidatedInput("Enter the email: ", "(^[a-zA-Z0-9._%+-]+@(gmail|yahoo)\\.com$)", "Invalid email format.");
        if (!emailExistsInFile(email))
        {
            cout << "Email not found!\n";
            return;
        }
        fstream file;
        vector<string> lines;

        file.open(USER_FILE, ios::in); // read
        if (file.is_open())
        {
            string line;
            while (getline(file, line))
            {
                stringstream ss(line);
                string token;
                getline(ss, token, '|');
                if (token != email)
                {
                    lines.push_back(line);
                }
            }
            file.close();
        } // else throw (File can't be open)

        file.open(USER_FILE, ios::out); // output
        if (file.is_open())
        {
            for (const auto &l : lines)
            {
                file << l << "\n";
            }
            file.close();
        } // else throw (File can't be open)
        cout << "Guest account successfully deleted!\n";
    }

    void viewAllUsers()
    {
        cout << string(50, '-') << " Users " << string(50, '-') << "\n";
        fstream file;
        string temp;
        file.open(USER_FILE, ios::in); // read
        if (file.is_open())
        {
            getline(file, temp);
            if (temp.empty())
            {
                cout << "No users found.\n";
                file.close();
                return;
            }
            file.close();
        } // else throw (File can't be open)

        cout << string(87, '-') << "\n";
        cout << left
             << setw(30) << "Email" << setw(35) << "Full name" << setw(22) << "Contact number" << "\n";
        cout << string(87, '-') << "\n";

        string line;
        file.open(USER_FILE, ios::in);
        if (file.is_open())
        {
            while (getline(file, line))
            {
                string token;
                stringstream ss(line);
                vector<string> tokens;
                while (getline(ss, token, '|'))
                {
                    tokens.push_back(token);
                }
                if (tokens.size() == 6)
                {
                    cout << left << setw(30) << tokens[0] << setw(35) << tokens[4] << setw(22) << tokens[5] << "\n";
                }
            }
            file.close();
            cout << string(87, '-') << "\n\n";
        } // else throw (File can't be open)

        string choice = InputValidator::get().getValidatedInput("Do you want to delete a user account [Y/N]? ", "^[YyNn]$", "Please enter [Y/N].");
        if (choice == "N" || choice == "n")
        {
            cout << "Going back to menu...\n";
            return;
        }
        else
        {
            deleteGuest();
            return;
        }
    }

    Guest signingPage()
    {
        int choice;
        Guest loggedInGuest;

        do
        {
            cout << string(50, '-') << " Hotel Reservation System " << string(50, '-') << "\n";
            cout << "1. Sign In\n2. Sign Up\n3. Forgot Password\n4. Exit\n";
            int choice = stoi(InputValidator::get().getValidatedInput("Enter your choice [1 - 4]: ", "^[1-4]$", "Invalid choice. Please enter [1 - 4]."));
            switch (choice)
            {
            case 1:
                loggedInGuest = signIn();
                break;
            case 2:
                signUp();
                break;
            case 3:
                forgotPassword();
                break;
            case 4:
                cout << "Exiting..." << endl;
                return Guest("", "", "", "", "", "");
            default:
                cout << "Invalid choice." << endl;
            }
        } while (loggedInGuest.getEmail().empty());
        return loggedInGuest;
    }
};

class Room
{
private:
    string roomNumber, roomType;
    int capacity; // Moved capacity here
    string roomFeatures;
    double roomPricePerNight;

public:
    Room(string roomNumber, string roomType, int capacity, string roomFeatures, double roomPricePerNight)
        : roomNumber(roomNumber), roomType(roomType), capacity(capacity), roomFeatures(roomFeatures), roomPricePerNight(roomPricePerNight) {}

    string getRoomNumber() const { return roomNumber; }
    string getRoomType() const { return roomType; }
    int getCapacity() const { return capacity; } // Getter remains unchanged
    string getRoomFeatures() const { return roomFeatures; }
    double getRoomPricePerNight() const { return roomPricePerNight; }

    void displayRoomDetails() const
    {
        cout << "Room Number: " << roomNumber << endl;
        cout << "Room Type: " << roomType << endl;
        cout << "Capacity: " << capacity << " person(s)" << endl;
        cout << "Price per Night: $" << fixed << setprecision(2) << roomPricePerNight << endl;
        cout << "Room Features: " << roomFeatures << endl; // Display room features last
    }
};

class RoomManager
{
private:
    RoomManager()
    {
        loadRooms();
    }
    RoomManager(const RoomManager &) = delete;
    RoomManager &operator=(const RoomManager &) = delete;

    ~RoomManager()
    {
        SaveRooms();
    }

    vector<Room> roomsList;

    void loadRooms()
    {
        string line;
        fstream hotel_rooms;
        hotel_rooms.open("hotel_rooms.txt", ios::in);
        if (hotel_rooms.is_open())
        {
            while (getline(hotel_rooms, line))
            {
                stringstream ss(line);
                string roomNumber, roomType, roomFeatures;
                string capacityStr, priceStr;

                getline(ss, roomNumber, '|');
                getline(ss, roomType, '|');
                getline(ss, capacityStr, '|'); // Read as string
                getline(ss, priceStr, '|');    // Read price before features
                getline(ss, roomFeatures);     // Last field, no delimiter needed

                int capacity = stoi(capacityStr);
                double roomPricePerNight = stod(priceStr);

                roomsList.push_back(Room(roomNumber, roomType, capacity, roomFeatures, roomPricePerNight));
            }
            hotel_rooms.close();
        }
    }

    void SaveRooms()
    {
        fstream hotel_rooms;
        hotel_rooms.open("hotel_rooms.txt", ios::out);
        if (hotel_rooms.is_open())
        {
            for (const auto &room : roomsList)
            {
                hotel_rooms << room.getRoomNumber() << "|"
                            << room.getRoomType() << "|"
                            << room.getCapacity() << "|"
                            << fixed << setprecision(2) << room.getRoomPricePerNight() << "|"
                            << room.getRoomFeatures() << "\n"; // Save room features last
            }
            hotel_rooms.close();
        }
    }

    vector<Room>::iterator findRoom(const string &number)
    {
        return find_if(roomsList.begin(), roomsList.end(),
                       [&number](const Room &room)
                       {
                           return room.getRoomNumber() == number;
                       });
    }

public:
    static RoomManager &get()
    {
        static RoomManager instance;
        return instance;
    }

    auto getRoomsList() const
    {
        return roomsList;
    }

    void addRoom()
    {
        string roomNumber;
        bool roomExists = true;
        while (roomExists)
        {
            roomNumber = InputValidator::get().getValidatedInput("Enter room number: ", "^\\d{3}$", "Invalid room number format. Room number must be 3 digits.");
            roomExists = findRoom(roomNumber) != roomsList.end();
            if (roomExists)
            {
                cout << "Room number already exists. Please enter a different room number." << endl;
            }
        }
        string roomType = InputValidator::get().getValidatedInput("Enter room type: ", "^.+$", "Invalid room type format.");
        int capacity = stoi(InputValidator::get().getValidatedInput("Enter room capacity: ", "^\\d+$", "Invalid capacity format. Capacity must be a positive integer."));
        string roomFeatures = InputValidator::get().getValidatedInput("Enter room features: ", "^.+$", "Invalid room features format.");
        double roomPricePerNight = stod(InputValidator::get().getValidatedInput("Enter price per night: ", "^\\d+\\.\\d{2}$", "Invalid price format. Price must be a number with two decimal places."));

        roomsList.push_back(Room(roomNumber, roomType, capacity, roomFeatures, roomPricePerNight));
    }

    void displayRooms()
    {
        cout << string(50, '-') << " Available Rooms " << string(50, '-') << "\n";
        for (const auto &room : roomsList)
        {
            room.displayRoomDetails();
            cout << string(50, '-') << endl;
        }
    }

    void updateRoom()
    {
        string roomNumber = InputValidator::get().getValidatedInput("Enter room number to update: ", "^\\d{3}$", "Invalid room number format. Room number must be 3 digits.");
        auto it = findRoom(roomNumber);

        if (it == roomsList.end())
        {
            cout << "Room not found." << endl;
            return;
        }

        cout << "Updating room details for room number: " << roomNumber << endl;
        cout << string(50, '-') << " Room Details " << string(50, '-') << "\n";
        it->displayRoomDetails();
        cout << string(50, '-') << endl;
        string roomType = InputValidator::get().getValidatedInput("Enter new room type: ", "^.+$", "Invalid room type format.");
        int capacity = stoi(InputValidator::get().getValidatedInput("Enter new room capacity: ", "^\\d+$", "Invalid capacity format. Capacity must be a positive integer.")); // Input capacity after room type
        string roomFeatures = InputValidator::get().getValidatedInput("Enter new room features: ", "^.+$", "Invalid room features format.");
        double roomPricePerNight = stod(InputValidator::get().getValidatedInput("Enter new price per night: ", "^\\d+\\.\\d{2}$", "Invalid price format. Price must be a number with two decimal places."));

        *it = Room(roomNumber, roomType, capacity, roomFeatures, roomPricePerNight);
    }

    void deleteRoom()
    {
        string roomNumber = InputValidator::get().getValidatedInput("Enter room number to remove: ", "^\\d{3}$", "Invalid room number format. Room number must be 3 digits.");
        auto it = findRoom(roomNumber);

        if (it == roomsList.end())
        {
            cout << "Room not found." << endl;
            return;
        }

        cout << "Removing room number: " << roomNumber << endl;
        cout << string(50, '-') << " Room Details " << string(50, '-') << "\n";
        it->displayRoomDetails();
        cout << string(50, '-') << endl;
        string confirmation = InputValidator::get().getValidatedInput("Are you sure you want to delete this room? (y/n): ", "^(y|n)$", "Invalid input. Please enter 'y' or 'n'.");

        if (confirmation == "n")
        {
            cout << "Room removal cancelled." << endl;
            return;
        }

        roomsList.erase(it);
        cout << "Room successfully removed." << endl;
    }
};

class PaymentStrategy
{
public:
    virtual void processPayment(const double &amount) = 0;
};

class CashPayment : public PaymentStrategy
{
public:
    void processPayment(const double &amount) override
    {
        cout << "You selected cash payment" << endl;
        cout << string(50, '-') << endl;
        cout << "Please pay the total amount of $" << fixed << setprecision(2) << amount << " at the hotel front desk.\nupon check-in. Keep your booking ID for reference." << endl;
    }
};

class GcashPayment : public PaymentStrategy
{
public:
    void processPayment(const double &amount) override
    {
        cout << "You selected Gcash payment" << endl;
        cout << string(50, '-') << endl;
        cout << "Gcash Number: 0912-345-6789" << endl;
        cout << "Account Name: DLSL Hotel" << endl;

        cout << "\nPlease send the total amount of $" << fixed << setprecision(2) << amount << " and take a screenshot\nas proof of payment. Email it to: dlslhotel@gmail.com" << endl;
    }
};

class CardPayment : public PaymentStrategy
{
public:
    void processPayment(const double &amount) override
    {
        cout << "You selected card payment." << endl;
        cout << string(50, '-') << endl;
        string cardNumber = InputValidator::get().getValidatedInput("Enter your card number: ", "^\\d{4}-\\d{4}-\\d{4}-\\d{4}$", "Invalid card number format. Must be in the format XXXX-XXXX-XXXX-XXXX.");
        string expirationDate = InputValidator::get().getValidatedInput("Enter your card expiration date: ", "^(0[1-9]|1[0-2])/(2[5-9]|[3-9][0-9])$", "Invalid expiration date format. Year must be 25 or above and in the format MM/YY.");
        string cvv = InputValidator::get().getValidatedInput("Enter your card CVV: ", "^\\d{3}$", "Invalid CVV format. Must be 3 digits.");
        string cardName = InputValidator::get().getValidatedInput("Enter your card name: ", "^.+$", "Invalid card name format.");

        cout << "\nProcessing payment of $" << fixed << setprecision(2) << amount << endl;
        cout << "Payment successful!" << endl;
    }
};

class BankTransferPayment : public PaymentStrategy
{
public:
    void processPayment(const double &amount) override
    {
        cout << "You selected bank transfer payment." << endl;
        cout << string(50, '-') << endl;
        cout << "Bank Name: Bank of the Philippine Islands (BPI)" << endl;
        cout << "Account Number: 1234-5678-9123" << endl;
        cout << "Account Name: DLSL Hotel" << endl;
        cout << "Reference: Your Booking ID" << endl;

        cout << "\nPlease transfer the total amount of $" << fixed << setprecision(2) << amount << " and email\nthe confirmation receipt to: dlslhotel@gmail.com" << endl;
    }
};

class PaymentContext
{
private:
    PaymentStrategy *paymentStrategy;

public:
    PaymentContext() : paymentStrategy(nullptr) {}

    void setPaymentStrategy(PaymentStrategy *strategy)
    {
        paymentStrategy = strategy;
    }

    void processPayment(const double &amount)
    {
        paymentStrategy->processPayment(amount);
    }
};

class BookingManager
{
private:
    vector<Booking> bookingsList;
    int bookingIdCounter = 1;

    void loadBookingIdCounter()
    {
        fstream counterFile;
        counterFile.open("booking_id_counter.txt", ios::in);
        if (counterFile.is_open())
        {
            string line;
            while (getline(counterFile, line))
            {
                bookingIdCounter = stoi(line);
            }
            counterFile.close();
        }
    }

    void saveBookingIdCounter()
    {
        fstream counterFile;
        counterFile.open("booking_id_counter.txt", ios::out);
        if (counterFile.is_open())
        {
            counterFile << bookingIdCounter << endl;
            counterFile.close();
        }
    }

    void loadBookings()
    {
        string line;
        fstream hotel_bookings;
        hotel_bookings.open("hotel_bookings.txt", ios::in);
        if (hotel_bookings.is_open())
        {
            while (getline(hotel_bookings, line))
            {
                stringstream ss(line);
                string bookingID, guestEmail, guestName, roomNumber, checkInDate, checkOutDate, paymentMethod;
                string totalPriceStr;

                getline(ss, bookingID, '|');
                getline(ss, guestEmail, '|');
                getline(ss, guestName, '|');
                getline(ss, roomNumber, '|');
                getline(ss, checkInDate, '|');
                getline(ss, checkOutDate, '|');
                getline(ss, totalPriceStr, '|');
                getline(ss, paymentMethod); // Read paymentMethod

                double totalPrice = stod(totalPriceStr);

                bookingsList.push_back(Booking(bookingID, guestEmail, guestName, roomNumber, checkInDate, checkOutDate, totalPrice, paymentMethod));
            }
            hotel_bookings.close();
        }
    }

    void SaveBookings()
    {
        fstream hotel_bookings;
        hotel_bookings.open("hotel_bookings.txt", ios::out);
        if (hotel_bookings.is_open())
        {
            for (const auto &booking : bookingsList)
            {
                hotel_bookings << booking.getBookingID() << "|"
                               << booking.getGuestEmail() << "|"
                               << booking.getGuestName() << "|"
                               << booking.getRoomNumber() << "|"
                               << booking.getCheckInDate() << "|"
                               << booking.getCheckOutDate() << "|"
                               << fixed << setprecision(2) << booking.getTotalPrice() << "|"
                               << booking.getPaymentMethod() << "\n"; // Save paymentMethod
            }
            hotel_bookings.close();
        }
    }

    bool isValidDate(const string &date)
    {
        int year, month, day;
        sscanf(date.c_str(), "%d-%d-%d", &year, &month, &day);
        if (month < 1 || month > 12 || day < 1 || day > 31)
            return false;

        // Check for months with fewer than 31 days
        if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30)
            return false;

        // Check for February and leap years
        if (month == 2)
        {
            bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
            if (day > (isLeapYear ? 29 : 28))
                return false;
        }

        return true;
    }

    bool isFutureDate(const string &date)
    {
        time_t now = time(0);
        int year, month, day;
        sscanf(date.c_str(), "%d-%d-%d", &year, &month, &day);
        tm inputDate = {};
        inputDate.tm_year = year - 1900;
        inputDate.tm_mon = month - 1;
        inputDate.tm_mday = day;
        time_t inputTime = mktime(&inputDate);
        return difftime(inputTime, now) > 0;
    }

    int calculateDaysDifference(const string &checkInDate, const string &checkOutDate)
    {
        tm checkIn = {}, checkOut = {};
        sscanf(checkInDate.c_str(), "%d-%d-%d", &checkIn.tm_year, &checkIn.tm_mon, &checkIn.tm_mday);
        sscanf(checkOutDate.c_str(), "%d-%d-%d", &checkOut.tm_year, &checkOut.tm_mon, &checkOut.tm_mday);

        // Adjust for tm structure (year since 1900, month 0-based)
        checkIn.tm_year -= 1900;
        checkIn.tm_mon -= 1;
        checkOut.tm_year -= 1900;
        checkOut.tm_mon -= 1;

        time_t checkInTime = mktime(&checkIn);
        time_t checkOutTime = mktime(&checkOut);

        // Calculate difference in seconds and convert to days
        return difftime(checkOutTime, checkInTime) / (60 * 60 * 24);
    }

    // Helper: Check if a room is available for a date range and number of people, optionally skipping a booking ID
    bool isRoomAvailable(const Room &room, const string &checkInDate, const string &checkOutDate, int numberOfPeople, const string &skipBookingId = "")
    {
        if (room.getCapacity() < numberOfPeople)
            return false;
        for (const auto &booking : bookingsList)
        {
            if (!skipBookingId.empty() && booking.getBookingID() == skipBookingId)
                continue;
            if (booking.getRoomNumber() == room.getRoomNumber() &&
                ((checkInDate >= booking.getCheckInDate() && checkInDate < booking.getCheckOutDate()) ||
                 (checkOutDate > booking.getCheckInDate() && checkOutDate <= booking.getCheckOutDate()) ||
                 (checkInDate <= booking.getCheckInDate() && checkOutDate >= booking.getCheckOutDate())))
            {
                return false;
            }
        }
        return true;
    }

    // Optimized: Get available rooms for a date range and number of people, optionally skipping a booking ID
    vector<Room> getAvailableRooms(const string &checkInDate, const string &checkOutDate, int numberOfPeople, const string &skipBookingId = "")
    {
        vector<Room> availableRooms;
        const auto &rooms = RoomManager::get().getRoomsList();
        availableRooms.reserve(rooms.size());
        for (const auto &room : rooms)
        {
            if (isRoomAvailable(room, checkInDate, checkOutDate, numberOfPeople, skipBookingId))
                availableRooms.push_back(room);
        }
        return availableRooms;
    }

public:
    BookingManager()
    {
        loadBookingIdCounter();
        loadBookings();
    }

    ~BookingManager()
    {
        saveBookingIdCounter();
        SaveBookings();
    }

    auto &getBookingsList()
    {
        return bookingsList;
    }

    void addBookingAdmin()
    {
        cout << string(50, '-') << " Find a Guest " << string(50, '-') << "\n";

        while (true)
        {
            string guestEmail = InputValidator::get().getValidatedInput("Enter guest email: ", EMAIL_REGEX, "Invalid email format.");

            fstream hotel_users;
            hotel_users.open("hotel_users.txt", ios::in);
            bool found = false;
            Guest foundGuest;
            if (hotel_users.is_open())
            {
                string line;
                while (getline(hotel_users, line))
                {
                    stringstream ss(line);
                    string email, password, firstName, lastName, fullName, contactNumber;

                    getline(ss, email, '|');
                    getline(ss, password, '|');
                    getline(ss, firstName, '|');
                    getline(ss, lastName, '|');
                    getline(ss, fullName, '|');
                    getline(ss, contactNumber, '|');

                    if (guestEmail == email)
                    {
                        cout << string(25, '-') << " Guest Found " << string(25, '-') << "\n";
                        cout << "Email: " << email << endl;
                        cout << "Full Name: " << fullName << endl;
                        cout << "Contact Number: " << contactNumber << endl;

                        foundGuest = Guest(email, password, firstName, lastName, fullName, contactNumber);
                        found = true;
                        break;
                    }
                }
                hotel_users.close();
            }

            if (found)
            {
                string confirm = InputValidator::get().getValidatedInput("Is this the correct guest? [Y/N]: ", "^[YyNn]$", "Please enter [Y/N].");
                if (confirm == "Y" || confirm == "y")
                {
                    addBookingGuest(foundGuest);
                    break;
                }
                else
                {
                    cout << "Let's try again.\n";
                }
            }
            else
            {
                cout << "Guest not found. Please try again.\n";
            }
        }
    }

    void addBookingGuest(const Guest &guest)
    {
        cout << string(50, '-') << " Make a Booking " << string(50, '-') << "\n";
        string checkInDate, checkOutDate;
        int numberOfPeople;
        vector<Room> availableRooms;

        while (true)
        {
            // Input and validate dates
            while (true)
            {
                checkInDate = InputValidator::get().getValidatedInput("Enter check-in date (YYYY-MM-DD): ", "^(\\d{4}-\\d{2}-\\d{2})$", "Invalid date format. Use YYYY-MM-DD.");
                if (isValidDate(checkInDate) && isFutureDate(checkInDate))
                    break;
                cout << "Check-in date must be a valid future date." << endl;
            }
            while (true)
            {
                checkOutDate = InputValidator::get().getValidatedInput("Enter check-out date (YYYY-MM-DD): ", "^(\\d{4}-\\d{2}-\\d{2})$", "Invalid date format. Use YYYY-MM-DD.");
                if (isValidDate(checkOutDate) && isFutureDate(checkOutDate) && checkOutDate > checkInDate)
                    break;
                cout << "Check-out date must be a valid date later than the check-in date." << endl;
            }
            numberOfPeople = stoi(InputValidator::get().getValidatedInput("Enter number of people: ", "^[1-9]\\d*$", "Invalid number of people. Must be a positive integer."));

            availableRooms = getAvailableRooms(checkInDate, checkOutDate, numberOfPeople);
            if (availableRooms.empty())
            {
                cout << "No available rooms for the specified date range and number of people." << endl;
                continue;
            }
            break;
        }
        cout << string(90, '-') << endl;
        cout << left << setw(15) << "Room Number"
             << setw(20) << "Room Type"
             << setw(10) << "Capacity"
             << setw(15) << "Price/Night"
             << setw(20) << "Features" << endl;
        for (const auto &room : availableRooms)
        {
            cout << left << setw(15) << room.getRoomNumber()
                 << setw(20) << room.getRoomType()
                 << setw(10) << room.getCapacity()
                 << setw(15) << fixed << setprecision(2) << room.getRoomPricePerNight()
                 << setw(20) << room.getRoomFeatures() << endl;
        }
        while (true)
        {
            string roomNumber = InputValidator::get().getValidatedInput("Enter the room number to book: ", "^\\d{3}$", "Invalid room number format. Room number must be 3 digits.");
            auto it = find_if(availableRooms.begin(), availableRooms.end(),
                              [&roomNumber](const Room &room)
                              {
                                  return room.getRoomNumber() == roomNumber;
                              });

            if (it == availableRooms.end())
            {
                cout << "Invalid room selection. Room number not found in available rooms." << endl;
                continue;
            }

            Room selectedRoom = *it;
            double totalPrice = selectedRoom.getRoomPricePerNight() * calculateDaysDifference(checkInDate, checkOutDate);
            string bookingID = to_string(bookingIdCounter++);

            cout << "Choose payment method:\n";
            cout << "1. Cash\n2. Gcash\n3. Credit/Debit Card\n4. Bank Transfer\n";
            int paymentChoice = stoi(InputValidator::get().getValidatedInput("Enter your choice [1 - 4]: ", "^[1-4]$", "Invalid choice. Please enter [1 - 4]."));
            string paymentMethod;
            PaymentContext paymentContext;
            switch (paymentChoice)
            {
            case 1:
                paymentContext.setPaymentStrategy(new CashPayment());
                paymentContext.processPayment(totalPrice);
                paymentMethod = "Cash";
                break;
            case 2:
                paymentContext.setPaymentStrategy(new GcashPayment());
                paymentContext.processPayment(totalPrice);
                paymentMethod = "Gcash";
                break;
            case 3:
                paymentContext.setPaymentStrategy(new CardPayment());
                paymentContext.processPayment(totalPrice);
                paymentMethod = "Credit/Debit Card";
                break;
            case 4:
                paymentContext.setPaymentStrategy(new BankTransferPayment());
                paymentContext.processPayment(totalPrice);
                paymentMethod = "Bank Transfer";
                break;
            }
            bookingsList.push_back(Booking(bookingID, guest.getEmail(), guest.getFullName(), selectedRoom.getRoomNumber(), checkInDate, checkOutDate, totalPrice, paymentMethod));

            cout << "\nBooking successful! (ID: " << bookingID << ")" << endl;
            break;
        }
    }

    void displayBookingsAdmin()
    {
        cout << string(50, '-') << " Bookings List " << string(50, '-') << "\n";
        for (const auto &booking : bookingsList)
        {
            booking.displayBookingDetailsAdmin();
            cout << string(50, '-') << endl;
        }
    }

    void displayBookingsGuest(const Guest &guest)
    {
        cout << string(50, '-') << " Your Bookings " << string(50, '-') << "\n";
        for (const auto &booking : bookingsList)
        {
            if (booking.getGuestEmail() == guest.getEmail())
            {
                booking.displayBookingDetailsGuest();
                cout << string(50, '-') << endl;
            }
        }
    }

    void editBooking()
    {
        cout << string(50, '-') << " Edit Booking " << string(50, '-') << "\n";
        while (true)
        {
            string bookingID = InputValidator::get().getValidatedInput("Enter booking ID to edit: ", "^\\d+$", "Invalid booking ID format. Booking ID must be a number.");
            auto booking_it = find_if(bookingsList.begin(), bookingsList.end(),
                                      [&bookingID](const Booking &booking)
                                      {
                                          return booking.getBookingID() == bookingID;
                                      });

            if (booking_it == bookingsList.end())
            {
                cout << "Booking not found." << endl;
                continue;
            }

            cout << string(25, '-') << " Booking found " << string(25, '-') << endl;
            booking_it->displayBookingDetailsAdmin();
            cout << string(50, '-') << endl;
            string confirm = InputValidator::get().getValidatedInput("Is this the correct booking? [Y/N]: ", "^[YyNn]$", "Please enter [Y/N].");

            if (confirm == "N" || confirm == "n")
            {
                cout << "Let's try again.\n";
                continue;
            }

            vector<Room> availableRooms;
            string checkInDate, checkOutDate;
            int numberOfPeople;
            while (true)
            {
                while (true)
                {
                    checkInDate = InputValidator::get().getValidatedInput("Enter new check-in date (YYYY-MM-DD): ", "^(\\d{4}-\\d{2}-\\d{2})$", "Invalid date format. Use YYYY-MM-DD.");
                    if (isValidDate(checkInDate) && isFutureDate(checkInDate))
                        break;
                    cout << "Check-in date must be a valid future date." << endl;
                }
                while (true)
                {
                    checkOutDate = InputValidator::get().getValidatedInput("Enter new check-out date (YYYY-MM-DD): ", "^(\\d{4}-\\d{2}-\\d{2})$", "Invalid date format. Use YYYY-MM-DD.");
                    if (isValidDate(checkOutDate) && isFutureDate(checkOutDate) && checkOutDate > checkInDate)
                        break;
                    cout << "Check-out date must be a valid date later than the check-in date." << endl;
                }
                string numberOfPeopleStr = InputValidator::get().getValidatedInput("Enter new number of people: ", "^[1-9]\\d*$", "Invalid number of people. Must be a positive integer.");
                numberOfPeople = stoi(numberOfPeopleStr);

                availableRooms = getAvailableRooms(checkInDate, checkOutDate, numberOfPeople, bookingID);

                if (availableRooms.empty())
                {
                    cout << "No available rooms for the specified date range and number of people." << endl;
                    continue;
                }
                break;
            }
            cout << left << setw(15) << "Room Number"
                 << setw(20) << "Room Type"
                 << setw(10) << "Capacity"
                 << setw(15) << "Price/Night"
                 << setw(20) << "Features" << endl;
            for (const auto &room : availableRooms)
            {
                cout << left << setw(15) << room.getRoomNumber()
                     << setw(20) << room.getRoomType()
                     << setw(10) << room.getCapacity()
                     << setw(15) << fixed << setprecision(2) << room.getRoomPricePerNight()
                     << setw(20) << room.getRoomFeatures() << endl;
            }
            while (true)
            {
                string roomNumber = InputValidator::get().getValidatedInput("Enter the new room number to book: ", "^\\d{3}$", "Invalid room number format. Room number must be 3 digits.");
                auto it = find_if(availableRooms.begin(), availableRooms.end(),
                                  [&roomNumber](const Room &room)
                                  {
                                      return room.getRoomNumber() == roomNumber;
                                  });
                if (it == availableRooms.end())
                {
                    cout << "Invalid room selection. Room number not found in available rooms." << endl;
                    continue;
                }
                Room selectedRoom = *it;
                double totalPrice = selectedRoom.getRoomPricePerNight() * calculateDaysDifference(checkInDate, checkOutDate);

                booking_it->setCheckInDate(checkInDate);
                booking_it->setCheckOutDate(checkOutDate);
                booking_it->setRoomNumber(selectedRoom.getRoomNumber());
                booking_it->setTotalPrice(totalPrice);
                break;
            }
            break;
        }
    }
};

int main()
{
    GuestManager guestManager;
    BookingManager bookingManager;
    while (true)
    {
        Guest loggedInGuest = guestManager.signingPage();

        if (loggedInGuest.getEmail() == "")
        {
            break;
        }

        if (loggedInGuest.getEmail() == "admin@gmail.com")
        {
            int choice;
            do
            {
                cout << string(50, '-') << " Admin Menu " << string(50, '-') << "\n";
                cout << "1. Manage Users\n2. Manage Rooms\n3. Manage Bookings\n4. Logout\n";
                choice = stoi(InputValidator::get().getValidatedInput("Enter your choice [1 - 4]: ", "^[1-4]$", "Invalid choice. Please enter [1 - 4]."));
                switch (choice)
                {
                case 1:
                    int adminUserChoice;
                    do
                    {
                        cout << string(50, '-') << " Manage Users " << string(50, '-') << "\n";
                        cout << "1. Add Users\n2. View Users\n3. Update Users\n4. Remove Users\n5. Back to Admin Menu" << endl;
                        adminUserChoice = stoi(InputValidator::get().getValidatedInput("Enter your choice [1 - 5]: ", "^[1-5]$", "Invalid choice. Please enter [1 - 5]."));
                        switch (adminUserChoice)
                        {
                        case 1:
                            // guestManager.();
                            break;
                        case 2:
                            guestManager.viewAllUsers();
                            break;
                        case 3:
                            // guestManager.editGuestInfo(bookingManager.getBookingsList());
                            break;
                        case 4:
                            guestManager.deleteGuest();
                            break;
                        case 5:
                            cout << "Returning to Admin Menu..." << endl;
                            break;
                        }
                    } while (adminUserChoice != 5);
                    break;
                case 2:
                    int adminRoomChoice;
                    do
                    {
                        cout << string(50, '-') << " Manage Rooms " << string(50, '-') << "\n";
                        cout << "1. Add Room\n2. View Rooms\n3. Update Room\n4. Remove Room\n5. Back to Admin Menu" << endl;
                        adminRoomChoice = stoi(InputValidator::get().getValidatedInput("Enter your choice [1 - 5]: ", "^[1-5]$", "Invalid choice. Please enter [1 - 5]."));
                        switch (adminRoomChoice)
                        {
                        case 1:
                            RoomManager::get().addRoom();
                            break;
                        case 2:
                            RoomManager::get().displayRooms();
                            break;
                        case 3:
                            RoomManager::get().updateRoom();
                            break;
                        case 4:
                            RoomManager::get().deleteRoom();
                            break;
                        case 5:
                            cout << "Returning to Admin Menu..." << endl;
                            break;
                        }
                    } while (adminRoomChoice != 5);
                    break;
                case 3:
                    int adminBookingChoice;
                    do
                    {
                        cout << string(50, '-') << " Manage Bookings " << string(50, '-') << "\n";
                        cout << "1. Add Booking\n2. View Bookings\n3. Edit Booking\n4. Remove Booking\n5. Back to Admin Menu" << endl;
                        adminBookingChoice = stoi(InputValidator::get().getValidatedInput("Enter your choice [1 - 5]: ", "^[1-5]$", "Invalid choice. Please enter [1 - 5]."));
                        switch (adminBookingChoice)
                        {
                        case 1:
                            bookingManager.addBookingAdmin();
                            break;
                        case 2:
                            bookingManager.displayBookingsAdmin();
                            break;
                        case 3:
                            bookingManager.editBooking();
                            break;
                        case 4:
                            // bookingManager.deleteBooking();
                            break;
                        case 5:
                            cout << "Returning to Admin Menu..." << endl;
                            break;
                        }
                    } while (adminBookingChoice != 5);
                    break;
                case 4:
                    cout << "Logging Out..." << endl;
                    break;
                }
            } while (choice != 4);
        }
    }

    return 0;
}