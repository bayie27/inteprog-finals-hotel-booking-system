#include <iostream>
#include <string>
#include <regex>
#include <fstream>
#include <vector>
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
public:
    static bool isValidInput(const string &input, const regex &compiledPattern)
    {
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

int main()
{
    GuestManager guestManager;
    Guest loggedInGuest = guestManager.signingPage();
    guestManager.editGuestInfo();
    guestManager.viewAllUsers();

    return 0;
}