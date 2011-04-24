#include <boost/test/unit_test.hpp>
#include <vector>
#include <array>
#include "../sha1.h"

using namespace std;

BOOST_AUTO_TEST_CASE(empty_message_test)
{
    const vector<unsigned char> message;
    const vector<unsigned char> digest(sha1::hash(message));
    BOOST_CHECK(digest.size() == 160 / 8);
}

BOOST_AUTO_TEST_CASE(normal_message_test)
{
    const array<unsigned char, 20> expected {
        0x05, 0xc9, 0x15, 0xb5, 0xed, 
        0x4e, 0x4c, 0x4a, 0xff, 0xfc, 
        0x20, 0x29, 0x61, 0xf3, 0x17, 
        0x43, 0x71, 0xe9, 0x0b, 0x5c};

    const vector<unsigned char> message {
        0x97, 0x77, 0xcf, 0x90, 0xdd, 
        0x7c, 0x7e, 0x86, 0x35, 0x06};

    const vector<unsigned char> digest(sha1::hash(message));
    BOOST_CHECK(digest.size() == 160 / 8);
    BOOST_CHECK(equal(digest.begin(), digest.end(), expected.begin()));
}
