# Courtesty of the official Repository at: https://github.com/emeryberger/Hoard/tree/master/benchmarks

# The benchmarks will be used to evaluate other mallocs against our implementation of Hoard.

Concurrent Memory Allocator Benchmarks

Emery Berger <emery@cs.umass.edu>
http://www.cs.umass.edu/~emery


This is the suite of concurrent benchmarks used in the paper "Hoard: A
Scalable Memory Allocator for Multithreaded Applications". If you use
these benchmarks, please cite that paper as follows:

@inproceedings{Berger:2000:HSM:378993.379232,
 author = {Berger, Emery D. and McKinley, Kathryn S. and Blumofe, Robert D. and Wilson, Paul R.},
 title = {Hoard: a scalable memory allocator for multithreaded applications},
 booktitle = {Proceedings of the ninth international conference on Architectural support for programming languages and operating systems},
 series = {ASPLOS-IX},
 year = {2000},
 isbn = {1-58113-317-0},
 location = {Cambridge, Massachusetts, United States},
 pages = {117--128},
 numpages = {12},
 url = {http://doi.acm.org/10.1145/378993.379232},
 doi = {http://doi.acm.org/10.1145/378993.379232},
 acmid = {379232},
 publisher = {ACM},
 address = {New York, NY, USA},
} 

Here is a brief description of the benchmarks, including usage and
sample parameters.  P denotes the number of processors (cores) in your
system.

* cache-scratch:
 
  This benchmark is referred to in the paper as "passive-false", and
  tests resilience against passive false sharing (see the paper for
  details).

  Parameters: <threads> <inner-loop> <object-size> <iterations>

  % cache-scratch 1 100 8 1000000
  % cache-scratch P 100 8 1000000

* cache-thrash:

  This benchmark is referred to in the paper as "active-false", and
  also tests resilience against active false sharing (see the paper
  for details).

  It uses the same parameters as cache-scratch.

* larson:

  This benchmark is courtesy of Paul Larson at Microsoft Research. It
  simulates a server: each thread allocates and deallocates objects,
  and then transfers some objects (randomly selected) to other threads
  to be freed.

  Parameters: (from the command line)
  <seconds> <min-obj-size> <max-obj-size> <objects> <iterations> <rng seed> <num-threads>

  Suggested parameters: ./larson 10 7 8 1000 10000 1 P

* shbench:

  A benchmark where each thread allocates and randomly frees a number
  of random-sized objects.

  You may download the source from:

    http://www.microquill.com/smartheap/shbench/bench.zip

  NOTE: this benchmark differs from the one used in the Hoard paper,
  but it is unfortunately not possible to include that code here.

* threadtest:

  A basic benchmark that measures raw per-thread allocation
  throughput: each thread repeatedly allocates and then deallocates
  100,000/P objects.

  Parameters: <number-of-threads> <iterations> <num-objects> <work-interval> <object-size>

  % threadtest P 1000 10000 0 8


Additional benchmarks not in the original Hoard paper:

* linux-scalability:

  This benchmark from the University of Michigan also tests allocator
  throughput.

  Parameters: <object-size> <iterations> <number-of-threads>
  Example: 8 10000000 P
