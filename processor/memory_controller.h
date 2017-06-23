#ifndef MEMORY_CONTROLLER_H
#define MEMORY_CONTROLLER_H
#include "systemc.h"
#include "constants.h"

class memory_controller: public sc_module {

public:
    //inputs
    sc_in_clk clk;
    sc_in<bool> rst;

    //from decoder
    sc_in<bool> en;
    sc_in<bool> rw_in;
    sc_in<bool> load;
    sc_in<sc_int<DATA_SIZE>> load_data;
    sc_in<sc_uint<ADDR_SIZE>> load_addr;

    //from counter
    sc_in<sc_uint<ADDR_SIZE> > addr_in;
    sc_in<bool> request_in; //from counter

    //from dma
    sc_in<bool> dma_rdy;
    sc_in<bool> dma_dack;

    //outputs
    sc_out<bool> request_out; //to dma
    sc_out<bool> rw_out; //to dma
    sc_out<sc_uint<ADDR_SIZE>> addr_out;
    sc_out<sc_int<DATA_SIZE>> data_out;

public:
    sc_signal<uint> waiting_counter;

public:
    void prc_mem_controller();

    SC_HAS_PROCESS(memory_controller);
    memory_controller(sc_module_name name) : sc_module(name) {
        SC_CTHREAD(prc_mem_controller, clk.pos());
        reset_signal_is(rst, true);
    }
};
#endif // MEMORY_CONTROLLER_H
