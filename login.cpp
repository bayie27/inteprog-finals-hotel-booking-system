#include <iostream>
#include <map>
#include <string>
#include <regex>
#include <memory>
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
    static map<int, shared_ptr<Guest>> guestMap;
    static map<string, int> emailMap;
    static int nextGuestId;

public:
    Guest(int _guestId, string _firstName, string _lastName, string _fullName, string _email, string _password, string _contactNumber) : guestId(_guestId), firstName(_firstName), lastName(_lastName), fullName(_fullName), email(_email), password(_password), contactNumber(_contactNumber) {}
    int getGuestId() const { return guestId; }
    string getFirstName() const { return firstName; }
    string getLastName() const { return lastName; }
    string getFullName() const { return fullName; }
    string getEmail() const { return email; }
    string getPassword() const { return password; }

    static void signUp()
    {
        string email = InputValidator::getValidatedInput("Enter your email: ", "^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,}$", "Invalid email format.");
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

        auto newGuest = make_shared<Guest>(nextGuestId, firstName, lastName, fullName, email, password, contactNumber);
        guestMap[nextGuestId] = newGuest;
        emailMap[email] = nextGuestId;
        cout << "Account created successfully! Welcome, " << guestMap[nextGuestId]->fullName << "!\n";
        nextGuestId++;
    }
    static shared_ptr<Guest> signIn()
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
            cout << "Welcome, " << guestMap[it->second]->fullName << "!\n";
            break;
        }
        return guestMap[it->second];
    }

    static shared_ptr<Guest> signingPage()
    {
        int choice;
        shared_ptr<Guest> loggedInGuest = nullptr;

        do
        {
            cout << string(50, '-') << " Hotel Reservation System " << string(50, '-') << "\n";
            cout << "1. Login\n2. Sign Up\n3. Exit\n";
            string tempChoice = InputValidator::getValidatedInput("Enter your choice: ", "^[1-3]$", "Invalid choice. Please enter [1 - 3].");
            switch (stoi(tempChoice))
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
                return 0;
            default:
                cout << "Invalid choice." << endl;
            }
        } while (!loggedInGuest);
        return loggedInGuest;
    }
};

map<int, shared_ptr<Guest>> Guest::guestMap;
map<string, int> Guest::emailMap;
int Guest::nextGuestId = 1000;

int main()
{
    shared_ptr<Guest> loggedInGuest = Guest::signingPage();
}