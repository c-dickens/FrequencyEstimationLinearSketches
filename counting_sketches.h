//
// Created by Charlie Dickens on 04/05/2022.
// This is a base class that will be used for both CountMin and Count sketch classes and contains functionality that
// will be common to both sketches.
//

#ifndef LINEARSKETCHES_COUNTING_SKETCHES_H
#define LINEARSKETCHES_COUNTING_SKETCHES_H


#include <cstdio>
#include <cmath>
#include <vector>

class CountingSketch{
public:
    CountingSketch(const uint64_t &num_hashes, const uint64_t &num_buckets, const uint64_t &seed) ;
    virtual ~CountingSketch(){} ;

    // Getters
    const uint64_t &get_num_hashes() const { return num_hashes; }
    const uint64_t &get_num_buckets() const { return num_buckets; }
    const uint64_t &get_seed() const { return seed; } // nb will need this for merging.
    std::vector<uint64_t> get_table_shape() ;


protected:
    uint64_t num_hashes, num_buckets, seed ;
    uint64_t large_prime = pow(2, 5) - 1 ;
    std::vector<std::vector<uint64_t>> table;
    std::vector<uint64_t> init_hash_parameters(uint64_t n_r, uint64_t l, uint64_t u) ;
    uint64_t total_weight = 0.0 ; // This tracks how much weight has been added to the stream.
    // Would like to put epsilon and delta in here as they are common to both CountMin and Count sketches.

};

#endif //LINEARSKETCHES_COUNTING_SKETCHES_H
