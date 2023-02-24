# stringslib
## Memory safe library for manage strings 

-------------------------------

# Strings Core Functions

## Functions

|                | Name                                                                         |
| -------------- | ---------------------------------------------------------------------------- |
| String         | **string_new**(const size_t cap)<br>Allocate a new Buffer of capacity `cap`. |
| String         | **string_new_c**(const char *str)<br>Allocate a new Buffer and copy string.  |
| String         | **string_dup**(const String buf)<br>Duplicate string.                        |
| bool           | **string_resize**(String *pbuf, const size_t newcap)<br>Resize capacity.     |
| const char*    | **string_data**(const String buf)<br>Return Data of Buffered string.         |
| void           | **string_reset**(String buf)<br>Reset Buffered string content.               |

-------------------------------

# Strings Manipulation Functions

## Functions

|                | Name                                                                                                                     |
| -------------- | ------------------------------------------------------------------------------------------------------------------------ |
| String         | **string_left**(const String buf, uint32_t pos)<br>Substring left from position.                                         |
| String         | **string_right**(const String buf, uint32_t pos)<br>Substring right from position.                                       |
| String         | **string_mid**(const String buf, uint32_t left, uint32_t right)<br>Substring left from position left to position right.  |
| String         | **string_concat**(const String str1, const String str2)<br>Concatenation of strings.                                     |
| String         | **string_insert**(const String buf, const String str, uint32_t pos)<br>Insert string on position.                        |
| String         | **string_delete**(const String buf, uint32_t pos1, uint32_t pos2)<br>Delete substring from pos1 to pos2.                 |
| String         | **string_replace**(const String buf, const String search, String replace, uint32_t pos)<br>Replace string.               |
| uint32_t       | **string_find**(const String buf, const String search, uint32_t pos)<br>Find substring starting at position.             |
| uint32_t       | **string_find_c**(const String buf, char c, uint32_t pos)<br>Find character starting at position.                        |
| String         | **string_toupper**(const String buf)<br>To upper string.                                                                 |
| String         | **string_tolower**(const String buf)<br>To lower string.                                                                 |
| String         | **string_ltrim**(const String buf)<br>Left trim string                                                                   |
| String         | **string_rtrim**(const String buf)<br>Right trim string                                                                  |
| String         | **string_trim**(const String buf)<br>Trim string.                                                                        |
| String         | **string_split**(const String buf, const char *search, String *right)<br>Split string and return left and right Strings  |
| uint32_t       | **string_append**(String buf, const char *fmt, ... )<br>Append a formatted c-string to `buf`.<br>If new data would exceed capacity, `buf` stays unmodified.  |
| uint32_t       | **string_write**(String buf, const char *fmt, ... )<br>Write a formatted c-string at beginning of `buf`.<br>If new data would exceed capacity, `buf` stays unmodified.  |
| bool           | **string_equals**(const String str1, const String str2)<br>Compares two strings.                                         |
| bool           | **string_equals_c**(const String a, const char *b)<br>Compare strings equality.                                          |
| bool           | **string_isinteger**(const String buf)<br>Check if string is a valid integer.                                            |
| bool           | **string_isfloat**(const String buf)<br>Check if string is a valid float.                                                |
| uint8_t        | **string_isrealexp**(const String buf)<br>Check if string is a valid scientific notation.                                |
| long           | **string_tolong**(const String buf)<br>Convert string to integer. Max value: LONG_MAX_MAX - 1.                           |
| double         | **string_todouble**(const String buf)<br>Convert string to float. Max value: DBL_MAX - 1.                                |
| string_hash_t  | **string_hash**(const String buf, uint8_t version, uint8_t key[16])<br>String hash.                                      |

-------------------------------

