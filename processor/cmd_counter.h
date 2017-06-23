#include "systemc.h"
#include "constants.h"
#ifndef CMD_COUNTER_H
#define CMD_COUNTER_H

class cmd_counter: public sc_module{
public:
    sc_in_clk clk;
    sc_in<bool> reset;
    sc_in<bool> ena;
    sc_in<bool> cmd_up;
    sc_in<bool> addr_up;
    sc_in<bool> load_addr;
    sc_in<sc_uint<ADDR_SIZE>> addr_data;

    sc_out<bool> request;
    sc_out<sc_uint<ADDR_SIZE>> addr_out;

    //var
public:
    sc_signal<sc_uint<ADDR_SIZE>> commands;
    sc_signal<sc_uint<ADDR_SIZE>> address;
    //functions
public:
    void cmd_count();
    void addr_count();

    SC_HAS_PROCESS(cmd_counter);
    cmd_counter(sc_module_name name): sc_module(name){
        SC_CTHREAD(cmd_count, clk.pos());
        reset_signal_is(reset, true);
        SC_CTHREAD(addr_count, clk.pos());
        reset_signal_is(reset, true);
    }
};
#endif // CMD_COUNTER_H
