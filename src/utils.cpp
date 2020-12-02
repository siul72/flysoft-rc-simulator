#include "utils.h"

utils::utils(){

}

void utils::print_hex(unsigned char *buffer_in, int buffer_size, char * msg){

    for(int j = 0; j < buffer_size; j++)
        sprintf(&msg[5*j], "0x%02X,", buffer_in[j]);

}
