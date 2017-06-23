#include "dma_controller.h"

void dma_controller::prc_dma_controller(){
    rw_out = 0;
    addr_to_ram = 0;
    data_to_ram = 0;
    mem_request = 0;
    rdy = 0;
    data_out = 0;
    ack_0 = 0;
    ack_1 = 0;
    ack_2 = 0;
    dack_0 = 0;
    dack_1 = 0;
    dack_2 = 0;
    wait();
    while(1){
        if(en){
            ack_0 = 0;
            ack_1 = 0;
            ack_2 = 0;
            dack_0 = 0;
            dack_1 = 0;
            dack_2 = 0;
            rdy = 1;

            /*while(!request_0 && !request_1 && !request_2){
                cout << "DMA CONTROLLER: ready, wait request" << endl;
                wait();
            }*/
            while(true){
                cout << "DMA CONTROLLER: ready, wait request" << endl;
                //cout << "REQUEST_0: " << request_0.read() << endl;
                //cout << "REQUEST_1: " << request_1.read() << endl;
                //cout << "REQUEST_2: " << request_2.read() << endl;
                if(request_0.read() || request_1.read()|| request_2.read())
                    break;
                wait();
            }

            cout << "dma catch  request" << endl;
            rdy = 0;
            if(request_0){
                device_num = 0;
                dack_0 = 1;
            }
            else if(request_1){
                device_num = 1;
                dack_1 = 1;
            }
            else{
                device_num = 2;
                dack_2 = 1;
            }
            wait();

            dack_0 = 0;
            dack_1 = 0;
            dack_2 = 0;
            //change to out_regs??????
            if(device_num.read() == 0){
                addr_in_reg = addr_in_0.read();
                data_in_reg = data_in_0.read();
                rw_reg = rw_in_0.read();
            }
            else if(device_num.read() == 1){
                addr_in_reg = addr_in_1.read();
                data_in_reg = data_in_1.read();
                rw_reg = rw_in_1.read();
            }
            else{
                addr_in_reg = addr_in_2.read();
                data_in_reg = data_in_2.read();
                rw_reg = rw_in_2.read();
            }
            wait();
            cout << "latch data from device" << endl;

            rw_out = rw_reg.read();
            addr_to_ram = addr_in_reg.read();
            data_to_ram = data_in_reg.read();
            mem_request = 1;
            cout << "request to cache" << endl;
            wait();
            while(!mem_ack){
                mem_request = 0;
                wait();
            }

            data_out = data_from_ram.read();
            if(device_num.read() == 0){
                ack_0 = 1;
            }
            else if(device_num.read() == 1){
                ack_1 = 1;
            }
            else{
                ack_2 = 1;
            }
        }
        wait();
    }
}
