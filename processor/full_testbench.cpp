#include "stdio.h"
#include <iomanip>
#include "systemc.h"
#include "ram.h"
#include "constants.h"
#include "memory_controller.h"
#include "cache.h"
#include "top.h"
#include "dma_controller.h"
#include "device_reader.h"
#include "device_writer.h"

int sc_main(int argc, char* argv[])
{
    //decoder signals
    sc_clock clk("clk", 5, SC_NS);
    sc_signal<bool> en {"en"};
    sc_signal<bool> rst {"rst"};

    sc_signal<bool> cache_en;
    sc_signal<bool> ram_en;

    sc_signal<bool> request_from_processor;                 //to cache
    sc_signal<bool> rw_out_from_processor;                  //to cache
    sc_signal<sc_uint<ADDR_SIZE>> addr_out_from_processor;  //to cache
    sc_signal<sc_int<DATA_SIZE>> data_out_from_processor;   //to cache
    sc_signal<bool> memory_rdy;

    sc_signal<sc_uint<6>> state_out;
    sc_signal<bool> stop;

    sc_signal<bool> cache_rdy;
    sc_signal<sc_int<DATA_SIZE>> data_from_ram;
    sc_signal<bool> rw_to_ram;
    sc_signal<bool> ask_ram;
    sc_signal<sc_uint<ADDR_SIZE>> addr_to_ram_out;
    sc_signal<sc_int<DATA_SIZE>> data_to_ram_out;

    sc_signal<sc_int<DATA_SIZE>> data_in;

    sc_signal<bool> dma_en;
    sc_signal<bool> rw_from_dma;
    sc_signal<bool> request_from_dma;
    sc_signal<sc_uint<ADDR_SIZE>> addr_from_dma;
    sc_signal<sc_int<DATA_SIZE>> data_from_dma;
    sc_signal<sc_int<DATA_SIZE>> data_from_cache;
    sc_signal<bool> dma_rdy;
    sc_signal<bool> ack_0;
    sc_signal<bool> dack_0;

    sc_signal<sc_int<DATA_SIZE>> device_data_1;
    sc_signal<sc_uint<ADDR_SIZE>> device_addr_1;
    sc_signal<bool> device_rw_1;
    sc_signal<bool> device_request_1;
    sc_signal<sc_int<DATA_SIZE>> device_data_2;
    sc_signal<sc_uint<ADDR_SIZE>> device_addr_2;
    sc_signal<bool> device_rw_2;
    sc_signal<bool> device_request_2;
    sc_signal<bool> ack_1;
    sc_signal<bool> dack_1;
    sc_signal<bool> ack_2;
    sc_signal<bool> dack_2;

    sc_signal<sc_int<DATA_SIZE>> GPI;
    sc_signal<sc_int<DATA_SIZE>> GPO;

    sc_signal<bool> device_en;

    top proc("proc");
    proc.clk(clk);
    proc.en(en);
    proc.rst(rst);
    proc.rw_out(rw_out_from_processor);             // to cache (after to dma)
    proc.request_to_dma(request_from_processor);    // to cache (after to dma)
    proc.data_out(data_out_from_processor);         // to cache (after to dma)
    proc.addr_out(addr_out_from_processor);         // to cache (after to dma)
    proc.state_out(state_out);
    proc.stop(stop);
    proc.dma_rdy(dma_rdy);                        // from cache (after dma ack!!!)
    proc.dma_ack(ack_0);                            //from dma
    proc.dma_dack(dack_0);                          //from dma to controller
    proc.data_in(data_in);                          // from cache (after dma)
    proc.GPI(GPI);
    proc.GPO(GPO);


    dma_controller dma("dma");
    dma.clk(clk);
    dma.rst(rst);
    dma.en(dma_en);

    dma.request_0(request_from_processor);
    dma.addr_in_0(addr_out_from_processor);
    dma.data_in_0(data_out_from_processor);
    dma.rw_in_0(rw_out_from_processor);
    dma.request_1(device_request_1);
    dma.addr_in_1(device_addr_1);
    dma.data_in_1(device_data_1);
    dma.rw_in_1(device_rw_1);
    dma.request_2(device_request_2);
    dma.addr_in_2(device_addr_2);
    dma.data_in_2(device_data_2);
    dma.rw_in_2(device_rw_2);

    dma.rw_out(rw_from_dma);
    dma.mem_request(request_from_dma);
    dma.addr_to_ram(addr_from_dma);
    dma.data_to_ram(data_from_dma);
    dma.data_from_ram(data_from_cache); // from cache
    dma.mem_ack(cache_rdy);

    dma.rdy(dma_rdy);
    dma.data_out(data_in);
    dma.ack_0(ack_0);
    dma.dack_0(dack_0);
    dma.ack_1(ack_1);
    dma.dack_1(dack_1);
    dma.ack_2(ack_2);
    dma.dack_2(dack_2);


    cache cache_mem("cache");
    cache_mem.clk(clk);
    cache_mem.en(cache_en);
    cache_mem.data_out(data_from_cache);                // to dma
    cache_mem.rdy(cache_rdy);                   // to dma
    cache_mem.ask(request_from_dma);      // request from dma
    cache_mem.rw_in(rw_from_dma);    // rw from dma
    cache_mem.addr(addr_from_dma);   // addr from dma
    cache_mem.data_in(data_from_dma); // data from dma
    cache_mem.ram_rdy(memory_rdy);
    cache_mem.data_from_ram(data_from_ram);
    cache_mem.rw_out(rw_to_ram);                // to ram
    cache_mem.ask_ram(ask_ram);                 // to ram
    cache_mem.addr_to_ram_out(addr_to_ram_out); // to ram
    cache_mem.data_to_ram_out(data_to_ram_out); // to ram


    ram_mem memory("memory");
    memory.en(ram_en);
    memory.rw(rw_to_ram);//(rw_out_from_controller);
    memory.request(ask_ram); //(request_from_controller);
    memory.clk(clk);
    memory.addr(addr_to_ram_out); //(addr_out_from_controller);
    memory.data_in(data_to_ram_out); //(data_out_from_controller);
    memory.ack(memory_rdy);
    memory.data_out(data_from_ram); //(data_in); //now to cache


    device_reader reader("reader");
    reader.clk(clk);
    reader.en(device_en);
    reader.rst(rst);
    reader.data_in(data_in);
    reader.dma_ack(ack_1);
    reader.dma_rdy(dma_rdy);
    reader.dma_dack(dack_1);
    reader.addr_out(device_addr_1);
    reader.data_out(device_data_1);
    reader.request_out(device_request_1);
    reader.rw_out(device_rw_1);


    device_writer writer("writer");
    writer.clk(clk);
    writer.en(device_en);
    writer.rst(rst);
    writer.data_in(data_in);
    writer.dma_ack(ack_2);
    writer.dma_rdy(dma_rdy);
    writer.dma_dack(dack_2);
    writer.addr_out(device_addr_2);
    writer.data_out(device_data_2);
    writer.request_out(device_request_2);
    writer.rw_out(device_rw_2);


    //Test code
    const int array_size_sort = 100;
    int start_addr_sort = 1024;
    const int array_size_filter = 256;
    int start_addr_filter = 400;
    sc_signal<sc_int<32>> ram_copy_sort [array_size_sort];
    sc_signal<sc_int<32>> ram_copy_filter [array_size_filter];

    ram_en = 1;
    device_en = 1;
    GPI = 777;
    dma_en = 1;
    en = 0;
    cache_en = 1;
    rst = 1;
    cout << "@" << sc_time_stamp() << " START\n" << endl;
    sc_start(5, SC_NS);

    rst = 0;
    en = 1;
    sc_start(5, SC_NS);

    //sort test---------------------------------------------------------
    /*
    for(int i = start_addr_sort; i < start_addr_sort+array_size_sort; ++i){
        ram_copy_sort[i-1024] = memory.ram[i].read();
    }
    */
    //------------------------------------------------------------------

    //filter test
    //------------------------------------------------------------------
    /*
    for(int i = start_addr_filter; i < start_addr_filter+array_size_filter; ++i){
        ram_copy_filter[i-start_addr_filter] = memory.ram[i].read();
    }
    */
    //------------------------------------------------------------------

    while(true){
        if(proc.stop.read() != 1){
            sc_start(5, SC_NS);
        }
        else{
            break;
        }
    }



    cout << "end" << endl;
    cout << "@" << sc_time_stamp() << " Terminating simulation\n" << endl;

    for(int i=0; i<32; ++i)
        cout << "ron[" << i << "] == " << proc.aluu.ron[i] << endl;

    //sort test---------------------------------------------------------
    /*
    for(int i = 0; i < array_size_sort; ++i){
        cout << "ram_copy[" << i << "] == " << ram_copy_sort[i].read() << endl;
    }
    for(int i = start_addr_sort; i < start_addr_sort+array_size_sort; ++i){
        cout << "ram[" << i << "] == " << memory.ram[i].read() << endl;
    }

    cout << "GPO " << proc.GPO << endl;
    cout << "data_ron_in " << proc.dcdr.data_ron_in.read() << endl;

    cout << "reader reg == " << hex << reader.reg.read() << endl;
    cout << "memory 2045 after writer == " << dec << memory.ram[2045].read() << endl;
    */
    //------------------------------------------------------------------

    //matrix test ------------------------------------------------------

    const int n = 6;
    const int m = 4;
    const int p = 5;

    cout << endl;
    for(int i=0; i<n; ++i){
        for(int j=0; j<m; ++j){
            cout << " " << memory.ram[600+i*m+j].read() << " ";
        }
        cout << endl;
    }
    cout << endl;
    for(int i=0; i<m; ++i){
        for(int j=0; j<p; ++j){
            cout << " " << memory.ram[700+i*p+j].read() << " ";
        }
        cout << endl;
    }
    cout << endl;
    for(int i=0; i<n; ++i){
        for(int j=0; j<p; ++j){
            cout << " " << std::setw(3) << memory.ram[800+i*p+j].read() << " ";
        }
        cout << endl;
    }
    getchar();

    //------------------------------------------------------------------

    //filter test-------------------------------------------------------
    /*
    int image_w = 16;
    int image_h = 16;
    int window_w = 3;
    int window_h = 3;
    for(int i = 0; i<image_w; ++i){
        for(int j=0; j<image_h; ++j){
            cout << std::setw(3) << ram_copy_filter[i*image_h + j].read() << " ";
        }
        cout << endl;
    }
    cout << endl << endl;
    for(int i = 0; i<image_w; ++i){
        for(int j=0; j<image_h; ++j){
            cout << std::setw(3) << memory.ram[800 + i*image_h + j].read() << " ";
        }
        cout << endl;
    }
    cout << endl << endl;
    for(int i = 0; i<window_w; ++i){
        for(int j=0; j<window_h; ++j){
            cout << std::setw(3) << memory.ram[700 + i*window_h + j].read() << " ";
        }
        cout << endl;
    }
    */
    //------------------------------------------------------------------

    //reset and repeat programm

     sc_start(100, SC_NS);
     rst = 1;
     sc_start(5, SC_NS);
     rst = 0;
     sc_start(5, SC_NS);

    while(true){
        if(proc.stop.read() != 1){
            sc_start(5, SC_NS);
        }
        else{
            break;
        }
    }
    cout << "!!!!!!!!!!!!!!!!!!" << endl;

return 0;
}
