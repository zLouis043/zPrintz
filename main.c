#include <stdio.h>

#define ZPRINTZ_IMPLEMENTATION 
#include "src/zprintz.h"

int main(void){

   int count; 

   count = zprintz("{green}Decimal Z{white}: {yellow}{d}{white}\n", 10);
   zprintz("{green}Count{white} : {yellow}{d}{white}\n", count);

   count = printf("Decimal F: %d\n", 10);
   printf("Count : %d\n", count);

   count = zprintz("{green}Float Z{white}: {yellow}{f}{white}\n", 12.5);
   zprintz("{green}Count{white} : {yellow}{d}{white}\n", count);

   count = printf("Float F: %f\n", 12.5);
   printf("Count : %d\n", count);

   count = zprintz("{green}Rational Z{white}: {yellow}{q}{white}\n", 12.5);
   zprintz("{green}Count{white} : {yellow}{d}{white}\n", count);

   count = printf("Rational F: DOES NOT EXIST\n", 12.5);
   printf("Count : %d\n", count);

   count = zprintz("{green}Octal Z{white}: {yellow}{o}{white}\n", 10);
   zprintz("{green}Count{white} : {yellow}{d}{white}\n", count);

   count = printf("Octal F: %o\n", 10);
   printf("Count : %d\n", count);

   count = zprintz("{green}Hexadecimal Z{white}: {yellow}{x}{white}\n", 10);
   zprintz("{green}Count{white} : {yellow}{d}{white}\n", count);

   count = printf("Hexadecimal F: %x\n", 10);
   printf("Count : %d\n", count);

   count = zprintz("{green}Binary Z{white}: {yellow}{b}{white}\n", 10);
   zprintz("{green}Count{white} : {yellow}{d}{white}\n", count);

   count = printf("Binary F: DOES NOT EXIST\n");
   printf("Count : %d\n", count);

   char * p = "hi";

   count = zprintz("{green}Pointer Z{white}: {yellow}{p}{white}\n", p);
   zprintz("{green}Count{white} : {yellow}{d}{white}\n", count);

   count = printf("Pointer F: %-20p\n", p);
   printf("Count : %d\n", count);
   
   count = zprintz("{green}Char Z{white}: {yellow}{c}{white}\n", 'c');
   zprintz("{green}Count{white} : {yellow}{d}{white}\n", count);

   count = printf("Char F: %c\n", 'c');
   printf("Count : %d\n", count);

   count = zprintz("{green}String Z{white}: {yellow}{s}{white}\n", "Hello World!");
   zprintz("{green}Count{white} : {yellow}{d}{white}\n", count);

   count = printf("String F: %s\n", "Hello World!");
   printf("Count : %d\n", count);

   return 0; 
}
