#ifndef __shack_hexstream_h__
#define __shack_hexstream_h__

#include <istream>
#include <vector>

class hexstream : public std::istream
{
public:
    hexstream(std::streambuf* buf) : std::istream(buf) {}
    std::istream& operator>>(std::vector<unsigned char>& bits);

private:
    bool is_valid(char);
    unsigned char ch_to_bin(char);
    bool hex_chars_for_next_byte(char&, char&);
    bool end_of_string();
    void ignore_newline_and_remnant();
};

#endif
