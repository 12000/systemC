#ifndef CACHE_H
#define CACHE_H
#include "systemc.h"
#include "constants.h"

const int TAG_SIZE = ADDR_SIZE - CACHE_ADDR_SIZE;
const int INDEX_SIZE = CACHE_ADDR_SIZE;

class cache: public sc_module {
public:
    sc_in<bool> en;
    sc_in_clk clk;

    //from processor
    sc_in<bool> ask;
    sc_in<sc_uint<ADDR_SIZE>> addr; //addr 11
    sc_in <sc_int<DATA_SIZE>> data_in; //32
    sc_in<bool> rw_in;

    //to processor
    sc_out<bool> rdy;
    sc_out <sc_int<DATA_SIZE>> data_out;

    //from ram
    sc_in<bool> ram_rdy;
    sc_in<sc_int<DATA_SIZE>> data_from_ram;

    //to ram
    sc_out<bool> rw_out;
    sc_out<bool> ask_ram;
    sc_out<sc_uint<ADDR_SIZE>> addr_to_ram_out;
    sc_out<sc_int<DATA_SIZE>> data_to_ram_out;

public:
    //sc_int<CACHE_DATA_SIZE> cache_mem[CACHE_SIZE];
    sc_signal<sc_int<35>> cache_mem[CACHE_SIZE];
    sc_signal<bool> valid[CACHE_SIZE];
    sc_signal<sc_uint<TAG_SIZE>> tag;
    sc_signal<sc_uint<INDEX_SIZE>> index;

public:
    void prc_cache();

    SC_HAS_PROCESS(cache);
    cache(sc_module_name name) : sc_module(name) {
        for(uint i=0; i<CACHE_SIZE; ++i)
            valid[i] = 0;
        SC_CTHREAD(prc_cache, clk.pos());
    }
};
#endif // CACHE_H
