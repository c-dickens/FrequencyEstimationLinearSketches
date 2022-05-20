//
// Created by Charlie Dickens on 04/05/2022.
// CountMin sketch inherits from the counting_sketch class.
// An overview can be found at http://dimacs.rutgers.edu/~graham/pubs/papers/cmencyc.pdf
//

#ifndef LINEARSKETCHES_COUNTMINSKETCH_H
#define LINEARSKETCHES_COUNTMINSKETCH_H

#include "counting_sketches.h"

using namespace std ;

class CountMinSketch : public CountingSketch {
    public:
        CountMinSketch(uint64_t num_hashes, uint64_t num_buckets, uint64_t seed)  ;
        int64_t get_estimate(uint64_t item) ;
        void update(uint64_t item, int64_t weight=1) ;
        int64_t get_upper_bound(uint64_t item) ;
        int64_t get_lower_bound(uint64_t item) ;

private:
        void set_hash_parameters() ;
        uint64_t get_bucket_hash(uint64_t item, uint64_t a, uint64_t b) ;
        vector<uint64_t> a_hash_params, b_hash_params ;

};


#endif //LINEARSKETCHES_COUNTMINSKETCH_H
