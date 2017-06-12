#include "systemc.h"
#include "fifoparam.h"
#include "fifoparamextended.h"

int sc_main(int argc, char* argv[])
{
    sc_clock clock("clock", 5, SC_NS);
    sc_signal<bool> sreset_n;
    sc_signal<bool> push;
    sc_signal<bool> pop;
    sc_signal<char> data_in;
    sc_signal<char> data_out;
    sc_signal<bool> empty;
    sc_signal<bool> full;
    //----------------------------------------------
    sc_signal<bool> almost_empty;
    sc_signal<bool> almost_full;

    // Connect the DUT
    fifoParamExtended <int16_t, 16> test_fifo("test_fifo_1");
    //fifoParam <uint> test_fifo("test_fifo");
    test_fifo.clock(clock);
    test_fifo.sreset_n(sreset_n);
    test_fifo.push(push);
    test_fifo.pop(pop);
    test_fifo.data_in(data_in);
    test_fifo.data_out(data_out);
    test_fifo.empty(empty);
    test_fifo.full(full);
    //-----------------------------------------------
    test_fifo.almost_empty(almost_empty);
    test_fifo.almost_full(almost_full);

    // Open VCD file
    sc_trace_file *wf = sc_create_vcd_trace_file("fifoParamExtended_test_1_waveform");

    // Dump the desired signalsF
    sc_trace(wf, clock, "clock");
    sc_trace(wf, sreset_n, "sreset_n");
    sc_trace(wf, push, "push");
    sc_trace(wf, pop, "pop");
    sc_trace(wf, data_in, "data_in");
    sc_trace(wf, data_out, "data_out");
    sc_trace(wf, empty, "empty");
    sc_trace(wf, full, "full");
    //----------------------------------------------
    sc_trace(wf, almost_empty, "almost_empty");
    sc_trace(wf, almost_full, "almost_full");

    //Test code
    sreset_n = 0;
    push = 0;
    pop = 0;
    data_in = 6;
    cout << "@" << sc_time_stamp() << " Asserting sync reset_n\n" << endl;
    sc_start(5, SC_NS);
    sreset_n = 1;

    push = 1;
    for(uint i=0; i < test_fifo.size + 3; i++){
        sc_start(5, SC_NS);
        assert(test_fifo.empty.read() == 0);
        data_in = data_in + 1;
        if(i == test_fifo.size-1){
            assert(test_fifo.full.read() == 1);
        }
    }
    sc_start(5, SC_NS);

    push = 0;
    pop = 1;
    for(uint i=0; i < test_fifo.size-1; i++){
        sc_start(5, SC_NS);
        assert(test_fifo.empty.read() == 0);
        assert(test_fifo.full.read() == 0);
    }
    sc_start(20, SC_NS);
    assert(test_fifo.empty.read() == 1);

    data_in = data_in + 1;
    push = 1;
    sc_start(5, SC_NS);
    data_in = data_in + 1;
    sc_start(5, SC_NS);
    pop = 1;

    for(uint i=0; i < test_fifo.size + 10; i++){
        data_in = data_in + 1;
        sc_start(5, SC_NS);
        assert(test_fifo.empty.read() == 0);
        assert(test_fifo.full.read() == 0);
        assert(test_fifo.data_out.read() == (char)(data_in-1));
    }


    sc_start(20, SC_NS);
    //--------------
    sreset_n = 0;
    //--------------
    sc_start(2, SC_NS);
    assert(test_fifo.empty.read() == 1);
    assert(test_fifo.full.read() == 0);
    sc_start(48, SC_NS);

    cout << "@" << sc_time_stamp() << " Terminating simulation\n" << endl;
    sc_close_vcd_trace_file(wf);
return 0;
}
