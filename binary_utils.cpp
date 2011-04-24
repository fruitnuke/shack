#include "binary_utils.h"
#include <algorithm>
#include <iostream>
#include <cassert>
#include <tracedog/tracing.hpp>

using namespace std;

const char hex_lit[] = {
    '0', '1', '2', '3', '4', '5', '6', '7', 
    '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

ostream& std::operator<<(ostream& out, const vector<unsigned char>& bits)
{
    for (auto i = bits.begin(); i != bits.end(); ++i)
    {
        char high = (*i & 0xF0) >> 4;
        char low = *i & 0x0F;
        assert(high < 16);
        assert(low < 16);

        out << hex_lit[high] << hex_lit[low];
    }

    return out;
}

istream& std::operator>>(istream& in, vector<unsigned char>& bits)
{
    locale loc;
    char c;
    long len = 0;
    vector<unsigned char> buf;
    char byte = 0;
    char h_val = 0;

    for(in.get(c); !isspace(c, loc); in.get(c))
    {
        if (c >= 48 && c <= 57) 
            h_val = c - 48;
        else if (c >= 65 && c <= 70) 
            h_val = (c - 65) + 0xA;
        else if (c >= 97 && c <= 102)
            h_val = (c - 97) + 0xA;
        else 
        {
            throw hex::ParseError("invalid character in hex string");
        }

        ++len;

        if (len % 2 != 0)
        {
            byte = h_val << 4;
        }
        else
        {
            byte += h_val;
            buf.push_back(byte);
            byte = 0;
        }
    }
    
    if (len % 2 != 0)
    {
        throw hex::ParseError("hex string is not byte-oriented");
    }

    swap(bits, buf);
    return in;
}
