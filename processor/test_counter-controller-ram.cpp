/*#include "systemc.h"
#include "ram.h"
#include "cmd_counter.h"
#include "memory_controller.h"

int sc_main(int argc, char* argv[])
{
    sc_clock clk("clk", 5, SC_NS);
    //to ram
    sc_signal<bool> en_ram; //in
    sc_signal<bool> rdy_ram;//out
    sc_signal <sc_uint<DATA_SIZE> > data_ram; //inout

    //to cmd_counter
    sc_signal<bool> ena_cnt; //in
    sc_signal<bool> reset_cnt; //in
    sc_signal<bool> cmd_up_cnt;//in
    sc_signal<bool> addr_up_cnt;//in
    sc_signal<bool> load_addr_cnt;//in
    sc_signal<sc_uint<32>> addr_data_cnt;//in

    //to controller
    sc_signal<bool> en_controller;
    sc_signal<bool> rw_controller_from_decoder;


    //common
    sc_signal<sc_uint<ADDR_SIZE>> addr_cnt_to_controller; //to controller from counter
    sc_signal<sc_uint<ADDR_SIZE>> addr_controler_to_ram; // to ram from controller
    sc_signal<bool> request_contoller_to_ram;
    sc_signal<bool> ask_cnt_to_controller;
    sc_signal<bool> rw_controller_to_ram;

    // Connect the DUT
    //ram
    ram_mem ram_memory("ram_memory");
    ram_memory.clk(clk);//in
    ram_memory.en(en_ram);//in
    ram_memory.rw(rw_controller_to_ram);//common
    ram_memory.addr(addr_controler_to_ram);//common
    ram_memory.ask(request_contoller_to_ram);//common
    ram_memory.rdy(rdy_ram);//out
    ram_memory.data(data_ram);//inout

    //mem_controller
    memory_controller controller("contoller");
    controller.clk(clk);
    controller.en(en_controller);//in
    controller.addr_in(addr_cnt_to_controller);//common
    controller.rw_in(rw_controller_from_decoder);//in
    controller.ask_in(ask_cnt_to_controller);//common
    controller.rw_out(rw_controller_to_ram);//out
    controller.request(request_contoller_to_ram);//out
    controller.addr_out(addr_controler_to_ram);//out

    cout << "132";
    //counter
    cmd_counter counter("cmd_counter");
    counter.clk(clk);
    counter.ena(ena_cnt); //in
    counter.reset(reset_cnt);//in
    counter.cmd_up(cmd_up_cnt);//in
    counter.addr_up(addr_up_cnt);//in
    counter.load_addr(load_addr_cnt);//in
    counter.addr_data(addr_data_cnt); //to load in
    counter.ask(ask_cnt_to_controller); //out
    counter.addr_out(addr_cnt_to_controller);//out

    // Open VCD file
    sc_trace_file *wf = sc_create_vcd_trace_file("counter-controller-ram.vcd");

    // Dump the desired signals
    sc_trace(wf, clk, "clk");

    sc_trace(wf, ena_cnt, "ena_cnt");
    sc_trace(wf, reset_cnt, "reset_cnt");
    sc_trace(wf, addr_up_cnt, "addr_up_cnt");
    sc_trace(wf, addr_cnt_to_controller, "addr_cnt_to_controller");

    sc_trace(wf, rw_controller_from_decoder, "rw_controller_from_decoder");
    sc_trace(wf, en_controller, "en_controller");
    sc_trace(wf, ask_cnt_to_controller, "ask_cnt_to_controller");
    sc_trace(wf, request_contoller_to_ram, "request_to_ram");
    sc_trace(wf, addr_controler_to_ram, "addr_to_ram");
    sc_trace(wf, rw_controller_to_ram, "rw_to_ram");

    sc_trace(wf, en_ram, "en_ram");
    sc_trace(wf, data_ram, "data_ram");
    sc_trace(wf, rdy_ram, "rdy_ram");



    //Test code
    //counter
    ena_cnt = 0;
    reset_cnt = 1;
    cmd_up_cnt = 0;
    addr_up_cnt = 0;
    load_addr_cnt = 0; // -
    addr_data_cnt = 0; // -

    //contoller
    en_controller = 0;
    rw_controller_from_decoder = 0; //write - 0, read - 1

    //ram
    en_ram = 0;

    cout << "@" << sc_time_stamp() << " Asserting sync reset_n\n" << endl;
    sc_start(5, SC_NS);

    reset_cnt = 0;
    sc_start(5, SC_NS);

    ena_cnt = 1;
    en_controller = 1;
    en_ram = 1;
    cout << "start" << endl;
    sc_start(5, SC_NS);

    //mem init (0 - write, 1 - read)
    data_ram = 1488;
    rw_controller_from_decoder = 0; // write
    addr_up_cnt = 1;
    cout << "mem init" << endl;
    sc_start(5, SC_NS);

    addr_up_cnt = 0;
    sc_start(20, SC_NS);
    reset_cnt = 1;
    sc_start(5, SC_NS);
    data_ram = 0;
    sc_start(25, SC_NS);

    reset_cnt = 0;
    addr_up_cnt = 1;
    rw_controller_from_decoder = 1; //read
    sc_start(5, SC_NS);

    addr_up_cnt = 0;
    rw_controller_from_decoder = 1; //read
    sc_start(5, SC_NS);

    addr_up_cnt = 1;
    rw_controller_from_decoder = 1; //read
    sc_start(5, SC_NS);

    addr_up_cnt = 1;
    rw_controller_from_decoder = 1; //read
    sc_start(5, SC_NS);
    addr_up_cnt = 0;
    reset_cnt = 1;
    sc_start(5, SC_NS);
    reset_cnt = 0;
    sc_start(25, SC_NS);

    addr_up_cnt = 1;
    rw_controller_from_decoder = 1; //read
    sc_start(5, SC_NS);
    addr_up_cnt = 0;
    sc_start(5, SC_NS);

    sc_start(50, SC_NS);
    cout << "end" << endl;
    cout << "@" << sc_time_stamp() << " Terminating simulation\n" << endl;
    sc_close_vcd_trace_file(wf);
return 0;
}
*/
