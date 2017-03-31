#include "shift_reg.h"

void shift_reg::shift() {
    // We check if reset is active
    data = 0;
    dout = 0;
    out = 0;
    wait();
    while (true) {
        if(ena){
            data.write((data.read().range(6, 0), cin.read()));
        }
        dout.write(data.read());
        out = (bool)data.read().bit(7);
        cout << "@" << sc_time_stamp() << " :: Have stored " << dout.read() << endl;
        wait();
    }
}

