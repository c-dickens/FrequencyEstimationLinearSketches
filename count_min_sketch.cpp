//
// Created by Charlie Dickens on 04/05/2022.
//
#include <iostream>
#include <cmath>
#include <random>
#include "count_min_sketch.h"


// Constructor
CountMinSketch::CountMinSketch(uint64_t num_hashes, uint64_t num_buckets, uint64_t seed )
        : CountingSketch(num_hashes, num_buckets, seed){
    CountMinSketch::set_hash_parameters() ;
    epsilon = exp(1.0) / float(num_buckets) ;
    delta = 1.0 / exp(float(num_hashes)) ;
    confidence = 1.0 - delta ;
    std::cout << epsilon << " " << delta << std::endl ;
//    vector<uint64_t> a_hash_params(num_hashes) ;
//    vector<uint64_t> b_hash_params(num_hashes) ;
//    a_hash_params.resize(num_hashes) ;
//    b_hash_params.resize(num_hashes) ;
} ;

void CountMinSketch::set_hash_parameters(){
    /* Sets the array containing a and b parameters for hashing.
     * a_hash_params contains all values of a for the hashing (see ::get_bucket_hash)
     * a_hash_params contains all values of b
     * We generate 2*num_hashes from CountingSketch::init_hash_parameters and then iterate through to get
     * the values of a and b for the respective arrays.
     */
    default_random_engine rng(seed);
    uniform_int_distribution<uint64_t> uniform_random_ints(0, large_prime - 1);
    a_hash_params.reserve(num_hashes) ;
    b_hash_params.reserve(num_hashes) ;

    for(int i=0 ; i < num_hashes ; ++i){
        cout << uniform_random_ints(rng) << std::endl ;
        a_hash_params[i] = uniform_random_ints(rng);
        b_hash_params[i] = uniform_random_ints(rng);
    }

}

uint64_t CountMinSketch::get_bucket_hash(uint64_t item, uint64_t a, uint64_t b){
    /*
     * Performs bucket hashing, that is, for a given item and a given row in the sketch,
     * this function selects the bucket, meaning the column index of the sketch table.
     *
     * h = (a * x + b) % large_prime
     * h = h % nbuckets
     * This is executed once for each of the hash functions
     */
    return uint64_t ((a * item + b) % large_prime) % num_buckets ;
}

void CountMinSketch::update(uint64_t item, int64_t weight){
    /*
     * Updates the sketch with the item
     * iterates through the number of hash functions and gets the bucket index.
     * Then increment the sketch table at index (row, column) where row is one of the the hash
     * functions that are iterated over, and column is the corresponding bucket index.
     * Finally, increment the sketch table with the weight associated to the item.
     */
    for(uint64_t i=0; i < num_hashes; i++){
        uint64_t a = a_hash_params[i] ;
        uint64_t b = b_hash_params[i] ;
        uint64_t h = get_bucket_hash(item, a, b) ;
        table[i][h] += weight ;
    }
    total_weight += weight ;
}

int64_t CountMinSketch::get_estimate(uint64_t item) {
    /*
     * Returns the estimate from the sketch for the given item.
     */
    int64_t estimate = std::numeric_limits<int64_t>::max() ; // start arbitrarily large
    for(uint64_t i=0; i < num_hashes; i++){
        uint64_t a = a_hash_params[i] ;
        uint64_t b = b_hash_params[i] ;
        uint64_t h = get_bucket_hash(item, a, b) ;
        estimate = std::min(estimate, table[i][h]) ;
    }
    return estimate ;
}

int64_t CountMinSketch::get_upper_bound(uint64_t item) {
    /*
     * Returns the upper bound of the estimate as:
     * f_i - true frequency
     * est(f_i) - estimate frequency
     * f_i <= est(f_i)
     */
    return get_estimate(item) ;
}

int64_t CountMinSketch::get_lower_bound(uint64_t item) {
    /*
     * Returns the lower bound of the estimate as:
     * f_i - true frequency
     * est(f_i) - estimate frequency
     * f_i >= est(f_i) - epsilon*||f||_1 with ||f||_1 being the total weight in the sketch.
     */
    return get_estimate(item) - epsilon*total_weight ;
}