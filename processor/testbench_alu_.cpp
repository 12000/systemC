//#include "systemc.h"
//#include "alu.h"

//int sc_main(int argc, char* argv[])
//{
//    //in signals
//    sc_clock clk("clk", 5, SC_NS);
//    sc_signal<bool> en;
//    sc_signal<bool> rst;
//    sc_signal<bool> exec;
//    sc_signal<sc_uint<5>> ron_number_1;
//    sc_signal<sc_uint<5>> ron_number_2;
//    sc_signal<sc_int<DATA_SIZE>> data_in;
//    sc_signal<sc_uint<5>> operation_type;
//    sc_signal<sc_uint<5>> operation_number;
//    sc_signal<sc_int<DATA_SIZE>> data_ron_in;
//    sc_signal<sc_int<DATA_SIZE>> GPI;

//    //out signals
//    sc_signal<sc_int<DATA_SIZE>> data_ron_out;
//    sc_signal<bool> rdy;
//    sc_signal<sc_uint<6>> flags_out;

//    // Connect the DUT
//    alu alu_ex("alu");
//    alu_ex.clk(clk);
//    alu_ex.en(en);
//    alu_ex.rst(rst);
//    alu_ex.exec(exec);
//    alu_ex.ron_number_1(ron_number_1);
//    alu_ex.ron_number_2(ron_number_2);
//    alu_ex.data_in(data_in);
//    alu_ex.operation_type(operation_type);
//    alu_ex.operation_number(operation_number);
//    alu_ex.data_ron_in(data_ron_in);
//    alu_ex.data_ron_out(data_ron_out);
//    alu_ex.rdy(rdy);
//    alu_ex.flags_out(flags_out);
//    alu_ex.GPI(GPI);


//    // Open VCD file
//    sc_trace_file *wf = sc_create_vcd_trace_file("alu_test.vcd");

//    // Dump the desired signals
//    sc_trace(wf, clk, "clk");
//    sc_trace(wf, en, "en");
//    sc_trace(wf, rst, "rst");
//    sc_trace(wf, exec, "exec");
//    sc_trace(wf, ron_number_1, "ron_number_1");
//    sc_trace(wf, ron_number_2, "ron_number_2");
//    sc_trace(wf, data_in, "data_in");
//    sc_trace(wf, operation_type, "operation_type");
//    sc_trace(wf, operation_number, "operation_number");
//    sc_trace(wf, data_ron_in, "data_ron_in");

//    sc_trace(wf, data_ron_out, "data_ron_out");
//    sc_trace(wf, rdy, "rdy");
//    sc_trace(wf, flags_out, "flags_out");

//    //Test code
//    en = 0;
//    rst = 1;
//    exec = 0;
//    ron_number_1 = 0;
//    ron_number_2 = 0;
//    data_in = 0;
//    operation_type = 0;
//    operation_number = 0;
//    data_ron_in = 0;
//    sc_start(5, SC_NS);

//    rst = 0;
//    sc_start(5, SC_NS);

//    en = 1;
//    sc_start(5, SC_NS);

//    //1 0
//    operation_type = 1;
//    operation_number = 0;
//    data_ron_in = 20;
//    ron_number_1 = 6;
//    exec = 1;
//    sc_start(5, SC_NS);
//    exec = 0;
//    while(!rdy)
//        sc_start(5, SC_NS);
//    assert(alu_ex.ron[6].read() == 20);

//    //1 1
//    operation_type = 1;
//    operation_number = 1;
//    ron_number_1 = 5;
//    ron_number_2 = 6;
//    exec = 1;
//    sc_start(5, SC_NS);
//    exec = 0;
//    while(!rdy)
//        sc_start(5, SC_NS);
//    assert(alu_ex.ron[5].read() == 20);

//    //1 3
//    operation_type = 1;
//    operation_number = 3;
//    ron_number_1 = 5;
//    exec = 1;
//    sc_start(5, SC_NS);
//    exec = 0;
//    while(!rdy)
//        sc_start(5, SC_NS);
//    assert(data_ron_out.read() == 20);

//    //1 4
//    operation_type = 1;
//    operation_number = 4;
//    data_ron_in = 88;
//    ron_number_1 = 8;
//    exec = 1;
//    sc_start(5, SC_NS);
//    exec = 0;
//    while(!rdy)
//        sc_start(5, SC_NS);
//    assert(alu_ex.ron[8].read() == 88);

//    //1 5
//    operation_type = 1;
//    operation_number = 5;
//    GPI = 77;
//    ron_number_1 = 9;
//    exec = 1;
//    sc_start(5, SC_NS);
//    exec = 0;
//    while(!rdy)
//        sc_start(5, SC_NS);
//    assert(alu_ex.ron[9].read() == 77);

