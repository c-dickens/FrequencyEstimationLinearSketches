//
// Created by Charlie Dickens on 04/05/2022.
//
#include <iostream>
#include "counting_sketches.h"
#include <vector>
#include <random>

using namespace std ;

CountingSketch::CountingSketch(const uint64_t num_hashes, const uint64_t num_buckets, const uint64_t seed):
    num_hashes(num_hashes), num_buckets(num_buckets), seed(seed) {
    /*
     * Class wrappers for CountMin and Count sketches.
     * The CountMin operates in the "cash register" data stream model, meaning that the
     * underlying frequency vector is at least zero in every coordinate.
     * The Count sketch operates in the "turnstile" model where the underlying frequency
     * vector can have arbitrary positive or negative weight.
     */
    table.resize(num_hashes, std::vector<int64_t>(num_buckets));
    };

std::vector<std::vector<int64_t>> CountingSketch::get_table(){
    /*
     * Returns a copy of the sketch.
     */
    std::vector<std::vector<int64_t>> sketch(num_hashes, std::vector<int64_t>(num_buckets));
    for(int i=0; i<num_hashes; i++){
        for(int j=0; j<num_buckets; j++){
            sketch[i][j] = table[i][j] ;
        }
    }
    return sketch;
}

void CountingSketch::print_sketch(){
    /*
     * Prints the sketch to std output.
     */
    char eol ; // end of line character is either space for the same row or a newline
    for(int i=0; i<num_hashes; i++){
        for(int j=0; j<num_buckets; j++){
            eol = (j == num_buckets - 1) ? '\n' : ' ';
//            if(j == num_buckets - 1){
//                eol = '\n' ;
//            }
//            else{
//                eol = ' ' ;
//            }
            std::cout << table[i][j] << eol ;
        }
    }
}

//std::vector<uint64_t> CountingSketch::init_hash_parameters(uint64_t num_random_ints, uint64_t lower, uint64_t upper) {
//    /* Generates an array of `num_random_ints` many random integers in [lower, upper]. (Upper bound is closed)
//     * This is so that we can generate the hashes for bucket selection in both CountMin and CountSketch
//     * as well as the random signs for the CountSketch.
//     * int::num_rands -- how many random integers are to be chosen
//     * int::lower -- the smallest possible random integer we can select
//     * int::upper -- the largest possible random integer we can select.
//     */
//    std::cout << "Initialising " << num_random_ints << " hash parameters." << std::endl ;
//    vector<uint64_t> random_ints; // the vector we will populate
//    random_ints.reserve(num_random_ints) ;
//    default_random_engine rng(seed);
//    // uniform_int_distribution is inclusive of upper
//    uniform_int_distribution<uint64_t> uniform_random_ints(lower, upper);
//    for (int j = 0; j < num_random_ints; ++j) {
//        random_ints[j] = uniform_random_ints(rng);
//    }
//
//    for (int j = 0; j < num_random_ints; ++j) {
//        std::cout << "j = "<<j << " -- " << random_ints[j] << std::endl ; // = uniform_random_ints(rng);
//    }
//    return random_ints ;
//}

