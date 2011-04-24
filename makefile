VPATH=unit_tests
SHARED_OBJECTS=sha1.o binary_utils.o
SHACK_OBJECTS=main.o
TESTDIR=unit_tests/
TEST_OBJECTS=TestRunner.o sha1_tests.o binary_utils_tests.o
CXXFLAGS=-g -std=c++0x -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC -I/mnt/data/vol/src/lib/tracedog/inc -DTRACEDOG_ENABLE_MACROS
LFLAGS=/mnt/data/vol/src/lib/tracedog/lib/libtracedog.a

all: shared shack $(TESTDIR)tests
	$(TESTDIR)tests

shared: $(SHARED_OBJECTS)

shack: $(SHACK_OBJECTS) $(SHARED_OBJECTS)
	$(CXX) $(LFLAGS) $(SHACK_OBJECTS) $(SHARED_OBJECTS) -o $@

$(TESTDIR)tests: $(addprefix $(TESTDIR),$(TEST_OBJECTS)) $(SHARED_OBJECTS)
	$(CXX) $(LFLAGS) $(addprefix $(TESTDIR),$(TEST_OBJECTS)) $(SHARED_OBJECTS) -static -lboost_unit_test_framework -o $@

$(TESTDIR)sha1_tests.o: sha1.h
$(TESTDIR)binary_utils_tests.o: binary_utils.h

main.o: sha1.h binary_utils.h

.PHONY:clean

clean:
	rm -rf *.o shack
	rm -rf $(TESTDIR)*.o $(TESTDIR)tests