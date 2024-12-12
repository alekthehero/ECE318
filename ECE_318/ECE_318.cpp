#include <iostream>
#include <sstream>

using namespace std;
#pragma region SSTREAM
string demand(istringstream &stringStream, bool &success)
{
    string s;
    stringStream >> s;
    success = !stringStream.fail();
    return s;
}
bool done(istringstream &stringStream)
{
    return stringStream.eof();
}

void sstream()
{
    std::string s;
    getline(std::cin, s);
    std::istringstream stringStream(s);
    std::string cmd;
    stringStream >> cmd; // Extract the first word from the string
    if (cmd == "print")
    {
        bool ok;
        string a = demand(stringStream, ok);
    }
};
#pragma endregion

#pragma region Linear Probing
//Hash table with linear probing
#pragma endregion

#pragma region Binary Files
#include <iostream>
#include <fstream>
#include <string>

struct Record {
    int birthDate;
    char firstName[20];
    char lastName[20];
    int ssn;
    char creditCard[16];

    // Helper function to compare two records
    bool isLessThan(const Record &other) const {
        std::string thisLastName(lastName, 20);
        std::string otherLastName(other.lastName, 20);
        std::string thisFirstName(firstName, 20);
        std::string otherFirstName(other.firstName, 20);

        if (thisLastName == otherLastName) {
            return thisFirstName < otherFirstName;
        }
        return thisLastName < otherLastName;
    }
};

// Read a record from a specific position in the file
bool readRecordAt(std::fstream &file, Record &record, std::streampos position) {
    file.seekg(position);
    return file.read(reinterpret_cast<char*>(&record), sizeof(Record)).get();
}

// Write a record to a specific position in the file
void writeRecordAt(std::fstream &file, const Record &record, std::streampos position) {
    file.seekp(position);
    file.write(reinterpret_cast<const char*>(&record), sizeof(Record));
}

// Perform on-disk bubble sort
void bubbleSortOnDisk(const std::string &fileName) {
    std::fstream file(fileName, std::ios::binary | std::ios::in | std::ios::out);
    if (!file) {
        std::cerr << "Error: Cannot open file.\n";
        exit(1);
    }

    // Determine the number of records in the file
    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    int numRecords = fileSize / sizeof(Record);

    Record record1, record2;

    // Bubble sort logic
    for (int i = 0; i < numRecords - 1; ++i) {
        for (int j = 0; j < numRecords - i - 1; ++j) {
            std::streampos pos1 = j * sizeof(Record);
            std::streampos pos2 = (j + 1) * sizeof(Record);

            // Read two adjacent records
            if (!readRecordAt(file, record1, pos1) || !readRecordAt(file, record2, pos2)) {
                std::cerr << "Error: Failed to read records.\n";
                file.close();
                return;
            }

            // Compare and swap if necessary
            if (!record1.isLessThan(record2)) {
                writeRecordAt(file, record2, pos1);
                writeRecordAt(file, record1, pos2);
            }
        }
    }

    file.close();
}

int binary() {
    const std::string fileName = "StolenNumbers.dat";

    // Perform on-disk bubble sort
    bubbleSortOnDisk(fileName);

    std::cout << "Sorting completed. File sorted directly on disk.\n";
    return 0;
}
#pragma endregion

int main(int argc, char* argv[])
{
    // SSTREAM
    //sstream();

    // BINARY
    binary();
    return 0;
}
