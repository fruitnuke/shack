#include "binary_utils.h"
#include <algorithm>
#include <iostream>
#include <cassert>
#include <iomanip>
// #include <tracedog/tracing.hpp>

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

uint32_t bits::rotl32(uint32_t word, unsigned short places)
{
    assert(places >= 0);
    places %= 32 ;
    unsigned short word_size = sizeof(uint32_t) * 8;
    assert(places < word_size);
    return (word << places) | (word >> (word_size - places));
}

uint32_t bits::to_uint32(const array<unsigned char, 4>& bits)
{
    return bits[0] << 24 
        | bits[1] << 16 
        | bits[2] << 8 
        | bits[3];
}

array<unsigned char, 4> bits::from_uint32(uint32_t x)
{
    array<unsigned char, 4> bits = {
        static_cast<unsigned char>(x >> 24), 
        static_cast<unsigned char>(x >> 16 & 0xFF), 
        static_cast<unsigned char>(x >> 8 & 0xFF),
        static_cast<unsigned char>(x & 0xFF)}; 
    return bits;
}

class SaveFormat
{
public:
    SaveFormat(ios& stream) : 
        _stream(stream), _format(NULL) 
    {
        _format.copyfmt(stream);
    }
    ~SaveFormat() { _stream.copyfmt(_format); }
private:
    ios _format;
    ios& _stream;
};

void bits::print_bytes(ostream& out, const vector<unsigned char>& bits)
{
    SaveFormat format(out);
    out << std::hex << setfill('0') << right;
    for(auto i = bits.begin(); i != bits.end(); ++i)
        out << setw(2) << static_cast<unsigned short>(*i);
    out << endl;
}

void bits::print_bytes(ostream&, const std::array<unsigned char, 4>&)
{
}

void bits::print_bytes(ostream&, uint32_t)
{
}
