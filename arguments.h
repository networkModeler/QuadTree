////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// arguments.h : header file for command line arguments class
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef ARGUMENTS_H
#define ARGUMENTS_H

#include <string>
#include <unordered_map>
using namespace std;

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class Arguments
{
    public:

        // Constructor
        Arguments(int argc, char* argv[]);

        // Get an argument with the given key
        bool Arguments::get(string key, string& value);

        // Print all arguments
        void print();

    private:

        unordered_map<string, string> args;
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif  ARGUMENTS_H