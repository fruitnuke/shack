#include "sha1.h"
#include "binary_utils.h"
#include <iterator>
#include <iostream>

using namespace std;

int main(int argc, char** argv)
{
    istream_iterator<vector<unsigned char>> eos;
    istream_iterator<vector<unsigned char>> in (cin);
    for (; in != eos; ++in)
    {
        cout << sha1::hash(*in) << endl;
    }
    
    return 0;
}
