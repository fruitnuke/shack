#include <boost/test/unit_test.hpp>
#include <iostream>
#include <vector>
#include "../binary_utils.h"

using namespace std;

BOOST_AUTO_TEST_SUITE(hex_string_extractor)

BOOST_AUTO_TEST_CASE(extract_hexstring)
{
    istringstream in("0123456789abcdef\n");
    vector<unsigned char> expected {
        0x01, 0x23, 0x45, 0x67, 
        0x89, 0xab, 0xcd, 0xef}; 

    vector<unsigned char> bits;
    in >> bits;

    BOOST_CHECK(bits.size() == 8);
    BOOST_CHECK(equal(bits.begin(), bits.end(), expected.begin()));
}

BOOST_AUTO_TEST_CASE(invalid_character)
{
    istringstream in("azyx\n");
    vector<unsigned char> bits;
    BOOST_CHECK_THROW(in >> bits, hex::ParseError);
}

BOOST_AUTO_TEST_CASE(not_byte_oriented)
{
    istringstream in("abc\n");
    vector<unsigned char> bits;
    BOOST_CHECK_THROW(in >> bits, hex::ParseError);
}

BOOST_AUTO_TEST_CASE(handle_upper_and_lower_case)
{
    istringstream in("abcdefABCDEF\n");
    vector<unsigned char> expected {
        0xab, 0xcd, 0xef, 0xab, 0xcd, 0xef};
    
    vector<unsigned char> bits;
    in >> bits;

    BOOST_CHECK(bits.size() == 6);
    BOOST_CHECK(equal(bits.begin(), bits.end(), expected.begin()));
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(hex_string_inserter)

BOOST_AUTO_TEST_CASE(normal_case)
{
    ostringstream out;
    vector<unsigned char> bits {
        0x01, 0x23, 0x45, 0x67, 
        0x89, 0xab, 0xcd, 0xef};

    out << bits;
    BOOST_CHECK_EQUAL(out.str(), "0123456789abcdef");
}

BOOST_AUTO_TEST_CASE(empty_bitstring)
{
    ostringstream out;
    vector<unsigned char> bits;

    out << bits;
    BOOST_CHECK_EQUAL(out.str(), "");
}

BOOST_AUTO_TEST_CASE(not_really_empty_bitstring)
{
    ostringstream out;
    vector<unsigned char> bits {0x00};

    out << bits;
    BOOST_CHECK_EQUAL(out.str(), "00");
}


BOOST_AUTO_TEST_SUITE_END()