//    //2 0
//    operation_type = 2;
//    operation_number = 0;
//    data_in = 10;
//    ron_number_1 = 8;
//    exec = 1;
//    sc_start(5, SC_NS);
//    exec = 0;
//    while(!rdy)
//        sc_start(5, SC_NS);
//    assert(alu_ex.ron[8].read() == 98);

//    //2 1
//    operation_type = 2;
//    operation_number = 1;
//    ron_number_1 = 5;
//    ron_number_2 = 9;
//    exec = 1;
//    sc_start(5, SC_NS);
//    exec = 0;
//    while(!rdy)
//        sc_start(5, SC_NS);
//    assert(alu_ex.ron[5].read() == 97);


//    //2 2
//    operation_type = 2;
//    operation_number = 2;
//    ron_number_1 = 0;
//    ron_number_2 = 8;
//    data_in = 2;
//    exec = 1;
//    sc_start(5, SC_NS);
//    exec = 0;
//    while(!rdy)
//        sc_start(5, SC_NS);
//    assert(alu_ex.ron[0].read() == 100);

//    //2 4
//    operation_type = 2;
//    operation_number = 4;
//    ron_number_1 = 0;
//    ron_number_2 = 0;
//    data_in = 2;
//    exec = 1;
//    sc_start(5, SC_NS);
//    exec = 0;
//    while(!rdy)
//        sc_start(5, SC_NS);
//    assert(alu_ex.ron[0].read() == 0);

//    //2 5
//    operation_type = 2;
//    operation_number = 5;
//    ron_number_1 = 8;
//    data_in = 8;
//    exec = 1;
//    sc_start(5, SC_NS);
//    exec = 0;
//    while(!rdy)
//        sc_start(5, SC_NS);
//    assert(alu_ex.ron[8].read() == 90);

//    //2 6
//    operation_type = 2;
//    operation_number = 6;
//    ron_number_1 = 8;
//    data_in = 20;
//    exec = 1;
//    sc_start(5, SC_NS);
//    exec = 0;
//    while(!rdy)
//        sc_start(5, SC_NS);
//    assert(alu_ex.ron[8].read() == 70);

//    //2 8
//    operation_type = 2;
//    operation_number = 8;
//    ron_number_1 = 8;
//    exec = 1;
//    sc_start(5, SC_NS);
//    exec = 0;
//    while(!rdy)
//        sc_start(5, SC_NS);
//    assert(alu_ex.ron[8].read() == 71);

//    //2 9
//    operation_type = 2;
//    operation_number = 9;
//    ron_number_1 = 8;
//    exec = 1;
//    sc_start(5, SC_NS);
//    exec = 0;
//    while(!rdy)
//        sc_start(5, SC_NS);
//    assert(alu_ex.ron[8].read() == 70);

//    //2 10
//    operation_type = 2;
//    operation_number = 10;
//    ron_number_1 = 8;
//    ron_number_2 = 5;
//    exec = 1;
//    sc_start(5, SC_NS);
//    exec = 0;
//    while(!rdy)
//        sc_start(5, SC_NS);
//    assert(alu_ex.ron[8].read() == 6790);

//    //3 0
//    operation_type = 3;
//    operation_number = 0;
//    data_in = 0;
//    ron_number_1 = 6;
//    exec = 1;
//    sc_start(5, SC_NS);
//    exec = 0;
//    while(!rdy)
//        sc_start(5, SC_NS);
//    assert(alu_ex.flags.read().bit(0) == 0 && alu_ex.flags.read().bit(1) == 1);

//    //3 1
//    operation_type = 3;
//    operation_number = 1;
//    ron_number_1 = 8;
//    ron_number_2 = 5;
//    exec = 1;
//    sc_start(5, SC_NS);
//    exec = 0;
//    while(!rdy)
//        sc_start(5, SC_NS);
//    assert(alu_ex.flags.read().bit(2) == 1);

//    //3 2
//    operation_type = 3;
//    operation_number = 2;
//    ron_number_1 = 31;
//    ron_number_2 = 0;
//    exec = 1;
//    sc_start(5, SC_NS);
//    exec = 0;
//    while(!rdy)
//        sc_start(5, SC_NS);
//    assert(alu_ex.flags.read().bit(4) == 1);

//    sc_start(50, SC_NS);
//    cout << "end" << endl;
//    cout << "@" << sc_time_stamp() << " Terminating simulation\n" << endl;
//    sc_close_vcd_trace_file(wf);
//return 0;
//}
