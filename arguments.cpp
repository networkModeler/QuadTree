////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// arguments.cpp : class for command line arguments
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "arguments.h"
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Constructor
Arguments::Arguments(int argc, char* argv[])
{
    // Check if there were no command line arguments
    if (argc <= 1)
    {
        return;
    }

    // Process each pair of arguments, key and value
    for (int i = 1; i < argc; i += 2)
    {
        this->args[argv[i]] = argv[i + 1];
    }

    if (this->args.size() != (argc - 1) / 2)
    {
        cout << "expected number of command line arguments: " << (argc - 1) / 2 << "\n";
        cout << "received number of command line arguments: " << this->args.size() << "\n";
        cout << endl;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Get an argument with the given key
bool Arguments::get(string key, string& value)
{
    auto iter = this->args.find(key);
    if (iter != this->args.end())
    {
        value = (*iter).second;
        return true;
    }
    else
    {
        value = "key not found";
        return false;
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Print all arguments
void Arguments::print()
{
    cout << "number of command line arguments: " << this->args.size() << "\n";

    for (auto iter = this->args.begin(); iter != this->args.end(); ++iter)
    {
        cout << setw(14) << (*iter).first << ": " << setw(7) << (*iter).second << "\n";
    }
    cout << endl;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
