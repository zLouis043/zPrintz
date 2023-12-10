#ifndef ZPRINTZ_H_
#define ZPRINTZ_H_

#include <stdarg.h>
#include <windows.h>
#include <io.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

#define ANSI_COLOR_RED     "\x1b[0;31m"
#define ANSI_COLOR_GREEN   "\x1b[0;32m"
#define ANSI_COLOR_YELLOW  "\x1b[0;33m"
#define ANSI_COLOR_BLUE    "\x1b[0;34m"
#define ANSI_COLOR_MAGENTA "\x1b[0;35m"
#define ANSI_COLOR_CYAN    "\x1b[0;36m"
#define ANSI_COLOR_RESET   "\x1b[0m"


/*!
    @param _Stream The output stream where to write the character
    @param c The character to write
    @return The number of characters written
*/
static inline int fzprintz_char(FILE * _Stream, const char c);

/*!
    @param _Stream The output stream where to write the character
    @param str The str to write
    @return The number of characters written
*/
static inline int fzprintz_string(FILE * _Stream, const char * str);

/*!
    @param _Stream The output stream where to write the character
    @param digit The digit to write
    @param base The base of the number to write
    @return The number of characters written
*/
static inline int fzprintz_digit(FILE * _Stream, long digit, int base);

/*!
    @param _Stream The output stream where to write the character
    @param specifier The type of the fmt specifier
    @param ap The argument to consider
    @return The number of characters written
*/
static inline int fzprintz_fmt(FILE * _Stream, const char specifier, va_list ap);

/*!
    @param _Stream The output stream where to write the character
    @param color The color to print
    @param color_name The name of the color argument
    @param fmt The string to format
    @return The number of characters written
*/
static inline int fzprintz_color(FILE * _Stream, const char * color, const char * color_name, const char ** fmt);

/*!
    @param _Stream The output stream where to write the character
    @param num The float to convert
    @param order The order of precision 
    @return The number of characters written
*/
static inline int fzprintz_rational(FILE * _Stream, double num, size_t order);

/*!
    @param _Stream The output stream where to write the character
    @param file_name The file where the function is being called
    @param line_number The line where the function is being called
    @param fmt The string to format
    @param ... The variadic arguments in the function call
    @return The number of characters written
*/
static inline int fzprintz_(FILE * _Stream, const char * file_name, size_t line_number,  const char * fmt, ...);

/*!
    @param ... The string to format and print 
    @return The number of characters written
*/
#define zprintz(...) fzprintz_(stdout, __FILE__, __LINE__ ,"" __VA_ARGS__)

/*! 
    @param _Stream The output stream where to write the character
    @param ... The string to format and print 
    @return The number of characters written
*/
#define fzprintz(_Stream, ...) fzprintz_(_Stream, __FILE__, __LINE__ ,"" __VA_ARGS__)

#endif // ZPRINTZ_H_

#ifdef ZPRINTZ_IMPLEMENTATION 

static inline int fzprintz_char(FILE * _Stream, const char c){

    return fwrite(&c, 1, 1, _Stream);

}

static inline int fzprintz_string(FILE * _Stream, const char * str){

    int count = 0;

    while(*str) count += fwrite(str++, 1, 1, _Stream);

    return count;

}

static inline int fzprintz_unsigned_number(FILE * _Stream, uint64_t number, int base){

    int count = 0;

    if(number == 0) return fzprintz_char(_Stream, '0');

    char buffer[17];

    while(number){
        int digit = number % base;
        if(digit >= 10){
            buffer[count++] = 'a' + (digit - 10);
        }else {
           buffer[count++] = '0' + digit;
        }

        number /= base;
    }

    for(int i = count - 1; i >= 0; i--){
        fzprintz_char(_Stream, buffer[i]);
    }

    return count - 1;
}

static inline int fzprintz_digit(FILE * _Stream, long digit, int base){

    int count = 0;

    char digits[] = "0123456789abcdef";

    if(digit < 0){
        fwrite("-", 1, 1, _Stream);
        return fzprintz_digit(_Stream, -digit, base) + 1;
    }else if(digit < base){
        return fzprintz_char(_Stream, digits[digit]);
    }else {
        count = fzprintz_digit(_Stream, digit / base, base);
        return count + fzprintz_digit(_Stream, digit % base, base);
    }

}


