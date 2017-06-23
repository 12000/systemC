#ifndef ALU_H
#define ALU_H
#include "constants.h"

class alu: public sc_module {
public:
    //in
    sc_in_clk clk;
    sc_in<bool> en;
    sc_in<bool> rst;
    sc_in<bool> exec; //start execute command
    sc_in<sc_uint<RON_NUM_SIZE>> ron_number_1;
    sc_in<sc_uint<RON_NUM_SIZE>> ron_number_2;
    sc_in<sc_int<DATA_SIZE>> data_in;
    sc_in<sc_uint<5>> operation_type;
    sc_in<sc_uint<5>> operation_number;
    sc_in<sc_int<DATA_SIZE>> data_ron_in;
    sc_in<sc_int<DATA_SIZE>> GPI;

    //out
    sc_out<sc_int<DATA_SIZE>> data_ron_out;
    sc_out<bool> rdy;
    sc_out<sc_uint<6>> flags_out;

public:
    sc_signal<sc_int<DATA_SIZE>> ron [32];
    sc_signal<sc_int<DATA_SIZE>> buffer;
    sc_signal<sc_int<DATA_SIZE*2>> mul_tmp;
    sc_signal<sc_uint<6>> flags;

private:
    void execute();

public:
    SC_HAS_PROCESS(alu);
    alu(sc_module_name name) : sc_module(name) {
        SC_CTHREAD(execute, clk.pos());
            reset_signal_is(rst, true);
    }
};
#endif // ALU_H
