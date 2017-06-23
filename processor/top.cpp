#include "top.h"

void top::GPIO_process(){
    GPI_reg = 0;
    GPO = 0;
    wait();
    while(true){
        GPI_reg = GPI.read();
        GPO = data_ron_in.read();
        wait();
    }
}
