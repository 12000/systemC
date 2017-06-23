#include "device_writer.h"


void device_writer::prc_device_writer(){
    request_out = 0;
    rw_out = 0;
    waiting_counter = 0;
    wait();
    while(1){
        request_out = 0;
        if(en){
                while(!dma_rdy){
                    cout << "DEVICE: dma not ready" << endl;
                    wait();
                }
                //dma ready
                cout << "DEVICE: dma ready" << endl;
                rw_out = 0; //write
                addr_out = 2045;
                data_out = 12345;
                request_out = 1; // 1 time - 1 period
                wait();

                //waiting dack
                request_out = 0;
                wait();
                while(!dma_dack){
                    cout << "DEVICE: waiting dma dack" << endl;
                    waiting_counter = waiting_counter.read() + 1;
                    if(waiting_counter.read() == 8){
                        if(dma_rdy){
                            cout << "DEVICE: repeat request to dma" << endl;
                            request_out = 1;
                        }
                        waiting_counter = 0;
                    }
                    else{
                        request_out = 0;
                    }
                    wait();
                } //end while (!dma_dack)
                cout << "DMA DACK catched by DEVICE" << endl;
               //wait();

                cout << "DEVICE: wait data from memory (from dma)" << endl;
                while(!dma_ack)
                    wait();

                reg = data_in.read();
                wait();
                wait();
                wait();
                wait();
                wait();
                wait();
                wait();
                wait();
                wait();
                wait();
                wait();

        }
        wait();
    }
}
