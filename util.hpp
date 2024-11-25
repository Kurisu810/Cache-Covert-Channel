// You may only use fgets() to pull input from stdin
// You may use any print function to stdout to print 
// out chat messages
#include <stdio.h>
#include <string>
#include <bitset>
#include <math.h>
#include <chrono>
#include <cstring>

// You may use memory allocators and helper functions 
// (e.g., rand()).  You may not use system().
#include <stdlib.h>

#include <inttypes.h>
#include <time.h>

#ifndef UTIL_H_
#define UTIL_H_

#define ADDR_PTR uint64_t 
#define CYCLES uint32_t

CYCLES measure_one_block_access_time(ADDR_PTR addr);

#endif

// Define L1 and LLC parameters
#define CACHE_LINE_SIZE 64 // Bytes 

#define LLC_SIZE (6 * 1024 * 1024) //  6 MB
#define L1_SIZE (32 * 1024)        // 32 KB

#define LLC_ASSOCIATIVITY 12
#define L1_ASSOCIATIVITY 8

#define LLC_NUM_SETS (LLC_SIZE / CACHE_LINE_SIZE / LLC_ASSOCIATIVITY) // 8192
#define L1_NUM_SETS (L1_SIZE / CACHE_LINE_SIZE / L1_ASSOCIATIVITY)    //   64

// Define clock period
#define PERIOD_US 100000

std::string char_to_binary_string(char input_char);
std::string binary_string_to_string(const std::string& binary_string);
