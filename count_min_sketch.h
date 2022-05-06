//
// Created by Charlie Dickens on 04/05/2022.
//

#ifndef LINEARSKETCHES_COUNTMINSKETCH_H
#define LINEARSKETCHES_COUNTMINSKETCH_H

#include "counting_sketches.h"

using namespace std ;

class CountMinSketch : public CountingSketch {
    public:
        CountMinSketch(uint64_t n_h, uint64_t n_b, uint64_t s)  ;
        void update(uint64_t item, uint64_t weight=1) ;
        uint64_t get_estimate(uint64_t item) ;

    private:
        void set_hash_parameters() ;
        uint64_t get_bucket_hash(uint64_t item, uint64_t a, uint64_t b) ;
        vector<uint64_t> a_hash_params, b_hash_params ;
        float epsilon ;
};


#endif //LINEARSKETCHES_COUNTMINSKETCH_H
