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
    CountingSketch(const uint64_t num_hashes, const uint64_t num_buckets, const uint64_t seed) ;
    virtual ~CountingSketch(){} ;

    // Getters
    const uint64_t get_num_hashes() const { return num_hashes; }
    const uint64_t get_num_buckets() const { return num_buckets; }
    const uint64_t get_seed() const { return seed; } // nb will need this for merging.
    std::pair<uint64_t, uint64_t> get_table_shape() const {return {get_num_hashes(), get_num_buckets()} ; } ;
    std::vector<std::vector<int64_t>> get_table() ;
    void print_sketch() ;

    // Virtual functions needed by subclasses.
    virtual int64_t get_total_weight() {return total_weight ; }
    virtual const float get_epsilon() const {return epsilon ;}
    virtual const float get_delta() const {return delta ;}
    virtual const float get_confidence() const {return confidence ;}
    virtual void update(uint64_t item, int64_t weight=1){} ; // Inserts item into the sketch with default weight 1
    virtual int64_t get_estimate(uint64_t item){return 0 ;} ; // Returns the estimate from the sketch
    //virtual int64_t get_upper_bound(uint64_t item){ return 0 ;} ;
    //virtual int64_t get_lower_bound(uint64_t item){ return 0 ;} ;
    //virtual uint64_t suggest_num_buckets(float relative_error){return 0 ; } ;
    //virtual static uint64_t suggest_num_hashes(float confidence){return 0 ; } ;
    static uint64_t suggest_num_buckets(float relative_error){return 0 ; } ;
    static uint64_t suggest_num_hashes(float confidence){return 0 ; } ;


protected:
    uint64_t num_hashes, num_buckets, seed ;
    uint64_t mersenne_exponent = 5 ;
    uint64_t large_prime = (1 << mersenne_exponent) - 1 ; // nb change this to a mersenne prime
    std::vector<std::vector<int64_t>> table;
    // std::vector<uint64_t> init_hash_parameters(uint64_t num_random_ints, uint64_t lower, uint64_t upper) ;
    int64_t total_weight = 0 ; // This tracks how much weight has been added to the stream.
    // Would like to put epsilon and delta in here as they are common to both CountMin and Count sketches.

    // Parameters
    float epsilon ; // Error parameter
    float delta = 0. ; // failure probability parameter
    float confidence = 1. - delta ;



};

#endif //LINEARSKETCHES_COUNTING_SKETCHES_H
