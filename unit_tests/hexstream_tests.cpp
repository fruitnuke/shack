#include <boost/test/unit_test.hpp>
#include <iostream>
#include <vector>
#include "../hexstream.h"

using namespace std;


BOOST_AUTO_TEST_SUITE(hex_string_extractor)

BOOST_AUTO_TEST_CASE(extract_hexstring)
{
    istringstream in("0123456789abcdef\n");
     vector<unsigned char> expected {
         0x01, 0x23, 0x45, 0x67, 
         0x89, 0xab, 0xcd, 0xef}; 

    hexstream hexs(in.rdbuf());
    vector<unsigned char> bits;

    hexs >> bits;
    BOOST_CHECK(hexs.good());
    BOOST_CHECK(bits.size() == 8);
    BOOST_CHECK(equal(bits.begin(), bits.end(), expected.begin()));
    
    hexs >> bits;
    BOOST_CHECK(hexs.fail() && hexs.eof());
}

BOOST_AUTO_TEST_CASE(invalid_character)
{
    istringstream in("azyx\n");
    hexstream hexs(in.rdbuf());
    vector<unsigned char> bits;

    hexs >> bits;
    BOOST_CHECK(hexs.fail());
    BOOST_CHECK(bits.size() == 0);

    hexs.clear();
    hexs >> bits;
    BOOST_CHECK(hexs.fail() && hexs.eof());
}

BOOST_AUTO_TEST_CASE(not_byte_oriented)
{
    istringstream in("abc\n");
    hexstream hexs(in.rdbuf());
    vector<unsigned char> bits;

    hexs >> bits;
    BOOST_CHECK(hexs.fail());
    BOOST_CHECK(bits.size() == 0);

    hexs.clear();
    hexs >> bits;
    BOOST_CHECK(hexs.fail() && hexs.eof());
}

BOOST_AUTO_TEST_CASE(handle_upper_and_lower_case)
{
    istringstream in("abcdefABCDEF\n");
    hexstream hexs(in.rdbuf());
    
    vector<unsigned char> expected {
        0xab, 0xcd, 0xef, 0xab, 0xcd, 0xef};
    vector<unsigned char> bits;

    hexs >> bits;
    BOOST_CHECK(bits.size() == 6);
    BOOST_CHECK(equal(bits.begin(), bits.end(), expected.begin()));

    hexs.clear();
    hexs >> bits;
    BOOST_CHECK(hexs.fail() && hexs.eof());
}

BOOST_AUTO_TEST_CASE(empty_string)
{
    istringstream in("\n");
    hexstream hexs(in.rdbuf());
    vector<unsigned char> bits;
    
    hexs >> bits;
    BOOST_CHECK(bits.size() == 0);
    BOOST_CHECK(hexs.good());
}

BOOST_AUTO_TEST_CASE(empty_stream)
{
    istringstream in("");
    hexstream hexs(in.rdbuf());
    vector<unsigned char> bits;
    
    hexs >> bits;
    BOOST_CHECK(bits.size() == 0);
    BOOST_CHECK(hexs.fail() && hexs.eof());
}

BOOST_AUTO_TEST_CASE(string_of_zeros)
{
    istringstream in("00\n");
    hexstream hexs(in.rdbuf());
    vector<unsigned char> bits;
    
    hexs >> bits;
    BOOST_CHECK(bits.size() == 1);
    BOOST_CHECK(hexs.good());
    BOOST_CHECK(bits[0] == 0);
}

BOOST_AUTO_TEST_CASE(delimit_by_end_of_stream)
{
    istringstream in("ab");
    hexstream hexs(in.rdbuf());
    vector<unsigned char> bits;
    
    hexs >> bits;
    BOOST_CHECK(bits.size() == 0);
    BOOST_CHECK(hexs.fail() && hexs.eof());
}

BOOST_AUTO_TEST_SUITE_END()
