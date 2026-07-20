#include "tuple.h"

Value value_int(int value)
{
    return (Value){.type = TYPE_INT, .value.int_value = value};
}

Value value_float(double value)
{
    return (Value){.type = TYPE_FLOAT, .value.float_value = value};
}

Value value_bool(bool value)
{
    return (Value){.type = TYPE_BOOL, .value.bool_value = value};
}

Value value_char(char value)
{
    return (Value){.type = TYPE_CHAR, .value.char_value = value};
}

Value value_string(char *value)
{
    return (Value){.type = TYPE_STRING, .value.string_value = value};
}

Tuple tuple(size_t size, Value *values)
{
    return (Tuple){.size = size, .items = values};
}
