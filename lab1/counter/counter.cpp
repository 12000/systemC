#include "counter.h"

void counter::count() {
    // We check if reset is active    
    cnt_reg = 0;
    dout.write(cnt_reg);
    wait();
    while (true) {
        if(load) cnt_reg = data.read();
        else cnt_reg +=1;
        cout << "@" << sc_time_stamp() << " :: Have stored " << cnt_reg << endl;
        dout.write(cnt_reg);
        wait();
    }
}

