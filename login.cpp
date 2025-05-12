#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>
using namespace std;

class InputValidator
{
public:
    static bool isValidInput(const string &input, const regex &compiledPattern)
    {
        if (input.empty())
        {
            return false;
        }
        return regex_match(input, compiledPattern);
    }
    static string getValidatedInput(const string &prompt, const string &pattern, const string &errorMessage)
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
    Guest() : email(""), password(""), firstName(""), lastName(""), fullName(""), contactNumber("") {}
    Guest(string _email, string _password, string _firstName, string _lastName, string _fullName, string _contactNumber) : email(_email), password(_password), firstName(_firstName), lastName(_lastName), fullName(_fullName), contactNumber(_contactNumber) {}

    void setEmail(const string &newEmail) { email = newEmail; }
    void setPassword(const string &newPassword) { password = newPassword; }
    void setFirstName(const string &newFirstName) { firstName = newFirstName; }
    void setLastName(const string &newLastName) { lastName = newLastName; }
    void setFullName(const string &newFullName) { fullName = newFullName; }
    void setContactNumber(const string &newContactNumber) { contactNumber = newContactNumber; }

    string getEmail() const { return email; }
    string getPassword() const { return password; }
    string getFirstName() const { return firstName; }
    string getLastName() const { return lastName; }
    string getFullName() const { return fullName; }
    string getContactNumber() const { return contactNumber; }
};

class GuestManager
{
private:
    Guest loggedInGuest;

    const string USER_FILE;
    // store newly user to records of user (txt file)

    bool emailExistsInFile(const string &email)
    {
        fstream file;

        file.open(USER_FILE, ios::in); // read
        if (file.is_open())
        {
            string line;
            while (getline(file, line))
            {
                string token;
                vector<string> tokens;
                stringstream ss(line);
                while (getline(ss, token, '|'))
                {
                    tokens.push_back(token);
                }
                if (tokens[0] == email)
                {
                    file.close();
                    return true; // email found
                }
            }
        }
        else
        {
            cout << "File can't be open." << endl;
            return false;
        }
        return false;
    }

    void addGuestToFile(string email, const Guest &guest)
    {
        fstream file;

        string line;
        vector<string> lines;

        file.open(USER_FILE, ios::in); // read
        if (file.is_open())
        {
            while (getline(file, line))
            {
                lines.push_back(line);
            }
            file.close();

        } // else throw (File can't be open.

        lines.push_back(guest.getEmail() + "|" + guest.getPassword() + "|" + guest.getFirstName() + "|" + guest.getLastName() + "|" + guest.getFullName() + "|" + guest.getContactNumber());

        file.open(USER_FILE, ios::out); // write
        if (file.is_open())
        {
            for (const auto &l : lines)
            {
                file << l << "\n";
            }
            file.close();
        } // else throw (File can't be open.)
    }

    bool findGuestByEmail(const string &email, const string &password)
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
                    loggedInGuest.setEmail(tokens[0]);
                    loggedInGuest.setPassword(tokens[1]);
                    loggedInGuest.setFirstName(tokens[2]);
                    loggedInGuest.setLastName(tokens[3]);
                    loggedInGuest.setFullName(tokens[4]);
                    loggedInGuest.setContactNumber(tokens[5]);
                    file.close();
                    return true;
                }
            }
            file.close();
        }
        return false; // email/password not found
    }

    void editGuestInfo(const Guest &guest)
    {
        /*Display the loggedInGuest info
        Input the guest informations
        Set the edited loggedInGuest info in runtime
        update the loggedInGuest info in hotel_users.txt*/
    }

