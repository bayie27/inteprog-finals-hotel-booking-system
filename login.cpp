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
        string email = InputValidator::getValidatedInput("Enter your email: ", EMAIL_REGEX, "Invalid email format.");
        if (emailExistsInFile(email))
        {
            cout << "That email is already registered. Try another.\n";
            return;
        }
        string password = InputValidator::getValidatedInput("Enter your password: ", PASSWORD_REGEX, "Password must be at least 6 characters long and can include letters, numbers, and special characters.");

        string firstName = InputValidator::getValidatedInput("Enter your first name: ", FIRSTNAME_REGEX, "First name can only contain letters and spaces.");
        string lastName = InputValidator::getValidatedInput("Enter your last name: ", LASTNAME_REGEX, "Last name can only be contain letters and spaces.");
        string contactNumber = InputValidator::getValidatedInput("Enter your contact number: ", CONTACT_REGEX, "Contact number must start with '09' or '+639' and be 11 digits long.");
        string fullName = firstName + " " + lastName;
        Guest newGuest(email, password, firstName, lastName, fullName, contactNumber);
        addGuestToFile(newGuest);
        cout << "Account created successfully! Welcome, " << newGuest.getFullName() << "!\n";
    }

    Guest signIn()
    {
        cout << string(50, '-') << " Sign In Page " << string(50, '-') << "\n";
        string email = InputValidator::getValidatedInput("Enter your email: ", EMAIL_REGEX, "Invalid email format.");
        string password = InputValidator::getValidatedInput("Enter your password: ", PASSWORD_REGEX, "Password must be at least 6 characters long and can include letters, numbers, and special characters.");
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
        string email = InputValidator::getValidatedInput("Enter your email: ", EMAIL_REGEX, "Invalid email format.");
        string password = InputValidator::getValidatedInput("Enter your password: ", PASSWORD_REGEX, "Password must be at least 6 characters long and can include letters, numbers, and special characters.");
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
                    string newPassword = InputValidator::getValidatedInput("Enter your new password: ", PASSWORD_REGEX, "Password must be at least 6 characters long and can include letters, numbers, and special characters.");
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

    void editGuestInfo()
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
        string firstName = InputValidator::getValidatedInput("Enter your first name: ", "^$|^([A-Z][a-z]{1,49})( [A-Z][a-z]{1,49})?$", "First name can only contain letters and spaces.");
        if (firstName.empty())
        {
            firstName = loggedInGuest.getFirstName();
        }
        string lastName = InputValidator::getValidatedInput("Enter your last name: ", "^$|^[A-Z][a-z]{1,49}(['-][A-Z][a-z]{1,49})?( (Jr\\.|Sr\\.|II|III|IV|V))?$", "Last name can only be contain letters and spaces.");
        if (lastName.empty())
        {
            lastName = loggedInGuest.getLastName();
        }
        string fullName = firstName + " " + lastName;
        string contactNumber = InputValidator::getValidatedInput("Enter your contact number: ", "^$|^(09|\\+639)\\d{9}$", "Contact number must start with '09' or '+639' and be 11 digits long.");
        if (contactNumber.empty())
        {
            contactNumber = loggedInGuest.getContactNumber();
        }
        string password = InputValidator::getValidatedInput("Enter your password: ", "^$|^[a-zA-Z0-9@#$%^&+=]{6,}$", "Password must be at least 6 characters long and can include letters, numbers, and special characters.");
        if (password.empty())
        {
            password = loggedInGuest.getPassword();
        }

        cout << "\nName: " << fullName << "\n"
             << "Phone: " << contactNumber << "\n"
             << "Email: " << loggedInGuest.getEmail() << "\n"
             << "Password: " << password << "\n";

        string choice = InputValidator::getValidatedInput("Confirm this changes [Y/N]? ", "^[YyNn]$", "Please enter [Y/N].");
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
        cout << "Your info has been updated successfully!\n";
    }

    void deleteGuest()
    {
        string email = InputValidator::getValidatedInput("Enter the email: ", "(^[a-zA-Z0-9._%+-]+@(gmail|yahoo)\\.com$)", "Invalid email format.");
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

        string choice = InputValidator::getValidatedInput("Do you want to delete a user account [Y/N]? ", "^[YyNn]$", "Please enter [Y/N].");
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
            int choice = stoi(InputValidator::getValidatedInput("Enter your choice [1 - 4]: ", "^[1-4]$", "Invalid choice. Please enter [1 - 4]."));
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

class BookingManager
{
private:
    vector<Booking> bookingsList;

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

public:
    BookingManager()
    {
        loadBookings();
    }

    ~BookingManager()
    {
        SaveBookings();
    }

    void addBooking(const Guest &guest)
    {
        cout << string(50, '-') << " Make a Booking " << string(50, '-') << "\n";
        string checkInDate, checkOutDate;

        while (true)
        {
            checkInDate = InputValidator::get().getValidatedInput("Enter check-in date (YYYY-MM-DD): ", "^(\\d{4}-\\d{2}-\\d{2})$", "Invalid date format. Use YYYY-MM-DD.");
            if (isValidDate(checkInDate) && isFutureDate(checkInDate))
            {
                break;
            }
            else
            {
                cout << "Check-in date must be a valid future date." << endl;
            }
        }

        while (true)
        {
            checkOutDate = InputValidator::get().getValidatedInput("Enter check-out date (YYYY-MM-DD): ", "^(\\d{4}-\\d{2}-\\d{2})$", "Invalid date format. Use YYYY-MM-DD.");
            if (isValidDate(checkOutDate) && isFutureDate(checkOutDate) && checkOutDate > checkInDate)
            {
                break;
            }
            else
            {
                cout << "Check-out date must be a valid date later than the check-in date." << endl;
            }
        }

        int numberOfPeople = stoi(InputValidator::get().getValidatedInput("Enter number of people: ", "^[1-9]\\d*$", "Invalid number of people. Must be a positive integer."));

        vector<Room> availableRooms;
        for (const auto &room : RoomManager::get().getRoomsList())
        {
            bool isAvailable = true;

            // Check if room capacity is sufficient
            if (room.getCapacity() < numberOfPeople)
            {
                continue;
            }

            // Check if room is already booked for the given date range
            for (const auto &booking : bookingsList)
            {
                if (booking.getRoomNumber() == room.getRoomNumber() &&
                    ((checkInDate >= booking.getCheckInDate() && checkInDate < booking.getCheckOutDate()) ||
                     (checkOutDate > booking.getCheckInDate() && checkOutDate <= booking.getCheckOutDate()) ||
                     (checkInDate <= booking.getCheckInDate() && checkOutDate >= booking.getCheckOutDate())))
                {
                    isAvailable = false;
                    break;
                }
            }

            if (isAvailable)
            {
                availableRooms.push_back(room);
            }
        }

        if (availableRooms.empty())
        {
            cout << "No available rooms for the specified date range and number of people." << endl;
            return;
        }

        cout << left << setw(15) << "Room Number"
             << setw(20) << "Room Type"
             << setw(10) << "Capacity"
             << setw(15) << "Price/Night"
             << setw(20) << "Features" << endl; // Display features last
        cout << string(90, '-') << endl;

        for (const auto &room : availableRooms)
        {
            cout << left << setw(15) << room.getRoomNumber()
                 << setw(20) << room.getRoomType()
                 << setw(10) << room.getCapacity()
                 << setw(15) << fixed << setprecision(2) << room.getRoomPricePerNight()
                 << setw(20) << room.getRoomFeatures() << endl; // Display features last
        }

        string roomNumber = InputValidator::get().getValidatedInput("Enter the room number to book: ", "^\\d{3}$", "Invalid room number format. Room number must be 3 digits.");
        auto it = find_if(availableRooms.begin(), availableRooms.end(),
                          [&roomNumber](const Room &room)
                          {
                              return room.getRoomNumber() == roomNumber;
                          });

        if (it == availableRooms.end())
        {
            cout << "Invalid room selection. Room number not found in available rooms." << endl;
            return;
        }

        Room selectedRoom = *it;
        double totalPrice = selectedRoom.getRoomPricePerNight() * calculateDaysDifference(checkInDate, checkOutDate);
        string bookingID = to_string(bookingsList.size() + 1);

        string paymentMethod = InputValidator::get().getValidatedInput("Enter payment method (e.g., Credit Card, PayPal): ", "^.+$", "Invalid payment method.");

        bookingsList.push_back(Booking(bookingID, guest.getEmail(), guest.getFullName(), selectedRoom.getRoomNumber(), checkInDate, checkOutDate, totalPrice, paymentMethod));

        cout << "Booking successful! (ID: " << bookingID << ")" << endl;
    }

    void displayBookings()
    {
        cout << string(50, '-') << " Bookings List " << string(50, '-') << "\n";
        for (const auto &booking : bookingsList)
        {
            booking.displayBookingDetailsAdmin();
            cout << string(50, '-') << endl;
        }
    }
};

int main()
{
    GuestManager guestManager;
    BookingManager bookingManager;
    Guest loggedInGuest = guestManager.signingPage();

    // if (loggedInGuest.getEmail() == "admin@gmail.com")
    // {
    //     int choice;
    //     do
    //     {
    //         cout << string(50, '-') << " Admin Menu " << string(50, '-') << "\n";
    //         cout << "1. Add Room\n2. Update Room Details\n3. Remove Room\n4. Display Rooms\n5. Exit\n";
    //         choice = stoi(InputValidator::get().getValidatedInput("Enter your choice [1 - 5]: ", "^[1-5]$", "Invalid choice. Please enter [1 - 5]."));
    //         switch (choice)
    //         {
    //         case 1:
    //             RoomManager::get().addRoom();
    //             break;
    //         case 2:
    //             RoomManager::get().updateRoom();
    //             break;
    //         case 3:
    //             RoomManager::get().deleteRoom();
    //             break;
    //         case 4:
    //             RoomManager::get().displayRooms();
    //             break;
    //         case 5:
    //             cout << "Exiting..." << endl;
    //             break;
    //         default:
    //             cout << "Invalid choice." << endl;
    //         }
    //     } while (choice != 5);
    // }
    // else
    // {
    //     cout << "You are not authorized to access this menu." << endl;
    // }

    bookingManager.addBooking(loggedInGuest);
    return 0;
}