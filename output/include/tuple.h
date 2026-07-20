#ifndef TUPLE_H
#define TUPLE_H

#include <stddef.h>
#include <stdbool.h>

typedef enum
{
    TYPE_INT,
    TYPE_FLOAT,
    TYPE_BOOL,
    TYPE_CHAR,
    TYPE_STRING,
} ValueType;

typedef union
{
    int int_value;
    double float_value;
    bool bool_value;
    char char_value;
    char *string_value;
} ValueData;

typedef struct
{
    ValueType type;
    ValueData value;
} Value;

typedef struct
{
    size_t size;
    Value *items;
} Tuple;

Value value_int(int value);
Value value_float(double value);
Value value_bool(bool value);
Value value_char(char value);
Value value_string(char *value);
Tuple tuple(size_t size, Value *values);

#endif