static inline int fzprintz_float(FILE * _Stream, double value){

    int count = 0;

    char * number = malloc(sizeof(char*) * 256);

    snprintf(number, 256, "%f", value);
    fwrite(number, strlen(number), 1, _Stream);

    count = strlen(number);

    return count;
    
}

/*
    Convert a float to a rationalized string. 
    Function inspired from https://github.com/kevinboone/rationalize
*/
static inline int fzprintz_rational(FILE * _Stream, double num, size_t order){

    int whole; 
    bool negative;
    bool improper;

    char sign = '+'; 
    negative = false;
    if (num < 0) 
    {
        negative = true;
        num = -num;
        sign = '-';
    }

    if(num < 1 && abs(num) > 0){
        improper = false;
    }else {
        improper = true;
    }

    if (!improper)
    {
    double frac = num - floor(num);
    whole = (int) floor(num);
    num = frac;
    }
    else
    whole = 0;

    // The largest possible value for "order" is 9, because we have
    //   to be able to fit "10^(order*2)" into the 64-bit integer type
    //   we are using for computation. 
    if (order > 9) order = 9;

    // In this version, we scale the original number so that it has at
    //  least as many non-fraction digits as "order". "scale" is the 
    //  scaling factor, that will be used throughout the computation.
    int64_t scale = pow(10, order);

    // Take "x" to be the scaled version of the number to be converted
    int64_t x = num * scale;
    int64_t orig_x = x;

    // Calculate the first continued fraction coefficient, but bear in mind
    //   that it's derived from a number that is "scale" times too large.
    int64_t a = orig_x / scale; 

    // Previous versions of the numerator and denominator, as in version 1.
    // (No scaling here)
    int64_t n = a;
    int64_t n1 = 1;
    int64_t n2;

    int64_t d = 1;
    int64_t d1 = 0;
    int64_t d2;

    // We expand the continued fraction as in version 1, but we have placed
    //   a limit "order" on the number of significant figures in the results.
    // Therefore, we never need to loop more times than order.
    int loops = 0;
    while (loops++ < order && x - a * scale)
    {
    // In version 1, we had "x = 1 / (x-a)", in floating-point
    //  numbers. Of course, we can't do this calculation in integers,
    //  but we can multiply by the scaling factor "scale". A first
    //  guess at the logic might be "x = scale / (x / scale - a)", but 
    //  this fails because "x / scale" is probably zero in integers. So
    // We need to multiple top and bottom by "scale" to get a version that
    //  will always succeed in integers. Of course, this limits the
    //  value of "order", as scale=10^order, and an int64 is only so big.
    x = scale * scale / (x - a * scale);

    // Calculate the next continued fraction coefficient, again bearing
    // in mind that it comes from a scaled number. Dividing by "scale"
    //  is the equivalent in this integer version as taking floor(x) in
    // version 1.
    a = x / scale; 

    // The expansion of the continued fraction into numerator and
    //   denominator is the same as in version 1.
    n2 = n1; 
    n1 = n;
    n = n2 + a * n1;

    d2 = d1; 
    d1 = d;
    d = d2 + a * d1;

    // If the value of n/d is _exact_ compared to the original number,
    //   stop here -- no point looping further, as all the following
    //   continued fraction coefficients will be zero.
    if (!d) break;
    if ( (( scale * n / d - orig_x ) > 0 ? ( scale * n / d - orig_x ) : -( scale * n / d - orig_x )) <= 1) break; 
    } 

    int count = 0;
    char * numb = (char*)malloc(256 * sizeof(char));

    if(n == 0){
        snprintf(numb, 256, "%c%d", sign, 0);
        count = fwrite(numb, strlen(numb), 1, _Stream);
    }else if(d == 1){
        snprintf(numb, 256, "%c%d", sign, n);
        count = fwrite(numb, strlen(numb), 1, _Stream);
    }else {
        snprintf(numb, 256, "%c%d/%d", sign, n, d);
        count = fwrite(numb, strlen(numb), 1, _Stream);
    }

    free(numb);

    return count;
}

