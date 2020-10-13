#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <inttypes.h>

//getting random numbers
int randomnum(void)
 {
        uint32_t  buffer[32];
        unsigned long urandom;
	uint32_t myrand[1];

        urandom = open("/dev/urandom", O_RDONLY);
        read(urandom, buffer,32);
        //buffer contains the random data
        close(urandom);
	myrand[0]=buffer[0];

return *myrand;
}
#define ROTL(a,b) (((a) << (b)) | ((a) >> (32 - (b))))
#define QR(a, b, c, d)( \
        b ^= ROTL(a + d, 7), \
        c ^= ROTL(b + a, 9), \
        d ^= ROTL(c + b,13), \
        a ^= ROTL(d + c,18))

#define ROUNDS 20

//uint32_t salsa20_block(uint32_t out[16], uint32_t const in[16]) {
uint32_t salsa20_block(uint32_t out[16], uint32_t const in[16]) {


        int i;
        uint32_t x[16];
        //uint32_t in[16];

        for(i = 0; i < 16; ++i){
                x[i] = in[i];
                printf("entrada   %u \n\r", in[i]);
                }
        //10loops × 2rounds/loop = 20 rounds

        for(i = 0; i < ROUNDS; i += 2) {
	      x[8]++; x[9]++;
                //Oddround
                QR(x[ 0], x[ 4], x[ 8], x[12]); //column1
                QR(x[ 5], x[ 9], x[13], x[ 1]);//column2
                QR(x[10], x[14], x[ 2], x[ 6]);//column3
                QR(x[15], x[ 3], x[ 7], x[11]);//column4
	      x[8]++; x[9]++;
                //Evenround
                QR(x[ 0], x[ 1], x[ 2], x[ 3]);//row1
                QR(x[ 5], x[ 6], x[ 7], x[ 4]);//row2
                QR(x[10], x[11], x[ 8], x[ 9]);//row3
                QR(x[15], x[12], x[13], x[14]);//row4
                }
        for(i = 0; i < 16; ++i){
                out[i] = x[i]+in[i];
                printf("Salsa20   %u \n\r", out[i]);
                }
        printf("\n\n\r---------------------\n\r");
        return *out;
}


int main ()
{
//getting the  start block
uint32_t x[16];
int i;
uint32_t  outA[16];
uint32_t  orig[16];

//Assigning Ci

 x[0]=0;x[5]=5;x[10]=10;x[15]=15;

//Assigning  k0-k3 (x[1]-x[4])
for (i=1; i<5; i++) {
x[i]= randomnum();
//printf("%u \n", x[i]);
}

//Assigning  k0-k3 (x[11]-x[14])
for (i=11; i<15; i++) {
x[i]= randomnum();
//printf("%u \n", x[i]);
}

//Assigning  n0-n1 (x[6]-x[7])
for (i=6; i<8; i++) {
x[i]= randomnum();
//printf("%u \n", x[i]);
}

//Assigning   (x[8]-x[9])
x[8]=0;
x[9]=0;

//running salsa20

salsa20_block(outA,x);
salsa20_block(orig, outA);

return 0;
}
