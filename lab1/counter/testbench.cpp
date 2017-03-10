#include "systemc.h"
#include "counter.h"

int sc_main(int argc, char* argv[])
{
    sc_clock clock("clock", 5, SC_NS);
    sc_signal<bool> reset_n;
    sc_signal<bool> areset_n;
    sc_signal<bool> load;
    sc_signal<sc_uint<8>> data;
    sc_signal<sc_uint<8>> cnt_out;

    // Connect the DUT
    counter test_counter("test_counter");
    test_counter.clock(clock);
    test_counter.reset_n(reset_n);
    test_counter.areset_n(areset_n);
    test_counter.load(load);
    test_counter.data(data);
    test_counter.cnt_out(cnt_out);

    // Open VCD file
    sc_trace_file *wf = sc_create_vcd_trace_file("counter_waveform");

    // Dump the desired signals
    sc_trace(wf, clock, "clock");
    sc_trace(wf, reset_n, "reset_n");
    sc_trace(wf, areset_n, "areset_n");
    sc_trace(wf, load, "load");
    sc_trace(wf, data, "data");
    sc_trace(wf, cnt_out, "cnt_out");

    //Test code
    data = 0;
    load = 0;
    reset_n = 1;
    areset_n = 0;
    cout << "@" << sc_time_stamp() << " Asserting async reset_n\n" << endl;
    sc_start(5, SC_NS);
    areset_n = 1;

    //count check
    for (int i=0; i<16; i++){
        sc_start(5, SC_NS);
        assert(cnt_out.read() == i+1);
    }

    //load check
    sc_start(4, SC_NS);
    data = 5;
    load = 1;
    sc_start(2, SC_NS);
    load = 0;
    sc_start(2, SC_NS);
    assert(cnt_out.read() == 5);
    sc_start(2, SC_NS); //cnt+1
    for (int i=0; i<16; i++){
        sc_start(5, SC_NS);
        assert(cnt_out.read() == i+6);
    }

    //synch reset check
    sc_start(2, SC_NS);
    int tmp = cnt_out.read();
    cout << endl << "tmp = " << tmp << endl;
    reset_n = 0;
    sc_start(1, SC_NS);
    assert(cnt_out.read() == tmp);
    sc_start(4, SC_NS);
    assert(cnt_out.read() == 0);
    sc_start(3, SC_NS);
    reset_n = 1;

    //asynch reset check
    for (int i=0; i<5; i++){
        sc_start(5, SC_NS);
    }
    sc_start(1, SC_NS);
    areset_n = 0;
    sc_start(1, SC_NS);
    assert(cnt_out.read() == 0);
    sc_start(7, SC_NS);
    areset_n = 1;

    sc_start(10, SC_NS);
    cout << endl;


    cout << "@" << sc_time_stamp() << " Terminating simulation\n" << endl;
    sc_close_vcd_trace_file(wf);
return 0;
}
