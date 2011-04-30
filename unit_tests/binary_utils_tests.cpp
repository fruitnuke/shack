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


BOOST_AUTO_TEST_SUITE(binary_functions)

BOOST_AUTO_TEST_CASE(rotate_left)
{
    BOOST_CHECK(bits::rotl32(0x11111111, 1) == 0x22222222);
    BOOST_CHECK(bits::rotl32(0x11111111, 2) == 0x44444444);
    BOOST_CHECK(bits::rotl32(0x11111111, 3) == 0x88888888);
    BOOST_CHECK(bits::rotl32(0x11111111, 4) == 0x11111111);
    BOOST_CHECK(bits::rotl32(0x00000001, 32) == 0x00000001);
    BOOST_CHECK(bits::rotl32(0x00000001, 31) == 0x80000000);
    BOOST_CHECK(bits::rotl32(0x00000001, 33) == 0x00000002);
    BOOST_CHECK(bits::rotl32(0x00000001, 0) == 0x00000001);
}

BOOST_AUTO_TEST_CASE(bits_to_uint32)
{
    {
        array<unsigned char, 4> bits {0x00, 0x00, 0x00, 0xFF};
        BOOST_CHECK(bits::to_uint32(bits) == 0xFF);
    }
    {
        array<unsigned char, 4> bits {0x00, 0x00, 0xFF, 0x00};
        BOOST_CHECK(bits::to_uint32(bits) == 0xFF00);
    }
    {
        array<unsigned char, 4> bits {0x00, 0xFF, 0x00, 0x00};
        BOOST_CHECK(bits::to_uint32(bits) == 0xFF0000);
    }
    {
        array<unsigned char, 4> bits {0xFF, 0x00, 0x00, 0x00};
        BOOST_CHECK(bits::to_uint32(bits) == 0xFF000000);
    }
    {
        array<unsigned char, 4> bits {0x01, 0x23, 0x45, 0x67};
        BOOST_CHECK(bits::to_uint32(bits) == 19088743);
    }
}

BOOST_AUTO_TEST_CASE(bits_from_uint32)
{
    {
        array<unsigned char, 4> expected {0x01, 0x23, 0x45, 0x67};
        array<unsigned char, 4> bits = bits::from_uint32(19088743);
        BOOST_CHECK(equal(bits.begin(), bits.end(), expected.begin()));
    }
    {
        array<unsigned char, 4> expected {0x00, 0x00, 0x00, 0xFF};
        array<unsigned char, 4> bits = bits::from_uint32(0xFF);
        BOOST_CHECK(equal(bits.begin(), bits.end(), expected.begin()));
    }
    {
        array<unsigned char, 4> expected {0x00, 0x00, 0xFF, 0x00};
        array<unsigned char, 4> bits = bits::from_uint32(0xFF00);
        BOOST_CHECK(equal(bits.begin(), bits.end(), expected.begin()));
    }
    {
        array<unsigned char, 4> expected {0x00, 0xFF, 0x00, 0x00};
        array<unsigned char, 4> bits = bits::from_uint32(0xFF0000);
        BOOST_CHECK(equal(bits.begin(), bits.end(), expected.begin()));
    }
    {
        array<unsigned char, 4> expected {0xFF, 0x00, 0x00, 0x00};
        array<unsigned char, 4> bits = bits::from_uint32(0xFF000000);
        BOOST_CHECK(equal(bits.begin(), bits.end(), expected.begin()));
    }
}

BOOST_AUTO_TEST_SUITE_END()


