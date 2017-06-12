#ifndef fifoParam_H
#define fifoParam_H

#include "systemc.h"

template <typename T> class fifoParam : public sc_module {

public:
    sc_in_clk clock {"clk"}; // Clock input of the design
    sc_in<bool> sreset_n {"sreset_n"}; // negative high, synchronous Reset input
    sc_in<bool> push {"push"};
    sc_in<bool> pop {"pop"};
    sc_in<char> data_in {"data_in"};
    sc_out<char> data_out {"data_out"};
    sc_out<bool> empty {"empty"};
    sc_out<bool> full {"full"};

    uint size;

    T* data;
    uint pos; //position
    sc_signal<sc_uint<32>> pos_pop;
    sc_signal<sc_uint<32>> pos_push;
    sc_signal<bool> push_over;
    sc_signal<bool> pop_over;
    sc_uint<32> push_size;
    sc_uint<32> pop_size;

public:
    void push_method();
    void pop_method();
    void current_size();


    SC_HAS_PROCESS(fifoParam);
    fifoParam(sc_module_name name, uint size = 8) : sc_module(name), size(size) {
        data = new T[size];

        SC_CTHREAD(push_method, clock.pos());
        reset_signal_is(sreset_n, false);
        SC_CTHREAD(pop_method, clock.pos());
        reset_signal_is(sreset_n, false);
        SC_METHOD(current_size);
        sensitive << pos_pop << pos_push << sreset_n;
    }
};

template <typename T> void fifoParam<T>::push_method() {
    pos_push = 0;
    wait();
    while (true){
        if(push && !full){
            data[pos_push.read()] = data_in.read();
            if(pos_push.read() == size-1){
                push_over = !push_over;
                pos_push = 0;
            }
            else {
                pos_push = pos_push.read() + 1;
            }
        }
        wait();
    }
}

template <typename T> void fifoParam<T>::pop_method() {
    pos_pop = 0;
    wait();
    while (true){
        if(pop && !empty){
            data_out = data[pos_pop.read()];
            if(pos_pop.read() == size - 1){
                pop_over = !pop_over;
                pos_pop = 0;
            }
            else {
                pos_pop = pos_pop.read() + 1;
            }
        }
        wait();
    }
}

template <typename T> void fifoParam<T>::current_size(){
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
}
#endif // fifoParam_H
