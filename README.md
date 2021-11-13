# Intro:
Little program for practicing the conversion between ASCII characters, binary,
decimal and hexadecimal numbers.

## Build:
```
gcc ./convert.c -o convert
```

## Usage:
After compilation you can simply run the program without any parameters or you can
check the help message like:
```
./convert --help
Usage: ./convert [options]
--src=<ascii|bin|dec|hex>      specify source number format
--dst=<ascii|bin|dec|hex>      specify destination number format
--start=<decimal-number>       generate source number >= to this
--end=<decimal-number>         generate destination number < to this
```

## Example:
Convert hexadecimal number to binary string
```
./convert --src=hex --dst=bin
config.src = 16
config.dst = 2
config.start = 0
config.end = 256
0xda:11011010
OK
0xd1:00000000
00000000 is ivalid. Expected:11010001
```
Convert the capital letters of the ASCII table to hexadecimal numbers
```
./convert --src=ascii --dst=hex --start=65 --end=91
config.src = 0
config.dst = 16
config.start = 65
config.end = 91
H:48
OK
L:4C
OK
Y:00
0x00 is ivalid. Expected:0x59
```
