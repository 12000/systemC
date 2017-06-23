#ifndef TOP_H
#define TOP_H
#include "systemc.h"
#include "constants.h"
#include "stdio.h"
#include <iomanip>
#include "decoder.h"
#include "ram.h"
#include "alu.h"
#include "cmd_counter.h"
#include "memory_controller.h"
#include "cache.h"

class top: public sc_module {
public:
    sc_in<bool> en;
    sc_in_clk clk;
    sc_in<bool> rst;

    sc_out<bool> rw_out;                    // to dma from mem controller
    sc_out<bool> request_to_dma;            // to dma from mem controller
    sc_out<sc_int<DATA_SIZE>> data_out;     // to dma from mem controller
    sc_out<sc_uint<ADDR_SIZE>> addr_out;    // to dma from mem controller

    sc_out<sc_uint<6>> state_out;
    sc_out<bool> stop;

    sc_in<bool> dma_rdy;
    sc_in<bool> dma_dack;
    sc_in<bool> dma_ack;
    sc_in<sc_int<DATA_SIZE>> data_in;       // data from memory (from dma)

    sc_in<sc_int<DATA_SIZE>> GPI;
    sc_out<sc_int<DATA_SIZE>> GPO;

private:
    sc_signal<bool> alu_rdy;
    sc_signal<sc_uint<6>> flags_in;
    sc_signal<sc_int<DATA_SIZE>> data_ron_in;
    sc_signal<bool> memory_rdy;

    //to controller
    sc_signal<bool> mem_controller_en;
    sc_signal<bool> mem_controller_rw;
    sc_signal<bool> mem_controller_load;
    sc_signal<sc_int<DATA_SIZE>> mem_controller_load_data;
    sc_signal<sc_uint<ADDR_SIZE>> mem_controller_load_addr;
    //to counter
    sc_signal<bool> counter_en;
    sc_signal<bool> cmd_up;
    sc_signal<bool> addr_up;
    sc_signal<bool> counter_load;
    sc_signal<sc_uint<ADDR_SIZE>> counter_load_addr;
    //to alu
    sc_signal<bool> alu_en;
    sc_signal<sc_int<DATA_SIZE>> alu_data_out;
    sc_signal<sc_uint<RON_NUM_SIZE>> ron_number_1_out;
    sc_signal<sc_uint<RON_NUM_SIZE>> ron_number_2_out;
    sc_signal<sc_uint<5>> cmd_type_out;
    sc_signal<sc_uint<5>> cmd_num_out;
    sc_signal<sc_int<DATA_SIZE>> data_ron_out;
    sc_signal<bool> exec;


    //counter signals
    sc_signal<bool> ask_from_counter; //to controller
    sc_signal<sc_uint<ADDR_SIZE>> addr_from_counter; //to to controller

public:
    sc_signal<sc_int<DATA_SIZE>> GPI_reg;
    //sc_signal<sc_int<DATA_SIZE>> GPO_reg;
    void GPIO_process();

public:
    decoder dcdr;
    alu aluu;
    cmd_counter counter;
    memory_controller controller;

    SC_HAS_PROCESS(top);
    top(sc_module_name name) : sc_module(name), dcdr("dcdr"), aluu("aluu"), counter("counter"), controller("mem_contoller") {
        dcdr.clk(clk);
        dcdr.en(en);
        dcdr.rst(rst);
        dcdr.alu_rdy(alu_rdy);
        dcdr.memory_rdy(dma_ack);       // from dma
        dcdr.flags_in(flags_in);        // from alu
        dcdr.data_in(data_in);          // from dma
        dcdr.data_ron_in(data_ron_in);  // from alu
        dcdr.data_ron_out(data_ron_out);// from alu

        dcdr.mem_controller_en(mem_controller_en);
        dcdr.mem_controller_rw(mem_controller_rw);
        dcdr.mem_controller_load_data(mem_controller_load_data);
        dcdr.mem_controller_load_addr(mem_controller_load_addr);
        dcdr.mem_controller_load(mem_controller_load);

        dcdr.counter_en(counter_en);
        dcdr.cmd_up(cmd_up);
        dcdr.addr_up(addr_up);
        dcdr.counter_load_addr(counter_load_addr);
        dcdr.counter_load(counter_load);
        dcdr.alu_data_out(alu_data_out);
        dcdr.alu_en(alu_en);
        dcdr.state_out(state_out);
        dcdr.ron_number_1_out(ron_number_1_out);
        dcdr.ron_number_2_out(ron_number_2_out);
        dcdr.cmd_type_out(cmd_type_out);
        dcdr.cmd_num_out(cmd_num_out);
        dcdr.exec(exec);
        dcdr.stop(stop);

        aluu.clk(clk);
        aluu.rst(rst);
        aluu.exec(exec);
        aluu.ron_number_1(ron_number_1_out);
        aluu.ron_number_2(ron_number_2_out);
        aluu.data_in(alu_data_out);
        aluu.operation_type(cmd_type_out);
        aluu.operation_number(cmd_num_out);
        aluu.data_ron_in(data_ron_out);
        aluu.data_ron_out(data_ron_in);
        aluu.flags_out(flags_in);
        aluu.rdy(alu_rdy);
        aluu.en(alu_en);
        aluu.GPI(GPI_reg);

        counter.clk(clk);
        counter.reset(rst);
        counter.ena(counter_en);
        counter.cmd_up(cmd_up);
        counter.addr_up(addr_up);
        counter.load_addr(counter_load);
        counter.addr_data(counter_load_addr);
        counter.request(ask_from_counter);
        counter.addr_out(addr_from_counter);

        controller.clk(clk);
        controller.rst(rst);
        controller.en(mem_controller_en);
        controller.rw_in(mem_controller_rw);            // from decoder
        controller.load(mem_controller_load);           // from decoder
        controller.load_addr(mem_controller_load_addr); // from decoder
        controller.load_data(mem_controller_load_data); // from decoder
        controller.addr_in(addr_from_counter);          // from counter
        controller.request_in(ask_from_counter);            // from counter
        controller.request_out(request_to_dma);         // to dma (out)
        controller.rw_out(rw_out);                      // to dma (out)
        controller.addr_out(addr_out);                  // to dma (out)
        controller.data_out(data_out);                  // to dma (out)
        controller.dma_rdy(dma_rdy);
        controller.dma_dack(dma_dack);

        SC_CTHREAD(GPIO_process, clk.pos());
            reset_signal_is(rst, true);
    }
};

#endif // TOP_H
