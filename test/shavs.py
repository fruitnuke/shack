import subprocess

from bitstring import BitArray


"""Context Manager class for establishing and terminating
communication with the SHA Implementation Under Test (IUT)."""
class IUT:
    def __enter__(self):
        self.p = subprocess.Popen(
            ["python", "-u", "../proto.py"],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE)
        return self

    def sha(self, msg):
        self.p.stdin.write(msg + '\n')
        self.p.stdin.flush()
        return self.p.stdout.readline().strip()
    
    def __exit__(self, type, value, traceback):
        self.p.terminate()


"""Return the value from a string such as "Len = 20/r/n" representing
a name-value pair read from a SHAVS test file."""
def parse(s):
    return s.strip("[]\r\n").partition("=")[2].strip()


def hash_test_cases(filename):
    with open(filename) as f:
        while True:
            line = f.readline()
            if not line:
                break
            if line.startswith("Len"):
                length = int(parse(line))
                msg = parse(f.readline())
                # Hack: test file contains msg 0x00 of length 0, and
                # bitstring won't 'these values as initializers'
                if length == 0:
                    msg = ''
                digest = parse(f.readline())
                yield (msg, digest)


def chain_test_case(filename):
    seed = None
    checkpoints = []
    with open(filename) as f:
        for line in f:
            if line.startswith("Seed"):
                seed = parse(line)
            elif line.startswith("MD"):
                checkpoints.append(parse(line))
    yield (seed, checkpoints)

            
def hash_test(iut, options, (msg, expected_digest)):
    iut.p.stdin.write(msg + '\n')
    iut.p.stdin.flush()
    actual_digest = iut.p.stdout.readline().strip()
    if actual_digest != expected_digest:
        faildetails = {}
        faildetails["message"] = msg
        faildetails["expected"] = expected_digest
        faildetails["actual"] = actual_digest
        return False, faildetails
    return True, {}

            
def chain_test(iut, options, (seed_str, checkpoints)):
    seed = BitArray(hex=seed_str)
    for j in range(0, int(options.num_checkpoints)):
        m1 = m2 = m3 = seed
        for i in range(0, 1000):
            msg = m3 + m2 + m1
            digest = BitArray(hex=iut.sha(msg.hex[2:]))
            m3 = m2
            m2 = m1
            m1 = digest
        seed = digest
        if digest.hex[2:] != checkpoints[j]:
            failpoint = {}
            failpoint["seed"] = BitArray(hex=seed_str).hex[2:]
            failpoint["checkpoint"] = j
            failpoint["expected"] = checkpoints[j]
            failpoint["actual"] = digest.hex[2:]
            return False, failpoint
    return True, {}


def do_tests(iut, options, test_f, tests):
    for t in tests:
        passed, faildetails = test_f(iut, options, t)
        yield passed, faildetails

