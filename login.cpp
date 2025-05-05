#include <iostream>
#include <map>
#include <string>
#include <regex>
#include <memory>
#include <fstream>
#include <vector>
using namespace std;

class InputValidator
{
public:
    static bool isValidInput(const string &input, const string &pattern)
    {
        static map<string, regex> regexCache;
        if (input.empty())
        {
            return false;
        }
        if (regexCache.find(pattern) == regexCache.end())
        {
            regexCache[pattern] = regex(pattern);
        }
        return regex_match(input, regexCache[pattern]);
    }
    static string getValidatedInput(const string &prompt, const string &pattern, const string &errorMessage)
    {
        string input;
        while (true)
        {
            cout << prompt;
            getline(cin, input);
            if (isValidInput(input, pattern))
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
    int guestId;
    string firstName, lastName, fullName, email, password, contactNumber;

public:
    Guest(int _guestId, string _firstName, string _lastName, string _fullName, string _email, string _password, string _contactNumber) : guestId(_guestId), firstName(_firstName), lastName(_lastName), fullName(_fullName), email(_email), password(_password), contactNumber(_contactNumber) {}

    int getGuestId() const { return guestId; }
    string getFirstName() const { return firstName; }
    string getLastName() const { return lastName; }
    string getFullName() const { return fullName; }
    string getEmail() const { return email; }
    string getPassword() const { return password; }
    string getContactNumber() const { return contactNumber; }
};

class GuestManager
{
private:
    map<int, shared_ptr<Guest>> guestMap;
    map<string, int> emailMap;
    int nextGuestId = 1;

    const string USER_FILE = "hotel_users.txt";
    // store newly user to records of user (txt file)
    void addGuestToFile(int guestId, const Guest &guest)
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
        } // else throw (File can't be open.)

        if (!lines.empty())
        {
            lines[0] = to_string(nextGuestId);
        }

        // guestid wala pa (need to fix)
        lines.push_back(guestId + "|" + guest.getFirstName() + "|" + guest.getLastName() + "|" + guest.getFullName() + "|" + guest.getEmail() + "|" + guest.getPassword() + "|" + guest.getContactNumber());

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

public:
    GuestManager() : USER_FILE("hotel_users.txt"), nextGuestId(1) {}

    // need forgot password (update password)
    void signUp()
    {
        string email = InputValidator::getValidatedInput("Enter your email: ", "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$", "Invalid email format.");
        // needed to scan first txt file to search if existing user (using txt file to scan as if existing user)
        if (emailMap.find(email) != emailMap.end())
        {
            cout << "That email is taken. Try another.\n";
            return;
        }

        string password = InputValidator::getValidatedInput("Enter your password: ", "^[a-zA-Z0-9@#$%^&+=]{6,}$", "Password must be at least 6 characters long and can include letters, numbers, and special characters.");

        string firstName = InputValidator::getValidatedInput("Enter your first name: ", "^([A-Z][a-z]{1,49})( [A-Z][a-z]{1,49})?$", "First name can only contain letters and spaces.");
        string lastName = InputValidator::getValidatedInput("Enter your last name: ", "^[A-Z][a-z]{1,49}(['-][A-Z][a-z]{1,49})?( (Jr\\.|Sr\\.|II|III|IV|V))?$", "Last name can only be contain letters and spaces.");
        string contactNumber = InputValidator::getValidatedInput("Enter your contact number: ", "^09\\d{9}$", "Contact number must be 10 digits long.");
        string fullName = firstName + " " + lastName;

        // Create the new guest object
        auto newGuest = make_shared<Guest>(nextGuestId, firstName, lastName, fullName, email, password, contactNumber);

        // Store the new guest in the maps
        guestMap[nextGuestId] = newGuest;
        emailMap[email] = nextGuestId;

        // store newly user to records of user (txt file)
        addGuestToFile(nextGuestId, *newGuest);

        cout << "Account created successfully! Welcome, " << guestMap[nextGuestId]->getFullName() << "!\n";
        nextGuestId++;
    }
    shared_ptr<Guest> signIn()
    {
        string email;
        for (;;)
        {
            email = InputValidator::getValidatedInput("Enter your email: ", "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$", "Invalid email format.");
            if (!(emailMap.count(email)))
            {
                cout << "Couldn't find your email. Signup first.\n\n";
                return nullptr;
            }
            break;
        }

        string password;
        const auto &it = emailMap.find(email);
        for (;;)
        {
            password = InputValidator::getValidatedInput("Enter your password: ", "^[a-zA-Z0-9@#$%^&+=]{6,}$", "Password must be at least 6 characters long and can include letters, numbers, and special characters.");
            if (guestMap[it->second]->getPassword() != password)
            {
                cout << "Wrong password. Try again." << endl;
                continue;
            }
            cout << "Welcome, " << guestMap[it->second]->getFullName() << "!\n";
            break;
        }
        return guestMap[it->second];
    }

    shared_ptr<Guest> signingPage()
    {
        int choice;
        shared_ptr<Guest> loggedInGuest = nullptr;

        do
        {
            cout << string(50, '-') << " Hotel Reservation System " << string(50, '-') << "\n";
            cout << "1. Login\n2. Sign Up\n3. Exit\n";
            int choice = stoi(InputValidator::getValidatedInput("Enter your choice: ", "^[1-3]$", "Invalid choice. Please enter [1 - 3]."));
            switch (choice)
            {
            case 1:
                cout << string(50, '-') << " Login Page " << string(50, '-') << "\n";
                loggedInGuest = signIn();
                if (loggedInGuest)
                    return loggedInGuest;
                break;
            case 2:
                cout << string(50, '-') << " Sign Up Page " << string(50, '-') << "\n";
                signUp();
                break;
            case 3:
                cout << "Exiting..." << endl;
                return nullptr;
            default:
                cout << "Invalid choice." << endl;
            }
        } while (!loggedInGuest);
        return loggedInGuest;
    }
};

int main()
{
    GuestManager guestManager;
    shared_ptr<Guest> loggedInGuest = guestManager.signingPage();
    if (!loggedInGuest)
    {
        cout << "No user logged in. Exiting program." << endl;
        return 0;
    }
}