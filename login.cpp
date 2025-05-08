#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <vector>
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

int main()
{
    GuestManager guestManager;
    Guest loggedInGuest = guestManager.signingPage();
    return 0;
}