#ifndef FIFO_H
#define FIFO_H
#include "systemc.h"

class fifo : public sc_module {

public:
    sc_in_clk clock {"clk"}; // Clock input of the design
    sc_in<bool> sreset_n {"sreset_n"}; // negative high, synchronous Reset input
    sc_in<bool> push {"push"}; //push signal
    sc_in<bool> pop {"pop"}; //pop signa;
    sc_in<char> data_in {"data_in"}; //data input
    sc_out<char> data_out {"data_out"}; //data output
    sc_out<bool> empty {"empty"}; //empty signal
    sc_out<bool> full {"full"}; // full signal

    uint size;
    char* data;
    uint pos; //position
    sc_signal<sc_uint<32>> pos_pop; //pop pointer
    sc_signal<sc_uint<32>> pos_push; //push pointer
    sc_signal<bool> push_over; //push pointer overflow  signal
    sc_signal<bool> pop_over; //pop pointer overflow

public:
    void push_method();
    void pop_method();
    void current_state();

    SC_HAS_PROCESS(fifo);
    fifo(sc_module_name name, uint size = 8) : sc_module(name), size(size) {
        data = new char[size];

        SC_CTHREAD(push_method, clock.pos());
        reset_signal_is(sreset_n, false);
        SC_CTHREAD(pop_method, clock.pos());
        reset_signal_is(sreset_n, false);
        SC_METHOD(current_state);
        sensitive << pos_pop << pos_push << sreset_n;
    }
};

#endif // FIFO_H
