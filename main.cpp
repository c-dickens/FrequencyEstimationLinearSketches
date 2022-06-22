#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include <iostream>
#include <vector>
#include <cmath>
#include "counting_sketches.h"
#include "count_min_sketch.h"
#include "catch.hpp"


using namespace std ;

TEST_CASE("Testing COUNTING SKETCH constructors", "[constructors]"){
    std::cout << "Testing COUNTING SKETCH constructors." << std::endl ;
    uint64_t n_hashes = 2 ;
    uint64_t n_buckets = 5 ;
    uint64_t seed = 1;


    CountingSketch C(n_hashes, n_buckets, seed) ;
    REQUIRE(C.get_num_hashes() == n_hashes) ;
    REQUIRE(C.get_num_buckets() == n_buckets) ;
    std::pair<uint64_t, uint64_t> shape = C.get_table_shape();
    REQUIRE(shape.first == n_hashes) ;
    REQUIRE(shape.second == n_buckets) ;

    // Ensures that the initial table is entirely zero throughout.
    std::vector<std::vector<int64_t>> s = C.get_table() ;
    for (int i = 0; i<n_hashes; i++) {
        for(int j = 0; j < n_buckets ; j++){
            REQUIRE(s[i][j] == 0) ;
        }
    } // end loop through sketch
    REQUIRE(C.get_estimate(100) == 0) ;  // any item should have frequency of zero before population

}

TEST_CASE("Testing COUNT MIN constructors", "[constructors]"){
    std::cout << "Testing COUNT MIN constructors." << std::endl ;
    uint64_t n_hashes = 2 ;
    uint64_t n_buckets = 5 ;
    uint64_t seed = 1;
    float eps = exp(1.0) / float(n_buckets) ;
    float delta = 1.0 / exp(float(n_hashes)) ;

    // This is testing an empty sketch
    // To do: how do we assert that the sketch table S has S_ij == 0 and they are all
    // the correct type?
    CountMinSketch C(n_hashes, n_buckets, seed) ;
    REQUIRE(C.get_num_hashes() == n_hashes) ;
    REQUIRE(C.get_num_buckets() == n_buckets) ;
    REQUIRE(C.get_epsilon() == eps) ;
    REQUIRE(C.get_delta() == delta) ;
    std::pair<uint64_t, uint64_t> shape = C.get_table_shape();
    REQUIRE(shape.first == n_hashes) ;
    REQUIRE(shape.second == n_buckets) ;
}

TEST_CASE("Testing COUNT MIN after updates", "[updates]"){
    std::cout << "Testing COUNT MIN updates." << std::endl ;
    uint64_t n_hashes = 2 ;
    uint64_t n_buckets = 5 ;
    uint64_t seed = 1;

    // This is testing an empty sketch
    // TODO: how do we assert that the sketch table S has S_ij == 0 and they are all
    // the correct type?
    CountMinSketch C(n_hashes, n_buckets, seed) ;
    REQUIRE(C.get_total_weight() == 0) ;
    float sketch_epsilon = C.get_epsilon() ;

    // ensure sketch does not accept negative **items**
    REQUIRE_THROWS(C.update(-1), "Item must be nonnegative.") ;

    // Update with weight of 1 implicitly defined
    int64_t x = 1 ;
    int64_t upper, lower ;
    C.update(x) ;
    int64_t estimate = C.get_estimate(x) ;
    REQUIRE(C.get_total_weight() == 1) ;
    REQUIRE(estimate >= 1) ; // Item frequency estimate should be at least true frequency
    REQUIRE(estimate <= 1 + sketch_epsilon*C.get_total_weight()) ;

    upper = C.get_upper_bound(x) ;
    lower = C.get_lower_bound(x) ;
    REQUIRE(estimate >= lower) ;
    REQUIRE(estimate <= upper) ;

    // Now we "remove" the item x=1 by inserting it with a weight of -1
    C.update(x, -1) ;
    REQUIRE(C.get_total_weight() == 0) ; // Ensure the mass of one reduces by one to zero
    estimate = C.get_estimate(x) ;
    lower = C.get_lower_bound(x) ;
    upper = C.get_upper_bound(x) ;
    REQUIRE(estimate >= lower) ;
    REQUIRE(estimate <= upper) ;
}

