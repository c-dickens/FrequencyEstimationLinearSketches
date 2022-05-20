#include <iostream>
#include <vector>
#include "counting_sketches.h"
#include "count_min_sketch.h"

using namespace std ;

int main() {
    /* Make basic zipfian data whose frequencies half as each index increases
     * eg
     * {1 appears 2^(number_of_items) times,
     * 2 appears 2^(number_of_items - 1) times,
     * ...
     * (number_of_items - 1) appears 2 times.
    */
    int number_of_items = 10 ;
    std::vector<uint64_t> data(number_of_items) ;
    std::vector<uint64_t> frequencies(number_of_items) ;

    // Populate data vector
    for(int i=0; i < number_of_items; i++){
        data.at(i) = i;
        frequencies.at(i) = 1 << (number_of_items - i) ;
    }
    for(int i=0; i < number_of_items; i++){
        cout << data[i] << " " << " Freq: " << frequencies[i] << endl ;
    }

    // Insert the data into the sketch
    uint64_t n_hashes = 3, n_buckets=10, seed=100 ;
    CountMinSketch CM(n_hashes, n_buckets, seed) ;
    std::pair<uint64_t, uint64_t> sketch_shape = CM.get_table_shape() ;
    cout << "Table shape: (" << sketch_shape.first << ", " << sketch_shape.second << ")" << std::endl ;
    for(int i=0 ; i < number_of_items ; i++) {
        uint64_t value = data[i] ;
        uint64_t freq = frequencies[i] ;
        CM.update(value, freq) ;
    }

    // Output the results:
    for(int i=0; i < number_of_items; i++){
        cout << data[i]   << " "
             << " Freq: " << frequencies[i]
             << "  Est: " << CM.get_estimate(i)
             << "  UB:  " << CM.get_upper_bound(i)
             << "  LB:  " << CM.get_lower_bound(i)
             << endl ;
    }


    return 0 ;
}
