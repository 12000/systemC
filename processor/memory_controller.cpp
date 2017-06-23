#include "memory_controller.h"

void memory_controller::prc_mem_controller(){
    request_out = 0;
    rw_out = 0;
    waiting_counter = 0;
    wait();
    while(1){
        request_out = 0;
        if(en){
            if(request_in){
                cout << "mem_controller: ask from counter, addr == " << addr_in.read() << endl;
                while(!dma_rdy){
                    cout << "MEM CONTROLLER: dma not ready" << endl;
                    wait();
                }

                //dma ready
                cout << "MEM CONTROLLER: dma ready" << endl;
                rw_out = rw_in.read();
                addr_out = addr_in.read();
                request_out = 1; // 1 time - 1 period
                wait();

                //waiting dack
                request_out = 0;
                wait();
                while(!dma_dack){
                    cout << "MEM CONTROLLER: waiting dma dack" << endl;

                    waiting_counter = waiting_counter.read() + 1;
                    if(waiting_counter.read() == 1){
                        if(dma_rdy){
                            cout << "MEM CONTROLLER: repeat request to dma" << endl;
                            request_out = 1;
                        }
                        waiting_counter = 0;
                    }
                    else{
                        request_out = 0;
                    }
                    wait();
                } //end while (!dma_dack)
                cout << "DMA DACK catched" << endl;
            }//end if ask_in
            else if(load){
                cout <<"mem_controller: load from decoder" << endl;
                while(!dma_rdy)
                    wait();

                //dma ready
                cout << "MEM CONTROLLER: dma ready" << endl;
                rw_out = rw_in.read();
                addr_out = load_addr.read();
                data_out = load_data.read();
                request_out = 1;
                wait();

                //waiting dack
                request_out = 0;
                wait();
                cout << "MEM CONTROLLER: waiting dma" << endl;
                while(!dma_dack){
                    waiting_counter = waiting_counter.read() + 1;
                    if(waiting_counter.read() == 3){
                        if(dma_rdy){
                            cout << "MEM CONTROLLER: repeat request to dma" << endl;
                            request_out = 1;
                        }
                        waiting_counter = 0;
                    }
                    else{
                        request_out = 0;
                    }
                    wait();
                } //end while (!dma_dack)
            }
        }
        wait();
    }
}
