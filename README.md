# stringslib
## Memory safe library for manage strings 

-------------------------------

# string_s

Buffered string structure. 

## Public Attributes

|                | Name           |
| -------------- | -------------- |
| uint32_t | **cap**  |
| uint32_t | **len**  |
| char[] | **data**  |

## Public Attributes Documentation

### variable cap

```cpp
uint32_t cap;
```

capacity


### variable len

```cpp
uint32_t len;
```

current length 


### variable data

```cpp
char[] data;
```

null-terminated string 

## Types

|                | Name           |
| -------------- | -------------- |
| typedef struct string_s | **string_t**  |
| typedef string_t * | **String**  |

## Types Documentation

### typedef string_t

```cpp
typedef struct string_s string_t;
```

Buffered string internal type


### typedef String

```cpp
typedef string_t* String;
```

Buffered string main type

-------------------------------

# Strings Buffer Functions

## Functions

|                | Name           |
| -------------- | -------------- |
| String | **string_buf_new**(const size_t cap)<br>Allocate a new Buffer of capacity `cap`.  |
| String | **string_buf_init**(const char * str)<br>Allocate a new Buffer of capacity `cap` and copy string.  |
| int | **string_buf_append**(String buf, const char * fmt, ... )<br>Append a formatted c-string to `buf`. If new data would exceed capacity, `buf` stays unmodified.  |
| int | **string_buf_write**(String buf, const char * fmt, ... )<br>Write a formatted c-string at beginning of `buf`. If new data would exceed capacity, `buf` stays unmodified.  |
| bool | **string_buf_equal**(const String a, const String b)<br>Compare strings equality.  |
| bool | **string_buf_equal_const**(const String a, const char * b)<br>Compare strings equality.  |
| String | **string_buf_dup**(const String buf)<br>Duplicate string.  |
| bool | **string_buf_resize**(String * pbuf, const size_t newcap)<br>Resize capacity.  |
| size_t | **string_buf_cap**(const String buf)<br>Return capacity.  |
| const char * | **string_buf_data**(const String buf)<br>Return Data of Buffered string.  |
| void | **string_buf_reset**(String buf)<br>Reset Buffered string content.  |

## Functions Documentation

### function string_buf_new

```cpp
String string_buf_new(
    const size_t cap
)
```

Allocate a new Buffer of capacity `cap`. 

**Parameters**: 

  * **cap** Capacity 


**Return**: Buffered string 

### function string_buf_init

```cpp
String string_buf_init(
    const char * str
)
```

Allocate a new Buffer of capacity `cap` and copy string. 

**Parameters**: 

  * **str** String 


**Return**: Buffered string|NULL 

### function string_buf_append

```cpp
int string_buf_append(
    String buf,
    const char * fmt,
    ... 
)
```

Append a formatted c-string to `buf`. If new data would exceed capacity, `buf` stays unmodified. 

**Parameters**: 

  * **buf** Buffered string 
  * **fmt** Format 


**Return**: Change in length. 

### function string_buf_write

```cpp
int string_buf_write(
    String buf,
    const char * fmt,
    ... 
)
```

Write a formatted c-string at beginning of `buf`. If new data would exceed capacity, `buf` stays unmodified. 

**Parameters**: 

  * **buf** Buffered string 
  * **fmt** Format 


**Return**: New length or zero on failure. 

### function string_buf_equal

```cpp
bool string_buf_equal(
    const String a,
    const String b
)
```

Compare strings equality. 

**Parameters**: 

  * **a** Buffered string 
  * **b** Buffered string 


**Return**: Boolean 

### function string_buf_equal_const

```cpp
bool string_buf_equal_const(
    const String a,
    const char * b
)
```

Compare strings equality. 

**Parameters**: 

  * **a** Buffered string 
  * **b** String 


**Return**: Boolean 

### function string_buf_dup

```cpp
String string_buf_dup(
    const String buf
)
```

Duplicate string. 

**Parameters**: 

  * **buf** Buffered string 


**Return**: Buffer 

### function string_buf_resize

```cpp
bool string_buf_resize(
    String * pbuf,
    const size_t newcap
)
```

Resize capacity. 

**Parameters**: 

  * **pbuf** Buffered string 
  * **newcap** New capacity 


**Return**: Boolean 

### function string_buf_cap

```cpp
size_t string_buf_cap(
    const String buf
)
```

Return capacity. 

