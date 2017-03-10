#include "systemc.h"
#include "shift_reg.h"

int sc_main(int argc, char* argv[])
{
    sc_clock clock("clock", 5, SC_NS);
    sc_signal<bool> reset_n;
    sc_signal<bool> areset_n;
    sc_signal<bool> ena;
    sc_signal<sc_uint<8>> data;
    sc_signal<bool> in;
    sc_signal<sc_uint<8>> data_out;
    sc_signal<bool> out;

    // Connect the DUT
    shift_reg shift_reg_test("shift_reg");
    shift_reg_test.clock(clock);
    shift_reg_test.reset_n(reset_n);
    shift_reg_test.areset_n(areset_n);
    shift_reg_test.ena(ena);
    shift_reg_test.in(in);
    shift_reg_test.data_out(data_out);
    shift_reg_test.out(out);

    // Open VCD file
    sc_trace_file *wf = sc_create_vcd_trace_file("shift_reg_waveform");

    // Dump the desired signals
    sc_trace(wf, clock, "clock");
    sc_trace(wf, reset_n, "reset_n");
    sc_trace(wf, areset_n, "areset_n");
    sc_trace(wf, ena, "ena");
    sc_trace(wf, in, "in");
    sc_trace(wf, data_out, "data_out");
    sc_trace(wf, out, "out");

    //Test code
    ena = 1;
    in = 0;
    reset_n = 1;
    areset_n = 0;
    cout << "@" << sc_time_stamp() << " Asserting async reset_n\n" << endl;
    sc_start(5, SC_NS);
    areset_n = 1;
    sc_start(4, SC_NS);

    //shift check
    in = 1;
    sc_start(1, SC_NS);
    sc_start(5, SC_NS);
    assert(data_out.read() == 0);
    sc_start(5, SC_NS);
    assert(data_out.read() == 1);
    sc_start(2, SC_NS);
    in = 0;
    sc_start(3, SC_NS);
    assert(data_out.read() == 3);
    sc_start(5, SC_NS);
    assert(data_out.read() == 7);

    sc_start(5, SC_NS);
    assert(data_out.read() == 0x0e);// 0000_1110
    sc_start(5, SC_NS);
    assert(data_out.read() == 0x1c);// 0001_1100
    sc_start(5, SC_NS);
    assert(data_out.read() == 0x38);// 0011_1000

    //synch reset check
    sc_start(2, SC_NS);
    uint tmp = data_out.read(); //save current state
    cout << endl << "tmp = " << tmp << endl;
    reset_n = 0;
    sc_start(1, SC_NS);
    assert(data_out.read() == tmp);
    sc_start(4, SC_NS);
    assert(data_out.read() == 0);
    sc_start(3, SC_NS);
    reset_n = 1;

    in = 1;
    for(int i = 0; i<5; i++) sc_start(5, SC_NS);
    sc_start(1, SC_NS);
    areset_n = 0;
    sc_start(1, SC_NS);
    assert(data_out.read() == 0);
    sc_start(7, SC_NS);
    areset_n = 1;

    sc_start(10, SC_NS);
    cout << "@" << sc_time_stamp() << " Terminating simulation\n" << endl;
    sc_close_vcd_trace_file(wf);
return 0;
}
