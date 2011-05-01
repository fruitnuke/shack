#include "sha1.h"
#include "binary_utils.h"
#include <array>
#include <cstdint>
#include <cassert>
#include <algorithm>

// #include <tracedog/tracing.hpp>
#include <iostream>
#include <iomanip>

using namespace std;


const unsigned int BLOCK_SIZE = 16 * sizeof(uint32_t);

const array<uint32_t, 5> SEED = {
    0x67452301,
    0xefcdab89,
    0x98badcfe,
    0x10325476,
    0xc3d2e1f0};

const array<uint32_t, 4> HASH_CONSTANTS = {
    0x5a827999,
    0x6ed9eba1,
    0x8f1bbcdc,
    0xca62c1d6};

typedef uint32_t (*function_p) (uint32_t, uint32_t, uint32_t);


void hash_block(const array<uint32_t, 16>&, array<uint32_t, 5>&);
void pad(vector<unsigned char>&);
array<uint32_t, 80> make_schedule(const array<uint32_t, 16>&);
uint32_t ch(uint32_t, uint32_t, uint32_t);
uint32_t maj(uint32_t, uint32_t, uint32_t);
uint32_t parity(uint32_t, uint32_t, uint32_t);
function_p hash_function(int);
uint32_t hash_constant(int);


vector<unsigned char> 
sha1::hash(const vector<unsigned char>& message)
{
    vector<unsigned char> padded(message);
    pad(padded);
    
    array<uint32_t, 5> hash = SEED;
        
    for (auto block_it = padded.begin(); 
         block_it < padded.end(); 
         advance(block_it, BLOCK_SIZE))
    {
        // cout << "block  " << std::hex << setfill('0');
        // for(auto i = block_it; i < block_it + BLOCK_SIZE; ++i)
        //     cout << setw(2) << static_cast<unsigned short>(*i);
        // cout << dec << endl;

        array<uint32_t, 16> block;
        array<unsigned char, 4> word;

        for (int i = 0; i < 16; ++i)
        {
            copy_n(block_it + (i*4), 4, word.begin()); 
            block[i] = bits::to_uint32(word);
        }

        // cout << "uint32 " << std::hex << setfill('0');
        // for (auto i = block.begin(); i != block.end(); ++i)
        // {
        //     cout << setw(8) << *i;
        // }
        // cout << dec << endl;

        hash_block(block, hash);
    }

    // conversion from uint32 back to bit stream.
    vector<unsigned char> digest;
    array<unsigned char, 4> word_bits;
    for (auto i = hash.begin(); i != hash.end(); ++i)
    {
        word_bits = bits::from_uint32(*i);
        digest.insert(digest.end(), 
                      word_bits.begin(), 
                      word_bits.end());
    }
     
    return digest;
}


void pad(vector<unsigned char>& message)
{
    vector<unsigned char> tmp(message);

    uint64_t bit_length = tmp.size() * 8;
    // cout << "(pad) bit length " << dec << bit_length << endl;

    unsigned int pad_bytes = (64 - (tmp.size() + 9)) % 64;
    // cout << "(pad) pad_bytes " << dec << pad_bytes << endl;

    tmp.push_back(0x80); 
    tmp.insert(tmp.end(), pad_bytes, 0x00);

    // cast to unsigned char instead?
    static const uint64_t byte_mask = 0xFF;
    for (int i = 7; i > -1; --i)
        tmp.push_back((bit_length >> (i * 8)) & byte_mask);

    swap(message, tmp);
}


void hash_block(const array<uint32_t, 16>& block, 
                array<uint32_t, 5>& hash)
{
    array<uint32_t, 80> schedule = make_schedule(block);
    
    array<uint32_t, 5> wv {0};
    copy(hash.begin(), hash.end(), wv.begin());

    uint32_t T;
    for (int t = 0; t < 80; ++t)
    {
        T = bits::rotl32(wv[0], 5) 
            + (*hash_function(t))(wv[1], wv[2], wv[3]) 
            + wv[4]
            + HASH_CONSTANTS[t / 20]
            + schedule[t];
        wv[4] = wv[3];
        wv[3] = wv[2];
        wv[2] = bits::rotl32(wv[1], 30);
        wv[1] = wv[0];
        wv[0] = T;
    }

    for (int i = 0; i < 5; ++i)
        hash[i] = wv[i] + hash[i];
}

array<uint32_t, 80> make_schedule(const array<uint32_t, 16>& block)
{
    array<uint32_t, 80> schedule;

    copy(block.begin(), block.end(), schedule.begin());
    for (auto i = schedule.begin() + 16; i != schedule.end(); ++i)
        *i = bits::rotl32(*(i-3) ^ *(i-8) ^ *(i-14) ^ *(i-16), 1);

    return schedule;
}

uint32_t ch(uint32_t x, uint32_t y, uint32_t z)
{
    return (x & y) ^ (~x & z);
}

uint32_t parity(uint32_t x, uint32_t y, uint32_t z)
{
    return x ^ y ^ z;
}

uint32_t maj(uint32_t x, uint32_t y, uint32_t z)
{
    return (x & y) ^ (x & z) ^ (y & z);
}

function_p hash_function(int t)
{
    assert(t >= 0 && t <= 79);
    if (t >= 0 && t <= 19)
        return &ch;
    else if (t >= 40 && t <= 59)
        return &maj;
    else if ((t >= 20 && t <= 39) ||
             (t >= 60 && t <= 79))
        return &parity;
}

uint32_t hash_constant(int t)
{
    assert(t >= 0 && t<=79);
    if (t >=0 && t <= 19)
        return 0x5a827999;
    else if (t >= 20 && t <= 39)
        return 0x6ed9eba1;
    else if (t >= 40 && t <= 59)
        return 0x8f1bbcdc;
    else if (t >= 60 && t <= 79)
        return 0xca62c1d6;
}
