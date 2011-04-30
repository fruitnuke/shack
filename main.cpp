#include "sha1.h"
#include "binary_utils.h"
#include <iterator>
#include <iostream>
#include <tracedog/tracing.hpp>

using namespace std;

int main(int argc, char** argv)
{
    TRDG_INITIALIZE_LIB();
    TRDG_START_TRACING();

    istream_iterator<vector<unsigned char>> eos;
    istream_iterator<vector<unsigned char>> in (cin);
    try
    {
        for (; in != eos; ++in)
        {
            cout << sha1::hash(*in) << endl;
        }
    }
    catch (const hex::ParseError& e)
    {
        cout << "Parse error: " << e.what() << endl;
    }
    
    return 0;
}
