#include "cache.h"

void cache::prc_cache() {
    rdy = 0;
    wait();
    while(1){
        if(en){
            while (!ask){
                cout << "CACHE: wait ask" << endl;
                wait();
            }
            index = addr.read().range(INDEX_SIZE-1,0);
            tag = addr.read().range(ADDR_SIZE-1,INDEX_SIZE);
            cout << "CACHE: input addr " << dec << addr.read() << endl;
            cout << "CACHE: input index " << hex << index << endl;
            cout << "CACHE: input tag " << hex << tag << endl;
            //getchar();

            wait();
            if(!rw_in){ //write
                cout << "write" << endl;
                cache_mem[index.read()] = (tag.read(), data_in.read());
                valid[index.read()] = 1;
                addr_to_ram_out = (tag.read(), index.read());
                data_to_ram_out = data_in.read();
                rw_out = 0; //write
                ask_ram = 1;
                while(!ram_rdy){
                    cout << "CACHE: wait ram rdy of write" << endl;
                    wait();
                    ask_ram = 0;
                }
                rdy = 1;
            }
            else{//read
                cout << "read" << endl;
                if(valid[index.read()].read()==1 && tag.read() == cache_mem[index.read()].read().range(CACHE_DATA_SIZE-1,DATA_SIZE)){
                    cout <<"CACHE: cached data!" << endl;
                    data_out = cache_mem[index.read()].read().range(DATA_SIZE-1,0);
                    wait();
                    rdy = 1;
                }
                else{
                    cout <<"CACHE: NOT cached data! Update line" << endl;
                    addr_to_ram_out = addr.read();
                    rw_out = 1;
                    ask_ram = 1;
                    while(!ram_rdy){
                        cout << "CACHE: wait ram rdy of read" << endl;
                        wait();
                        ask_ram = 0;
                    }
                    cache_mem[index.read()] = (tag.read(), data_from_ram.read());
                    valid[index.read()] = 1;
                    wait();
                    data_out = cache_mem[index.read()].read().range(DATA_SIZE-1,0);
                    wait();
                    rdy = 1;
                }
            }//end read
        }

        wait();
        rdy = 0;
    }
}

