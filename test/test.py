import sys
#import getopt
from optparse import OptionParser

import shavs

# class Options:
#     verbose = False
#     num_checkpoints = 100
#     run_all = True
#     run_short = False
#     run_long = False
#     run_pseudo = False


class Output:
    def __init__(self, name, options, fail_message):
        self.name = name
        self.options = options
        self.fail_message = fail_message
        self.suite_passed = True
        self.total = 0
        self.failed = 0

    def preamble(self):
        sys.stdout.write("Running %s message tests ..." % self.name)

    def on_test(self, passed, fail_details):
        if self.suite_passed and not passed:
            self.suite_passed = False
            if self.options.verbose:
                sys.stdout.write("\n")

        if self.options.verbose and not passed:
            self.fail_message(fail_details)
            self.failed += 1
            
        self.total += 1
        
    def postamble(self):
        if self.suite_passed:
            print " OK (%i)" % self.total
        elif not self.suite_passed:
            print " FAILED (%i of %i)" % (self.failed, self.total)

            
def chain_fail_msg(fail_details):
    print "[FAIL] seed %s... @ checkpoint %s: %s... != %s..." % (
        fail_details["seed"][:7].ljust(7),
        fail_details["checkpoint"],
        fail_details["actual"][:10],
        fail_details["expected"][:10])            

    
def hash_fail_msg(fail_details):
    message = fail_details["message"]
    print "[FAIL] %s (%s): %s... != %s..." % (
        message[:7].ljust(7),
        str(len(message)).ljust(3),
        fail_details["actual"][:10],
        fail_details["expected"][:10])


def do_tests(iut, options, test_f, tests, ui):
    ui.preamble()
    for passed, faildetails in shavs.do_tests(iut, options, test_f, tests):
        ui.on_test(passed, faildetails)
    ui.postamble()


def all_tests(options):    
    with shavs.IUT(options) as iut:
        if options.run_all or options.short:
            do_tests(iut, options, shavs.hash_test,
                     shavs.hash_test_cases("./byte-vectors/SHA1ShortMsg.txt"),
                     Output("short", options, hash_fail_msg))
        if options.run_all or options.long:
            do_tests(iut, options, shavs.hash_test,
                     shavs.hash_test_cases("./byte-vectors/SHA1LongMsg.txt"),
                     Output("long", options, hash_fail_msg))
        if options.run_all or options.pseudo:
            do_tests(iut, options, shavs.chain_test,
                     shavs.chain_test_case("./byte-vectors/SHA1Monte.txt"),
                     Output("pseudorandom generated", options, chain_fail_msg))


# def usage():
#     print """Usage: python test.py [OPTION]... executable
# Run informal NIST secure hash algorithm tests (SHAVS) using test files
# in the subdirectories.

#   -v, --verbose        Display more information during testing.
#       --help           Display this help and exit.
#       --checkpoints=N  Number of checkpoints for the pseudo-random message
#                        test.
#       --short          Run short message tests (default runs all).
#       --long           Run long message tests (default runs all).
#       --pseudo         Run pseudo-random message tests (default runs all).

# (C) copyright Dan Boswell 2011"""
    
    
def process_args():
    usage = "usage: %prog [options] executable args"
    parser = OptionParser(usage, version="%prog 1.0")
    parser.add_option("-v", "--verbose", action="store_true", dest="verbose")
    parser.add_option("--checkpoints", type="int", dest="checkpoints", default=100)
    parser.add_option("--short", action="store_true", dest="short")
    parser.add_option("--long", action="store_true", dest="long")
    parser.add_option("--pseudo", action="store_true", dest="pseudo")
    (options, args) = parser.parse_args()
    
    if len(args) != 1:
        parser.error("need to specify an executable to test")
    options.exelist = args[0].split(" ")
    
    if options.short or options.long or options.pseudo:
        options.run_all = False
    else:
        options.run_all = True

    return options
    # options = Options()
    # try:
    #     opts, args = getopt.getopt(
    #         sys.argv[1:], "v", ["help", "verbose", "checkpoints=",
    #                             "short", "long", "pseudo"])
    # except getopt.GetoptError, err:
    #     print str(err)
    #     usage()
    #     sys.exit(1)
    # verbose = False
    # for o, a in opts:
    #     if o in ("--short", "--long", "--pseudo"):
    #         options.run_all = False
    #     if o in ("-v", "--verbose"):
    #         options.verbose = True
    #     elif o in ("--short"):
    #         options.run_short = True
    #     elif o in ("--long"):
    #         options.run_long = True
    #     elif o in ("--pseudo"):
    #         options.run_pseudo = True
    #     elif o in ("--checkpoints"):
    #         options.num_checkpoints = a
    #     elif o == "--help":
    #         usage()
    #         sys.exit()
    #     else:
    #         assert False, "unhandled option"
    # return options

        
def main():
    options = process_args()
    all_tests(options)

    
if __name__ == "__main__":
    main()
        
