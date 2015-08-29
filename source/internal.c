#include <stack.h>
#include <runtime.h>
#include <stdio.h>
#include <internal.h>
#include <string.h>

/* TAKEN FROM IL2VM */

void System_String__ctor2()
{
    /* new String(char character, int repetitions) */
    /* On stack: repetitions, character */
    int32_t repetitions = stack_pop_int32();
    char character = (char) stack_pop_int32();
    char* str = malloc(sizeof(char) * (1 + repetitions));

    /* Create string */
    int i = 0;
    for(; i < repetitions; i++)
        str[i] = character;

    /* String ends on a NULL and push string on stack */
    str[i] = '\0';
    stack_push_str(str);
}

void System_String_FastAllocateString1()
{
    /* String.FastAllocateString(int length) */
    int32_t length = stack_pop_int32();
    char* str = malloc(CHAR_SIZE * (1 + length));
    stack_push_str(str);
}

void System_String_FillStringChecked3()
{
    /* String.FillStringChecked(string dest, int destPos, string src) */
    char* src = stack_pop_str();
    int32_t destPos = stack_pop_int32();
    char* dest = stack_pop_str();
    if(src == NULL)
        return;

    strcpy(dest + destPos, src);
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

void passThrough()
{
    /* Incoming is a string that needs to be converted */
    /* Outgoing is a converted string */
    /* Just passing incoming -> outgoing */
}

void System_Globalization_NumberFormatInfo_get_CurrentInfo0()
{
    /* TODO */
    stack_push_obj(NULL);
}

void System_Number_FormatInt323()
{
    /* Number.FormatInt32(value, format, numberFormatInfo) */
    /* Format and numberformatinfo are currently ignored */
    void* numberFormatInfo = stack_pop_obj();
    char* format = stack_pop_str();
    int32_t value = stack_pop_int32();

    /* Disable warnings of unused variables */
    (void) format;
    (void) numberFormatInfo;

    /* Longest int is Int32.MinValue (-2147483648) */
    /* That's 11 chars long */
    char* str = malloc(CHAR_SMALL_SIZE * (1 + 11));
    sprintf(str, "%d", value);

    stack_push_str(str);
}

void System_Object_ToString0()
{
    /* Object.ToString() */
    /* On stack: object */
    uint8_t type = stack_peek_type();
    if(type == TYPE_STRING)
    {
        /* Keep as-is */
    }
    else if(type == TYPE_OBJ)
    {
        stack_pop_obj();
        stack_push_str("OBJECT");
    }
    else if(type == TYPE_INT32)
    {
        char* str = malloc(CHAR_SMALL_SIZE * (1 + 11));
        sprintf(str, "%d", stack_pop_int32());
        stack_push_str(str);
    }
    else if(type == TYPE_INT64)
    {
        char* str = malloc(CHAR_SMALL_SIZE * (1 + 20));
        sprintf(str, "%ld", stack_pop_int64());
        stack_push_str(str);
    }
    else if(type == TYPE_FLOAT)
    {
        char* str = malloc(CHAR_SMALL_SIZE * (1 + 18));
        sprintf(str, "%g", stack_pop_float());
        stack_push_str(str);
    }
    else if(type == TYPE_DOUBLE)
    {
        char* str = malloc(CHAR_SMALL_SIZE * (1 + 18));
        sprintf(str, "%g", stack_pop_double());
        stack_push_str(str);
    }
}

/* Runtime methods */
runtime_func runtimeMethods[] =
{
    System_String__ctor2,
    System_String_FastAllocateString1,
    System_String_FillStringChecked3,
    System_String_get_Length0,
    System_String_get_Chars1,
    passThrough /*System_Environment_GetResourceString1*/,
    System_Globalization_NumberFormatInfo_get_CurrentInfo0,
    System_Number_FormatInt323,
    passThrough /*System_Exception_GetMessageFromNativeResources1*/,
    System_Object_ToString0
};