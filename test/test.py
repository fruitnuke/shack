import sys
import getopt
import subprocess

version = 1.00
verbose = False


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
        
    def __exit__(self, type, value, traceback):
        self.p.terminate()


"""Return the value from a string such as "Len = 20/r/n" representing
a name-value pair read from a SHAVS test file."""
def parse(s):
    return s.strip("[]\r\n").partition("=")[2].strip()


def read_hash_tests(filename):
    tests = []
    with open(filename) as f:
        while True:
            line = f.readline()
            if not line:
                break
            if line.startswith("Len"):
                length = int(parse(line))
                msg = parse(f.readline())
                # Hack, because test file contains msg 0x00 of length
                # 0, and bitstring doesn't like these values as
                # initializers'
                if length == 0:
                    msg = ''
                digest = parse(f.readline())
                tests.append((length, msg, digest))
    return tests


def read_chain_tests(filename):
    tests = []
    with open(filename) as f:
        for line in f:
            if line.startswith(("Seed", "MD")):
                tests.append(parse(line))
    return tests


# def hash_test(tests):
#     results = []
#     with IUT() as iut:
#         for test in tests:
#             iut.p.stdin.write(test[1] + '\n')
#             iut.p.stdin.flush()
#             digest = iut.p.stdout.readline().strip()
#             results.append((test[2] == digest, test))

#     return results

def hash_test(tests):
    with IUT() as iut:
        for test in tests:
            iut.p.stdin.write(test[1] + '\n')
            iut.p.stdin.flush()
            digest = iut.p.stdout.readline().strip()
            yield test[2] == digest, test, digest

def chain_test(seed):
    tohash = seed
    results = []
    with IUT() as iut:
        for i in range(1, 100000):
            iut.p.stdin.write(tohash + '\n')
            iut.p.stdin.flush()
            digest = iut.p.stdout.readline().strip()
            if (i % 1000) == 0:
                results.append(digest)
            tohash = digest
    return results


def print_results(results):
    global verbose
    success = reduce(lambda x, y: x and y[0], results, True)
    if success:
        print " OK."
    else:
        print " FAILED"
        if verbose:
            for r in results:
                if not r[0]:
                    print "Hashing %s-bit message: FAILED" % r[1][0]


def usage():
    print """Usage: python test.py [OPTION]...
Run informal NIST secure hash algorithm tests (SHAVS) using test files
in the subdirectories.

Mandatory arguments to long options are mandatory for short options too.
  -v, --verbose  display more information during testing.
      --help     display this help and exit
      --version  output version information and exit

(C) copyright Dan Boswell 2011"""
                
                
def run_tests():
    global verbose, version

    try:
        opts, args = getopt.getopt(
            sys.argv[1:], "v", ["help", "verbose", "version"])
    except getopt.GetoptError, err:
        print str(err)
        usage()
        sys.exit(1)
    verbose = False
    for o, a in opts:
        if o in ("-v", "--verbose"):
            verbose = True
        elif o == "--help":
            usage()
            sys.exit()
        elif o == "--version":
            print "test.py version %s" % version
            sys.exit()
        else:
            assert False, "unhandled option"
    
    filename = "./byte-vectors/SHA1ShortMsg.txt"
    short_tests = read_hash_tests(filename)
    filename = "./byte-vectors/SHA1LongMsg.txt"
    long_tests = read_hash_tests(filename)
    # filename = "./byte-vectors/SHA1Monte.txt"
    # monte_chain = read_chain_tests(filename)

    failed = False
    sys.stdout.write("Running short message tests (%i) ..." % len(short_tests))
    for passed, test, digest in hash_test(short_tests):
        if not passed:
            if not failed:
                failed = True
                if verbose:
                    sys.stdout.write("\n")
            if verbose:
                print "[FAIL] %s (%s): %s... != %s..." % (
                    test[1][:7].ljust(7), str(test[0]).ljust(3), digest[:10], test[2][:10])
    if not failed:
        print " OK"
    if failed and not verbose:
        print " FAILED"
        
#     sys.stdout.write("Running long message tests (%i) ..." % len(short_tests))
#     print_results(hash_test(long_tests))

#     sys.stdout.write("Running pseudorandomly generated messages test ...")
#     results = chain_test(monte_chain[0])
#     success = reduce(lambda a, u: a and (u[0] == u[1]),
#                      zip(results, monte_chain), True)
#     if success:
#         print " OK"
#     else:
#         print " FAILED"
#         if verbose: print results

        
if __name__ == "__main__":
    run_tests()
