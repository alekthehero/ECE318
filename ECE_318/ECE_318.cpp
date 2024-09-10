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

int main(int argc, char* argv[])
{
    // SSTREAM
    sstream();
    return 0;
}
