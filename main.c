#include <stdio.h>

#define ZPRINTZ_IMPLEMENTATION 
#include "src/zprintz.h"

int main(void){

   int count; 

   count = printz("{green}Decimal Z{white}: {yellow}{d}{white}\n", 10);
   printz("{green}Count{white} : {yellow}{d}{white}\n", count);

   count = printf("Decimal F: %d\n", 10);
   printf("Count : %d\n", count);

   count = printz("{green}Float Z{white}: {yellow}{f}{white}\n", 12.5);
   printz("{green}Count{white} : {yellow}{d}{white}\n", count);

   count = printf("Float F: %f\n", 12.5);
   printf("Count : %d\n", count);

   count = printz("{green}Rational Z{white}: {yellow}{q}{white}\n", 12.5);
   printz("{green}Count{white} : {yellow}{d}{white}\n", count);

   count = printf("Rational F: DOES NOT EXIST\n", 12.5);
   printf("Count : %d\n", count);

   count = printz("{green}Octal Z{white}: {yellow}{x}{white}\n", 10);
   printz("{green}Count{white} : {yellow}{d}{white}\n", count);

   count = printf("Octal F: %o\n", 10);
   printf("Count : %d\n", count);

   count = printz("{green}Hexadecimal Z{white}: {yellow}{x}{white}\n", 10);
   printz("{green}Count{white} : {yellow}{d}{white}\n", count);

   count = printf("Hexadecimal F: %x\n", 10);
   printf("Count : %d\n", count);

   count = printz("{green}Binary Z{white}: {yellow}{b}{white}\n", 10);
   printz("{green}Count{white} : {yellow}{d}{white}\n", count);

   count = printf("Binary F: DOES NOT EXIST\n");
   printf("Count : %d\n", count);
   
   return 0; 
}
