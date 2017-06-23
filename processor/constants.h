#include "systemc.h"
#ifndef CONSTANTS_H
#define CONSTANTS_H

const int DATA_SIZE = 32;
const int ADDR_SIZE = 11;
const int MEM_SIZE = 2048;
const int CACHE_ADDR_SIZE = 8;
const int CACHE_DATA_SIZE = DATA_SIZE + (ADDR_SIZE - CACHE_ADDR_SIZE);
const int CACHE_SIZE = 256;
const int MAX_VALUE = 65535;
const int RON_NUM = 32;
const int RON_NUM_SIZE = 5;
#endif // CONSTANTS_H
