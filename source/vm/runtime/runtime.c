#include <runtime.h>

/* System.String */
void System_String__ctor2();
void System_String_FastAllocateString1();
void System_String_FillStringChecked3();
void System_String_get_Length0();
void System_String_get_Chars1();
/* System.Globalization */
void System_Globalization_NumberFormatInfo_get_CurrentInfo0();
/* System.Number */
void System_Number_FormatInt323();
/* System.Object */
void System_Object_ToString0();

/**
 *
 * Do nothing
 *
**/
void passThrough()
{
}

/* Runtime functions */
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