TEST_CASE("Testing hash & bucket suggestions", "[configuration]"){
    std::cout << "Testing bucket settings." << std::endl ;

    uint64_t n_hashes = 2 ;
    uint64_t n_buckets = 5 ;
    uint64_t seed = 1;

    // Should always return 0
    //CountingSketch C(n_hashes, n_buckets, seed) ;
    REQUIRE(CountingSketch::suggest_num_buckets(1.0) == 0) ;
    REQUIRE(CountingSketch::suggest_num_buckets(0.5) == 0) ;

    //CountMinSketch CM(n_hashes, n_buckets, seed) ;

    // Bucket suggestions
    REQUIRE_THROWS(CountMinSketch::suggest_num_buckets(-1.0), "Confidence must be between 0 and 1.0 (inclusive)." ) ;
    REQUIRE(CountMinSketch::suggest_num_buckets(0.2) == 14) ;
    REQUIRE(CountMinSketch::suggest_num_buckets(0.1) == 28) ;
    REQUIRE(CountMinSketch::suggest_num_buckets(0.05) == 55) ;
    REQUIRE(CountMinSketch::suggest_num_buckets(0.01) == 272) ;

    // Hash suggestions
    REQUIRE_THROWS(CountMinSketch::suggest_num_hashes(10.0), "Confidence must be between 0 and 1.0 (inclusive)." ) ;
    REQUIRE_THROWS(CountMinSketch::suggest_num_hashes(-1.0), "Confidence must be between 0 and 1.0 (inclusive)." ) ;
    // TODO: CHECK THE FLOATS IN 1.0 - x CORRESPOND TO CORRECT STDDEV AMOUNTS DUE TO BIASING IN
    // THE COUNT MIN SKETCH PROCEDURE.
    REQUIRE(CountMinSketch::suggest_num_hashes(0.682689492) == 2) ; // 1 STDDEV
    REQUIRE(CountMinSketch::suggest_num_hashes(0.954499736) == 4) ; // 2 STDDEV
    REQUIRE(CountMinSketch::suggest_num_hashes(0.997300204) == 6) ; // 3 STDDEV
}

TEST_CASE("Testing COUNT MIN SKETCH single stream", "[performance - single]"){
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
        data[i] = i;
        frequencies[i] = 1 << (number_of_items - i) ;
    }
    for(int i=0; i < number_of_items; i++){
        cout << data[i] << " " << " Freq: " << frequencies[i] << endl ;
    }
    float relative_error = 0.1 ;
    float confidence = 0.99 ;
    uint64_t n_buckets = CountMinSketch::suggest_num_buckets(relative_error) ;
    uint64_t n_hashes = CountMinSketch::suggest_num_hashes(confidence) ;
    uint64_t seed = 100 ;
    cout << n_buckets << std::endl ;
    cout << n_hashes << std::endl ;
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
        int64_t est = CM.get_estimate(i) ;
        int64_t upp = CM.get_upper_bound(i) ;
        int64_t low = CM.get_lower_bound(i) ;
        std::cout << data[i]   << " "
             << " Freq: " << frequencies[i]
             << "  Est: " << est
             << "   UB: " << upp
             << "   LB: " << low
             << endl ;
        REQUIRE(est <= upp) ;
        REQUIRE(est >= low) ;
    }
}


TEST_CASE("Testing COUNT MIN SKETCH merged stream", "[performance - merge]"){


    float relative_error = 0.25 ;
    float confidence = 0.9 ;
    uint64_t n_buckets = CountMinSketch::suggest_num_buckets(relative_error) ;
    uint64_t n_hashes = CountMinSketch::suggest_num_hashes(confidence) ;
    uint64_t seed = 100 ;
    CountMinSketch s(n_hashes, n_buckets, seed) ;
    std::vector<uint64_t> s_config = s.get_config() ;
    REQUIRE(s_config[0] == n_hashes) ;
    REQUIRE(s_config[1] == n_buckets) ;
    REQUIRE(s_config[2] == seed) ;


    // Generate sketches that we cannot merge into ie they disagree on at least one of the config entries
    // TODO: implement more general merge procedure so different hashes or buckets are permitted.
    CountMinSketch s1(n_hashes+1, n_buckets, seed) ; // incorrect number of hashes
    CountMinSketch s2(n_hashes, n_buckets+1, seed) ;// incorrect number of buckets
    CountMinSketch s3(n_hashes, n_buckets, seed+1) ;// incorrect seed
    std::vector<CountMinSketch> sketches = {s1, s2, s3};

    // Fail cases
    REQUIRE_THROWS(s.merge(s), "Cannot merge a sketch with itself." ) ;
    for(CountMinSketch sk : sketches){
        REQUIRE_THROWS(s.merge(sk), "Incompatible sketch config." ) ;
    }

    // Passing case
    CountMinSketch t(s_config[0], s_config[1], s_config[2]) ;
    s.merge(t) ;

    std::vector<uint64_t> data = {0,1,2,3,4,5};
    for(auto d: data){
        s.update(d) ;
        t.update(d) ;
    }
    s.merge(t);

    REQUIRE(s.get_total_weight() == 2*t.get_total_weight());
    for (int64_t ii=0; ii < 6; ++ii) {
        REQUIRE(s.get_estimate(ii)==2);
    }

}

// int main() {
//    return 0 ;
//}
