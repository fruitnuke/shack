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
}

namespace hex
{
    class ParseError : public std::runtime_error
    {
    public:
    ParseError(const std::string& message) 
        : std::runtime_error(message)
        {
        }
    };
 }

/* Formatting extractor and inserter to parse hex strings into a
 * binary array (in this case vector<char>) with iostreams.  I'm not
 * using the standard hex manipulator as it casts to an integral value
 * and will only parse hex strings with a value under the max value of
 * the integral.
 *
 * Note that >> will throw a hex::ParseError if the input hex string
 * is invalid (contains non-hex characters or is not byte-oriented).
 *
 * Has to be able to deal with potential very large hex strings.
 */
    
namespace std 
{
    ostream& operator<<(ostream& out, const vector<unsigned char>& bits);
    istream& operator>>(istream& in, vector<unsigned char>& bits);
}

#endif
