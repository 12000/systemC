#include "decoder.h"

void decoder::get_next_state(){
    cout << "decoder reset" << endl;
    cmd_type_reg = 0;
    cmd_num_reg = 0;
    alu_data_reg = 0;
    ron_number_1_reg = 0;
    ron_number_2_reg = 0;
    data_to_ron_reg = 0;
    counter_load_addr_reg = 0;

    mem_controller_load_data_reg = 0;
    mem_controller_load_addr_reg = 0;

    state = 0;
    stop = 0;
    cout << "START STATE, current state " << state << endl;
    wait();
    while(true){
        if(en){
            switch(state.read()){
            case 0:
                cout << "current state " << state << endl;
                wait(); //wait();
                mem_controller_en = 1;
                alu_en = 1;
                counter_en = 1;
                //cache_en = 1;
                stop = 0;
                state_out = 0;

                state = 1;
                break;

            case 1:
                cout << "current state " << state << endl;
                cmd_up = 1;
                addr_up = 1;
                mem_controller_rw = 1; //read
                mem_controller_load = 0;
                state_out = 1;

                state = 2;
                break;

            case 2:
                cout << "current state " << state << endl;
                cmd_up = 0;
                addr_up = 0;
                state_out = 2;
                counter_load = 0;

                if(memory_rdy.read() != 1){
                    state = 2;
                    cout << "state 2 LOOP, wait mem_rdy" << endl;
                }
                else{
                    cmd_type_reg = (sc_uint<4>)data_in.read().range(31, 24);
                    cmd_num_reg = (sc_uint<4>)data_in.read().range(23, 16);
                    ron_number_1_reg = (sc_uint<RON_NUM_SIZE>)data_in.read().range(15, 8);
                    ron_number_2_reg = (sc_uint<RON_NUM_SIZE>)data_in.read().range(7, 0);
                    cout << "cmd_type = " << cmd_type_reg.read() << endl;
                    cout << "cmd_num = " << cmd_num_reg.read() << endl;
                    cout << "ron_number_1 = " << ron_number_1_reg.read() << endl;
                    cout << "ron_number_2 = " << ron_number_2_reg.read() << endl;
                    wait();
                    state = 3;
                }
                break;

            case 3: //CMD ANALYZE
                state_out = 3;
                cout << "current state " << state << endl;
                if(cmd_type_reg.read() == 1 || cmd_type_reg.read() == 2 || cmd_type_reg.read() == 3){ // MOV || ARITHMETICAL || compare
                    state = 4;
                }
                else if(cmd_type_reg.read() == 5){
                    state = 12;
                }
                else if(cmd_type_reg.read() == 4){
                    state = 19;
                }
                else if(cmd_type_reg.read() == 6){
                    state = 21;
                }
                else if(cmd_type_reg.read() == 00){
                    cout << "NOP" << endl;
                    state = 1;
                }
                else{
                    state = 3;
                    cout << "STATE 3 INFINITE LOOP" << endl;
                }
                break;

            case 4:
                state_out = 4;
                cout << "current state " << state << endl;
                if(cmd_num_reg.read() == 1 || (cmd_num_reg.read() == 5 && cmd_type_reg.read() == 1)
                        ||cmd_num_reg.read() == 3
                        || cmd_num_reg.read() == 4 || cmd_num_reg.read() == 8
                        || cmd_num_reg.read() == 9 || cmd_num_reg.read() == 10
                        || (cmd_type_reg.read() == 3 &&(cmd_num_reg.read() == 0 ||
                                                 cmd_num_reg.read() == 1 ||
                                                 cmd_num_reg.read() == 2))){ //1 word
                    cout << "state 4 goto 5, 1 word cmd" << endl;
                    state = 5;
                    break;
                }
                else if(cmd_num_reg.read() == 0 || (cmd_type_reg.read() == 2 && cmd_num_reg.read() == 2)
                        || cmd_num_reg.read() == 5){
                    cout << "state 4 goto 7" << endl;
                    state = 7;
                    break;
                }
                else{
                    cout << "state 4 goto 9" << endl;
                    state = 9;
                    break;
                }

            case 5:
                state_out = 5;
                cout << "current state " << state << endl;

                if(cmd_num_reg.read() == 4 && cmd_type_reg.read() == 1){
                    cmd_type_out = 1;
                    cmd_num_out = 3;
                    ron_number_1_out = ron_number_2_reg.read(); //Raddr
                    exec = 1;
                    wait();
                    exec = 0;
                    while(alu_rdy != 1){
                        cout << "state 5 wait alu_rdy" << endl;
                        wait();
                    }
                    mem_controller_load_addr_reg = (sc_uint<ADDR_SIZE>)data_ron_in.read().range(ADDR_SIZE-1, 0);
                    wait();
                    mem_controller_load_addr = mem_controller_load_addr_reg.read();
                    mem_controller_load = 1;
                    wait();
                    mem_controller_load = 0;
                    while(memory_rdy != 1){
                        cout << "state 5 wait mem_rdy" << endl;
                        wait();
                    }
                    data_to_ron_reg = data_in.read();
                    wait();
                    state = 6;
                }
                else{
                    state = 6;
                }
                break;

            case 6:
                state_out = 6;
                cout << "current state " << state << endl;

                alu_data_out = alu_data_reg.read();
                ron_number_1_out = ron_number_1_reg.read();
                ron_number_2_out = ron_number_2_reg.read();
                cmd_type_out = cmd_type_reg.read();
                cmd_num_out = cmd_num_reg.read();
                data_ron_out = data_to_ron_reg.read();
                exec = 1;
                state = 25;

                break;

            case 25:
                state_out = 25;
                cout << "current state " << state << endl;

                exec = 0;
                if(alu_rdy.read() != 1){
                    cout << "state 25 LOOP, wait alu_rdy" << endl;
                    state = 25;
                }
                else{
                    //wait();
                    state = 1;
                }
                break;

            case 7:
                state_out = 7;
                cout << "current state" << state << endl;
                addr_up = 1;
                mem_controller_rw = 1;
                state = 8;
                break;

            case 8:
                state_out = 8;
                cout << "current state" << state << endl;

                addr_up = 0;
                if(memory_rdy.read() != 1){
                    state = 8;
                    cout << "state 8 LOOP, wait mem_rdy" << endl;
                }
                else{
                    data_to_ron_reg = data_in;
                    alu_data_reg = data_in;
                    wait();
                    state = 6;
                }
                break;

            case 9:
                state_out = 9;
                cout << "current state" << state << endl;
                addr_up = 1;

                state = 10;
                break;

            case 10:
                state_out = 10;
                cout << "current state" << state << endl;
                addr_up = 0;

                if(memory_rdy.read() !=1){
                    state = 10;
                    cout << "state 10 LOOP, wait mem_rdy" << endl;
                }
                else{
                    mem_controller_load_addr_reg = (sc_uint<ADDR_SIZE>)data_in.read().range(ADDR_SIZE-1, 0);
                    wait();
                    mem_controller_load_addr = mem_controller_load_addr_reg.read();
                    mem_controller_rw = 1;
                    mem_controller_load = 1;

                    state = 11;
                }
                break;

            case 11:
                state_out = 11;
                cout << "current state" << state << endl;
                mem_controller_load = 0;

                if(memory_rdy.read() !=1){
                    state = 11;
                    cout << "state 11 LOOP, wait mem_rdy" << endl;
                }
                else{
                    data_to_ron_reg = data_in;
                    alu_data_reg = data_in;
                    wait();
                    state = 6;
                }
                break;

            case 12:
                state_out = 12;
                cout << "current state" << state << endl;

                if(cmd_num_reg.read() == 0){
                    cout << "state 12 to 13" << endl;
                    state = 13;
                }
                else{
                    cout << "state 12 to 17" << endl;
                    state = 17;
                }
                break;

            case 13:
                state_out = 13;
                cout << "current state" << state << endl;

                mem_controller_load_addr_reg = (sc_uint<ADDR_SIZE>)data_ron_in.read().range(ADDR_SIZE-1, 0); //??????? addr from ron
                wait();
                state = 14;
                break;

            case 14:
                state_out = 14;
                cout << "current state" << state << endl;
                cmd_type_out = 1;
                cmd_num_out = 3;
                ron_number_1_out = ron_number_1_reg.read();
                exec = 1;
                state = 15;
                break;

            case 15:
                state_out = 15;
                cout << "current state" << state << endl;
                exec = 0;

                if(alu_rdy.read() !=1){
                    state = 15;
                    cout << "state 15 LOOP, wait alu_rdy" << endl;
                }
                else{
                    mem_controller_load_data_reg = data_ron_in.read(); //data from ron
                    wait();
                    mem_controller_load_data = mem_controller_load_data_reg.read();
                    mem_controller_load_addr = mem_controller_load_addr_reg.read();
                    mem_controller_load = 1;
                    mem_controller_rw = 0; //write
                    state = 16;
                }
                break;

            case 16:
                state_out = 16;
                cout << "current state" << state << endl;
                mem_controller_load = 0;

                if(memory_rdy.read() !=1){
                    state = 16;
                    cout << "state 16 LOOP, wait mem_rdy" << endl;
                }
                else{
                    state = 1;
                }
                break;

            case 17:
                state_out = 17;
                cout << "current state" << state << endl;

                addr_up = 1;
                state = 18;
                break;

            case 18:
                state_out = 18;
                cout << "current state" << state << endl;

                addr_up = 0;
                if(memory_rdy.read() !=1){
                    state = 18;
                    cout << "state 18 LOOP, wait mem_rdy" << endl;
                }
                else{
                    mem_controller_load_addr_reg = (sc_uint<ADDR_SIZE>)data_in.read().range(ADDR_SIZE-1, 0);
                    wait();
                    state = 14;
                }
                break;

            case 19:
                state_out = 19;
                cout << "current state" << state << endl;

                addr_up = 1;
                state = 20;
                break;

            case 20:
                state_out = 20;
                cout << "current state" << state << endl;

                addr_up = 0;
                if(memory_rdy.read() !=1){
                    state = 20;
                    cout << "state 20 LOOP, wait mem_rdy" << endl;
                }
                else{
                    counter_load_addr_reg = (sc_uint<ADDR_SIZE>)data_in.read().range(ADDR_SIZE-1, 0);
                    wait();
                    mem_controller_rw = 1;
                    if((cmd_num_reg.read() == 1 && flags_in.read().bit(0) == 1) ||
                            (cmd_num_reg.read() == 2 && flags_in.read().bit(1) == 1) ||
                            (cmd_num_reg.read() == 3 && flags_in.read().bit(2) == 1) ||
                            (cmd_num_reg.read() == 4 && flags_in.read().bit(3) == 1) ||
                            (cmd_num_reg.read() == 5 && flags_in.read().bit(4) == 1) ||
                            (cmd_num_reg.read() == 6 && flags_in.read().bit(5) == 1) ||
                            cmd_num_reg.read() == 0) //condition true or dont care
                    {
                        counter_load_addr = counter_load_addr_reg.read();
                        counter_load = 1; //load & ask
                        state = 2; // wait cmd word from memory
                    }
                    else{ // condition false
                        state = 1; //counter continue
                    }
                }
                break;

            case 21:
                state_out = 21;
                cout << "current state" << state << endl;
                stop = 1;
                if(rst.read() !=1){
                    state = 21;
                    cout << " STOP STATE 21!" << endl;
                }
                else{
                    wait();
                    state = 0;
                }
                break;

            }//end switch
            wait();
        }//end while
    }//end en
}//end method
