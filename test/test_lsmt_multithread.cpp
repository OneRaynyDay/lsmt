#define CATCH_CONFIG_MAIN // Provide a main
#include <catch.hpp>
#include <lsmt.hpp>
#include <string>
#include <vector>
#include <thread>
#include <future>
#include <iostream>

using namespace lsmt;

// These tests are multithreaded. They will run a certain iteration to
// check that the results are probably consistent.

// Run each test 1000 times
static constexpr unsigned int NUM_ITERATIONS = 10;
// Run with 30 threads
static constexpr unsigned int NUM_THREADS = 30;

// Upserts the values <i, i> into t where begin <= i < end
void upsert_into_lsmtree(lsmtree<int, int, 0>& t, int begin, int end) {
    for(int i = begin; i < end; i++){
        t.upsert(i, i);
    } 
}

TEST_CASE( "Adding to an LSM tree with different keys concurrently", "[lsmt::upsert]" ) {
    for(int i = 0; i < NUM_ITERATIONS; i++){
        std::cout << "Running iteration " << i << " ..." << std::endl;
        lsmtree<int, int, 0> t({}, {});
        { // std::future will die at the end of this scope. Guarrantee join.
            std::vector<std::future<void>> futs;
            for(int i = 0; i < NUM_THREADS; i++){
                futs.push_back(std::async(std::launch::async, 
                               upsert_into_lsmtree, 
                               std::ref(t), 
                               i * NUM_THREADS,
                               (i+1) * NUM_THREADS));
            }
        }
        REQUIRE(t.size() == NUM_THREADS * NUM_THREADS);
    }
}
