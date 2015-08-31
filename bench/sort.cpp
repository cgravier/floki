
#include <limits>
#include <vector>
#include <chrono>
#include <iostream>
#include <random>
#include <functional>

#ifdef SIMD_BENCH
#include <floki/aa_sort.hpp>
#else
#include <algorithm>
#endif

using namespace std::chrono;

void displayResults(const std::vector <int32_t> & elements, const std::vector <int32_t> & range, int ** results) {

       std::cout << std::endl << std::endl << "Benchmarks results in miliseconds (ms). Lines are range values, and columns are the number of elements in that range that were sorted:" << std::endl;
       std::cout << "=============================================================" << std::endl;
       std::cout << " | \t\t | ";
       for (std::vector<int>::size_type elem = 0; elem != elements.size(); elem++) {
           std::cout << elements[elem] << " \t | ";
       }
       std::cout << std::endl;

       for (std::vector<int>::size_type ran = 0; ran != range.size(); ran++) {
            std::cout << " | " << range[ran] << " \t | \t";
            for (std::vector<int>::size_type elem = 0; elem != elements.size(); elem++) {
                std::cout << results[ran][elem] << " \t | \t";
            }
    	   std::cout << std::endl;
        }
        std::cout << std::endl << "=============================================================" << std::endl;

}


template <typename T> void random_test(const std::vector <int32_t> & elements, const std::vector <int32_t> & range, size_t iterations, const char* description)
{

    // 2D array for storing the results
    int **results = new int*[range.size()];
    for(int i = 0; i < elements.size(); ++i) {
    	results[i] = new int[elements.size()];
    }

    for (std::vector<int>::size_type ran = 0; ran != range.size(); ran++) {
        
        for (std::vector<int>::size_type elem = 0; elem != elements.size(); elem++) {

            int valrange = range[ran];
            int valnumber = elements[elem];


	    std::cout << std::endl << std::endl << "Elements : " << valnumber << " randomly picked using a uniform distribution in a [0;"<< valrange << "] range. " << std::endl;

            std::vector<T> values(elements[elem]);

            typedef typename std::conditional
                <std::is_integral<T>::value, typename std::uniform_int_distribution<T>,
                 typename std::uniform_real_distribution<T>>::type distribution_t;

            distribution_t distribution(0,valrange);
            std::mt19937 engine;
            auto generator = std::bind(distribution, engine);
            std::generate_n(begin(values), valnumber, generator);

            double total = 0;

            //std::cout << "starting benchmark : for a range of " << valrange << " values with " << valnumber << " elements, sorting " << elements << " " << description << "'s for " << iterations << " iterations. " << std::endl;
            std::cout << "Starting benchmark ...  ";

            for (int i = 0; i < iterations; ++i)
            {
                std::random_shuffle(values.begin(),values.end());
                auto start = system_clock::now();
        #ifdef SIMD_BENCH
                floki::sort(values.begin(), values.end());
        #else
                std::sort(values.begin(), values.end());
        #endif

                auto end = system_clock::now();
                total += (duration_cast<duration<float, std::milli>>(end - start)).count();
            }

	    // checks if the vector is sorted. If yes, stay silent, otherwise stop the benchmark.
	   if (!std::is_sorted(values.begin(), values.end())) {
		std::cout << "The presumably sorted vector is actually not sorted. The sort algorithm may be bogus.\nExiting the benchmark ..." << std::endl;
		exit(-1);
	   }
		
            //std::cout << "Sorted " << valnumber << " elements, " << iterations << " times in " << total << " ms. mean " << total / iterations <<  "ms. Values taken in [ " << *std::min_element(values.begin(),values.end()) << " ; " << *std::max_element(values.begin(),values.end()) << "]"  <<  std::endl;
	    results[ran][elem] = total;
	   std::cout << "Done.";
        }
    }
    displayResults(elements, range, results);

}



int main(int argc, char **argv)
{
    size_t iterations = 1;
    const int sizeElem = 3;
    const int sizeRange = 3;

    std::vector <int32_t> elements(sizeElem);
    elements[0] = 500000;
    elements[1] = 1000000;
    elements[2] = 5000000;
    //elements[3] = 10000000;
    //elements[4] = 25000000;
    //elements[5] = 50000000;

    std::vector <int32_t> range(sizeRange);
    range[0] = 500000;
    range[1] = 1000000;
    range[2] = 5000000;
    //range[3] = 100000000;
    //range[4] = 250000000;
    //range[5] = 500000000;

    uint32_t mode = 0;

    if (argc > 1)
        mode = atoi(argv[1]);

    switch (mode) {
    case 1:
        random_test<float>(elements,range,iterations,"float");
        break;
    case 2:
        random_test<double>(elements,range,iterations,"double");
        break;
    default:
        random_test<int32_t>(elements,range,iterations,"int32_t");
    }

    return 0;
}

