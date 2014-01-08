VPATH=unit_tests
SHARED_OBJECTS=sha1.o binary_utils.o hexstream.o
SHACK_OBJECTS=main.o
TESTDIR=unit_tests/
TEST_OBJECTS=TestRunner.o sha1_tests.o binary_utils_tests.o hexstream_tests.o
CXXFLAGS=-g -std=c++0x -DDEBUG
LFLAGS=-L/usr/local/lib
#-D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC

all: shared shack $(TESTDIR)tests
	$(TESTDIR)tests

shared: $(SHARED_OBJECTS)

shack: $(SHACK_OBJECTS) $(SHARED_OBJECTS)
	$(CXX) $(SHACK_OBJECTS) $(SHARED_OBJECTS) $(LFLAGS) -lboost_program_options -o $@

shavs: shack
	cd test; python test.py --verbose --short "../shack --hex"

correct:
# set debug heap, examing for memory leaks

profile: CXXFLAGS=-g -std=c++0x -pg
profile: $(SHACK_OBJECTS) $(SHARED_OBJECTS)
	$(CXX) $(SHACK_OBJECTS) $(SHARED_OBJECTS) -ltcmalloc -o $@
	./test/test.py --short ../shack


# build with -pg, no debug flags, call gprof

$(TESTDIR)tests: $(addprefix $(TESTDIR),$(TEST_OBJECTS)) $(SHARED_OBJECTS)
	$(CXX) $(addprefix $(TESTDIR),$(TEST_OBJECTS)) $(SHARED_OBJECTS) $(LFLAGS) -lboost_unit_test_framework -o $@

$(TESTDIR)sha1_tests.o: sha1.h $(TESTDIR)sha1_impl.h
$(TESTDIR)binary_utils_tests.o: binary_utils.h
$(TESTDIR)hexstream_tests.o: hexstream.h

main.o: sha1.h binary_utils.h

.PHONY:clean

clean:
	rm -rf *.o shack
	rm -rf $(TESTDIR)*.o $(TESTDIR)tests
