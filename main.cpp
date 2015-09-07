/*******************
Copyright 2015 Richard Davis
CSCI 3020 Section W1
Fall 2015
Assignment 1
Programmed on Arch Linux (kernel version 4.1.6-1-ARCH) using g++ (v5.2.0)
This program converts a legacy text file to an XML document
*******************/

#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib>  // included for exit codes

// stack overflow has convinced me "using namespace std;" is a bad idea
using std::cout;
using std::cin;

// since I used arrays in my structs, I used constants to initialize them to a large enough size
const int MAX_ACCOUNTS = 500;
const int MAX_COMPANIES = 500;

// defines several nested structures representing relationships within the data
struct Manager {
    std::string emp_id;  // since no calculations are performed, numeric datatype unnecessary
    std::string fname;
    std::string lname;
};

struct Location {
    std::string city;
    std::string state;
};

struct Companies {
    int count;
    std::string names[MAX_COMPANIES];
};

struct Account {
    Manager manager;
    Location location;
    Companies companies;
};

struct Accounts {
    Account account[MAX_ACCOUNTS];
};

// prototypes for the functions
void userInput(std::string &, std::string &);
void fileRead(std::string, Accounts &, int &);
void fileWrite(std::string, Accounts, int);

// where the magic happens
int main() {
    std::string ifile_name, ofile_name;
    Accounts m_accounts;
    int num_accounts;

    cout << "TXT-to-XML Converter\n";
    cout << "Author: Richard Davis\n";
    cout << "Only converts files that comply with legacy formatting rules!\n\n";

    userInput(ifile_name, ofile_name);

    fileRead(ifile_name, m_accounts, num_accounts);

    fileWrite(ofile_name, m_accounts, num_accounts);

    return 0;
}

// prompts user for input, repeats back, appends ".xml" to output file if not specified by user.
void userInput(std::string &in, std::string &out) {
    cout << "Enter the name of the input file =>";
    cin >> in;
    cout << "The input file you have entered is: " << in + "\n\n";

    cout << "Enter the name of the output file =>";
    cin >> out;
    if (out.find(".xml") == std::string::npos) {
        out = out + ".xml";
    }
    cout << "The output file you have entered is: " << out + "\n\n";
}

// opens input file, reads and stores values in nested structs, and then closes file
void fileRead(std::string name, Accounts &l_accounts, int &num_accounts) {
    std::ifstream file;
    int i = 0;
    std::string temp = " ";

    file.open(name.c_str(), std::ios::in);
    if (!file) {
        cout << "Unable to open the input file!\n";
        cout << "Check if the file exists and that you have read permissions.\n";
        exit(1);  // needed cstdlib for this
    } else {
        cout << "Opening file for reading...\n";
        while (!file.eof()) {
            if (file.eof()) {
                break;  //  wasn't breaking out correctly without this for me
            }
            bool end_companies = false;
            getline(file, l_accounts.account[i].manager.emp_id, ' ');
            getline(file, l_accounts.account[i].manager.lname, ' ');
            getline(file, l_accounts.account[i].manager.fname, ' ');
            getline(file, l_accounts.account[i].location.city, ' ');
            getline(file, l_accounts.account[i].location.state);
            int j = 0;
            while (!end_companies) {
                getline(file, temp);
                if (temp.find("--END_MANAGER_DATA--") == std::string::npos) {
                    l_accounts.account[i].companies.names[j] = temp;
                    j++;
                    l_accounts.account[i].companies.count = j;
                } else {
                    end_companies = true;
                }
            }
            i++;
            num_accounts = i;
        }
    }
    file.close();
    cout << "The input file has been successfully parsed.\n\n";
}

// opens/creates output file, uses for loops to traverse structs as it writes xml to file, and then closes file
void fileWrite(std::string name, Accounts l_accounts, int num_accounts) {
    std::ofstream file;
    file.open(name.c_str(), std::ios::out);
    if (!file) {
        cout << "Unable to open the output file!\n";
        cout << "Ensure you have write permissions in your working directory.\n";
        exit(1);  // needed cstdlib to do this
    } else {
        cout << "Opening file for writing...\n";
        file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        file << "<!-- Processed by Richard Davis’s converter -->\n";
        file << "<accounts>\n";
        for (int i = 0; i < num_accounts -1; i++) {
            file << "\t<account>\n";
            file << "\t\t<manager employeeId=\"" << l_accounts.account[i].manager.emp_id << "\">\n";
            file << "\t\t\t<firstName>" << l_accounts.account[i].manager.fname << "</firstName>\n";
            file << "\t\t\t<lastName>" << l_accounts.account[i].manager.lname << "</lastName>\n";
            file << "\t\t</manager>\n";
            file << "\t\t<location>\n";
            file << "\t\t\t<city>" << l_accounts.account[i].location.city << "</city>\n";
            file << "\t\t\t<state>" << l_accounts.account[i].location.state << "</state>\n";
            file << "\t\t</location>\n";
            file << "\t\t<companies count=\"" << l_accounts.account[i].companies.count << "\">\n";
            for (int j = 0; j < l_accounts.account[i].companies.count; j++) {
                file << "\t\t\t<company>" << l_accounts.account[i].companies.names[j] << "</company>\n";
            }
            file << "\t\t</companies>\n";
            file << "\t</account>\n";
        }
        file << "</accounts>";
    }
    file.close();
    cout << "The input file has been converted to an XML document.\n";
}
