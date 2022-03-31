
// Copyright 2022 Mariano Abad, weimaraner@gmail.com.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>
#include <stdint.h>

size_t VaruintEnc(uint32_t value, uint8_t* output, size_t offset) {
    size_t outputSize = 0;
    uint8_t byte = 0;
    do{
        byte = value & 0x7f;
        value >>= 7;
        byte |= value ? 0x80 : 0x00;
        output[offset+outputSize] = byte;
        outputSize++;
    }while(value);
    return outputSize;
}

size_t VaruintDec(uint32_t* value,uint8_t *buffer){
    uint8_t i= 0;
    uint8_t byte;
    uint32_t result=0,pos=0;

    for ( i=0; i<5; i++){
        byte = buffer[i];
        result |= (byte & 0x7f) << pos;
        pos += 7;
        if ((byte&0x80) == 0) {
            *value = result;
            return i+1;
        }
    }
    return 0; // error
}




void printBuffer(uint8_t * buffer,size_t sz){
    uint8_t i=0;
    for (i =0 ;i <sz;i++){
        printf("%02X",buffer[i]);
    }
}

void testValue ( uint32_t test){
    size_t sz = 0;
    uint8_t test_buffer[5] = {0,0,0,0,0};
    uint32_t result = 0;
    
    printf("test = %05X \r\n",test);
    
    sz = VaruintEnc(test,test_buffer,0);
    printf("buffer = ");
    printBuffer(test_buffer,sz);
    printf("\r\n");
    if (VaruintDec(&result,test_buffer)){
        printf("result = %05X\r\n \r\n",result);
    }else{
        printf("decoder error\r\n\r\n");
    }
}

int main ( int argc, char **argv )
{
    //8 bits
    testValue(0xA5); //1 byte
    testValue(0xFF); //2 bytes
    //16 bits
    testValue(0xA5FF); //2 bytes
    testValue(0xFFFF); //3 bytes
    //32 bits
    testValue(0x00ffFFFF); //4 bytes
    testValue(0xFFFFffff); //5 bytes
   
    
    return 0;
}
