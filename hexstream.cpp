#include "hexstream.h"
#include <cassert>
#include <cstring>
#include <array>
#include <boost/io/ios_state.hpp>

using namespace std;


bool hexstream::is_valid(char c)
{
    return (c >= '0' && c <= '9')
        || (c >= 'a' && c <= 'f') 
        || (c >= 'A' && c <= 'F');
}

unsigned char hexstream::ch_to_bin(char c)
{
    assert(is_valid(c));
    if (c >= '0' && c <= '9') 
        return c - '0';
    else if (c >= 'a' && c <= 'f')
        return c - 'a' + 0xA;
    else if (c >= 'A' && c <= 'F')
        return c - 'A' + 0xA;
}

bool hexstream::end_of_string()
{
    return peek() == '\n';
}

void hexstream::chomp_end()
{
    // Regardless of the state of the stream, we need it to be good in
    // order to chomp any remaining hex string and delimiter.
    boost::io::ios_iostate_saver error_state(*this);
    clear(); 
    
    while (!end_of_string())
        ignore();
    ignore();
}

bool hexstream::hex_chars_for_next_byte(char& high, char& low)
{
    array<char, 3> buf;

    if (get(buf.data(), buf.size()) 
        && strlen(buf.data()) == 2
        && is_valid(buf[0])
        && is_valid(buf[1]))
    {
        high = buf[0];
        low = buf[1];
        return true;
    }
    
    clear(rdstate() | ios_base::failbit);
    return false;
}

istream& hexstream::operator>>(vector<unsigned char>& bits)
{
    vector<unsigned char> buf;
    char high = 0;
    char low = 0;

    while (good() 
           && !end_of_string() 
           && hex_chars_for_next_byte(high, low))
    {
        buf.push_back(ch_to_bin(high) << 4 | ch_to_bin(low));
    }

    if (!eof())
        chomp_end();

    if (good())
        swap(bits, buf);
    
    return *this;
}
