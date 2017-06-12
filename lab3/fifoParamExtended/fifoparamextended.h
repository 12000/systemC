#ifndef FIFOPARAMEXTENDED_H
#define FIFOPARAMEXTENDED_H

#include "systemc.h"
#include "fifoparam.h"

template <typename T, uint size = 8>
class fifoParamExtended : public fifoParam<T>{
public:
    sc_out<bool> almost_empty {"almost_empty"};
    sc_out<bool> almost_full {"almost_full"};

public:
    void num_change();
    void current_almost_size();
    uint nums;

public:
    SC_HAS_PROCESS(fifoParamExtended);
    fifoParamExtended(sc_module_name name) : fifoParam<T>(name, size) {
        SC_CTHREAD(num_change, this->clock.pos());
        this->reset_signal_is(this->sreset_n, false);
        SC_METHOD(current_almost_size);
        sc_module::sensitive << this->sreset_n << this->pos_pop << this->pos_push << this->full << this->empty;
    }
};

template <typename T, uint size> void
fifoParamExtended<T, size>::num_change(){
    nums = 0;
    wait();
    while(true){
        if(this->push && !this->pop){
            if(nums < size){
                ++nums;
                cout << "++nums : " << nums << endl;
            }
        }
        if(this->pop && !this->push){
            if(nums > 0){
                --nums;
                cout << "--nums : " << nums << endl;
            }
        }
        wait();
    }
}

template <typename T, uint size> void fifoParamExtended<T, size>::current_almost_size(){
    if(!this->sreset_n){
        almost_empty = 0;
        almost_full = 0;
    }
    else{
        if(nums < size*0.3 && this->empty == 0){
            almost_empty = 1;
            almost_full = 0;
        }
        else if(nums > size*0.7 && this->full == 0){
            almost_empty = 0;
            almost_full = 1;
        }
        else{
            almost_empty = 0;
            almost_full = 0;
        }
    }
}

#endif // FIFOPARAMEXTENDED_H
