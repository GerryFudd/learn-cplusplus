[Language properties](../language_properties.md)

## Data structures

### Native

The native data types are available as part of the language's ecosystem. I've broken these into several smaller categories.

#### Primitive CS stuff

`bool` is either `true` or `false` and only takes up one byte. The `byte` data type takes on the values from `-128` to `127`, but doesn't support mathematical operations.

#### Numbers

| class | values | bytes |
| :--- | :----: | ---: |
| `int` | `-(2^(31))` to `2^(31)-1` | 4 |
| `unsigned int` | `0` to `2^(32)-1` | 4 |
| `short` | `-(2^(15))` to `2^(15)-1` | 2 |
| `unsigned short`| `0` to `2^(16)-1` | 2 |
| `long` | `-(2^(63))` to `2^(63)-1` | 8 |
| `unsigned long` | `0` to `2^(64)-1` | 8 |
| `float` | significant digits `-(2^{23})` to `2^(23)-1` | 4 |
|  | times `2^(-128)` to `2^(127)` | |
| `double` | significant digits `-(2^(51))` to `2^(51)-1` | 8 |
|  | times `2^(-1024)` to `2^(1023)` | |


#### Text and junk

`char` represents a single letter or other text symbol. This takes up one byte in memory. `wchar_t` is a character that uses two bytes and represents a symbol from a character set that doesn't fit in a single byte.
