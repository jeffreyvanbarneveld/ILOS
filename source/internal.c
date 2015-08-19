#include <stack.h>
#include <runtime.h>
#include <stdio.h>
#include <internal.h>
#include <string.h>

/* TAKEN FROM IL2VM */

void System_Console_WriteLine1()
{
    printf("%s\n", stack_pop_str());
}

void System_String_Concat2()
{
    /* Unions to store data into */
    data first, second;
    _st(&second);
    _st(&first);

    /* Allocate string */
    size_t str_size = _get_size_from_data(&first) + _get_size_from_data(&second);
    char *str = malloc(sizeof(char) * str_size);
    char *str_original = str;

    /* Create string */
    str = _append_data(str, &first);
    _append_data(str, &second);
    stack_push_str(str_original);
}

void System_String_Concat3()
{
    /* Unions to store data into */
    data first, second, third;
    _st(&third);
    _st(&second);
    _st(&first);

    /* Allocate string */
    size_t str_size = _get_size_from_data(&first) + _get_size_from_data(&second) + _get_size_from_data(&third);
    char *str = malloc(sizeof(char) * str_size);
    char *str_original = str;

    /* Create string */
    str = _append_data(str, &first);
    str = _append_data(str, &second);
    _append_data(str, &third);
    stack_push_str(str_original);
}

void System_String_get_Length0()
{
    char *str = stack_pop_str();
    stack_push_int32(strlen(str));
}

void System_String_get_Chars1()
{
    int32_t index = stack_pop_int32();
    char *str = stack_pop_str();
    stack_push_int32(str[index]);
}

/* Runtime methods */
runtime_func runtimeMethods[] =
{
    NULL,
    NULL,
    System_Console_WriteLine1,
    System_String_Concat2,
    System_String_Concat3,
    System_String_get_Length0,
    System_String_get_Chars1
};