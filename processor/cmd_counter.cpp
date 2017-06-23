#include "cmd_counter.h"

void cmd_counter::cmd_count(){
    commands = 0;
    wait();
    while(1){
        if(ena){
            if(cmd_up){
                cout << "cmd_counter: cmd count" << endl;
                commands = commands.read() + 1;
                wait();
            }
        }
        wait();
    }
}

void cmd_counter::addr_count(){
    cout << "addr reset!" << endl;
    address = 0;
    request = 0;
    wait();
    while(1){
        request = 0;
        if(ena){
            if(load_addr){
                cout << "cmd_counter: load addres " << addr_data.read() << endl;
                address = addr_data.read();
                wait();
                addr_out.write(address.read()); // to out
                request = 1;
            }
            else if(addr_up){
                cout << "cmd_counter: addr count" << endl;
                address = address.read() + 1;
                wait();
                addr_out.write(address.read()); // to out
                request = 1;
            }
        }
        wait();
    }
}
