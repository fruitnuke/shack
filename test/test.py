import sys
from optparse import OptionParser

import shavs


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
    print "[FAIL] %s (%i): %s... != %s..." % (
        message[:7].ljust(7),
        fail_details["len"],
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
                     shavs.hash_test_cases(options.testpath + "SHA1ShortMsg.txt"),
                     Output("short", options, hash_fail_msg))
        if options.run_all or options.long:
            do_tests(iut, options, shavs.hash_test,
                     shavs.hash_test_cases(options.testpath + "SHA1LongMsg.txt"),
                     Output("long", options, hash_fail_msg))
        if options.run_all or options.pseudo:
            do_tests(iut, options, shavs.chain_test,
                     shavs.chain_test_case(options.testpath + "SHA1Monte.txt"),
                     Output("pseudorandom generated", options, chain_fail_msg))


def process_args():
    usage = "usage: %prog [options] executable args"
    parser = OptionParser(usage, version="%prog 1.0")
    parser.add_option("-v", "--verbose", action="store_true", dest="verbose")
    parser.add_option("--checkpoints", type="int", dest="checkpoints", default=100)
    parser.add_option("--short", action="store_true", dest="short")
    parser.add_option("--long", action="store_true", dest="long")
    parser.add_option("--pseudo", action="store_true", dest="pseudo")
    parser.add_option("--bit", action="store_true", dest="bitoriented")
    (options, args) = parser.parse_args()
    
    if len(args) != 1:
        parser.error("need to specify an executable to test")
    options.exelist = args[0].split(" ")
    
    if options.short or options.long or options.pseudo:
        options.run_all = False
    else:
        options.run_all = True


    if options.bitoriented:
        options.testpath = "./bit-vectors/"
    else:
        options.testpath = "./byte-vectors/"
        
    return options

        
def main():
    options = process_args()
    all_tests(options)

    
if __name__ == "__main__":
    main()
        
