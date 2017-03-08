#include "st_machine.h"

void st_machine::ch_state() {
    state = 0;
    wait();
    while (true) {
        switch(x.read()){
            case 0:
                if (state.read() == 0) state = 0;
                else if (state.read() == 1) state = 2;
                else if (state.read() == 2) state = 0;
                else state = 0;
                break;
            case 1:
                if(state.read() == 0 || state.read() == 3) state = 2;
                //else state = state
                break;
            case 2:
                //state = state
                break;
            case 3:
                if (state.read() == 3) state = 1;
                else if (state.read() == 1) state = 3;
                else if (state.read() == 2) state = 1;
                else state = 1;
                break;
            default:
                state = 0;
        }

        wait();
    }
}

void st_machine::data_out_method() {
    switch(state.read()){
        case 0: y = 0; break;
        case 1: y = 2; break;
        case 2: y = 1; break;
        default: y = 3;
    }
    //cout << "state = " << state << " ; " << "y = " << y << endl;
}
