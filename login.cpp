#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <vector>
#include <iomanip>
#include <sstream>
#include <limits>
using namespace std;

const string EMAIL_REGEX = "(^[a-zA-Z0-9._%+-]+@(gmail|yahoo)\\.com$)";
const string EMAIL_B_REGEX = "(^[a-zA-Z0-9._%+-]+@(gmail|yahoo)\\.com$|[bB])";
const string PASSWORD_REGEX = "^[a-zA-Z0-9@#$%^&+=]{6,}$";
const string FIRSTNAME_REGEX = "^[A-Z][a-z]+([ -][A-Z][a-z]+)*$";
const string LASTNAME_REGEX = "^[A-Z][a-z]{1,49}([ '-][A-Z][a-z]{1,49})*( (Jr\\.|Sr\\.|II|III|IV|V))?$";
const string CONTACT_REGEX = "^(09|\\+639)\\d{9}$";
const string CONTACT_B_REGEX = "^(09\\d{9}|\\+639\\d{9}|[bB])$";

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
        bool invalidInput = true;
        while (invalidInput)
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
        return "";
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
    string getPaymentMethod() const { return paymentMethod; }

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

    void displayBookingDetailsAdminTable() const
    {
        cout << left << setw(15) << bookingID << setw(35) << guestEmail << setw(40) << guestName << setw(15) << roomNumber << setw(17) << checkInDate << setw(18) << checkOutDate << setw(15) << fixed << setprecision(2) << totalPrice << setw(20) << paymentMethod << endl; // Display paymentMethod
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

    void displayBookingDetailsGuestTable() const
    {
        cout << left << setw(15) << bookingID << setw(15) << roomNumber << setw(17) << checkInDate << setw(18) << checkOutDate << setw(15) << fixed << setprecision(2) << totalPrice << setw(20) << paymentMethod << endl; // Display paymentMethod
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

    bool contactNumberExistsInFile(const string &contactNumber)
    {
        fstream file;

        file.open(USER_FILE, ios::in); // read
        if (file.is_open())
        {
            string line;
            while (getline(file, line))
            {
                if (line.substr(line.rfind('|') + 1) == contactNumber)
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
            file << guest.getEmail() << "|" << guest.getPassword() << "|" << guest.getFirstName() << "|" << guest.getLastName() << "|" << guest.getFullName() << "|" << guest.getContactNumber() << "\n";
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

    bool loadGuestNumber(const string &email, const string &contactNumber)
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
                if (tokens.size() == 6 && tokens[0] == email && tokens[5] == contactNumber)
                {
                    loggedInGuest = Guest(tokens[0], tokens[1], tokens[2], tokens[3], tokens[4], tokens[5]);
                    file.close();
                    return true;
                }
            }
            file.close();
        } // else throw (File can't be open.
        return false; // email/contact number not found
    }

    bool loadGuestEmail(const string &email)
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
                if (tokens.size() == 6 && tokens[0] == email)
                {
                    loggedInGuest = Guest(tokens[0], tokens[1], tokens[2], tokens[3], tokens[4], tokens[5]);
                    file.close();
                    return true;
                }
            }
            file.close();
        } // else throw (File can't be open.
        return false; // email not found
    }

    bool noUsersInFile()
    {
        fstream file;
        string adminline, line;

        file.open(USER_FILE, ios::in);
        if (file.is_open())
        {
            getline(file, adminline);
            getline(file, line);
            if (line.empty())
            {
                file.close();
                return true;
            }
            file.close();
        } // else throw (File can't be open.
        return false;
    }

