#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>
#include <stdio.h>
#include <ArduinoSTL.h>

 
class Utils{
public:
    Utils();
    static void print_hex(unsigned char * buffer_in, int buffer_size, char * buffer_out);
    static void timeToString(char* string, size_t size);
    static std::vector<String> stringSplit(String data, char separator);
     
};

#endif // UTILS_H
