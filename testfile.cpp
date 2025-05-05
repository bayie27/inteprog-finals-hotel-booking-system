#include <iostream>
#include <fstream>
#include <string>
using namespace std;

int main()
{
    fstream myFile;

    myFile.open("paulo.txt", ios::app);
    if (myFile.is_open())
    {
        myFile << "Hello everyone!\n";
        myFile.close();
    }
    myFile.open("paulo.txt", ios::out); // write
    if (myFile.is_open())
    {
        myFile << "Hello guys!\n";
        myFile << "Hello world\n";
        myFile.close();
    }

    myFile.open("paulo.txt", ios::in); // read
    if (myFile.is_open())
    {
        string line;
        while (getline(myFile, line))
        {
            cout << line << "\n";
        }
    }
    myFile.open("paulo.txt");
    myFile << "Testing testing.";
    return 0;
}