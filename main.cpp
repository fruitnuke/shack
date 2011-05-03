#include "sha1.h"
#include "binary_utils.h"
#include "hexstream.h"
#include <iterator>
#include <iostream>
#include <tracedog/tracing.hpp>

using namespace std;

int main(int argc, char** argv)
{
    // Improve performance of stdin / stdout by not using c stdio
    // library.
    ios::sync_with_stdio(false);
    cin.exceptions(ios_base::badbit);   

    // What is this?
    // std::set_terminate (__gnu_cxx::__verbose_terminate_handler);
    
    TRDG_INITIALIZE_LIB();
    TRDG_START_TRACING();

    vector<unsigned char> bits;
    hexstream hexs(cin.rdbuf());
    while (!hexs.eof())
    {
        hexs >> bits;
        if (hexs.good())
        {
            cout << sha1::hash(bits) << endl;
        }
        else if (hexs.fail() && !hexs.eof())
        {
            cout << "Invalid hex string." << endl;
            hexs.clear();
        }
    }
    return 0;
}
