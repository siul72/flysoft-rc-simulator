#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <stdio.h>

class utils
{
public:
    utils();
    static void print_hex(unsigned char * buffer_in, int buffer_size, char * buffer_out);
};

#endif // UTILS_H
