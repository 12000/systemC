#include "systemc.h"
#include "fifoParam.h"

int sc_main(int argc, char* argv[])
{
    sc_clock clock("clock", 5, SC_NS);
    //--- signal for char test---------------
    sc_signal<bool> char_sreset_n;
    sc_signal<bool> char_push;
    sc_signal<bool> char_pop;
    sc_signal<char> char_data_in;
    sc_signal<char> char_data_out;
    sc_signal<bool> char_empty;
    sc_signal<bool> char_full;
    //---------------------------------------
    //--- signal for int test---------------
    sc_signal<bool> int_sreset_n;
    sc_signal<bool> int_push;
    sc_signal<bool> int_pop;
    sc_signal<char> int_data_in;
    sc_signal<char> int_data_out;
    sc_signal<bool> int_empty;
    sc_signal<bool> int_full;
    //---------------------------------------


    // Connect the DUT
    const uint size = 15;
    fifoParam <char> char_test_fifo("char_test_fifo", size);
    char_test_fifo.clock(clock);
    char_test_fifo.sreset_n(char_sreset_n);
    char_test_fifo.push(char_push);
    char_test_fifo.pop(char_pop);
    char_test_fifo.data_in(char_data_in);
    char_test_fifo.data_out(char_data_out);
    char_test_fifo.empty(char_empty);
    char_test_fifo.full(char_full);

    fifoParam <int8_t> int_test_fifo("int_test_fifo", size);
    int_test_fifo.clock(clock);
    int_test_fifo.sreset_n(int_sreset_n);
    int_test_fifo.push(int_push);
    int_test_fifo.pop(int_pop);
    int_test_fifo.data_in(int_data_in);
    int_test_fifo.data_out(int_data_out);
    int_test_fifo.empty(int_empty);
    int_test_fifo.full(int_full);

    // Open VCD file
    sc_trace_file *wf = sc_create_vcd_trace_file("fifoParam_test_1_waveform");

    // Dump the desired signalsF
    sc_trace(wf, clock, "clock");
    //-----char signals---------------------
    sc_trace(wf, char_sreset_n, "char_sreset_n");
    sc_trace(wf, char_push, "char_push");
    sc_trace(wf, char_pop, "char_pop");
    sc_trace(wf, char_data_in, "char_data_in");
    sc_trace(wf, char_data_out, "char_data_out");
    sc_trace(wf, char_empty, "char_empty");
    sc_trace(wf, char_full, "char_full");
    //--------------------------------------
    //-----int signals---------------------
    sc_trace(wf, int_sreset_n, "int_sreset_n");
    sc_trace(wf, int_push, "int_push");
    sc_trace(wf, int_pop, "int_pop");
    sc_trace(wf, int_data_in, "int_data_in");
    sc_trace(wf, int_data_out, "int_data_out");
    sc_trace(wf, int_empty, "int_empty");
    sc_trace(wf, int_full, "int_full");
    //--------------------------------------

    //Test code
    //char example
    char_sreset_n = 0;
    char_push = 0;
    char_pop = 0;
    char_data_in = 'A';
    cout << "@" << sc_time_stamp() << " Asserting sync reset_n\n" << endl;
    sc_start(5, SC_NS);
    char_sreset_n = 1;

    //push test (pop == 0)
    char_push = 1;
    for(uint i=0; i < char_test_fifo.size + 3; i++){
        sc_start(5, SC_NS);
        assert(char_test_fifo.empty.read() == 0);
        char_data_in = char_data_in + 1;
        if(i == char_test_fifo.size-1){
            assert(char_test_fifo.full.read() == 1);
        }
    }
    sc_start(5, SC_NS);

    // pop test (push == 0)
    char_push = 0;
    char_pop = 1;
    for(uint i=0; i < char_test_fifo.size-1; i++){
        sc_start(5, SC_NS);
        assert(char_test_fifo.empty.read() == 0);
        assert(char_test_fifo.full.read() == 0);
    }
    sc_start(20, SC_NS);
    assert(char_test_fifo.empty.read() == 1);

    // pop && push together
    char_data_in = char_data_in + 1;
    char_push = 1;
    sc_start(5, SC_NS);
    char_data_in = char_data_in + 1;
    sc_start(5, SC_NS);
    char_pop = 1;

    for(uint i=0; i < char_test_fifo.size + 10; i++){
        char_data_in = char_data_in + 1;
        sc_start(5, SC_NS);
        assert(char_test_fifo.empty.read() == 0);
        assert(char_test_fifo.full.read() == 0);
        assert(char_test_fifo.data_out.read() == (char)(char_data_in-1));
    }

    sc_start(20, SC_NS);
    //--------------
    char_sreset_n = 0;
    //--------------
    sc_start(2, SC_NS);
    assert(char_test_fifo.empty.read() == 1);
    assert(char_test_fifo.full.read() == 0);
    sc_start(48, SC_NS);

    cout << "@" << sc_time_stamp() << " Terminating char simulation\n" << endl;
    //sc_close_vcd_trace_file(wf);


    //int example
    int_sreset_n = 0;
    int_push = 0;
    int_pop = 0;
    int_data_in = 5;
    cout << "@" << sc_time_stamp() << " Asserting sync reset_n\n" << endl;
    sc_start(5, SC_NS);
    int_sreset_n = 1;

    // push test (pop == 0)
    int_push = 1;
    for(uint i=0; i < int_test_fifo.size + 3; i++){
        sc_start(5, SC_NS);
        assert(int_test_fifo.empty.read() == 0);
        int_data_in = int_data_in + 1;
        if(i == int_test_fifo.size-1){
            assert(int_test_fifo.full.read() == 1);
        }
    }
    sc_start(5, SC_NS);

    // pop test (push == 0)
    int_push = 0;
    int_pop = 1;
    for(uint i=0; i < int_test_fifo.size-1; i++){
        sc_start(5, SC_NS);
        assert(int_test_fifo.empty.read() == 0);
        assert(int_test_fifo.full.read() == 0);
    }
    sc_start(20, SC_NS);
    assert(int_test_fifo.empty.read() == 1);

    // pop && push together
    int_data_in = int_data_in + 1;
    int_push = 1;
    sc_start(5, SC_NS);
    int_data_in = int_data_in + 1;
    sc_start(5, SC_NS);
    int_pop = 1;

    for(uint i=0; i < int_test_fifo.size + 10; i++){
        int_data_in = int_data_in + 1;
        sc_start(5, SC_NS);
        assert(int_test_fifo.empty.read() == 0);
        assert(int_test_fifo.full.read() == 0);
        assert(int_test_fifo.data_out.read() == (int8_t)(int_data_in-1));
    }

    sc_start(20, SC_NS);
    //--------------
    int_sreset_n = 0;
    //--------------
    sc_start(2, SC_NS);
    assert(int_test_fifo.empty.read() == 1);
    assert(int_test_fifo.full.read() == 0);
    sc_start(48, SC_NS);

    cout << "@" << sc_time_stamp() << " Terminating int8 simulation\n" << endl;
    sc_close_vcd_trace_file(wf);

return 0;
}