public:
    GuestManager() : USER_FILE("hotel_users.txt") {}

    void adminAddGuest()
    {
        string choice, email, password, firstName, lastName, contactNumber, fullName;
        do
        {
            cout << string(58, '-') << " Add User " << string(58, '-') << "\n";
            for (;;)
            {
                email = InputValidator::get().getValidatedInput("Enter email ('B' to go back): ", EMAIL_B_REGEX, "Invalid email format. Enter a valid email or 'B' to go back.");
                if (email == "B" | email == "b")
                {
                    cout << "Going back to menu...\n";
                    return;
                }
                if (emailExistsInFile(email))
                {
                    cout << "That email is already registered. Try another.\n";
                    continue;
                }
                break;
            }
            password = InputValidator::get().getValidatedInput("Enter password: ", PASSWORD_REGEX, "Password must be at least 6 characters long and can include letters, numbers, and special characters.");
            firstName = InputValidator::get().getValidatedInput("Enter first name: ", FIRSTNAME_REGEX, "Invalid first name format. Numbers, lowercase starts, and special characters are not allowed.");
            lastName = InputValidator::get().getValidatedInput("Enter last name: ", LASTNAME_REGEX, "Invalid name format. Numbers, lowercase starts, and special characters are not allowed.");
            for (;;)
            {
                contactNumber = InputValidator::get().getValidatedInput("Enter contact number: ", CONTACT_B_REGEX, "Contact number must start with '09' or '+639' and be 11 digits long.");
                if (contactNumber == "B" | contactNumber == "b")
                {
                    cout << "Going back to menu...\n";
                    return;
                }
                if (contactNumberExistsInFile(contactNumber))
                {
                    cout << "That contact number is already registered. Try another or enter 'B' to go back.\n";
                    continue;
                }
                break;
            }
            fullName = firstName + " " + lastName;
            cout << string(50, '-') << "\n\n";
            cout << "Email: " << email << "\n"
                 << "Password: " << password << "\n"
                 << "First name: " << firstName << "\n"
                 << "Last name: " << lastName << "\n"
                 << "Phone: " << contactNumber << "\n";
            cout << string(50, '-') << "\n";
            choice = InputValidator::get().getValidatedInput("Confirm these details [Y|N]? ", "^[YyNn]$", "Please enter [Y/N].");
        } while (choice == "N" | choice == "n");

        Guest newGuest(email, password, firstName, lastName, fullName, contactNumber);
        addGuestToFile(newGuest);
        cout << newGuest.getFirstName() << "'s account created successfully!\n";
    }

    void signUp()
    {
        string choice, email, password, firstName, lastName, contactNumber, fullName;
        do
        {
            cout << string(56, '-') << " Sign Up Page " << string(56, '-') << "\n";
            for (;;)
            {
                email = InputValidator::get().getValidatedInput("Enter email ('B' to go back): ", EMAIL_B_REGEX, "Invalid email format. Enter a valid email or 'B' to go back.");
                if (email == "B" | email == "b")
                {
                    cout << "Going back to menu...\n";
                    return;
                }
                if (emailExistsInFile(email))
                {
                    cout << "That email is already registered. Try another.\n";
                    continue;
                }
                break;
            }
            password = InputValidator::get().getValidatedInput("Enter password: ", PASSWORD_REGEX, "Password must be at least 6 characters long and can include letters, numbers, and special characters.");
            firstName = InputValidator::get().getValidatedInput("Enter first name: ", FIRSTNAME_REGEX, "Invalid first name format. Numbers, lowercase starts, and special characters are not allowed.");
            lastName = InputValidator::get().getValidatedInput("Enter last name: ", LASTNAME_REGEX, "Invalid last name format. Numbers, lowercase starts, and special characters are not allowed.");
            for (;;)
            {
                contactNumber = InputValidator::get().getValidatedInput("Enter contact number: ", CONTACT_B_REGEX, "Contact number must start with '09' or '+639' and be 11 digits long.");
                if (contactNumber == "B" | contactNumber == "b")
                {
                    cout << "Going back to menu...\n";
                    return;
                }
                if (contactNumberExistsInFile(contactNumber))
                {
                    cout << "That contact number is already registered. Try another or enter 'B' to go back.\n";
                    continue;
                }
                break;
            }
            fullName = firstName + " " + lastName;
            cout << string(50, '-') << "\n";
            cout << "Email: " << email << "\n"
                 << "Password: " << password << "\n"
                 << "First name: " << firstName << "\n"
                 << "Last name: " << lastName << "\n"
                 << "Phone: " << contactNumber << "\n";
            cout << string(50, '-') << "\n";
            choice = InputValidator::get().getValidatedInput("Confirm these details [Y|N]? ", "^[YyNn]$", "Please enter [Y/N].");
        } while (choice == "N" | choice == "n");

        Guest newGuest(email, password, firstName, lastName, fullName, contactNumber);
        addGuestToFile(newGuest);
        cout << "\nAccount created successfully! Welcome, " << newGuest.getFullName() << "!\n";
    }

    Guest signIn()
    {
        cout << string(56, '-') << " Sign In Page " << string(56, '-') << "\n";
        for (;;)
        {
            string email = InputValidator::get().getValidatedInput("Enter email ('B' to go back): ", EMAIL_B_REGEX, "Invalid email format. Enter a valid email or 'B' to go back.");
            if (email == "B" | email == "b")
            {
                cout << "Going back to menu...\n";
                return Guest();
            }
            string password = InputValidator::get().getValidatedInput("Enter password: ", PASSWORD_REGEX, "Password must be at least 6 characters long and can include letters, numbers, and special characters.");
            if (!loadGuest(email, password))
            {
                cout << "Email not found or password is incorrect.\n";
                continue;
            }
            break;
        }
        cout << "\nWelcome, " << loggedInGuest.getFullName() << "!\n";
        return loggedInGuest;
    }

    void forgotPassword()
    {
        string email, contactNumber;
        cout << string(54, '-') << " Forgot Password " << string(54, '-') << "\n";
        for (;;)
        {
            email = InputValidator::get().getValidatedInput("Enter email ('B' to go back): ", EMAIL_B_REGEX, "Invalid email format. Enter a valid email or 'B' to go back.");
            if (email == "B" | email == "b")
            {
                cout << "Going back to menu...\n";
                return;
            }
            contactNumber = InputValidator::get().getValidatedInput("Enter contact number: ", CONTACT_REGEX, "Contact number must start with '09' or '+639' and be 11 digits long.");
            if (!loadGuestNumber(email, contactNumber))
            {
                cout << "Email not found or contact number is incorrect.\n";
                continue;
            }
            break;
        }
        cout << string(50, '-') << "\n";
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
                    string newPassword = InputValidator::get().getValidatedInput("Enter new password: ", PASSWORD_REGEX, "Password must be at least 6 characters long and can include letters, numbers, and special characters.");
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
        cout << "Password updated successfully!\n";
    }

    void adminEditGuestInfo(vector<Booking> &bookingsList, Guest guest)
    {
        string email;
        cout << string(57, '-') << " Edit Guest " << string(57, '-') << "\n";
        if (noUsersInFile())
        {
            cout << "No users found.\n";
            return;
        }
        for (;;)
        {
            email = InputValidator::get().getValidatedInput("Enter email ('B' to go back): ", EMAIL_B_REGEX, "Invalid email format. Enter a valid email or 'B' to go back.");
            if (email == "B" || email == "b")
            {
                cout << "Going back to menu...\n";
                return;
            }
            if (!loadGuestEmail(email))
            {
                cout << "Email not found!\n";
                continue;
            }
            string confirm = editGuestInfo(bookingsList, guest);
            if (confirm == "n")
            {
                continue;
            }

            break;
        }
    }

    string editGuestInfo(vector<Booking> &bookingsList, Guest guest)
    {
        string firstName, lastName, fullName, contactNumber, password;
        cout << string(52, '-') << " Edit Account Details " << string(52, '-') << "\n";
        cout << "Name: " << loggedInGuest.getFullName() << "\n"
             << "Phone: " << loggedInGuest.getContactNumber() << "\n"
             << "Email: " << loggedInGuest.getEmail() << "\n"
             << "Password: " << loggedInGuest.getPassword() << "\n"
             << string(50, '-') << "\n";

        if (guest.getEmail() == "admin@gmail.com")
        {
            string confirm = InputValidator::get().getValidatedInput("Is this the correct guest? [Y/N]: ", "^[YyNn]$", "Please enter [Y/N].");

            if (confirm == "N" || confirm == "n")
            {
                cout << "Let's try again.\n";
                return "n";
            }
        }

        cout << "\nEditing Info...\nNote: Leave the field blank and press ENTER to keep its current value.\n"
             << string(57, '-') << "\n";
        firstName = InputValidator::get().getValidatedInput("Enter first name ('B' to go back): ", "^$|^([A-Z][a-z]{1,49})( [A-Z][a-z]{1,49})?$|[bB]", "Invalid first name format. Numbers, lowercase starts, and special characters are not allowed. Enter 'B' to go back.");
        if (firstName == "B" || firstName == "b")
        {
            cout << "Going back to menu...\n";
            return "";
        }
        if (firstName.empty())
        {
            firstName = loggedInGuest.getFirstName();
        }
        lastName = InputValidator::get().getValidatedInput("Enter last name: ", "^$|^[A-Z][a-z]{1,49}(['-][A-Z][a-z]{1,49})?( (Jr\\.|Sr\\.|II|III|IV|V))?$", "Invalid first name format. Numbers, lowercase starts, and special characters are not allowed.");
        if (lastName.empty())
        {
            lastName = loggedInGuest.getLastName();
        }
        fullName = firstName + " " + lastName;
        for (;;)
        {
            contactNumber = InputValidator::get().getValidatedInput("Enter contact number: ", "(^$|^(09\\d{9}|\\+639\\d{9})$|^[bB]$)", "Contact number must start with '09' or '+639' and be 11 digits long.");
            if (contactNumber == "B" | contactNumber == "b")
            {
                cout << "Going back to menu...\n";
                return "";
            }
            if (contactNumberExistsInFile(contactNumber))
            {
                cout << "That contact number is already registered. Try another or enter 'B' to go back.\n";
                continue;
            }
            break;
        }
        if (contactNumber.empty())
        {
            contactNumber = loggedInGuest.getContactNumber();
        }
        password = InputValidator::get().getValidatedInput("Enter password: ", "^$|^[a-zA-Z0-9@#$%^&+=]{6,}$", "Password must be at least 6 characters long and can include letters, numbers, and special characters.");
        if (password.empty())
        {
            password = loggedInGuest.getPassword();
        }
        cout << string(50, '-') << "\n";
        cout << "\nName: " << fullName << "\n"
             << "Phone: " << contactNumber << "\n"
             << "Email: " << loggedInGuest.getEmail() << "\n"
             << "Password: " << password << "\n";
        cout << string(50, '-') << "\n";
        string choice = InputValidator::get().getValidatedInput("Confirm these changes [Y/N]? ", "^[YyNn]$", "Please enter [Y/N].");
        if (choice == "N" || choice == "n")
        {
            cout << "Changes discarded.\n";
            return "";
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
        cout << "Information has been updated successfully!\n";
        return "";
    }

    void deleteGuest()
    {
        string email;
        cout << string(56, '-') << " Remove Users " << string(56, '-') << "\n";
        if (noUsersInFile())
        {
            cout << "No users found.\n";
            return;
        }
        for (;;)
        {
            email = InputValidator::get().getValidatedInput("Enter email ('B' to go back): ", EMAIL_B_REGEX, "Invalid email format. Enter a valid email or 'B' to go back.");
            if (email == "B" || email == "b")
            {
                cout << "Going back to menu...\n";
                return;
            }
            if (!loadGuestEmail(email))
            {
                cout << "Email not found!\n";
                continue;
            }

            cout << "\nName: " << loggedInGuest.getFullName() << "\n"
                 << "Phone: " << loggedInGuest.getContactNumber() << "\n"
                 << "Email: " << loggedInGuest.getEmail() << "\n"
                 << string(50, '-') << "\n";
            string confirmation = InputValidator::get().getValidatedInput("Are you sure you want to remove this user [Y/N]? ", "^[YyNn]$", "Please enter [Y/N].");
            if (confirmation == "N" || confirmation == "n")
            {
                cout << "Let's try again." << endl;
                continue;
            }
            break;
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
        cout << string(57, '-') << " View Users " << string(57, '-') << "\n";
        if (noUsersInFile())
        {
            cout << "No users found.\n";
            return;
        }
        cout << string(97, '-') << "\n";
        cout << left
             << setw(35) << "Email" << setw(40) << "Full name" << setw(22) << "Contact number" << "\n";
        cout << string(97, '-') << "\n";

        fstream file;
        string line;
        file.open(USER_FILE, ios::in);
        if (file.is_open())
        {
            string adminLine;
            getline(file, adminLine);
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
                    cout << left << setw(35) << tokens[0] << setw(40) << tokens[4] << setw(22) << tokens[5] << "\n";
                }
            }
            file.close();
            cout << string(97, '-') << "\n\n";
        } // else throw (File can't be open)
        cout << "Press ENTER to go back to the menu..." << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    Guest signingPage()
    {
        int choice;
        Guest loggedInGuest;

        do
        {
            cout << string(50, '-') << " Hotel Reservation System " << string(50, '-') << "\n";
            cout << "1. Sign In\n2. Sign Up\n3. Forgot Password\n4. Exit\n"
                 << string(50, '-') << "\n";
            int choice = stoi(InputValidator::get().getValidatedInput("Enter choice [1 - 4]: ", "^[1-4]$", "Invalid choice. Please enter [1 - 4]."));
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

    void displayRoomDetailsTable() const
    {
        cout << left << setw(15) << roomNumber << setw(20) << roomType << setw(15) << capacity << setw(20) << fixed << setprecision(2) << roomPricePerNight << setw(30) << roomFeatures << endl;
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

    void saveRooms()
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
        string roomNumber, roomType, roomFeatures, choice;
        int capacity;
        double roomPricePerNight;
        bool roomExists = true;

        cout << string(58, '-') << " Add Room " << string(58, '-') << "\n";
        do
        {
            while (roomExists)
            {
                roomNumber = InputValidator::get().getValidatedInput("Enter room number ('B' to go back): ", "^(\\d{3}|[bB])$", "Invalid room number format. Room number must be 3 digits or 'B' to go back.");

                if (roomNumber == "B" || roomNumber == "b")
                {
                    cout << "Going back to menu..." << endl;
                    return;
                }

                roomExists = findRoom(roomNumber) != roomsList.end();
                if (roomExists)
                {
                    cout << "Room number already exists. Please enter a different room number." << endl;
                }
            }
            roomType = InputValidator::get().getValidatedInput("Enter room type: ", "^.+$", "Invalid room type format.");
            capacity = stoi(InputValidator::get().getValidatedInput("Enter room capacity: ", "^\\d+$", "Invalid capacity format. Capacity must be a positive integer."));
            roomPricePerNight = stod(InputValidator::get().getValidatedInput("Enter price per night: ", "^\\d+\\.\\d{2}$", "Invalid price format. Price must be a number with two decimal places."));
            roomFeatures = InputValidator::get().getValidatedInput("Enter room features: ", "^.+$", "Invalid room features format.");

            cout << string(50, '-') << "\n\n"
                 << "Room Number: " << roomNumber << "\n"
                 << "Room Type: " << roomType << "\n"
                 << "Capacity: " << capacity << " person(s)" << "\n"
                 << "Price per Night: $" << fixed << setprecision(2) << roomPricePerNight << "\n"
                 << "Room Features: " << roomFeatures << "\n";
            cout << string(50, '-') << "\n";
            choice = InputValidator::get().getValidatedInput("Confirm these details [Y|N]? ", "^[YyNn]$", "Please enter [Y/N].");
        } while (choice == "N" | choice == "n");
        roomsList.push_back(Room(roomNumber, roomType, capacity, roomFeatures, roomPricePerNight));
        saveRooms();
        cout << "Room added successfully!" << endl;
    }

    void displayRooms()
    {
        cout << string(57, '-') << " View Rooms " << string(57, '-') << "\n";
        if (roomsList.empty())
        {
            cout << "No rooms available." << endl;
            return;
        }
        cout << string(126, '-') << "\n";
        cout << left << setw(15) << "Room Number" << setw(20) << "Room Type" << setw(15) << "Capacity" << setw(20) << "Price per Night" << setw(30) << "Room Features" << endl;
        cout << string(126, '-') << "\n";

        for (const auto &room : roomsList)
        {
            room.displayRoomDetailsTable();
        }
        cout << string(126, '-') << "\n";
        cout << "\nPress ENTER to go back to the menu..." << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    void editRoom()
    {
        string roomNumber, roomType, roomFeatures, choice;
        int capacity;
        double roomPricePerNight;
        auto it = roomsList.end();
        cout << string(57, '-') << " Edit Room " << string(57, '-') << "\n";
        for (;;)
        {
            roomNumber = InputValidator::get().getValidatedInput("Enter room number to update ('B' to go back): ", "^(\\d{3}|[bB])$", "Invalid room number format. Room number must be 3 digits.");

            if (roomNumber == "B" || roomNumber == "b")
            {
                cout << "Going back to menu..." << endl;
                return;
            }

            it = findRoom(roomNumber);

            if (it == roomsList.end())
            {
                cout << "Room not found." << endl;
                continue;
            }

            cout << string(50, '-') << "\n";
            it->displayRoomDetails();
            cout << string(50, '-') << endl;
            string confirm = InputValidator::get().getValidatedInput("Is this the correct Room? [Y/N]: ", "^[YyNn]$", "Please enter [Y/N].");

            if (confirm == "N" || confirm == "n")
            {
                cout << "Let's try again.\n";
                continue;
            }

            break;
        }

        cout << "\nEdit Booking Details\nNote: Leave the field blank and press ENTER to keep its current value.\n";
        cout << string(57, '-') << endl;

        roomType = InputValidator::get().getValidatedInput("Enter new room type: ", "^$|^.+$", "Invalid room type format.");
        if (roomType.empty())
        {
            roomType = it->getRoomType();
        }
        try
        {
            capacity = stoi(InputValidator::get().getValidatedInput("Enter new room capacity: ", "^$|^\\d+$", "Invalid capacity format. Capacity must be a positive integer.")); // Input capacity after room type
        }
        catch (const invalid_argument &e)
        {
            capacity = it->getCapacity(); // If invalid, keep the old capacity
        }
        try
        {
            roomPricePerNight = stod(InputValidator::get().getValidatedInput("Enter new price per night: ", "^$|^\\d+\\.\\d{2}$", "Invalid price format. Price must be a number with two decimal places."));
        }
        catch (const invalid_argument &e)
        {
            roomPricePerNight = it->getRoomPricePerNight(); // If invalid, keep the old price
        }
        roomFeatures = InputValidator::get().getValidatedInput("Enter new room features: ", "^$|^.+$", "Invalid room features format.");
        if (roomFeatures.empty())
        {
            roomFeatures = it->getRoomFeatures();
        }
        cout << string(50, '-') << "\n\n"
             << "Room Number: " << roomNumber << "\n"
             << "Room Type: " << roomType << "\n"
             << "Capacity: " << capacity << " person(s)" << "\n"
             << "Price per Night: $" << fixed << setprecision(2) << roomPricePerNight << "\n"
             << "Room Features: " << roomFeatures << "\n";
        cout << string(50, '-') << "\n";
        choice = InputValidator::get().getValidatedInput("Confirm these details [Y|N]? ", "^[YyNn]$", "Please enter [Y/N].");

        if (choice == "N" || choice == "n")
        {
            cout << "Changes discarded.\n";
            return;
        }

        *it = Room(roomNumber, roomType, capacity, roomFeatures, roomPricePerNight);
        saveRooms();
        cout << string(50, '-') << "\n";
        cout << "Room details updated successfully!" << endl;
    }

    void deleteRoom()
    {
        cout << string(56, '-') << " Remove Room " << string(56, '-') << "\n";
        bool removing = true;
        while (removing)
        {
            string roomNumber = InputValidator::get().getValidatedInput("Enter room number to remove ('B' to go back): ", "^(\\d{3}|[bB])$", "Invalid room number format. Room number must be 3 digits.");

            if (roomNumber == "B" || roomNumber == "b")
            {
                cout << "Going back to menu..." << endl;
                return;
            }

            auto it = findRoom(roomNumber);

            if (it == roomsList.end())
            {
                cout << "Room not found." << endl;
                continue;
            }

            cout << string(50, '-') << "\n";
            it->displayRoomDetails();
            cout << string(50, '-') << endl;
            string confirmation = InputValidator::get().getValidatedInput("Are you sure you want to remove this room [Y/N]? ", "^[YyNn]$", "Please enter [Y/N].");

            if (confirmation == "N" || confirmation == "n")
            {
                cout << "Let's try again." << endl;
                cout << string(50, '-') << "\n";
                continue;
            }

            roomsList.erase(it);
            saveRooms();
            cout << "Room removed successfully." << endl;
            removing = false;
        }
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
        cout << "You selected cash payment." << endl;
        cout << string(50, '-') << endl;
        cout << "Please pay the total amount of $" << fixed << setprecision(2) << amount << " at the hotel front desk.\nupon check-in. Keep your booking ID for reference." << endl;
    }
};

class GcashPayment : public PaymentStrategy
{
public:
    void processPayment(const double &amount) override
    {
        cout << "You selected Gcash payment." << endl;
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
        string cardNumber = InputValidator::get().getValidatedInput("Enter your card number (XXXX-XXXX-XXXX-XXXX): ", "^\\d{4}-\\d{4}-\\d{4}-\\d{4}$", "Invalid card number format. Must be in the format XXXX-XXXX-XXXX-XXXX.");
        string expirationDate = InputValidator::get().getValidatedInput("Enter your card expiration date (MM/YY): ", "^(0[5-9]|1[0-2])/(2[5-9]|[3-9][0-9])$", "Invalid expiration date format. Must be a valid future date and the format MM/YY.");
        string cvv = InputValidator::get().getValidatedInput("Enter your card CVV (XXX): ", "^\\d{3}$", "Invalid CVV format. Must be 3 digits.");
        string cardName = InputValidator::get().getValidatedInput("Enter your card name: ", "^.+$", "Invalid card name format.");

        cout << string(50, '-') << "\n";
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

    void saveBookings()
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
        tm *now_tm = localtime(&now);
        tm today = *now_tm;
        today.tm_hour = 0;
        today.tm_min = 0;
        today.tm_sec = 0;
        time_t today_time = mktime(&today);

        int year, month, day;
        sscanf(date.c_str(), "%d-%d-%d", &year, &month, &day);
        tm inputDate = {};
        inputDate.tm_year = year - 1900;
        inputDate.tm_mon = month - 1;
        inputDate.tm_mday = day;
        time_t inputTime = mktime(&inputDate);

        // Calculate the date 1 year from today
        tm oneYearLater = today;
        oneYearLater.tm_year += 1;
        time_t oneYearLater_time = mktime(&oneYearLater);

        // Date must be today or later, but not more than 1 year in advance
        return difftime(inputTime, today_time) >= 0 && difftime(inputTime, oneYearLater_time) <= 0;
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
    bool isRoomAvailable(const Room &room, const string &checkInDate, const string &checkOutDate, const string &skipBookingId = "")
    {
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

    // Optimized: Get available rooms for a date range, optionally skipping a booking ID
    vector<Room> getAvailableRoomsByDateRange(const string &checkInDate, const string &checkOutDate, const string &skipBookingId = "")
    {
        vector<Room> availableRooms;
        const auto &rooms = RoomManager::get().getRoomsList();
        availableRooms.reserve(rooms.size());
        for (const auto &room : rooms)
        {
            if (isRoomAvailable(room, checkInDate, checkOutDate, skipBookingId))
                availableRooms.push_back(room);
        }
        return availableRooms;
    }

    vector<Room> getAvailableRoomsByNumberOfPeople(int numberOfPeople, vector<Room> &availableRooms)
    {
        vector<Room> filteredRooms;
        for (const auto &room : availableRooms)
        {
            if (room.getCapacity() >= numberOfPeople)
                filteredRooms.push_back(room);
        }
        return filteredRooms;
    }

public:
    BookingManager()
    {
        loadBookingIdCounter();
        loadBookings();
    }

    auto &getBookingsList()
    {
        return bookingsList;
    }

    void addBookingAdmin()
    {
        cout << string(56, '-') << " Add Booking " << string(56, '-') << "\n";

        bool findingGuest = true;
        while (findingGuest)
        {
            string guestEmail = InputValidator::get().getValidatedInput("Enter guest email ('B' to go back): ", EMAIL_B_REGEX, "Invalid email format. Enter a valid email or 'B' to go back.");

            if (guestEmail == "B" || guestEmail == "b")
            {
                cout << "Going back to menu..." << endl;
                return;
            }

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
                        cout << string(50, '-') << "\n\n";
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
                cout << string(50, '-') << "\n";
                string confirm = InputValidator::get().getValidatedInput("Is this the correct guest? [Y/N]: ", "^[YyNn]$", "Please enter [Y/N].");
                if (confirm == "Y" || confirm == "y")
                {
                    addBookingGuest(foundGuest);
                    findingGuest = false;
                }
                else
                {
                    cout << "Let's try again.\n"
                         << string(50, '-') << "\n";
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
        cout << string(55, '-') << " Make a Booking " << string(55, '-') << "\n";
        string checkInDate, checkOutDate;
        int numberOfPeople;
        string numberOfPeopleStr;
        vector<Room> availableRoomsByDateRange;
        vector<Room> availableRoomsByDateRangeAndNumberOfPeople;

        bool bookingLoop = true;
        while (bookingLoop)
        {
            // Input and validate dates
            bool validDates = false;
            while (!validDates)
            {
                checkInDate = InputValidator::get().getValidatedInput("Enter check-in date (YYYY-MM-DD or 'B' to go back): ", "^(\\d{4}-\\d{2}-\\d{2}|[bB])$", "Invalid date format. Use YYYY-MM-DD.");
                if (checkInDate == "B" || checkInDate == "b")
                {
                    cout << "Going back to menu..." << endl;
                    return;
                }
                if (isValidDate(checkInDate) && isFutureDate(checkInDate))
                    validDates = true;
                else
                    cout << "Check-in date must be a valid present or future date (up to 1 year from today)." << endl;
            }
            validDates = false;
            while (!validDates)
            {
                checkOutDate = InputValidator::get().getValidatedInput("Enter check-out date (YYYY-MM-DD): ", "^(\\d{4}-\\d{2}-\\d{2})$", "Invalid date format. Use YYYY-MM-DD.");
                if (isValidDate(checkOutDate) && isFutureDate(checkOutDate) && checkOutDate >= checkInDate)
                    validDates = true;
                else
                    cout << "Check-out date must be a valid date on or after the check-in date (up to 1 year from today)." << endl;
            }
            availableRoomsByDateRange = getAvailableRoomsByDateRange(checkInDate, checkOutDate);
            if (availableRoomsByDateRange.empty())
            {
                cout << "No available rooms for the specified date range." << endl;
                continue;
            }
            cout << string(50, '-') << "\n";
            bookingLoop = false;
        }
        cout << "\nAvailable Rooms by DATE RANGE...\n"
             << string(126, '-') << endl;
        cout << left << setw(15) << "Room Number"
             << setw(20) << "Room Type"
             << setw(10) << "Capacity"
             << setw(15) << "Price/Night"
             << setw(20) << "Features" << endl;
        cout << string(126, '-') << endl;
        for (const auto &room : availableRoomsByDateRange)
        {
            cout << left << setw(15) << room.getRoomNumber()
                 << setw(20) << room.getRoomType()
                 << setw(10) << room.getCapacity()
                 << setw(15) << fixed << setprecision(2) << room.getRoomPricePerNight()
                 << setw(20) << room.getRoomFeatures() << endl;
        }
        cout << string(126, '-') << endl;

        bookingLoop = true;
        while (bookingLoop)
        {
            numberOfPeopleStr = InputValidator::get().getValidatedInput("Enter number of people ('B' to go back): ", "^([1-9]\\d*|[bB])$", "Invalid number of people. Must be a positive integer.");
            if (numberOfPeopleStr == "B" || numberOfPeopleStr == "b")
            {
                cout << "Going back to menu..." << endl;
                return;
            }
            numberOfPeople = stoi(numberOfPeopleStr);
            availableRoomsByDateRangeAndNumberOfPeople = getAvailableRoomsByNumberOfPeople(numberOfPeople, availableRoomsByDateRange);
            if (availableRoomsByDateRangeAndNumberOfPeople.empty())
            {
                cout << "No available rooms for the specified number of people." << endl;
                continue;
            }
            cout << string(50, '-') << "\n";
            bookingLoop = false;
        }
        cout << "\nAvailable rooms by DATE RANGE and NUMBER OF PEOPLE....\n"
             << string(126, '-') << endl;
        cout << left << setw(15) << "Room Number"
             << setw(20) << "Room Type"
             << setw(10) << "Capacity"
             << setw(15) << "Price/Night"
             << setw(20) << "Features" << endl;
        cout << string(126, '-') << endl;
        for (const auto &room : availableRoomsByDateRangeAndNumberOfPeople)
        {
            cout << left << setw(15) << room.getRoomNumber()
                 << setw(20) << room.getRoomType()
                 << setw(10) << room.getCapacity()
                 << setw(15) << fixed << setprecision(2) << room.getRoomPricePerNight()
                 << setw(20) << room.getRoomFeatures() << endl;
        }
        cout << string(126, '-') << endl;
        bool selectingRoom = true;
        while (selectingRoom)
        {
            string roomNumber = InputValidator::get().getValidatedInput("Enter the room number to book: ", "^\\d{3}$", "Invalid room number format. Room number must be 3 digits.");
            auto it = find_if(availableRoomsByDateRangeAndNumberOfPeople.begin(), availableRoomsByDateRangeAndNumberOfPeople.end(),
                              [&roomNumber](const Room &room)
                              {
                                  return room.getRoomNumber() == roomNumber;
                              });

            if (it == availableRoomsByDateRangeAndNumberOfPeople.end())
            {
                cout << "Invalid room selection. Room number not found in available rooms." << endl;
                continue;
            }

            Room selectedRoom = *it;
            double totalPrice = selectedRoom.getRoomPricePerNight() * calculateDaysDifference(checkInDate, checkOutDate);
            string bookingID = to_string(bookingIdCounter++);

            cout << "\nChoose payment method:\n";
            cout << "1. Cash\n2. Gcash\n3. Credit/Debit Card\n4. Bank Transfer\n";
            cout << string(50, '-') << "\n";
            int paymentChoice = stoi(InputValidator::get().getValidatedInput("Enter choice [1 - 4]: ", "^[1-4]$", "Invalid choice. Please enter [1 - 4]."));
            cout << "\n";
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
            cout << string(50, '-') << "\n"
                 << "BOOKING DETAILS\n"
                 << "Booking ID: " << bookingID << "\n";
            selectedRoom.displayRoomDetails();
            cout << string(50, '-') << "\n";
            string choice = InputValidator::get().getValidatedInput("Confirm this booking [Y|N]? ", "^[YyNn]$", "Please enter [Y/N].");

            if (choice == "N" || choice == "n")
            {
                cout << "Booking cancelled.\n";
                return;
            }
            bookingsList.push_back(Booking(bookingID, guest.getEmail(), guest.getFullName(), selectedRoom.getRoomNumber(), checkInDate, checkOutDate, totalPrice, paymentMethod));
            saveBookings();
            saveBookingIdCounter();
            cout << "Booking successful!" << endl;
            selectingRoom = false;
        }
    }

    void displayBookingsAdmin()
    {
        cout << string(55, '-') << " View Bookings " << string(55, '-') << "\n";
        if (bookingsList.empty())
        {
            cout << "No bookings found." << endl;
            return;
        }
        cout << string(160, '-') << "\n";
        cout << left << setw(15) << "Booking ID"
             << setw(35) << "Guest Email"
             << setw(40) << "Guest Name"
             << setw(15) << "Room Number"
             << setw(17) << "Check-in Date"
             << setw(18) << "Check-out Date"
             << setw(15) << "Total Price"
             << setw(20) << "Payment Method" << endl;
        cout << string(160, '-') << "\n";

        for (const auto &booking : bookingsList)
        {
            booking.displayBookingDetailsAdminTable();
        }
        cout << string(160, '-') << "\n";

        cout << "\nPress ENTER to go back to the menu..." << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    void displayBookingsGuest(const Guest &guest)
    {
        cout << string(54, '-') << " View My Bookings " << string(54, '-') << "\n";
        bool found = false;
        for (const auto &booking : bookingsList)
        {
            if (booking.getGuestEmail() == guest.getEmail())
            {
                found = true;
            }
        }
        if (!found)
        {
            cout << "You have no bookings.\n";
            return;
        }
        cout << string(100, '-') << "\n";
        cout << left << setw(15) << "Booking ID"
             << setw(15) << "Room Number"
             << setw(17) << "Check-in Date"
             << setw(18) << "Check-out Date"
             << setw(15) << "Total Price"
             << setw(20) << "Payment Method" << endl;
        cout << string(100, '-') << "\n";
        for (const auto &booking : bookingsList)
        {
            if (booking.getGuestEmail() == guest.getEmail())
            {
                booking.displayBookingDetailsGuestTable();
            }
        }
        cout << string(100, '-') << "\n";

        cout << "\nPress ENTER to go back to the menu..." << endl;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }

    void editBooking()
    {
        cout << string(56, '-') << " Edit Booking " << string(56, '-') << "\n";
        bool editing = true;
        while (editing)
        {
            string bookingID = InputValidator::get().getValidatedInput("Enter booking ID to edit ('B' to go back): ", "^(\\d+|[bB])$", "Invalid booking ID format. Booking ID must be a number.");

            if (bookingID == "B" || bookingID == "b")
            {
                cout << "Going back to menu..." << endl;
                return;
            }

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

            cout << string(55, '-') << "\n";
            booking_it->displayBookingDetailsAdmin();
            cout << string(65, '-') << endl;
            string confirm = InputValidator::get().getValidatedInput("Is this the correct booking? [Y/N]: ", "^[YyNn]$", "Please enter [Y/N].");

            if (confirm == "N" || confirm == "n")
            {
                cout << "Let's try again.\n";
                continue;
            }

            vector<Room> availableRoomsByDateRange;
            vector<Room> availableRoomsByDateRangeAndNumberOfPeople;
            string checkInDate, checkOutDate;
            int numberOfPeople;
            string numberOfPeopleStr;

            cout << "\nEdit Booking Details\nNote: Leave the field blank and press ENTER to keep its current value.\n";

            bool validEdit = true;
            while (validEdit)
            {
                cout << string(50, '-') << "\n";
                bool validDates = false;
                while (!validDates)
                {
                    checkInDate = InputValidator::get().getValidatedInput("Enter new check-in date (YYYY-MM-DD): ", "^$|^(\\d{4}-\\d{2}-\\d{2})$", "Invalid date format. Use YYYY-MM-DD or leave blank to keep current.");
                    if (checkInDate.empty())
                    {
                        checkInDate = booking_it->getCheckInDate();
                        validDates = true;
                    }
                    else if (isValidDate(checkInDate) && isFutureDate(checkInDate))
                    {
                        validDates = true;
                    }
                    else
                    {
                        cout << "Check-in date must be a valid present or future date (up to 1 year from today)." << endl;
                    }
                }
                validDates = false;
                while (!validDates)
                {
                    checkOutDate = InputValidator::get().getValidatedInput("Enter new check-out date (YYYY-MM-DD): ", "^$|^(\\d{4}-\\d{2}-\\d{2})$", "Invalid date format. Use YYYY-MM-DD.");
                    if (checkOutDate.empty())
                    {
                        checkOutDate = booking_it->getCheckOutDate();
                        validDates = true;
                    }
                    else if (isValidDate(checkOutDate) && isFutureDate(checkOutDate) && checkOutDate > checkInDate)
                    {
                        validDates = true;
                    }
                    else
                    {
                        cout << "Check-out date must be a valid date on or after the check-in date (up to 1 year from today)." << endl;
                    }
                }
                availableRoomsByDateRange = getAvailableRoomsByDateRange(checkInDate, checkOutDate, booking_it->getBookingID());
                if (availableRoomsByDateRange.empty())
                {
                    cout << "No available rooms for the specified date range." << endl;
                    continue;
                }
                cout << string(50, '-') << "\n";
                validEdit = false;
            }
            cout << "\nAvailable Rooms by DATE RANGE...\n"
                 << string(126, '-') << endl;
            cout << left << setw(15) << "Room Number"
                 << setw(20) << "Room Type"
                 << setw(10) << "Capacity"
                 << setw(15) << "Price/Night"
                 << setw(20) << "Features" << endl;
            cout << string(126, '-') << endl;
            for (const auto &room : availableRoomsByDateRange)
            {
                cout << left << setw(15) << room.getRoomNumber()
                     << setw(20) << room.getRoomType()
                     << setw(10) << room.getCapacity()
                     << setw(15) << fixed << setprecision(2) << room.getRoomPricePerNight()
                     << setw(20) << room.getRoomFeatures() << endl;
            }
            cout << string(126, '-') << endl;
            validEdit = true;
            while (validEdit)
            {
                numberOfPeopleStr = InputValidator::get().getValidatedInput("Enter new number of people ('B' to go back): ", "^([1-9]\\d*|[bB])$", "Invalid number of people. Must be a positive integer.");
                if (numberOfPeopleStr == "B" || numberOfPeopleStr == "b")
                {
                    cout << "Going back to menu..." << endl;
                    return;
                }
                numberOfPeople = stoi(numberOfPeopleStr);
                availableRoomsByDateRangeAndNumberOfPeople = getAvailableRoomsByNumberOfPeople(numberOfPeople, availableRoomsByDateRange);
                if (availableRoomsByDateRangeAndNumberOfPeople.empty())
                {
                    cout << "No available rooms for the specified number of people." << endl;
                    continue;
                }
                cout << string(50, '-') << "\n";
                validEdit = false;
            }
            cout << "\nAvailable rooms by DATE RANGE and NUMBER OF PEOPLE....\n"
                 << string(126, '-') << endl;
            cout << left << setw(15) << "Room Number"
                 << setw(20) << "Room Type"
                 << setw(10) << "Capacity"
                 << setw(15) << "Price/Night"
                 << setw(20) << "Features" << "\n"
                 << string(126, '-') << "\n";
            for (const auto &room : availableRoomsByDateRangeAndNumberOfPeople)
            {
                cout << left << setw(15) << room.getRoomNumber()
                     << setw(20) << room.getRoomType()
                     << setw(10) << room.getCapacity()
                     << setw(15) << fixed << setprecision(2) << room.getRoomPricePerNight()
                     << setw(20) << room.getRoomFeatures() << endl;
            }
            cout << string(126, '-') << endl;
            bool selectingRoom = true;
            while (selectingRoom)
            {
                string roomNumber = InputValidator::get().getValidatedInput("Enter the new room number to book: ", "^\\d{3}$", "Invalid room number format. Room number must be 3 digits.");
                auto it = find_if(availableRoomsByDateRangeAndNumberOfPeople.begin(), availableRoomsByDateRangeAndNumberOfPeople.end(),
                                  [&roomNumber](const Room &room)
                                  {
                                      return room.getRoomNumber() == roomNumber;
                                  });
                if (it == availableRoomsByDateRangeAndNumberOfPeople.end())
                {
                    cout << "Invalid room selection. Room number not found in available rooms." << endl;
                    continue;
                }
                Room selectedRoom = *it;
                double totalPrice = selectedRoom.getRoomPricePerNight() * calculateDaysDifference(checkInDate, checkOutDate);

                cout << string(50, '-') << "\n\n";
                selectedRoom.displayRoomDetails();
                cout << string(50, '-') << "\n\n";

                string choice = InputValidator::get().getValidatedInput("Confirm these details [Y|N]? ", "^[YyNn]$", "Please enter [Y/N].");

                if (choice == "N" || choice == "n")
                {
                    cout << "Changes discarded.\n";
                    return;
                }

                booking_it->setCheckInDate(checkInDate);
                booking_it->setCheckOutDate(checkOutDate);
                booking_it->setRoomNumber(selectedRoom.getRoomNumber());
                booking_it->setTotalPrice(totalPrice);

                saveBookings();
                cout << "Booking updated successfully!" << endl;
                selectingRoom = false;
            }
            editing = false;
        }
    }

    void
    deleteBooking()
    {
        cout << string(55, '-') << " Remove Booking " << string(55, '-') << "\n";
        bool removing = true;
        while (removing)
        {
            string bookingID = InputValidator::get().getValidatedInput("Enter booking ID to remove ('B' to go back): ", "^(\\d+|[bB])$", "Invalid booking ID format. Booking ID must be a number.");

            if (bookingID == "B" || bookingID == "b")
            {
                cout << "Going back to menu..." << endl;
                return;
            }

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

            cout << string(50, '-') << endl;
            booking_it->displayBookingDetailsAdmin();
            cout << string(50, '-') << endl;
            string confirm = InputValidator::get().getValidatedInput("Are you sure you want to remove this booking? [Y/N]: ", "^[YyNn]$", "Please enter [Y/N].");

            if (confirm == "N" || confirm == "n")
            {
                cout << "Let's try again.\n";
                continue;
            }

            bookingsList.erase(booking_it);
            saveBookings();
            cout << "Booking successfully removed." << endl;
            removing = false;
        }
    }
};

int main()
{
    GuestManager guestManager;
    BookingManager bookingManager;
    bool menuRunning = true;
    while (menuRunning)
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
                cout << string(57, '-') << " Admin Menu " << string(57, '-') << "\n";
                cout << "1. Manage Users\n2. Manage Rooms\n3. Manage Bookings\n4. Logout\n"
                     << string(50, '-') << "\n";
                choice = stoi(InputValidator::get().getValidatedInput("Enter choice [1 - 4]: ", "^[1-4]$", "Invalid choice. Please enter [1 - 4]."));
                switch (choice)
                {
                case 1:
                    int adminUserChoice;
                    do
                    {
                        cout << string(56, '-') << " Manage Users " << string(57, '-') << "\n";
                        cout << "1. Add Users\n2. View Users\n3. Edit Users\n4. Remove Users\n5. Back to Admin Menu\n"
                             << string(50, '-') << endl;
                        adminUserChoice = stoi(InputValidator::get().getValidatedInput("Enter choice [1 - 5]: ", "^[1-5]$", "Invalid choice. Please enter [1 - 5]."));
                        switch (adminUserChoice)
                        {
                        case 1:
                            guestManager.adminAddGuest();
                            break;
                        case 2:
                            guestManager.viewAllUsers();
                            break;
                        case 3:
                            guestManager.adminEditGuestInfo(bookingManager.getBookingsList(), loggedInGuest);
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
                        cout << string(56, '-') << " Manage Rooms " << string(56, '-') << "\n";
                        cout << "1. Add Room\n2. View Rooms\n3. Edit Room\n4. Remove Room\n5. Back to Admin Menu\n"
                             << string(50, '-') << "\n";
                        adminRoomChoice = stoi(InputValidator::get().getValidatedInput("Enter choice [1 - 5]: ", "^[1-5]$", "Invalid choice. Please enter [1 - 5]."));
                        switch (adminRoomChoice)
                        {
                        case 1:
                            RoomManager::get().addRoom();
                            break;
                        case 2:
                            RoomManager::get().displayRooms();
                            break;
                        case 3:
                            RoomManager::get().editRoom();
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
                        cout << string(54, '-') << " Manage Bookings " << string(54, '-') << "\n";
                        cout << "1. Add Booking\n2. View Bookings\n3. Edit Booking\n4. Remove Booking\n5. Back to Admin Menu\n"
                             << string(50, '-') << "\n";
                        adminBookingChoice = stoi(InputValidator::get().getValidatedInput("Enter choice [1 - 5]: ", "^[1-5]$", "Invalid choice. Please enter [1 - 5]."));
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
                            bookingManager.deleteBooking();
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
        else
        {
            int choice;
            do
            {
                cout << string(57, '-') << " Guest Menu " << string(57, '-') << "\n";
                cout << "1. Make a Booking\n2. View My Bookings\n3. Edit Account Details\n4. Logout\n"
                     << string(50, '-') << endl;
                choice = stoi(InputValidator::get().getValidatedInput("Enter choice [1 - 4]: ", "^[1-4]$", "Invalid choice. Please enter [1 - 4]."));
                switch (choice)
                {
                case 1:
                    bookingManager.addBookingGuest(loggedInGuest);
                    break;
                case 2:
                    bookingManager.displayBookingsGuest(loggedInGuest);
                    break;
                case 3:
                    guestManager.editGuestInfo(bookingManager.getBookingsList(), loggedInGuest);
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