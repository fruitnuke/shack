#include <boost/test/unit_test.hpp>
#include <vector>
#include <array>
#include "../sha1.h"
#include "sha1_impl.h"
// #include <tracedog/tracing.hpp>
// #include <iomanip>

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

class Message
{
public:
    static const vector<unsigned char> bits;
    static unsigned int size() {return bits.size();} 
};

const vector<unsigned char> Message::bits {
    0x97, 0x77, 0xcf, 0x90, 0xdd, 
    0x7c, 0x7e, 0x86, 0x35, 0x06};


BOOST_AUTO_TEST_CASE(padded_message_is_512_bits)
{
    vector<unsigned char> msg = Message::bits;
    pad(msg);
    BOOST_CHECK(msg.size() % 64 == 0);
}

BOOST_AUTO_TEST_CASE(padded_message_is_bigger)
{ 
    vector<unsigned char> msg = Message::bits;
    pad(msg);
    BOOST_CHECK(msg.size() >= Message::size() + 8);
}

BOOST_AUTO_TEST_CASE(padded_message_last_64_bits_is_size)
{
    vector<unsigned char> msg = Message::bits;
    array<unsigned char, 8> expected_bits;
    uint64_t size = Message::size() * 8;
    uint64_t byte_mask = 0xFF;
    for (int i = expected_bits.size() - 1; i >= 0; --i)
    {
        expected_bits[i] = size & byte_mask;
        byte_mask = byte_mask << 8;
    }
    
    pad(msg);
    
    BOOST_REQUIRE(msg.size() >= 8);
    BOOST_CHECK(equal(msg.begin() + msg.size() - 8, 
                      msg.end(), 
                      expected_bits.begin()));
}

BOOST_AUTO_TEST_CASE(padded_message_check_padding)
{
    vector<unsigned char> msg = Message::bits;
    
    pad(msg);

    BOOST_REQUIRE(msg.size() > Message::size() + 8);
    auto i = msg.begin() + Message::size();
    BOOST_CHECK(*i = 0xF0);
    while (++i != msg.begin() + (msg.size() - 8))
    {
        BOOST_CHECK(*i == 0x00);
    }
}

BOOST_AUTO_TEST_CASE(make_schedule_normal_case)
{
    array<uint32_t, 16> block = {
        0x36800000, 0x00000000, 0x00000000, 0x00000000, 
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 
        0x00000000, 0x00000000, 0x00000000, 0x00000000, 
        0x00000000, 0x00000000, 0x00000000, 0x00000008};

    array<uint32_t, 80> expected = {
        0x36800000, 0x00000000, 0x00000000, 0x00000000, 
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000000,
        0x00000000, 0x00000000, 0x00000000, 0x00000008,
        0x6d000000, 0x00000000, 0x00000010, 0xda000000,
        0x00000000, 0x00000020, 0xb4000001, 0x00000010,
        0xda000040, 0x68000003, 0x00000000, 0x00000080,
        0xd0000006, 0x00000050, 0xb2000103, 0xa000003d,
        0x6e000001, 0x00000200, 0x4000005b, 0x68000143,
        0xc800040e, 0x80000076, 0x68000003, 0x00000840,
        0x6800006e, 0x00000500, 0x2000103b, 0x000003fa,
        0x54000017, 0x00002040, 0x680005f7, 0xe80014d5,
        0x8c0040e8, 0x00000768, 0x80000176, 0xc800844e,
        0xe80006e5, 0x00005200, 0x400103a9, 0x00003ee0,
        0x8800057b, 0x80020476, 0xe8005f75, 0x8001451e,
        0xa8040ee6, 0x000073c0, 0x48000750, 0x80084776,
        0x08006e4b, 0x00050000, 0x00103f20, 0x8003fa36,
        0x00001754, 0x00204000, 0x0005f768, 0x0014d568,
        0xd040e88a, 0x00076c00, 0x800177b6, 0xa08448c5,
        0xc006e5c5, 0x00520600, 0xc103ad6d, 0x803eefb6};

    array<uint32_t, 80> schedule = make_schedule(block);
    
    BOOST_CHECK(equal(schedule.begin(), 
                      schedule.end(), 
                      expected.begin()));
}

