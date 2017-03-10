#ifndef SHIFT_REG_H
#define SHIFT_REG_H

#include <systemc.h>
SC_MODULE(shift_reg) {
    sc_in_clk clock; // Clock input of the design
    sc_in<bool> reset_n; // negative high, synchronous Reset input
    sc_in<bool> areset_n; //asynchronous reser, negative high
    sc_in<bool> ena;
    sc_in<bool> in; //bit input
    sc_out<sc_uint<8> > data_out; // 8 bit vector input
    sc_out<bool> out; // output

    //------------Local Variables Here---------------------
    sc_signal<sc_uint<8>> data;

    //------------Code Starts Here-------------------------
    // Below function implements actual register logic

    void shift();

    //Constructor
    SC_CTOR(shift_reg) :
            clock("clock"),
            reset_n("reset_n"),
            areset_n("areset_n"),
            ena("ena"),
            data_out("data_out"),
            out("out") {
        SC_CTHREAD(shift, clock.pos());
        reset_signal_is(reset_n, false);
        async_reset_signal_is(areset_n, false);
    } // End of Constructor

}; // End of Module

#endif // SHIFT_REG_H
