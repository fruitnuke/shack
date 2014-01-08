#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE shack_unit_tests
#define BOOST_TEST_NO_MAIN
#define BOOST_TEST_ALTERNATIVE_INIT_API
#include <boost/test/unit_test.hpp>
// #include <tracedog/tracing.hpp>

bool start_tracing()
{
    // TRDG_PRESET_FNAME("trace.log");
    // TRDG_INITIALIZE_LIB();
    // TRDG_START_TRACING();
}

int BOOST_TEST_CALL_DECL
main( int argc, char* argv[] )
{
    return ::boost::unit_test::unit_test_main(&start_tracing, argc, argv);
}
