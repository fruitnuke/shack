import unittest
import random
import sys

from bitstring import BitArray

import proto


def randombits(n):
    return BitArray([random.randint(0,1) for n in range(0, n)])


class PreprocessingTests(unittest.TestCase):

    def test_pad(self):
        for n in range(0, 513):        
            m = randombits(n)
            mp = proto.pad(m)
            self.assertEqual(mp.len % 512, 0)
            self.assertEqual(mp[-64:].int, m.len)
            self.assertEqual(mp[m.len], True)
            self.assertEqual(mp[m.len+1:-64].int, False)
            self.assertEqual(mp[:m.len], m)


    def test_parse(self):
        blocks = proto.parse(randombits(0))
        self.assertEqual(blocks, [])

        m1 = randombits(512)
        blocks = proto.parse(m1)
        self.assertEqual(blocks, [m1])

        m2 = randombits(512)
        blocks = proto.parse(m1 + m2)
        self.assertEqual(blocks, [m1, m2])
        
        
if __name__ == "__main__":
    unittest.main()
