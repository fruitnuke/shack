import sys

from bitstring import BitArray, ConstBitArray


def pad(message):
    m = BitArray(message)
    padsz = 512 - ((m.len + 65) % 512)
    m.append([1] + BitArray(padsz) + BitArray(uint=m.len, length=64))
    return m


def parse(message):
    return [message[i:i+1:512] for i in range(0, message.len / 512)]


def preprocess(message):
    blocks = parse(pad(message))
    seed = BitArray(hex='67452301efcdab8998badcfe10325476c3d2e1f0')
    return blocks, seed


def schedule(block):
    s = [block[t:t+1:32] for t in range(0, block.len / 32)]
    for t in range(16, 80):
        x = (s[t-3] ^ s[t-8] ^ s[t-14] ^ s[t-16])
        x.rol(1)
        s.append(x)
    return s 


def hash_constant(t):
    if 0 <= t <= 19:
        return BitArray(hex='5a827999')
    elif 20 <= t <= 39:
        return BitArray(hex='6ed9eba1')
    elif 40 <= t <= 59:
        return BitArray(hex='8f1bbcdc')
    elif 60 <= t <= 79:
        return BitArray(hex='ca62c1d6')
    

def hash_function(t):
    if 0 <= t <= 19:
        return lambda x, y, z: (x&y)^(~x&z)
    elif 40 <= t <= 59:
        return lambda x, y, z: (x&y)^(x&z)^(y&z)
    else:
        return lambda x, y, z: x^y^z

    
def mod_add(x, y):
    return BitArray(uintbe=(x.uintbe + y.uintbe)%(2**32), length=32)


def t_iter(t, a, b, c, d, e, sched):
    f = hash_function(t)
    K = hash_constant(t)
    xa = BitArray(a)
    xa.rol(5)
    T = mod_add(xa, f(b,c,d))
    T = mod_add(T, e)
    T = mod_add(T, K)
    T = mod_add(T, sched[t])
    e = d
    d = c
    b.rol(30)
    c = b
    b = a
    a = T
    return a, b, c, d, e


def dohash(blocks, seed):
    ihash = seed
    for block in blocks:
        sched = schedule(block)
        a, b, c, d, e = [n for n in ihash.cut(32)]
        for t in range(0, 80):
            a, b, c, d, e = t_iter(t, a, b, c, d, e, sched)
        h0, h1, h2, h3, h4 = [n for n in ihash.cut(32)]
        ihash = BitArray()
        ihash.append(mod_add(a, h0))
        ihash.append(mod_add(b, h1))
        ihash.append(mod_add(c, h2))
        ihash.append(mod_add(d, h3))
        ihash.append(mod_add(e, h4))
    return ihash.hex[2:]
            

def sha1(message):
    blocks, seed = preprocess(message)
    return dohash(blocks, seed)


def main():
    m = sys.stdin.readline().strip()
    while m != "quit":
        print sha1(BitArray(hex=m))
        m = sys.stdin.readline().strip()


if __name__ == "__main__":
    main()
