#include "shift_reg.h"

void shift_reg::shift() {
    // We check if reset is active
    data = 0;
    data_out = 0;
    out = 0;
    wait();
    while (true) {
        if(ena){
            data.write((data.read().range(6, 0), in.read()));
        }
        data_out.write(data.read());
        //out.write;
        cout << "@" << sc_time_stamp() << " :: Have stored " << data_out.read() << endl;
        wait();
    }
}

