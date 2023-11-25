# zPrintz
 A simple library that contains a custom printf for C with new format arguments

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
![GitHub last commit (branch)](https://img.shields.io/github/last-commit/zLouis043/zPrintz/main)
![GitHub top language](https://img.shields.io/github/languages/top/zLouis043/zPrintz)

## How-To
Just include the header file your project and define its implementation: 

```c

#define ZPRINTZ_IMPLEMENTATION
#include "zprintz.h"

```

## Test and functionalities 

### To build and run the test file

```console

$ ./bar.bat 

```

### Example 

```c

#include <stdio.h>

#define ZPRINTZ_IMPLEMENTATION
#include "src/zprintz.h"

int main(void){

  zprintz("{green}Decimal Z{white}: {yellow}{d}{white}\n", 10);

  return 0;
}

Output > Decimal Z : 10
```
### Format Specifiers

| Format Argument | What it does |
|-----------------|--------------|
|      {d}        | Format specifier for the integers. | 
|      {x}        | Format specifier for the hexadecimals. | 
|      {o}        | Format specifier for the octals. | 
|      {b}        | Format specifier for the binary. | 
|      {f}        | Format specifier for the floats. | 
|      {r}        | Format specifier for the rational rapresentation for the floats. | 
|      {c}        | Format specifier for the chars. | 
|      {s}        | Format specifier for the strings. | 

## To change the color of the text to print 

| Colors Arguments | What it does |
|--------|----------|
| {red} | Turn the text red |
| {green} | Turn the text green |
| {yellow} | Turn the text yellow |
| {blue} | Turn the text blue |
| {magenta} | Turn the text magenta |
| {cyan} | Turn the text cyan |
| {white} | Turn the text white |

### Example

```c
 zprintz("{green}Decimal Z{white}: {yellow}{d}{white}\n", 10);
```