**Parameters**: 

  * **buf** Buffered string 


**Return**: Capacity 

### function string_buf_data

```cpp
const char * string_buf_data(
    const String buf
)
```

Return Data of Buffered string. 

**Parameters**: 

  * **buf** Buffered string 


**Return**: String 

### function string_buf_reset

```cpp
void string_buf_reset(
    String buf
)
```

Reset Buffered string content. 

**Parameters**: 

  * **buf** Buffered string 

-------------------------------

# Strings Manipulation Functions

## Functions

|                | Name           |
| -------------- | -------------- |
| size_t | **string_len**(const String buf)<br>Buffered string length.  |
| String | **string_left**(const String buf, uint32_t pos)<br>Substring left from position.  |
| String | **string_right**(const String buf, uint32_t pos)<br>Substring right from position.  |
| String | **string_mid**(const String buf, uint32_t left, uint32_t right)<br>Substring left from position left to position right.  |
| String | **string_concat**(const String str1, const String str2)<br>Concatenation of strings.  |
| String | **string_insert**(const String buf, const String str, uint32_t pos)<br>Insert string on position.  |
| String | **string_delete**(const String buf, uint32_t pos1, uint32_t pos2)<br>Delete substring from pos1 to pos2.  |
| String | **string_replace**(const String buf, const String search, String replace)<br>Replace string.  |
| uint32_t | **string_find**(const String buf, const String search)<br>Find substring.  |
| String | **string_toupper**(const String buf)<br>To upper string.  |
| String | **string_tolower**(const String buf)<br>To lower string.  |
| String | **string_trim**(const String buf)<br>Trim string.  |

## Functions Documentation

### function string_len

```cpp
size_t string_len(
    const String buf
)
```

Buffered string length. 

**Parameters**: 

  * **buf** Buffered string 


**Return**: Size 

### function string_left

```cpp
String string_left(
    const String buf,
    uint32_t pos
)
```

Substring left from position. 

**Parameters**: 

  * **buf** Buffered string 
  * **pos** Position 


**Return**: Buffered string 

### function string_right

```cpp
String string_right(
    const String buf,
    uint32_t pos
)
```

Substring right from position. 

**Parameters**: 

  * **buf** Buffered string 
  * **pos** Position 


**Return**: Buffered string 

### function string_mid

```cpp
String string_mid(
    const String buf,
    uint32_t left,
    uint32_t right
)
```

Substring left from position left to position right. 

**Parameters**: 

  * **buf** Buffered string 
  * **left** Position 
  * **right** Position 


**Return**: Buffered string 

### function string_concat

```cpp
String string_concat(
    const String str1,
    const String str2
)
```

Concatenation of strings. 

**Parameters**: 

  * **str1** Buffered string 
  * **str2** Buffered string 


**Return**: Buffered string 

### function string_insert

```cpp
String string_insert(
    const String buf,
    const String str,
    uint32_t pos
)
```

Insert string on position. 

**Parameters**: 

  * **buf** Buffered string 
  * **str** Buffered string 
  * **pos** Position 


**Return**: Buffered string 

### function string_delete

```cpp
String string_delete(
    const String buf,
    uint32_t pos1,
    uint32_t pos2
)
```

Delete substring from pos1 to pos2. 

**Parameters**: 

  * **buf** Buffered string 
  * **pos1** Position 
  * **pos2** Position 


**Return**: Buffered string 

### function string_replace

```cpp
String string_replace(
    const String buf,
    const String search,
    String replace
)
```

Replace string. 

**Parameters**: 

  * **buf** Buffered string 
  * **search** Buffered string 
  * **replace** Buffered string 


**Return**: Buffered string 

### function string_find

```cpp
uint32_t string_find(
    const String buf,
    const String search
)
```

Find substring. 

**Parameters**: 

  * **buf** Buffered string 
  * **search** Buffered string 


**Return**: Position 

### function string_toupper

```cpp
String string_toupper(
    const String buf
)
```

To upper string. 

**Parameters**: 

  * **buf** Buffered string 


**Return**: Buffered string 

### function string_tolower

```cpp
String string_tolower(
    const String buf
)
```

To lower string. 

**Parameters**: 

  * **buf** Buffered string 


**Return**: Buffered string 

### function string_trim

```cpp
String string_trim(
    const String buf
)
```

Trim string. 

**Parameters**: 

  * **buf** Buffered string 


**Return**: Buffered string 

-------------------------------

