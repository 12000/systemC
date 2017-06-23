/*#include "systemc.h"
#include "decoder.h"

int sc_main(int argc, char* argv[])
{
    //in signals
    sc_clock clk("clk", 5, SC_NS);
    sc_signal<bool> en {"en"};
    sc_signal<bool> rst {"rst"};

    sc_signal<bool> alu_rdy {"alu_rdy"};
    sc_signal<sc_uint<6>> flags_in;
    sc_signal<sc_uint<DATA_SIZE>> data_ron_in;
    sc_signal<bool> memory_rdy {"memory_rdy"};
    sc_signal<sc_uint<DATA_SIZE>> data_in {"data_in"};

    //out
    //controller
    sc_signal<bool> mem_controller_en;
    sc_signal<bool> mem_controller_rw;
    sc_signal<bool> mem_controller_load;
    sc_signal<sc_uint<DATA_SIZE>> mem_controller_load_data;
    sc_signal<sc_uint<ADDR_SIZE>> mem_controller_load_addr;

    //counter
    sc_signal<bool> counter_en;
    sc_signal<bool> cmd_up;
    sc_signal<bool> addr_up;
    sc_signal<bool> counter_load;
    sc_signal<sc_uint<ADDR_SIZE>> counter_load_addr;

    //alu
    sc_signal<bool> alu_en;
    sc_signal<bool> alu_data_out;
    sc_signal<sc_uint<RON_NUM_SIZE>> ron_number_1_out;
    sc_signal<sc_uint<RON_NUM_SIZE>> ron_number_2_out;
    sc_signal<sc_uint<4>> cmd_type_out;
    sc_signal<sc_uint<4>> cmd_num_out;
    sc_signal<sc_uint<DATA_SIZE>> data_ron_out;
    sc_signal<bool> exec;

    //state (additional output)
    sc_signal<sc_uint<5>> state_out;

    sc_signal<bool> stop;


    // Connect the DUT

    decoder dcdr("decoder");
    dcdr.clk(clk);
    dcdr.en(en);
    dcdr.rst(rst);
    dcdr.alu_rdy(alu_rdy);
    dcdr.memory_rdy(memory_rdy);
    dcdr.flags_in(flags_in);
    dcdr.data_in(data_in);
    dcdr.data_ron_in(data_ron_in);
    dcdr.data_ron_out(data_ron_out);
    dcdr.mem_controller_en(mem_controller_en);
    dcdr.mem_controller_rw(mem_controller_rw);
    dcdr.mem_controller_load_data(mem_controller_load_data);
    dcdr.mem_controller_load_addr(mem_controller_load_addr);
    dcdr.mem_controller_load(mem_controller_load);
    dcdr.counter_en(counter_en);
    dcdr.cmd_up(cmd_up);
    dcdr.addr_up(addr_up);
    dcdr.counter_load_addr(counter_load_addr);
    dcdr.counter_load(counter_load);
    dcdr.alu_data_out(alu_data_out);
    dcdr.alu_en(alu_en);
    dcdr.state_out(state_out);
    dcdr.ron_number_1_out(ron_number_1_out);
    dcdr.ron_number_2_out(ron_number_2_out);
    dcdr.cmd_type_out(cmd_type_out);
    dcdr.cmd_num_out(cmd_num_out);
    dcdr.exec(exec);
    dcdr.stop(stop);

    // Open VCD file
    sc_trace_file *wf = sc_create_vcd_trace_file("decoder_test.vcd");

    // Dump the desired signals
    sc_trace(wf, clk, "clk");
    sc_trace(wf, en, "en");
    sc_trace(wf, rst, "rst");
    sc_trace(wf, alu_rdy, "alu_rdy");
    sc_trace(wf, memory_rdy, "memory_rdy");
    sc_trace(wf, data_in, "data_in");
    sc_trace(wf, mem_controller_en, "mem_controller_en");
    sc_trace(wf, mem_controller_rw, "mem_controller_rw");

    sc_trace(wf, mem_controller_load_data, "mem_controller_load_data");
    sc_trace(wf, mem_controller_load_addr, "mem_controller_load_addr");
    sc_trace(wf, mem_controller_load, "mem_controller_load");

    sc_trace(wf, counter_en, "counter_en");
    sc_trace(wf, cmd_up, "cmd_up");
    sc_trace(wf, addr_up, "addr_up");
    sc_trace(wf, counter_load_addr, "counter_load_addr");
    sc_trace(wf, counter_load, "counter_load");
    sc_trace(wf, alu_en, "alu_en");
    sc_trace(wf, state_out, "state_out");
    sc_trace(wf, alu_data_out, "alu_data_out");
    sc_trace(wf, ron_number_1_out, "ron_number_1_out");
    sc_trace(wf, ron_number_2_out, "ron_number_2_out");
    sc_trace(wf, cmd_type_out, "cmd_type_out");
    sc_trace(wf, cmd_num_out, "cmd_num_out");
    sc_trace(wf, data_ron_out, "data_ron_out");
    sc_trace(wf, data_ron_in, "data_ron_in");
    sc_trace(wf, stop, "stop");

    //Test code
    en = 0;
    rst = 1;
    alu_rdy = 0;
    memory_rdy = 0;
    data_in = 0;
    cout << "@" << sc_time_stamp() << " START\n" << endl;
    sc_start(5, SC_NS);

    rst = 0;
    en = 1;
    sc_start(25, SC_NS);

    data_in = 0x01040301; //cmd

    memory_rdy = 1;
    sc_start(5, SC_NS);
    memory_rdy = 0;
    sc_start(35, SC_NS);

    //prochitali cmd -> state 5

    data_ron_in = 1024; //addr from ron
    alu_rdy = 1;
    sc_start(5, SC_NS);
    alu_rdy = 0;
    sc_start(35, SC_NS);
    // peredali addr na decoder i zhdem mem_rdy s data po etomu addresu

    data_in = 8080;
    memory_rdy = 1;
    sc_start(5, SC_NS);
    memory_rdy = 0;
    sc_start(35, SC_NS);
    //perehod v state 6
    //zhdem alu_rdy

    sc_start(35, SC_NS);
    alu_rdy = 1;
    sc_start(5, SC_NS);
    alu_rdy = 0;
    sc_start(55, SC_NS);
    //cmd executed

    //exec OUT from RON 3

    data_in = 0x01030907; //cmd

    memory_rdy = 1;
    sc_start(5, SC_NS);
    memory_rdy = 0;
    sc_start(35, SC_NS);

    //zhdem alu_rdy

    sc_start(35, SC_NS);
    alu_rdy = 1;
    sc_start(5, SC_NS);
    alu_rdy = 0;
    sc_start(5, SC_NS);

    sc_start(50, SC_NS);

    cout << "end" << endl;
    cout << "@" << sc_time_stamp() << " Terminating simulation\n" << endl;
    sc_close_vcd_trace_file(wf);
return 0;
}
*/
