#include "ram.h"

void ram_mem::prc_memory() {
    wait();
    while(1){
        if(en) {
            ack = 0;
            if(rw && request){ //read
                cout << "memory ask, rw == " << rw.read() << endl;
                cout << "addr == " << dec<< addr.read() << endl;
                if (addr.read() < MEM_SIZE){
                    data_out = ram[addr.read()].read();
                    ack = 1;
                }
            }
            else if(!rw && request){ // write
                cout << "RAM: write" << endl;
                if (addr.read() < MEM_SIZE){
                    ram[addr.read()] = data_in.read();
                    ack = 1;
                }
            }
        }
        wait();
    }
}
