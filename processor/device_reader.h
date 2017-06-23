#ifndef DEVICE_READER_H
#define DEVICE_READER_H
#include "systemc.h"
#include "constants.h"

class device_reader: public sc_module {

public:
    //inputs
    sc_in_clk clk;
    sc_in<bool> rst;
    sc_in<bool> en;

    //from dma
    sc_in<bool> dma_rdy;
    sc_in<bool> dma_dack;

    //from dma
    sc_in<sc_int<DATA_SIZE>> data_in;
    sc_in<bool> dma_ack;

    //to dma
    sc_out<bool> request_out;
    sc_out<bool> rw_out;
    sc_out<sc_uint<ADDR_SIZE>> addr_out;
    sc_out<sc_int<DATA_SIZE>> data_out;

public:
    sc_signal<sc_int<DATA_SIZE>> reg;
    sc_signal<uint> waiting_counter;

public:
    void prc_device_reader();

    SC_HAS_PROCESS(device_reader);
    device_reader(sc_module_name name) : sc_module(name) {
        SC_CTHREAD(prc_device_reader, clk.pos());
        reset_signal_is(rst, true);
    }
};
#endif