public:
    GuestManager() : USER_FILE("hotel_users.txt") {}

    // need forgot password (update password)
    void signUp()
    {
        string email = InputValidator::getValidatedInput("Enter your email: ", "(^[a-zA-Z0-9._%+-]+@(gmail|yahoo)\\.com$)", "Invalid email format.");
        // needed to scan first txt file to search if existing user (using txt file to scan as if existing user)
        if (emailExistsInFile(email))
        {
            cout << "That email is already registered. Try another.\n";
            return;
        }

        string password = InputValidator::getValidatedInput("Enter your password: ", "^[a-zA-Z0-9@#$%^&+=]{6,}$", "Password must be at least 6 characters long and can include letters, numbers, and special characters.");

        string firstName = InputValidator::getValidatedInput("Enter your first name: ", "^([A-Z][a-z]{1,49})( [A-Z][a-z]{1,49})?$", "First name can only contain letters and spaces.");
        string lastName = InputValidator::getValidatedInput("Enter your last name: ", "^[A-Z][a-z]{1,49}(['-][A-Z][a-z]{1,49})?( (Jr\\.|Sr\\.|II|III|IV|V))?$", "Last name can only be contain letters and spaces.");
        string contactNumber = InputValidator::getValidatedInput("Enter your contact number: ", "^(09|\\+639)\\d{9}$", "Contact number must start with '09' or '+639' and be 11 digits long.");
        string fullName = firstName + " " + lastName;

        // Create the new guest object
        Guest newGuest(email, password, firstName, lastName, fullName, contactNumber);

        // store newly user to records of user (txt file)
        addGuestToFile(email, newGuest);

        cout << "Account created successfully! Welcome, " << newGuest.getFullName() << "!\n";
    }

    Guest signIn()
    {
        string email = InputValidator::getValidatedInput("Enter your email: ", "(^[a-zA-Z0-9._%+-]+@(gmail|yahoo)\\.com$)", "Invalid email format.");
        string password = InputValidator::getValidatedInput("Enter your password: ", "^[a-zA-Z0-9@#$%^&+=]{6,}$", "Password must be at least 6 characters long and can include letters, numbers, and special characters.");
        // Checks email and password
        if (!findGuestByEmail(email, password))
        {
            cout << "Incorrect email or password.\n";
            return Guest();
        }
        cout << "Welcome, " << loggedInGuest.getFullName() << "!\n";
        return Guest(loggedInGuest.getEmail(), loggedInGuest.getPassword(), loggedInGuest.getFirstName(), loggedInGuest.getLastName(), loggedInGuest.getFullName(), loggedInGuest.getContactNumber());
    }

    Guest signingPage()
    {
        int choice;
        Guest loggedInGuest;

        do
        {
            cout << string(50, '-') << " Hotel Reservation System " << string(50, '-') << "\n";
            cout << "1. Sign In\n2. Sign Up\n3. Exit\n";
            int choice = stoi(InputValidator::getValidatedInput("Enter your choice [1 - 3]: ", "^[1-3]$", "Invalid choice. Please enter [1 - 3]."));
            switch (choice)
            {
            case 1:
                cout << string(50, '-') << " Sign In Page " << string(50, '-') << "\n";
                loggedInGuest = signIn();
                break;
            case 2:
                cout << string(50, '-') << " Sign Up Page " << string(50, '-') << "\n";
                signUp();
                break;
            case 3:
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
        cout << "Capacity: " << capacity << " person(s)" << endl; // Display capacity after room type
        cout << "Room Features: " << roomFeatures << endl;
        cout << "Price per Night: $" << fixed << setprecision(2) << roomPricePerNight << endl;
    }
};

class RoomManager
{
private:
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
                getline(ss, roomFeatures, '|');
                getline(ss, priceStr); // Last field, no delimiter needed

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
                            << room.getCapacity() << "|" // Save capacity after room type
                            << room.getRoomFeatures() << "|"
                            << fixed << setprecision(2) << room.getRoomPricePerNight() << "\n";
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
    RoomManager()
    {
        loadRooms();
    }

    ~RoomManager()
    {
        SaveRooms();
    }

    void addRoom()
    {
        string roomNumber;
        bool roomExists = true;
        while (roomExists)
        {
            roomNumber = InputValidator::getValidatedInput("Enter room number: ", "^\\d{3}$", "Invalid room number format. Room number must be 3 digits.");
            roomExists = findRoom(roomNumber) != roomsList.end();
            if (roomExists)
            {
                cout << "Room number already exists. Please enter a different room number." << endl;
            }
        }
        string roomType = InputValidator::getValidatedInput("Enter room type: ", "^.+$", "Invalid room type format.");
        int capacity = stoi(InputValidator::getValidatedInput("Enter room capacity: ", "^\\d+$", "Invalid capacity format. Capacity must be a positive integer.")); // Input capacity after room type
        string roomFeatures = InputValidator::getValidatedInput("Enter room features: ", "^.+$", "Invalid room features format.");
        double roomPricePerNight = stod(InputValidator::getValidatedInput("Enter price per night: ", "^\\d+\\.\\d{2}$", "Invalid price format. Price must be a number with two decimal places."));

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
        string roomNumber = InputValidator::getValidatedInput("Enter room number to update: ", "^\\d{3}$", "Invalid room number format. Room number must be 3 digits.");
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
        string roomType = InputValidator::getValidatedInput("Enter new room type: ", "^.+$", "Invalid room type format.");
        int capacity = stoi(InputValidator::getValidatedInput("Enter new room capacity: ", "^\\d+$", "Invalid capacity format. Capacity must be a positive integer.")); // Input capacity after room type
        string roomFeatures = InputValidator::getValidatedInput("Enter new room features: ", "^.+$", "Invalid room features format.");
        double roomPricePerNight = stod(InputValidator::getValidatedInput("Enter new price per night: ", "^\\d+\\.\\d{2}$", "Invalid price format. Price must be a number with two decimal places."));

        *it = Room(roomNumber, roomType, capacity, roomFeatures, roomPricePerNight);
    }

    void deleteRoom()
    {
        string roomNumber = InputValidator::getValidatedInput("Enter room number to remove: ", "^\\d{3}$", "Invalid room number format. Room number must be 3 digits.");
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
        string confirmation = InputValidator::getValidatedInput("Are you sure you want to delete this room? (y/n): ", "^(y|n)$", "Invalid input. Please enter 'y' or 'n'.");

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
    string bookingID, guestEmail, guestName, roomNumber, checkInDate, checkOutDate;
    double totalPrice;

public:
    Booking(string bookingID, string guestEmail, string guestName, string roomNumber, string checkInDate, string checkOutDate, double totalPrice)
        : bookingID(bookingID), guestEmail(guestEmail), guestName(guestName), roomNumber(roomNumber), checkInDate(checkInDate), checkOutDate(checkOutDate), totalPrice(totalPrice) {}

    string getBookingID() const { return bookingID; }
    string getGuestEmail() const { return guestEmail; }
    string getGuestName() const { return guestName; }
    string getRoomNumber() const { return roomNumber; }
    string getCheckInDate() const { return checkInDate; }
    string getCheckOutDate() const { return checkOutDate; }
    double getTotalPrice() const { return totalPrice; }

    void displayBookingDetailsAdmin() const
    {
        cout << "Booking ID: " << bookingID << endl;
        cout << "Guest Email: " << guestEmail << endl;
        cout << "Guest Name: " << guestName << endl;
        cout << "Room Number: " << roomNumber << endl;
        cout << "Check-In Date: " << checkInDate << endl;
        cout << "Check-Out Date: " << checkOutDate << endl;
        cout << "Total Price: $" << fixed << setprecision(2) << totalPrice << endl;
    }

    void displayBookingDetailsGuest() const
    {
        cout << "Booking ID: " << bookingID << endl;
        cout << "Room Number: " << roomNumber << endl;
        cout << "Check-In Date: " << checkInDate << endl;
        cout << "Check-Out Date: " << checkOutDate << endl;
        cout << "Total Price: $" << fixed << setprecision(2) << totalPrice << endl;
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
                string bookingID, guestEmail, guestName, roomNumber, checkInDate, checkOutDate;
                string totalPriceStr;

                getline(ss, bookingID, '|');
                getline(ss, guestEmail, '|');
                getline(ss, guestName, '|');
                getline(ss, roomNumber, '|');
                getline(ss, checkInDate, '|');
                getline(ss, checkOutDate, '|');
                getline(ss, totalPriceStr); // Last field, no delimiter needed

                double totalPrice = stod(totalPriceStr);

                bookingsList.push_back(Booking(bookingID, guestEmail, guestName, roomNumber, checkInDate, checkOutDate, totalPrice));
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
                               << fixed << setprecision(2) << booking.getTotalPrice() << "\n";
            }
            hotel_bookings.close();
        }
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

    void addBooking()
    {
        cout << string(50, '-') << " Make a Booking " << string(50, '-') << "\n";
        string checkInDate, checkOutDate;

        while (true)
        {
            checkInDate = InputValidator::getValidatedInput("Enter check-in date (YYYY-MM-DD): ", "^(\\d{4}-\\d{2}-\\d{2})$", "Invalid date format. Use YYYY-MM-DD.");
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
            checkOutDate = InputValidator::getValidatedInput("Enter check-out date (YYYY-MM-DD): ", "^(\\d{4}-\\d{2}-\\d{2})$", "Invalid date format. Use YYYY-MM-DD.");
            if (isValidDate(checkOutDate) && isFutureDate(checkOutDate) && checkOutDate > checkInDate)
            {
                break;
            }
            else
            {
                cout << "Check-out date must be a valid date later than the check-in date." << endl;
            }
        }

        int numberOfPeople = stoi(InputValidator::getValidatedInput("Enter number of people: ", "^[1-9]\\d*$", "Invalid number of people. Must be a positive integer."));

        
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
    RoomManager roomManager;
    BookingManager bookingManager;
    // Guest loggedInGuest = guestManager.signingPage();

    // if (loggedInGuest.getEmail() == "admin@gmail.com")
    // {
    //     int choice;
    //     do
    //     {
    //         cout << string(50, '-') << " Admin Menu " << string(50, '-') << "\n";
    //         cout << "1. Add Room\n2. Update Room Details\n3. Remove Room\n4. Display Rooms\n5. Exit\n";
    //         choice = stoi(InputValidator::getValidatedInput("Enter your choice [1 - 5]: ", "^[1-5]$", "Invalid choice. Please enter [1 - 5]."));
    //         switch (choice)
    //         {
    //         case 1:
    //             roomManager.addRoom();
    //             break;
    //         case 2:
    //             roomManager.updateRoom();
    //             break;
    //         case 3:
    //             roomManager.deleteRoom();
    //             break;
    //         case 4:
    //             roomManager.displayRooms();
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

    bookingManager.addBooking();

    return 0;
}