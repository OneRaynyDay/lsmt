#define CATCH_CONFIG_MAIN // Provide a main
#include <catch.hpp>
#include <lsmt.hpp>
#include <string>

using namespace lsmt;

// NOTE: All of these tests are single-threaded. We will need another set of tests for multithreaded testing.

// Construction
TEST_CASE( "Initializing an LSM Tree with no sstables is okay", "[lsmt::lsmt]" ) {
    lsmtree<std::string, int, 0> t({}, {});
    REQUIRE( t.size() == 0 );
}

// Upserts & finds
TEST_CASE( "Adding to an LSM Tree will increase its size by 1", "[lsmt::upsert]" ) {
    lsmtree<std::string, int, 0> t({}, {});
    t.upsert("test", 3);
    REQUIRE( t.size() == 1 );
    REQUIRE( t.find("test") == 3 );
    REQUIRE( t.find("not_in_table") == int{} );

    t.upsert("test", 4);
    REQUIRE( t.size() == 1 );
    REQUIRE( t.find("test") == 4 );
}

// Deletes
TEST_CASE( "Removing an item not in an LSM Tree will also increase its size by 1", "[lsmt::remove]" ) {
    lsmtree<std::string, int, 0> t({}, {});
    t.upsert("test", 3);
    REQUIRE( t.size() == 1 );
    // Preserves the key but changes the value
    t.remove("test");
    REQUIRE( t.size() == 1 );

    t.remove("another_test");
    REQUIRE( t.size() == 2 );

    REQUIRE( t.find("test") == int{} );
    REQUIRE( t.find("another_test") == int{} );
}
