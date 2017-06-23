#ifndef DECODER_H
#define DECODER_H
#include "constants.h"

class decoder: public sc_module {
public:
    //in
    sc_in_clk clk;
    sc_in<bool> en;
    sc_in<bool> rst;

    sc_in<bool> alu_rdy;
    sc_in<sc_uint<6>> flags_in;
    sc_in<sc_int<DATA_SIZE>> data_ron_in;
    sc_in<bool> memory_rdy;
    sc_in<sc_int<DATA_SIZE>> data_in;

    //out
    //controller
    sc_out<bool> mem_controller_en;
    sc_out<bool> mem_controller_rw;
    sc_out<bool> mem_controller_load;
    sc_out<sc_int<DATA_SIZE>> mem_controller_load_data;
    sc_out<sc_uint<ADDR_SIZE>> mem_controller_load_addr;

    //counter
    sc_out<bool> counter_en;
    sc_out<bool> cmd_up;
    sc_out<bool> addr_up;
    sc_out<bool> counter_load;
    sc_out<sc_uint<ADDR_SIZE>> counter_load_addr;

    //alu
    sc_out<bool> alu_en;
    sc_out<sc_int<DATA_SIZE>> alu_data_out;
    sc_out<sc_uint<RON_NUM_SIZE>> ron_number_1_out;
    sc_out<sc_uint<RON_NUM_SIZE>> ron_number_2_out;
    sc_out<sc_uint<5>> cmd_type_out;
    sc_out<sc_uint<5>> cmd_num_out;
    sc_out<sc_int<DATA_SIZE>> data_ron_out;
    sc_out<bool> exec;

    //other
    sc_out<bool> stop;

    //state (additional output)
    sc_out<sc_uint<6>> state_out;


public:
    sc_signal<sc_uint<4>> cmd_type_reg;
    sc_signal<sc_uint<4>> cmd_num_reg;
    sc_signal<sc_int<DATA_SIZE>> alu_data_reg;
    sc_signal<sc_uint<RON_NUM_SIZE>> ron_number_1_reg;
    sc_signal<sc_uint<RON_NUM_SIZE>> ron_number_2_reg;
    sc_signal<sc_int<DATA_SIZE>> data_to_ron_reg;
    sc_signal<sc_int<DATA_SIZE>> mem_controller_load_data_reg;
    sc_signal<sc_uint<ADDR_SIZE>> mem_controller_load_addr_reg;
    sc_signal<sc_uint<ADDR_SIZE>> counter_load_addr_reg;
    sc_signal<sc_uint<5>> state;

public:
    void get_next_state();

    SC_HAS_PROCESS(decoder);
    decoder(sc_module_name name) : sc_module(name) {
        SC_CTHREAD(get_next_state, clk.pos());
                reset_signal_is(rst, true);
    }
};
#endif // DECODER_H

