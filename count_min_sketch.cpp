//
// Created by Charlie Dickens on 04/05/2022.
//
#include <iostream>
#include <cmath>
#include "count_min_sketch.h"

// Constructor
CountMinSketch::CountMinSketch(uint64_t n_hashes, uint64_t n_buckets, uint64_t s )
        : CountingSketch(n_hashes, n_buckets, s){
    CountMinSketch::set_hash_parameters() ;
} ;

void CountMinSketch::set_hash_parameters(){
    /* Sets the array containing a and b parameters for hashing.
     * a_hash_params contains all values of a for the hashing (see ::get_bucket_hash)
     * a_hash_params contains all values of b
     * We generate 2*num_hashes from CountingSketch::init_hash_parameters and then iterate through to get
     * the values of a and b for the respective arrays.
     */
    vector<uint64_t> all_hash_params ;
    all_hash_params = CountingSketch::init_hash_parameters(2*num_hashes, 0, large_prime - 1);

    for(int i=0 ; i < num_hashes ; ++i){
        CountMinSketch::a_hash_params.push_back(all_hash_params[i]) ;
        CountMinSketch::b_hash_params.push_back(all_hash_params[num_hashes+i]) ;
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

void CountMinSketch::update(uint64_t item, uint64_t weight){
    /*
     * Updates the sketch with the item
     * iterates through the number of hash functions and gets the bucket index.
     * Then increment the sketch table at index (row, column) where row is one of the the hash
     * functions that are iterated over, and column is the corresponding bucket index.
     * Finally, increment the sketch table with the weight associated to the item.
     */
    for(uint64_t i=0; i < num_hashes; i++){
        uint64_t a = a_hash_params.at(i) ;
        uint64_t b = b_hash_params.at(i) ;
        uint64_t h = CountMinSketch::get_bucket_hash(item, a, b) ;
        table[i][h] += weight ;
    }
    total_weight += weight ;
}

uint64_t CountMinSketch::get_estimate(uint64_t item) {
    /*
     * Returns the estimate from the sketch for the given item.
     */
    uint64_t min_frequency = std::numeric_limits<uint64_t>::max() ; // start arbitrarily large
    for(uint64_t i=0; i < num_hashes; i++){
        uint64_t a = a_hash_params.at(i) ;
        uint64_t b = b_hash_params.at(i) ;
        uint64_t h = CountMinSketch::get_bucket_hash(item, a, b) ;
        uint64_t estimate = table[i][h] ;
        if (estimate < min_frequency){
            min_frequency = estimate ;
        }
    }
    return min_frequency ;
}