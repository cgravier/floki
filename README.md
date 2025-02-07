## Algorithms

Research level implementations of some SIMD algorithms. 

### Access Aligned Sort

Implementation based on the algorithm presented in this [blog post](http://seven-degrees-of-freedom.blogspot.com/2010/07/question-of-sorts.html)

The original [paper](http://www.dia.eui.upm.es/asignatu/pro_par/articulos/AASort.pdf) is here. 



#### Example Usage

```cpp

std::vector<int32_t> values
    = { 0,    4,    6,    20,   40,   60,   90,   155,
        1188, 2002, 2244, 2296, 3124, 3226, 3334, 4443,
        10,   24,   46,   120,  140,  260,  390,  455,
        2188, 3002, 4244, 5296, 6124, 6226, 6334, 6443 };

floki::sort(begin(values),end(values));

```

## Tested With

Clang 3.4 on Linux

There are some known issues with g++ and Boost SIMD.  See the comments about aliasing on this [issue](https://github.com/MetaScale/nt2/issues/741).  Until this is resolved, it won't compile without setting the following flags.  


```
 -fno-strict-aliasing -DBOOST_SIMD_NO_STRICT_ALIASING

 ```




## Requires

[Boost](http://boost.org)

[Boost SIMD](http://nt2.metascale.fr/doc/html/the_boost_simd_library.html)

Optional unit tests require the [Bandit](http://banditcpp.org/)
## Benchmark


```
Intel(R) Core(TM) i7-4770S CPU @ 3.10GHz

compiled with clang 3.4 and flags -O3 -mavx
```

65536 random elements in std::vector


data type  | std::sort | floki::sort
------------- | ------------- | -------------
int32_t  | 2.99ms | 0.92ms
float    | 3.33ms | 1.10ms

## Building

There is a CMake build file included that builds the benchmark and unit tests. 

I generate the make file using clang as follows

```
mkdir build-floki
cd build-floki

CXX=/usr/bin/clang++-3.5 cmake ~/coding/sorting//floki/ -DCMAKE_CXX_FLAGS="-march=native -O3"

make
```

Be sure to use the `O3` flag for performance !

If Boost SIMD is installed to a path other than /usr/local, then set BoostSIMD_INCLUDE_DIR accordingly.



## Modifications brought by this fork

The benchmark is now run using hard-coded variables (quick an dirty test!) that are two `std:vector` :
- one for specifying the set of number of elements in the vector to be sorted.
- one for specifying the set of ranges in which the elements values are drawn from a uniform distribution. Each range value encode the max value in the distribution, the minimum is 0. The distribution is therefore drawn from [0;range_value], for each range_value in the `ranges` vector.

The benchmark is then run for each possible pairs of ( #elements; #range ).

The vector is checked to be sorted for verification.
A dirty for useful standard ouput prints the benchmark results.


