#ifndef __shack_sha1_impl_h__
#define __shack_sha1_impl_h__

/* Header file for use by unit test modules.  Exposes the internal
 * implementation functions for unit testing.  Not to be used for any
 * other function. */

#include <array>
#include <cstdint>
#include <vector>

typedef uint32_t (*function_p) (uint32_t, uint32_t, uint32_t);

void hash_block(const std::array<uint32_t, 16>&, std::array<uint32_t, 5>&);
void pad(std::vector<unsigned char>&);
std::array<uint32_t, 80> make_schedule(const std::array<uint32_t, 16>&);
uint32_t ch(uint32_t, uint32_t, uint32_t);
uint32_t maj(uint32_t, uint32_t, uint32_t);
uint32_t parity(uint32_t, uint32_t, uint32_t);
function_p hash_function(int);
uint32_t hash_constant(int);


#endif
