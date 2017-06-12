/*#include "fifo_template.h"

template <typename T> void fifo_template<T>::push_method() {
    empty = 1;
    wait();
    while (true){
        if(push && pos != 8){
            data[pos] = data_in.read();
            pos++;
        }
        if(pos == 0) empty = 1;
        else empty = 0;
        wait();
    }
}


template <typename T> void fifo_template<T>::pop_method() {
    // We check if reset is active
   for(uint i=0; i<size; i++) data[i] = 0;
    data_out.write(data[0]);
    pos = 0;
    full = 0;
    wait();
    while (true){
        if(pop){
            //if data is in queue
            if(pos != 0){
                data_out.write(data[0]);
                for(uint i=0; i<size; i++) data[i] = data[i+1];
                cout << "pop " << data_out.read() << endl;
                pos--;
                full = 0;
            }
            else{ //queue is empty
                data_out.write('\0');
            }
        }
        if(pos == 8) full = 1;
        else full = 0;
        wait();
    }
}
*/
