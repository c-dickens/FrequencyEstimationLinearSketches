//
// Created by Charlie Dickens on 04/05/2022.
//

#include "counting_sketches.h"
#include <vector>
#include <random>
#include <iostream>

using namespace std ;

// Counting Sketch class functions

// Constructor
CountingSketch::CountingSketch(const uint64_t &num_hashes, const uint64_t &num_buckets, const uint64_t &seed):
    num_hashes(num_hashes), num_buckets(num_buckets), seed(seed) {
        table.resize(num_hashes, std::vector<uint64_t>(num_buckets));
        // Can remove this function CountingSketch::init_hash_parameters(0,10)  ;
    };

std::vector<uint64_t> CountingSketch::init_hash_parameters(uint64_t num_rands, uint64_t lower, uint64_t upper) {
    /* Generates an array of random integers in [lower, upper]. (Upper bound is closed)
     * This is so that we can generate the hashes for bucket selection in both CountMin and CountSketch
     * as well as the random signs for the CountSketch.
     * int::num_rands -- how many random integers are to be chosen
     * int::lower -- the smallest possible random integer we can select
     * int::upper -- the largest possible random integer we can select.
     */
    vector<uint64_t> random_ints; // the vector we will populate
    default_random_engine rng(seed);
    // uniform_int_distribution is inclusive of upper
    uniform_int_distribution<uint64_t> uniform_random_ints(lower, upper);
    for (int j = 0; j < num_rands; ++j) {
        random_ints.push_back(uniform_random_ints(rng));
    }

    //    cout << "****** PRINTING THE RANDOM INTS ****** " << std::endl ;
    //    for (const int r: random_ints) {
    //        std::cout << r << " ";
    //    }
    //    std::cout << std::endl ;
    return random_ints ;
}

// Getters
std::vector<uint64_t> CountingSketch::get_table_shape(){
    std::vector<uint64_t> shape(2);
    shape.at(0) = CountingSketch::get_num_hashes() ;
    shape.at(1) = CountingSketch::get_num_buckets() ;
    return shape ;
}


