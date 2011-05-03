#ifndef __shack_binary_utils_h__
#define __shack_binary_utils_h__

#include <iosfwd>
#include <vector>
#include <stdexcept>
#include <array>

namespace bits
{
    uint32_t rotl32(uint32_t, unsigned short places);
    uint32_t to_uint32(const std::array<unsigned char, 4>&);
    std::array<unsigned char, 4> from_uint32(uint32_t);

    void print_bytes(std::ostream&, const std::vector<unsigned char>&);
    void print_bytes(std::ostream&, const std::array<unsigned char, 4>&);
    void print_bytes(std::ostream&, uint32_t);

    /* class ParseError : public std::runtime_error */
    /* { */
    /* public: */
    /* ParseError(const std::string& message)  */
    /*     : std::runtime_error(message) */
    /*     { */
    /*     } */
    /* }; */

    /* void test_throw() { throw bits::ParseError("test throw"); } */
 }

namespace std
{
    ostream& operator<<(ostream& out, const vector<unsigned char>& bits);
/*     istream& operator>>(istream& in, vector<unsigned char>& bits); */
}

#endif
