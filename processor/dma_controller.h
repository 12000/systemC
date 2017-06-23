#ifndef DMA_CONTROLLER_H
#define DMA_CONTROLLER_H
#include "systemc.h"
#include "constants.h"

class dma_controller: public sc_module {

public:
    //inputs
    sc_in_clk clk;
    sc_in<bool> en;
    sc_in<bool> rst;

    //from devices
    //0
    sc_in<bool> request_0;
    sc_in<sc_uint<ADDR_SIZE>> addr_in_0;
    sc_in<sc_int<DATA_SIZE>> data_in_0;
    sc_in<bool> rw_in_0;
    //1
    sc_in<bool> request_1;
    sc_in<sc_uint<ADDR_SIZE>> addr_in_1;
    sc_in<sc_int<DATA_SIZE>> data_in_1;
    sc_in<bool> rw_in_1;
    //2
    sc_in<bool> request_2;
    sc_in<sc_uint<ADDR_SIZE>> addr_in_2;
    sc_in<sc_int<DATA_SIZE>> data_in_2;
    sc_in<bool> rw_in_2;

    //to devices
    sc_out<bool> rdy;
    sc_out<sc_int<DATA_SIZE>> data_out;
    sc_out<bool> ack_0;
    sc_out<bool> ack_1;
    sc_out<bool> ack_2;
    sc_out<bool> dack_0;
    sc_out<bool> dack_1;
    sc_out<bool> dack_2;

    //to memory
    sc_out<bool> rw_out;
    sc_out<sc_uint<ADDR_SIZE>> addr_to_ram;
    sc_out<sc_int<DATA_SIZE>> data_to_ram;
    sc_out<bool> mem_request;

    //from memory
    sc_in<sc_int<DATA_SIZE>> data_from_ram;
    sc_in<bool> mem_ack;

public:
    sc_signal<uint> device_num;
    sc_signal<sc_uint<ADDR_SIZE>> addr_in_reg;
    sc_signal<sc_int<DATA_SIZE>> data_in_reg;
    sc_signal<bool> rw_reg;

public:
    void prc_dma_controller();

    SC_HAS_PROCESS(dma_controller);
    dma_controller(sc_module_name name) : sc_module(name) {
        SC_CTHREAD(prc_dma_controller, clk.pos());
            reset_signal_is(rst, true);
    }
};
#endif // DMA_CONTROLLER_H
