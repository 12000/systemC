/*#include "systemc.h"
#include "ram.h"
#include "cmd_counter.h"

//========ram_test===================================================

int sc_main(int argc, char* argv[]){
    sc_clock clk("clk", 5, SC_NS);
    sc_signal<bool> en;
    sc_signal<bool> rw;
    sc_signal<bool> ask;
    sc_signal<sc_uint<ADDR_SIZE> > addr;
    sc_signal<bool> rdy;
    sc_signal <sc_uint<DATA_SIZE> > data_in;
    sc_signal <sc_uint<DATA_SIZE> > data_out;


    // Connect the DUT
    ram_mem ram_memory("ram_memory");
    ram_memory.clk(clk);
    ram_memory.en(en);
    ram_memory.rw(rw);
    ram_memory.ask(ask);
    ram_memory.addr(addr);
    ram_memory.rdy(rdy);
    ram_memory.data_in(data_in);
    ram_memory.data_out(data_out);

    // Open VCD file
    sc_trace_file *wf = sc_create_vcd_trace_file("ram_memory_vcd");

    // Dump the desired signals
    sc_trace(wf, clk, "clk");
    sc_trace(wf, en, "en");
    sc_trace(wf, rw, "rw");
    sc_trace(wf, addr, "addr");
    sc_trace(wf, ask, "ask");
    sc_trace(wf, rdy, "rdy");
    sc_trace(wf, data_in, "data_in");
    sc_trace(wf, data_out, "data_out");

    //Test code
    en = 0;
    rw = 0;
    ask = 0;
    cout << "@" << sc_time_stamp() << " Asserting sync reset_n\n" << endl;
    sc_start(5, SC_NS);

    en = 1;
    ask = 1;
    rw = 0; //write
    for(uint i = 0; i < 20; ++i){
        addr = i;
        data_in = i+4;
        sc_start(5, SC_NS);
    }
    ask = 0;
    sc_start(5, SC_NS);

    //read
    rw = 1;
    addr = 10;
    ask = 1;
    sc_start(5, SC_NS);
    ask = 0;
    sc_start(5, SC_NS);
    addr = 16;
    ask = 1;
    sc_start(5, SC_NS);
    ask = 0;
    sc_start(20, SC_NS);
    cout << "@" << sc_time_stamp() << " Terminating simulation\n" << endl;
    sc_close_vcd_trace_file(wf);
return 0;
}
//===========================================================================
*/
/*
int sc_main(int argc, char* argv[])
{
    sc_clock clk("clk", 5, SC_NS);
    //to ram
    sc_signal<bool> en_ram;
    sc_signal<bool> rw_ram;
    sc_signal<bool> ask_ram;
    sc_signal<bool> rdy_ram;
    sc_signal <sc_uint<DATA_SIZE> > data_ram;

    //to cmd_counter
    sc_signal<bool> ena_cnt;
    sc_signal<bool> reset_cnt;
    sc_signal<bool> cmd_up_cnt;
    sc_signal<bool> addr_up_cnt;
    sc_signal<bool> load_addr_cnt;
    sc_signal<sc_uint<32>> addr_data_cnt;

    //common
    sc_signal<sc_uint<ADDR_SIZE> > addr;

    // Connect the DUT
    //ram
    ram_mem ram_memory("ram_memory");
    ram_memory.clk(clk);
    ram_memory.en(en_ram);
    ram_memory.rw(rw_ram);
    ram_memory.addr(addr);
    ram_memory.ask(ask_ram);
    ram_memory.rdy(rdy_ram);
    ram_memory.data(data_ram);

    //counter
    cmd_counter counter("cnt_counter");
    counter.clk(clk);
    counter.ena(ena_cnt);
    counter.reset(reset_cnt);
    counter.cmd_up(cmd_up_cnt);
    counter.addr_up(addr_up_cnt);
    counter.load_addr(load_addr_cnt);
    counter.addr_data(addr_data_cnt); //to load
    counter.addr_out(addr);

    // Open VCD file
    sc_trace_file *wf = sc_create_vcd_trace_file("ram+counter.vcd");

    // Dump the desired signals
    sc_trace(wf, clk, "clk");
    sc_trace(wf, en_ram, "en_ram");
    sc_trace(wf, rw_ram, "rw_ram");
    sc_trace(wf, addr, "addr");
    sc_trace(wf, ask_ram, "ask_ram");
    sc_trace(wf, rdy_ram, "rdy_ram");
    sc_trace(wf, data_ram, "data_ram");

    sc_trace(wf, ena_cnt, "ena_cnt");
    sc_trace(wf, reset_cnt, "reset_cnt");
    sc_trace(wf, cmd_up_cnt, "cmd_up_cnt");
    sc_trace(wf, addr_up_cnt, "addr_up_cnt");
    sc_trace(wf, load_addr_cnt, "load_addr_cnt");
    sc_trace(wf, addr_data_cnt, "addr_data_cnt");


    //Test code
    en_ram = 0;
    rw_ram = 0;
    addr = 0;

    reset_cnt = 1;
    ena_cnt = 0;
    load_addr_cnt = 0;
    cmd_up_cnt = 0;
    addr_up_cnt = 0;
    cout << "@" << sc_time_stamp() << " Asserting sync reset_n\n" << endl;
    sc_start(5, SC_NS);

    //mem init (0 - write, 1 - read)
    cout << "mem_init" << endl;
    en_ram = 1;
    ask_ram = 1;
    rw_ram = 0; //write
    for(uint i=0; i < 32; i++){
        addr = i;
        if(i == 3 || i == 13)
            data_ram = 88;
        else
            data_ram = i;
        sc_start(5, SC_NS);
    }
    ask_ram = 0;
    sc_start(5, SC_NS);

    //reading data
    cout << "data reading" << endl;
    reset_cnt = 0;
    ena_cnt = 1;
    sc_start(5, SC_NS);
    ask_ram = 1;
    rw_ram = 1; //read
    sc_start(5, SC_NS);

    for(uint i=0; i<20; ++i){
        if(i == 13)
            ask_ram = 0;
        else
            ask_ram = 1;
        cmd_up_cnt = 1;
        addr_up_cnt = 1;
        sc_start(5, SC_NS);
    }
    //addr = 0;
    //sc_start(5, SC_NS);
    //for(uint i=0; i<25; i++){
    //    addr = addr.read() + i;
    //    sc_start(5, SC_NS);
    //}
    sc_start(20, SC_NS);

    cmd_up_cnt = 0;
    addr_up_cnt = 0;
    sc_start(5, SC_NS);
    sc_start(20, SC_NS);

    cout << "end" << endl;
    en_ram = 0;
    ena_cnt = 0;
    sc_start(20, SC_NS);

    cout << "@" << sc_time_stamp() << " Terminating simulation\n" << endl;
    sc_close_vcd_trace_file(wf);
return 0;
}
*/
