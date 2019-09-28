#define CATCH_CONFIG_MAIN // Provide a main
#include <catch.hpp>
#include <lsmt.hpp>
#include <string>

using namespace lsmt;

// Construction
TEST_CASE( "Initializing an LSM Tree with no sstables is okay", "[lsmt::lsmt]" ) {
    lsmtree<std::string, int, 0> t({}, {});
    REQUIRE( t.size() == 0 );
}

// Inserts
TEST_CASE( "Adding to an LSM Tree will increase its size by 1", "[lsmt::insert]" ) {
    lsmtree<std::string, int, 0> t({}, {});
    t.insert("test", 3);
    REQUIRE( t.size() == 1 );
}
