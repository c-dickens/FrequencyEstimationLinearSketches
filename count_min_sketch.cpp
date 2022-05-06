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
    /* Sets the hash_a_b array containing the a and b parameters for hashing.
     *
     */
    vector<uint64_t> all_hash_params ;
    all_hash_params = CountingSketch::init_hash_parameters(2*num_hashes, 0, large_prime - 1);

    for(int i=0 ; i < num_hashes ; ++i){
        CountMinSketch::a_hash_params.push_back(all_hash_params[i]) ;
        CountMinSketch::b_hash_params.push_back(all_hash_params[num_hashes+i]) ;
    }
//    for(int i=0 ; i < num_hashes ; ++i){
//        std::cout << "hash_param(a)[i] = " << CountMinSketch::a_hash_params[i] << std::endl ;
//        std::cout << "hash_param(b)[i] = " << CountMinSketch::b_hash_params[i] << std::endl ;
//    }
}

uint64_t CountMinSketch::get_bucket_hash(uint64_t item, uint64_t a, uint64_t b){
    /*
     * Performs bucket hashing
     * h = (a * x + b) % large_prime
     * h = h % nbuckets
     * This is executed once per number of hash functions
     */
    return uint64_t ((a * item + b) % large_prime) % CountMinSketch::num_buckets ;
}

void CountMinSketch::update(uint64_t item, uint64_t weight){
    /*
     * Updates the sketch with the item
     * iterates through the number of hash functions and updates the sketch table with the weight.
     */
    for(uint64_t i=0; i < num_hashes; i++){
        uint64_t a = a_hash_params.at(i) ;
        uint64_t b = b_hash_params.at(i) ;
        uint64_t h = CountMinSketch::get_bucket_hash(item, a, b) ;
        //cout << "Updating location " << i << " "<< h ;
        table[i][h] += weight ;
        //cout << " Frequency " << table[i][h] << endl ;
    }
    total_weight += weight ;
}

uint64_t CountMinSketch::get_estimate(uint64_t item) {
    /*
     * Returns the estimate from the sketch for the given item.
     */
    uint64_t min_frequency = std::numeric_limits<uint64_t>::max() ; //start arbitrarily large
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