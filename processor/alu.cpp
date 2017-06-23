#include "alu.h"

void alu::execute(){
    //reset
    for(int i=0; i<32; ++i)
        ron[i] = i;
    ron[31] = 0;
    buffer = 0;
    flags = 0;
    rdy = 0;
    wait();
    while(true){
        rdy = 0;
        if(exec.read() == 1){
            cout << "alu: start execute" << endl;
            if(operation_type.read() == 1){ //mov operations
                if(operation_number.read() == 0 ||
                        operation_number.read() == 4){ //write to ron #data or write to ron data from RONaddr
                    ron[ron_number_1.read()] = data_ron_in.read();
                    cout << "operation 1 1" << endl;
                    wait();
                    rdy = 1;
                    cout << ron[ron_number_1.read()] << endl;
                }
                else if(operation_number.read() == 1){ //copy
                    //op_1_1(ron_number_1.read(), ron_number_2.read());
                    sc_int<DATA_SIZE>tmp = ron[ron_number_2.read()];
                    wait();
                    ron[ron_number_1.read()] = tmp;
                    wait();
                    rdy = 1;
                }
                else if(operation_number.read() == 2){ //write to ron @data
                    //op_1_0
                }
                else if(operation_number.read() == 3){ //out from ron
                    data_ron_out = ron[ron_number_1.read()];
                    cout << "ron out" << endl;
                    wait();
                    rdy = 1;
                }
                else if(operation_number.read() == 5){
                    cout << "1 5" << endl;
                    ron[ron_number_1.read()] = GPI.read();
                    wait();
                    rdy = 1;
                }
                else{
                    cout << "unknown cmd number, type = 1" << endl;
                }
            }
            else if(operation_type.read() == 2){ //arithmetical
                if(operation_number.read() == 0){
                    buffer = data_in.read();
                    wait();
                    if((ron[ron_number_1.read()].read() == MAX_VALUE && buffer.read() > 0) ||
                            (buffer.read() == MAX_VALUE && ron[ron_number_1.read()].read() > 0)){
                        //flags.bit(3) = 1; // C
                        flags = (flags.read().range(5,4), 1, flags.read().range(2,0));
                    }
                    else{
                        //flags.bit(3) = 0; // C
                        flags = (flags.read().range(5,4), 0, flags.read().range(2,0));
                    }
                    ron[ron_number_1.read()] = ron[ron_number_1.read()].read() + buffer.read();
                    if(ron[ron_number_1.read()].read() == 0){
                        //flags.bit(0) = 1; // Z
                        //flags.bit(1) = 0; // NZ
                        flags = (flags.read().range(5,2), 0, 1);
                    }
                    else{
                        //flags.bit(0) = 0; // Z
                        //flags.bit(1) = 1; // NZ
                        flags = (flags.read().range(5,2), 1, 0);
                    }
                    wait();
                    rdy = 1;
                }
                else if(operation_number.read() == 1){
                    if((ron[ron_number_1.read()].read() == MAX_VALUE && ron[ron_number_2.read()].read() > 0) ||
                            (ron[ron_number_2.read()].read() == MAX_VALUE &&
                             ron[ron_number_1.read()].read() > 0)){
                        //flags.bit(3) = 1; // C
                        flags = (flags.read().range(5,4), 1, flags.read().range(2,0));
                    }
                    else{
                        //flags.bit(3) = 0; // C
                        flags = (flags.read().range(5,4), 0, flags.read().range(2,0));
                    }
                    ron[ron_number_1.read()] = ron[ron_number_1.read()].read() + ron[ron_number_2.read()].read();
                    if(ron[ron_number_1.read()].read() == 0){
                        //flags.bit(0) = 1; // Z
                        //flags.bit(1) = 0; // NZ
                        flags = (flags.read().range(5,2), 0, 1);
                    }
                    else{
                        //flags.bit(0) = 0; // Z
                        //flags.bit(1) = 1; // NZ
                        flags = (flags.read().range(5,2), 1, 0);
                    }
                    wait();
                    rdy = 1;
                }
                else if(operation_number.read() == 2){
                    buffer = data_in.read();
                    wait();
                    if((ron[ron_number_2.read()].read() == MAX_VALUE && buffer.read() > 0) ||
                            (buffer.read() == MAX_VALUE && ron[ron_number_2.read()].read() > 0)){
                        //flags.bit(3) = 1; // C
                        flags = (flags.read().range(5,4), 1, flags.read().range(2,0));
                    }
                    else{
                        //flags.bit(3) = 0; // C
                        flags = (flags.read().range(5,4), 0, flags.read().range(2,0));
                    }
                    ron[ron_number_1.read()] = ron[ron_number_2.read()].read() + buffer.read();
                    if(ron[ron_number_1.read()].read() == 0){
                        //flags.bit(0) = 1; // Z
                        //flags.bit(1) = 0; // NZ
                        flags = (flags.read().range(5,2), 0, 1);
                    }
                    else{
                        //flags.bit(0) = 0; // Z
                        //flags.bit(1) = 1; // NZ
                        flags = (flags.read().range(5,2), 1, 0);
                    }
                    wait();
                    rdy = 1;
                }
                else if(operation_number.read() == 3){
                    //delete
                }
                else if(operation_number.read() == 4){ //sub Ra = Ra - Rb
                    cout << "SUB Ra=Ra - Rb" << endl;
                    if(ron[ron_number_1.read()].read() >= ron[ron_number_2.read()].read()){
                        //flags.bit(5) = 0; //CC
                        flags = (0, flags.read().range(4,0));
                    }
                    else{
                        //flags.bit(5) = 1; //CC
                        flags = (1, flags.read().range(4,0));
                    }
                    ron[ron_number_1.read()] = ron[ron_number_1.read()].read() - ron[ron_number_2.read()].read();
                    if(ron[ron_number_1.read()].read() == 0){
                        //flags.bit(0) = 1; //Z
                        //flags.bit(1) = 0; //NZ
                        //flags.bit(4) = 0; // E
                        flags = (flags.read().bit(5), 0, flags.read().range(3,2), 0, 1);
                    }
                    else{
                        //flags.bit(0) = 0; //Z
                        //flags.bit(1) = 1; //NZ
                        //flags.bit(4) = 0; // E
                        flags = (flags.read().bit(5), 0, flags.read().range(3,2), 1, 0);
                    }
                    wait();
                    rdy = 1;
                }
                else if(operation_number.read() == 5){ //sub Rn - data4
                    buffer = data_in.read();
                    wait();
                    cout << buffer.read() << endl;
                    if(ron[ron_number_1.read()].read() > buffer.read()){
                        //flags.bit(0) = 0; // Z
                        //flags.bit(1) = 1; // NZ
                        //flags.bit(5) = 0; // CC
                        flags = (0, flags.read().range(4,2), 1, 0);
                    }
                    else if(ron[ron_number_1.read()].read() == buffer.read()){
                        //flags.bit(0) = 1; // Z
                        //flags.bit(1) = 0; // NZ
                        //flags.bit(5) = 0; // CC
                        flags = (0, flags.read().range(4,2), 0, 1);
                    }
                    else{
                        //flags.bit(0) = 0; // Z
                        //flags.bit(1) = 1; // NZ
                        //flags.bit(5) = 1; // CC
                        flags = (1, flags.read().range(4,2), 1, 0);
                    }
                    ron[ron_number_1.read()] = ron[ron_number_1.read()].read() - buffer.read();
                    wait();
                    rdy = 1;
                }
                else if(operation_number.read() == 6){
                    buffer = data_in.read();
                    wait();
                    if(ron[ron_number_1.read()].read() > buffer.read()){
                        //flags.bit(0) = 0; // Z
                        //flags.bit(1) = 1; // NZ
                        //flags.bit(5) = 0; // CC
                        flags = (0, flags.read().range(4,2), 1, 0);
                    }
                    else if(ron[ron_number_1.read()].read() == buffer.read()){
                        //flags.bit(0) = 1; // Z
                        //flags.bit(1) = 0; // NZ
                        //flags.bit(5) = 0; // CC
                        flags = (0, flags.read().range(4,2), 0, 1);
                    }
                    else{
                        //flags.bit(0) = 0; // Z
                        //flags.bit(1) = 1; // NZ
                        //flags.bit(5) = 1; // CC
                        flags = (1, flags.read().range(4,2), 1, 0);
                    }
                    ron[ron_number_1.read()] = ron[ron_number_1.read()].read() - buffer.read();
                    wait();
                    rdy = 1;
                }
                else if(operation_number.read() == 7){
                    //delete
                }
                else if(operation_number.read() == 8){ //INC Ra
                    if(ron[ron_number_1.read()].read() == MAX_VALUE){
                        //flags.bit(3) = 1; //C
                        flags = (flags.read().range(5,4), 1, flags.read().range(2,0));
                    }
                    ron[ron_number_1.read()] = ron[ron_number_1.read()].read() + 1;
                    if(ron[ron_number_1.read()].read() == 0){
                        //flags.bit(0) = 1; // Z
                        //flags.bit(1) = 0; // NZ
                        //flags.bit(3) = 0; // C
                        flags = (flags.read().range(5,4), 0, flags.read().bit(2), 0, 1);
                    }
                    else if(ron[ron_number_1.read()].read() != 0){
                        //flags.bit(0) = 0; // Z
                        //flags.bit(1) = 1; // NZ
                        //flags.bit(3) = 0; // C
                        flags = (flags.read().range(5,4), 0, flags.read().bit(2), 1, 0);
                    }
                    wait();
                    rdy = 1;
                }
                else if(operation_number.read() == 9){ //DEC Ra
                    if(ron[ron_number_1.read()].read() == 0){
                        //flags.bit(5) = 1; // CC
                        flags = (1, flags.read().range(4,0));
                    }
                    else{
                        //flags.bit(5) = 0; // CC
                        flags = (0, flags.read().range(4,0));
                    }
                    ron[ron_number_1.read()] = ron[ron_number_1.read()].read() - 1;
                    if(ron[ron_number_1.read()].read() == 0){
                        //flags.bit(0) = 1; // Z
                        //flags.bit(1) = 0; // NZ
                        flags = (flags.read().range(5,2), 0, 1);
                    }
                    else if(ron[ron_number_1.read()].read() != 0){
                        //flags.bit(0) = 0; // Z
                        //flags.bit(1) = 1; // NZ
                        flags = (flags.read().range(5,2), 1, 0);
                    }
                    wait();
                    rdy = 1;
                }
                else if(operation_number.read() == 10){ //MUL
                    cout << "MUL " << ron[ron_number_1.read()].read() << " * " << ron[ron_number_2.read()].read() << endl;

                    mul_tmp = ron[ron_number_1.read()].read() * ron[ron_number_2.read()].read();
                    wait();
                    //ron[ron_number_2.read()] = tmp.range(DATA_SIZE*2-1, DATA_SIZE);
                    ron[ron_number_1.read()] = mul_tmp.read().range(DATA_SIZE-1, 0);

                    cout << "A " << ron[ron_number_1.read()].read() << endl;
                    cout << "B " << ron[ron_number_2.read()].read() << endl;
                    wait();
                    rdy = 1;
                }
                else{
                    cout << "unknown cmd number, type = 2" << endl;
                }

            }
            else if(operation_type.read() == 3){ // compare
                if(operation_number.read() == 0){ //equal 0
                    if(ron[ron_number_1.read()].read() == 0){
                        //flags.bit(0) = 1; //Z
                        //flags.bit(1) = 0; //NZ
                        //cout << "flags " << flags << endl;
                        flags = (flags.read().range(5,2), 0, 1);
                    }
                    else{
                        //flags.bit(0) = 0; //Z
                        //flags.bit(1) = 1; //NZ
                        //cout << "flags " << flags << endl;
                        flags = (flags.read().range(5,2), 1, 0);
                    }
                    wait();
                    rdy = 1;
                    cout << "flags " << flags.read() << endl;
                }
                else if(operation_number.read() == 1){ // A>B
                    if(ron[ron_number_1.read()].read() > ron[ron_number_2.read()].read()){
                        //flags.bit(2) = 1;
                        cout << "flags " << flags << endl;
                        flags = (flags.read().range(5,3), 1, flags.read().range(1,0));
                        cout << "M = 1!" << endl;
                    }
                    else{
                        //flags.bit(2) = 0;
                        flags = (flags.read().range(5,3), 0, flags.read().range(1,0));
                    }
                    wait();
                    rdy = 1;
                    cout << "flags " << flags.read() << endl;
                }
                else if(operation_number.read() == 2){ // A=B
                    if(ron[ron_number_1.read()].read() == ron[ron_number_2.read()].read()){
                        //flags.bit(4) = 1;
                        flags = (flags.read().bit(5), 1, flags.read().range(3,0));
                    }
                    else{
                        //flags.bit(4) = 0;
                        flags = (flags.read().bit(5), 0, flags.read().range(3,0));
                    }
                    wait();
                    rdy = 1;
                    cout << "flags " << flags.read() << endl;
                }
                else{
                    cout << "unknown cmd number, type = 3" << endl;
                }
            }
            else{
                cout << "unknown command" << endl;
                continue;
            }
        }//end exec.read() == 1
        wait();
        flags_out = flags;
    }
}
