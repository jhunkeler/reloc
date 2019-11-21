# Reloc

Replace strings in binary executables and data files.

![](https://github.com/jhunkeler/reloc/workflows/ci/badge.svg)

## System Requirements

* `gcc`
* _or_ `clang`
* _or_ Microsoft Visual Studio (`cl.exe`)p
* `>=cmake-2.18.11`

## Installing

### Linux / MacOS

```bash
$ git clone https://github.com/jhunkeler/reloc
$ mkdir build
$ cd build
$ cmake -DCMAKE_INSTALL_PREFIX=/some/place -DCMAKE_BUILD_TYPE=Release ..
$ make install
```

### Windows

```cmd
> git clone https://github.com/jhunkeler/reloc
> cd reloc
> mkdir build
> cd build
> cmake -DCMAKE_INSTALL_PREFIX=c:\some\place -DCMAKE_BUILD_TYPE=Release ..
> cmake --build . --target INSTALL
```

## Usage
```
$ reloc <str1> <str2> <input_file> <output_file>

Arguments:
str1 - Pattern to search for
str2 - Replace str1 with contents of str2
input_file - Path to input file
output_file - Path to output file

Example:
reloc /original/path /new/path input.bin output.bin
```

## Showcase

### Before

```
$ xxd -u -g1 test.dat
00000000: DE AD BE EF 2F 5F 30 31 32 33 34 35 36 37 38 39  ..../_0123456789
00000010: 5F 30 31 32 33 34 35 36 37 38 39 5F 30 31 32 33  _0123456789_0123
00000020: 34 35 36 37 38 39 5F 30 31 32 33 34 35 36 37 38  456789_012345678
00000030: 39 5F 30 31 32 33 34 35 36 37 38 39 5F 30 31 32  9_0123456789_012
00000040: 33 34 35 36 37 38 39 5F 30 31 32 33 34 35 36 37  3456789_01234567
00000050: 38 39 5F 2F 74 65 73 74 00 52 41 4E 44 4F 4D 20  89_/test.RANDOM
00000060: 44 41 54 41 20 54 48 41 54 20 43 41 4E 4E 4F 54  DATA THAT CANNOT
00000070: 20 42 45 20 4D 4F 56 45 44 00 FE CE FA CE 2F 5F   BE MOVED...../_
00000080: 30 31 32 33 34 35 36 37 38 39 5F 30 31 32 33 34  0123456789_01234
00000090: 35 36 37 38 39 5F 30 31 32 33 34 35 36 37 38 39  56789_0123456789
000000a0: 5F 30 31 32 33 34 35 36 37 38 39 5F 30 31 32 33  _0123456789_0123
000000b0: 34 35 36 37 38 39 5F 30 31 32 33 34 35 36 37 38  456789_012345678
000000c0: 39 5F 30 31 32 33 34 35 36 37 38 39 5F 2F 73 75  9_0123456789_/su
000000d0: 66 66 69 78 00 FF FF FF FF FF FF FF FF FF        ffix..........
```

### During

```bash
$ reloc \
    /_0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_0123456789_ \
    /tmp/_installed \
    test.dat \
    output.dat
```

### After

```
xxd -u -g1 output.dat
00000000: DE AD BE EF 2F 74 6D 70 2F 5F 69 6E 73 74 61 6C  ..../tmp/_instal
00000010: 6C 65 64 2F 74 65 73 74 00 00 00 00 00 00 00 00  led/test........
00000020: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
00000030: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
00000040: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
00000050: 00 00 00 00 00 00 00 00 00 52 41 4E 44 4F 4D 20  .........RANDOM
00000060: 44 41 54 41 20 54 48 41 54 20 43 41 4E 4E 4F 54  DATA THAT CANNOT
00000070: 20 42 45 20 4D 4F 56 45 44 00 FE CE FA CE 2F 74   BE MOVED...../t
00000080: 6D 70 2F 5F 69 6E 73 74 61 6C 6C 65 64 2F 73 75  mp/_installed/su
00000090: 66 66 69 78 00 00 00 00 00 00 00 00 00 00 00 00  ffix............
000000a0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
000000b0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
000000c0: 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00 00  ................
000000d0: 00 00 00 00 00 FF FF FF FF FF FF FF FF FF        ..............
```