static inline int fzprintz_fmt(FILE * _Stream, const char specifier, va_list ap){

    int count = 0;

    if(specifier == 'c' || specifier == 'C'){
        count = fzprintz_char(_Stream, va_arg(ap, int));
    }else if(specifier == 's' || specifier == 'S'){
        count = fzprintz_string(_Stream, va_arg(ap, char *));
    }else if(specifier == 'd' || specifier == 'D'){
        count = fzprintz_digit(_Stream, (long)va_arg(ap, int), 10);
    }else if(specifier == 'x' || specifier == 'X'){
        count = fzprintz_digit(_Stream, (long)va_arg(ap, unsigned int), 16);
    }else if(specifier == 'o' || specifier == 'O'){
        count = fzprintz_digit(_Stream, (long)va_arg(ap, unsigned int), 8);
    }else if(specifier == 'b' || specifier == 'B'){
        count = fzprintz_digit(_Stream, (long)va_arg(ap, unsigned int), 2);
    }else if(specifier == 'f' || specifier == 'F'){
        count += fzprintz_float(_Stream, (double)va_arg(ap, double));
    }else if(specifier == 'q' || specifier == 'Q'){
        count += fzprintz_rational(_Stream, (double)va_arg(ap, double), 10);
    }else if(specifier == 'p' || specifier == 'P'){

        #if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
        const char *prefix = "0x";
        #elif _WIN32
        const char *prefix = "0000";
        #endif 

        void *p = va_arg(ap, void *);
        count += fwrite(prefix, 1, strlen(prefix), _Stream);
        count += fzprintz_unsigned_number(_Stream, (uint64_t)p, 16);
    }else {
        count += fwrite(&specifier, 1, 1, _Stream);
    }

    return count;

}

static inline int fzprintz_color(FILE * _Stream, const char * color, const char * color_name, const char ** fmt){

    int color_len = strlen(color);
    int color_name_len = strlen(color_name);

    int count = fwrite(color, color_len, 1, _Stream);
    *fmt += color_name_len;

    return count;
}

static inline int fzprintz_(FILE * _Stream, const char * file_name, size_t line_number, const char * fmt, ...){
    
    va_list ap;

    int count = 0;

    va_start(ap, fmt);

    while(*fmt){
        if(*fmt == '\\'){

            fmt++;
            if(*fmt == '{'){
                count += fwrite(fmt, 1, 1, _Stream);
            }
            
        }else if(*fmt == '{'){

            fmt++;

            if(strncmp(fmt, "RED", 3) == 0 || strncmp(fmt, "red", 3) == 0){
                count += fzprintz_color(_Stream, ANSI_COLOR_RED, "RED", &fmt);
            }else if(strncmp(fmt, "GREEN", 6) == 0 || strncmp(fmt, "green", 5) == 0){
                count += fzprintz_color(_Stream, ANSI_COLOR_GREEN, "GREEN", &fmt);
            }else if(strncmp(fmt, "YELLOW", 6) == 0 || strncmp(fmt, "yellow", 6) == 0){
                count += fzprintz_color(_Stream, ANSI_COLOR_YELLOW, "YELLOW", &fmt);
            }else if(strncmp(fmt, "BLUE", 4) == 0 || strncmp(fmt, "blue", 4) == 0){
                count += fzprintz_color(_Stream, ANSI_COLOR_BLUE, "YELLOW", &fmt);
            }else if(strncmp(fmt, "MAGENTA", 7) == 0 || strncmp(fmt, "magenta", 7) == 0){
                count += fzprintz_color(_Stream, ANSI_COLOR_MAGENTA, "YELLOW", &fmt);
            }else if(strncmp(fmt, "CYAN", 4) == 0 || strncmp(fmt, "cyan", 4) == 0){
                count += fzprintz_color(_Stream, ANSI_COLOR_CYAN, "CYAN", &fmt);
            }else if(strncmp(fmt, "WHITE", 5) == 0 || strncmp(fmt, "white", 5) == 0){
                count += fzprintz_color(_Stream, ANSI_COLOR_RESET, "WHITE", &fmt);
            }else {
                count += fzprintz_fmt(_Stream, *fmt, ap);
                fmt++;
            }

            if(*fmt != '}'){
                printf("Char : %c", *fmt);
                fprintf(stderr, "\n%s[ERROR]%s: Missing closing '}' in %s%s:%zu%s\n", ANSI_COLOR_RED, ANSI_COLOR_RESET, ANSI_COLOR_YELLOW, file_name, line_number, ANSI_COLOR_RESET);
                exit(EXIT_FAILURE);
            }

        }else{

            count += fwrite(fmt, 1, 1, _Stream);
        }

        fmt++;
    }

    va_end(ap);

    count += fzprintz_color(_Stream, ANSI_COLOR_RESET, "WHITE", &fmt);

    return count;

}

#endif // ZPRINTZ_IMPLEMENTATION 
