#include "fifo.h"

void fifo::push_method() {
    pos_push = 0;
    wait();
    while (true){
        if(push && !full){
            data[pos_push.read()] = data_in.read();
            if(pos_push.read() == size-1){
                push_over = !push_over;
                pos_push = 0;
            }
            else pos_push = pos_push.read() + 1;
        }
        wait();
    }
}

void fifo::pop_method() {
    pos_pop = 0;
    wait();
    while (true){
        if(pop && !empty){
            data_out = data[pos_pop.read()];
            if(pos_pop.read() == size - 1){
                pop_over = !pop_over;
                pos_pop = 0;
            }
            else pos_pop = pos_pop.read() + 1;
        }
        wait();
    }
}

void fifo::current_state(){
    if(!sreset_n){
        empty = 1;
        full = 0;
    }
    else if(pos_pop.read() == pos_push.read()){
        if(pop_over.read() != push_over.read()){
            empty = 0;
            full = 1;
        }
        else{
            empty = 1;
            full = 0;
        }
    }
    else{
        empty = 0;
        full = 0;
    }
    cout << "---------------------------------------------------------------" << endl;
    cout << " | pos_pop == " << pos_pop.read() << " | pos_push == " << pos_push.read() << endl;
    cout << " empty == " << empty.read() << " | full == " << full.read() << endl;
    cout << "---------------------------------------------------------------" << endl;
}
