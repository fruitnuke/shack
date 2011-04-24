#ifndef __shack_sha1_h__
#define __shack_sha1_h__

#include <vector>

namespace sha1 
{
    std::vector<unsigned char> hash(const std::vector<unsigned char>& message);
}

#endif
