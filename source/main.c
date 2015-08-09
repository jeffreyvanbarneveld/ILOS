#include <stdint.h>
#include <multiboot.h>
#include <stdint.h>
#include <string.h>
#include <ops.h>
#include <kheap.h>
#include <memory.h>

/* Types on stack */
#define TYPE_INT     0
#define TYPE_FLOAT   1
#define TYPE_DOUBLE  2
#define TYPE_STRING  3


extern void kprintf(const char *str, ...);


/* Stack */
uintptr_t stack = 0;                /* Address to stack */
size_t   *stack_sizes = (void *)0;       /* Stores the sizes of each element in the stack */
uint8_t  *stack_types = (void *)0;       /* Stores the types of each element in the stack */
int32_t   stack_offset = 0;         /* Offset from starting address */ 
int32_t   stack_element_offset = 0; /* Offset from starting element */

void perror(char *str)
{
	/*int colour = 0xFF;
    volatile char *video = (volatile char*)0xB8000;
    while( *str != 0 )
    {
        *video++ = *str++;
        *video++ = colour;
    }*/
        //kprintf("perror(%s)\n", str);
}

void _exit(int code)
{
	for(;;);
}


/* Type of data in local */
typedef union _data
{
    int32_t num_int;
    float   num_float;
    double  num_double;
    char   *str;
} data_t;

/* Item on local */
typedef struct
{
    data_t  data;
    uint8_t type;
} local_item;

/* The default size of the stack is 1MB */
const unsigned int STACK_SIZE = 1024 * 1024;

/* Function pointer type */
typedef void (*runtime_func)();



/* Push macro for stack */
#define STACK_PUSH(name, type, T) \
void name(T val) \
{ \
    /* Check if there's still place in the stack */ \
    const size_t size = sizeof(T); \
    if(stack_offset + size >= STACK_SIZE) \
    { \
        /** TODO: throw exception or something like that */ \
        perror("STACK OVERFLOW"); \
    } \
 \
    /* Store in stack */ \
    *(T *) (stack + stack_offset) = val; \
    stack_offset += size; \
    stack_sizes[stack_element_offset] = size; \
    stack_types[stack_element_offset] = type; \
    stack_element_offset++; \
}

/* Pop macro for stack */
#define STACK_POP(name, T) \
T name() \
{ \
    /* Check if there's something to pop */ \
    size_t size = stack_sizes[--stack_element_offset]; \
    if(stack_offset < size) \
    { \
        /* TODO: throw exception */ \
        perror("STACK UNDERFLOW"); \
    } \
 \
    /* Get and remove from stack */ \
    stack_offset -= size; \
    return *(T *) (stack + stack_offset); \
}

/* Stack methods */
STACK_PUSH(stack_push_int, TYPE_INT, int32_t);
STACK_PUSH(stack_push_float, TYPE_FLOAT, float);
STACK_PUSH(stack_push_double, TYPE_DOUBLE, double);
STACK_PUSH(stack_push_str, TYPE_STRING, char*);
STACK_POP(stack_pop_int, int32_t);
STACK_POP(stack_pop_float, float);
STACK_POP(stack_pop_double, double);
STACK_POP(stack_pop_str, char*);

/* Get the type of the top element on the stack */
uint8_t stack_peek_type()
{
    return stack_types[stack_element_offset - 1];
}





/* Internal methods */
void System_Console_WriteLine1()
{
    char *str = stack_pop_str();
    kprintf("%s\n",str);
}
void System_String_Concat2()
{
    /* TODO: un-ungly this */
    /* TODO: free string */

    /* Unions to store data into */
    data_t first, second;

    /* INT is 11 bytes (10 bytes for number and 1 for sign) are needed */
    /* FLOAT / DOUBLE is 18 bytes */
    /* STRING size is calculated using strlen */
    size_t str_size = 1;

    /* Get types and store data in unions */
    uint8_t type_first = stack_peek_type();
    if(type_first == TYPE_INT)
    {
        first.num_int = stack_pop_int();
        str_size += 11;
    }
    else if(type_first == TYPE_FLOAT)
    {
        first.num_float = stack_pop_float();
        str_size += 18;
    }
    else if(type_first == TYPE_DOUBLE)
    {
        first.num_double = stack_pop_double();
        str_size += 18;
    }
    else if(type_first == TYPE_STRING)
    {
        first.str = stack_pop_str();
        str_size += strlen(first.str);
    }

    /* Get types and store data in unions */
    uint8_t type_second = stack_peek_type();
    if(type_second == TYPE_INT)
    {
        second.num_int = stack_pop_int();
        str_size += 11;
    }
    else if(type_second == TYPE_FLOAT)
    {
        second.num_float = stack_pop_float();
        str_size += 18;
    }
    else if(type_second == TYPE_DOUBLE)
    {
        second.num_double = stack_pop_double();
        str_size += 18;
    }
    else if(type_second == TYPE_STRING)
    {
        second.str = stack_pop_str();
        str_size += strlen(second.str);
    }

    /* Allocate string */
    char *str = malloc(sizeof(char) * str_size);
    char *str_o = str;


    if(type_second == TYPE_INT)              str += sprintf(str, "%d", second.num_int);
    else if(type_second == TYPE_FLOAT)       str += sprintf(str, "%g", second.num_float);
    else if(type_second == TYPE_DOUBLE)      str += sprintf(str, "%g", second.num_double);
    else if(type_second == TYPE_STRING)      str += sprintf(str, "%s", second.str);

    if(type_first == TYPE_INT)               str += sprintf(str, "%d", first.num_int);
    else if(type_first == TYPE_FLOAT)        str += sprintf(str, "%g", first.num_float);
    else if(type_first == TYPE_DOUBLE)       str += sprintf(str, "%g", first.num_double);
    else if(type_first == TYPE_STRING)       str += sprintf(str, "%s", first.str);


    


    stack_push_str(str_o);
}


#include <include.h>



/* Initialize the stack */
void stack_init()
{
    /* Allocate stack for storing stack elements */
    stack = (uintptr_t) malloc(STACK_SIZE);
    if(!stack)
    {
        perror("Could not reserve memory needed for VM (stack).");
    }

    /* We also need an array defining the sizes of each element */
    /* in the stack */

    stack_sizes = (size_t *)malloc(sizeof(size_t) * STACK_SIZE);
    if(!stack_sizes)
    {
        free((void *) stack);
        perror("Could not reserve memory needed for VM (stack_sizes).");
    }

    /** We also need an array defining the types of each element */
    /* in the stack */
    stack_types = (uint8_t *)malloc(sizeof(uint8_t) * STACK_SIZE);
    if(!stack_types)
    {
        free((void *) stack);
        free(stack_sizes);
        perror("Could not reserve memory needed for VM (stack_types).");
    }

    //kprintf("stack=%x\nstack_sizes=%x\nstack_types=%x\n", (uint32_t)stack, (uint32_t)stack_sizes, (uint32_t)stack_types);
}

/* Free the stack memory */
void stack_destroy()
{
}

/* STLOC */
void _stloc(local_item* local)
{
    /* Get and store type */
    uint8_t type = stack_peek_type();
    local->type = type;

    //kprintf("_stloc: %d\n", type);

    /* Find a method to store it */
    if(type == TYPE_INT)                    local->data.num_int = stack_pop_int();
    else if(type == TYPE_FLOAT)             local->data.num_float = stack_pop_float();
    else if(type == TYPE_DOUBLE)            local->data.num_double = stack_pop_double();
    else if(type == TYPE_STRING)            local->data.str = stack_pop_str();
    else                                    perror("ERROR: _stloc");
}

/* LDLOC */
void _ldloc(local_item* local)
{
    /* Find a method to store it */
    if(local->type == TYPE_INT)             stack_push_int(local->data.num_int);
    else if(local->type == TYPE_FLOAT)      stack_push_float(local->data.num_float);
    else if(local->type == TYPE_DOUBLE)     stack_push_double(local->data.num_double);
    else if(local->type == TYPE_STRING)     stack_push_str(local->data.str);
    else                                    perror("ERROR: _ldloc");
}

/* Executes the IL code */
void execIL(int32_t method)
{
    /* Get method data */
    int* instructions = methods[method];
    char** dataStrs = strings[method];
    int* dataInts = ints[method];
    float* dataFloats = floats[method];
    double* dataDoubles = doubles[method];
    runtime_func* dataMethodRefs = methodRefs[method];
    size_t instructionsCount = methodsSize[method];

    /* Locals */
    local_item locals[localsCount[method]];
    memset(&locals, 0, sizeof(locals));


    /* Loop through all instructions */
    for(int32_t i = 0; i < instructionsCount; i++)
    {
        /* Check instruction */
        switch(instructions[i])
        {
            case OP_RUNTIME_CALL:
            {
                /* The method reference ID is the next entry in the array */
                (*dataMethodRefs[instructions[++i]])();
                break;
            }

            case IL_LDSTR:
            {
                /* The string ID is the next entry in the array, push it on the stack */
                stack_push_str(dataStrs[instructions[++i]]);
                break;
            }

            case IL_CALL:
            {
                /* The method ID is the next entry in the array */
                execIL(instructions[++i]);
                break;
            }

            case IL_STLOC_0:
            {
                _stloc(&locals[0]);
                break;
            }

            case IL_STLOC_1:
            {
                _stloc(&locals[1]);
                break;
            }

            case IL_STLOC_2:
            {
                _stloc(&locals[2]);
                break;
            }

            case IL_STLOC_3:
            {
                _stloc(&locals[3]);
                break;
            }

            case IL_STLOC_S:
            {
                /* The local ID is the next entry in the array */
                _stloc(&locals[instructions[++i]]);
                break;
            }

            case IL_LDLOC_0:
            {
                _ldloc(&locals[0]);
                break;
            }

            case IL_LDLOC_1:
            {
                _ldloc(&locals[1]);
                break;
            }

            case IL_LDLOC_2:
            {
                _ldloc(&locals[2]);
                break;
            }

            case IL_LDLOC_3:
            {
                _ldloc(&locals[3]);
                break;
            }

            case IL_LDLOC_S:
            {
                /* The local ID is the next entry in the array */
                _ldloc(&locals[instructions[++i]]);
                break;
            }

            case IL_LDARG_0:
            {
                stack_push_int(0);
                break;
            }

            case IL_LDC_R4:
            {
                stack_push_float(dataFloats[instructions[++i]]);
                break;
            }

            case IL_LDC_R8:
            {
                stack_push_double(dataDoubles[instructions[++i]]);
                break;
            }

            case IL_LDC_I4:
            case IL_LDC_I4_S:
            {
                stack_push_int(dataInts[instructions[++i]]);
                break;
            }

            case IL_LDC_I4_m1:
            {
                stack_push_int(-1);
                break;
            }

            case IL_LDC_I4_0:
            {
                stack_push_int(0);
                break;
            }

            case IL_LDC_I4_1:
            {
                stack_push_int(1);
                break;
            }

            case IL_LDC_I4_2:
            {
                stack_push_int(2);
                break;
            }

            case IL_LDC_I4_3:
            {
                stack_push_int(3);
                break;
            }

            case IL_LDC_I4_4:
            {
                stack_push_int(4);
                break;
            }

            case IL_LDC_I4_5:
            {
                stack_push_int(5);
                break;
            }

            case IL_LDC_I4_6:
            {
                stack_push_int(6);
                break;
            }

            case IL_LDC_I4_7:
            {
                stack_push_int(7);
                break;
            }

            case IL_LDC_I4_8:
            {
                stack_push_int(8);
                break;
            }

            case IL_ADD:
            {
                uint8_t type = stack_peek_type();
                if(type == TYPE_INT)
                {
                    int32_t a = stack_pop_int();
                    int32_t b = stack_pop_int();
                    stack_push_int(b + a);
                }
                else if(type == TYPE_FLOAT)
                {
                    float a = stack_pop_float();
                    float b = stack_pop_float();
                    stack_push_float(b + a);
                }
                else if(type == TYPE_DOUBLE)
                {
                    double a = stack_pop_double();
                    double b = stack_pop_double();
                    stack_push_float(b + a);
                }
                break;
            }

            case IL_AND:
        	{
                int32_t a = stack_pop_int();
                int32_t b = stack_pop_int();
                stack_push_int(b & a);
            	break;
            }

            case IL_XOR:
            {
                int32_t a = stack_pop_int();
                int32_t b = stack_pop_int();
                stack_push_int(b ^ a);
            	break;
            }

            case IL_SUB:
            {
                uint8_t type = stack_peek_type();
                if(type == TYPE_INT)
                {
                    int32_t a = stack_pop_int();
                    int32_t b = stack_pop_int();
                    stack_push_int(b - a);
                }
                else if(type == TYPE_FLOAT)
                {
                    float a = stack_pop_float();
                    float b = stack_pop_float();
                    stack_push_float(b - a);
                }
                else if(type == TYPE_DOUBLE)
                {
                    double a = stack_pop_double();
                    double b = stack_pop_double();
                    stack_push_float(b - a);
                }
                break;
            }

            case IL_MUL:
            {
                uint8_t type = stack_peek_type();
                if(type == TYPE_INT)
                {
                    int32_t a = stack_pop_int();
                    int32_t b = stack_pop_int();
                    stack_push_int(b * a);
                }
                else if(type == TYPE_FLOAT)
                {
                    float a = stack_pop_float();
                    float b = stack_pop_float();
                    stack_push_float(b * a);
                }
                else if(type == TYPE_DOUBLE)
                {
                    double a = stack_pop_double();
                    double b = stack_pop_double();
                    stack_push_float(b * a);
                }
                break;
            }

            case IL_DIV:
            {
                /* TODO: divide by zero exception */
                uint8_t type = stack_peek_type();
                if(type == TYPE_INT)
                {
                    int32_t a = stack_pop_int();
                    int32_t b = stack_pop_int();
                    stack_push_int(b / a);
                }
                else if(type == TYPE_FLOAT)
                {
                    float a = stack_pop_float();
                    float b = stack_pop_float();
                    stack_push_float(b / a);
                }
                else if(type == TYPE_DOUBLE)
                {
                    double a = stack_pop_double();
                    double b = stack_pop_double();
                    stack_push_float(b / a);
                }

                break;
            }

            case IL_STIND_I1:
            {
                int8_t val = (int8_t) stack_pop_int();
                int8_t *addr = (int8_t *) ((intptr_t) stack_pop_int());
                *addr = val;
                break;
            }

            case IL_STIND_I2:
            {

                int16_t val = (int16_t) stack_pop_int();
                int16_t *addr = (int16_t *) ((intptr_t) stack_pop_int());
                *addr = val;
                break;
            }

            case IL_STIND_I:
            case IL_STIND_I4:
            {
                int32_t val = stack_pop_int();
                int32_t *addr = (int32_t *) ((intptr_t) stack_pop_int());
                *addr = val;
                break;
            }

            case IL_STIND_I8:
            {
                int64_t val = stack_pop_int();
                int64_t *addr = (int64_t *) ((intptr_t) stack_pop_int());
                *addr = val;
                break;
            }

            case IL_STIND_R4:
            {
                float val = stack_pop_float();
                float *addr = (float *) ((intptr_t) stack_pop_int());
                *addr = val;
                break;
            }

            case IL_STIND_R8:
            {
                double val = stack_pop_double();
                double *addr = (double *) ((intptr_t) stack_pop_int());
                *addr = val;
                break;
            }

            case IL_CEQ:
            {
                if(stack_pop_int() == stack_pop_int())
                    stack_push_int(1);
                else
                    stack_push_int(0);

                break;
            }

            case IL_CLT:
            {
                int32_t a = stack_pop_int();
                int32_t b = stack_pop_int();
                if(b < a)
                    stack_push_int(1);
                else
                    stack_push_int(0);

                break;
            }

            case IL_CGT:
            {
                int32_t a = stack_pop_int();
                int32_t b = stack_pop_int();
                if(b > a)
                    stack_push_int(1);
                else
                    stack_push_int(0);

                break;
            }

            case IL_BRFALSE_S:
            {
                // Substract one because we increase it at the end of the loop
                int32_t next = instructions[++i] - 1;
                if(stack_pop_int() == 0)
                    i = next;

                break;
            }

            case IL_BRTRUE_S:
            {
                // Substract one because we increase it at the end of the loop
                int32_t next = instructions[++i] - 1;
                if(stack_pop_int() == 1)
                    i = next;

                break;
            }

            case IL_BEQ_S:
            case IL_BEQ:
            {
                // Substract one because we increase it at the end of the loop
                int32_t next = instructions[++i] - 1;
                if(stack_pop_int() == stack_pop_int())
                    i = next;

                break;
            }

            case IL_BGE_UN:
            case IL_BGE_UN_S:
            case IL_BGE_S:
            case IL_BGE:
            {
                // Substract one because we increase it at the end of the loop
                int32_t a = stack_pop_int();
                int32_t b = stack_pop_int();
                int32_t next = instructions[++i] - 1;
                if(b >= a)
                    i = next;

                break;
            }

            case IL_BGT_UN:
            case IL_BGT_UN_S:
            case IL_BGT_S:
            case IL_BGT:
            {
                // Substract one because we increase it at the end of the loop
                int32_t a = stack_pop_int();
                int32_t b = stack_pop_int();
                int32_t next = instructions[++i] - 1;
                if(b > a)
                    i = next;

                break;
            }

            case IL_BLE_UN:
            case IL_BLE_UN_S:
            case IL_BLE_S:
            case IL_BLE:
            {
                // Substract one because we increase it at the end of the loop
                int32_t a = stack_pop_int();
                int32_t b = stack_pop_int();
                int32_t next = instructions[++i] - 1;
                if(b <= a)
                    i = next;

                break;
            }

            case IL_BLT_UN:
            case IL_BLT_UN_S:
            case IL_BLT_S:
            case IL_BLT:
            {
                // Substract one because we increase it at the end of the loop
                int32_t a = stack_pop_int();
                int32_t b = stack_pop_int();
                int32_t next = instructions[++i] - 1;
                if(b < a)
                    i = next;

                break;
            }

            case IL_BNE_UN:
            case IL_BNE_UN_S:
            {
                // Substract one because we increase it at the end of the loop
                int32_t next = instructions[++i] - 1;
                if(stack_pop_int() != stack_pop_int())
                    i = next;

                break;
            }

            case IL_BR:
            case IL_BR_S:
            {
                // Substract one because we increase it at the end of the loop
                int kak = instructions[++i] - 1;
                i = kak;
                break;
            }

            case IL_RET:
            {
                return;
            }

            case IL_CONV_U1:
            {
                //stack.push(Math.abs(stack.pop()) & 255);
                break;
            }

            case IL_CONV_U2:
            {
                //stack.push(Math.abs(stack.pop()) & 65535);
                break;
            }

            case IL_CONV_U4:
            case IL_CONV_U:
            {
                //stack.push(Math.abs(stack.pop()) & 4294967293);
                break;
            }

            case IL_CONV_U8:
            {
                //stack.push(Math.abs(stack.pop()) | 0);
                break;
            }

            case IL_CONV_I1:
            {
                //stack.push(stack.pop() & 127);
                break;
            }

            case IL_CONV_I2:
            {
                //stack.push(stack.pop() & 32767);
                break;
            }

            case IL_CONV_I4:
            case IL_CONV_I:
            {
                //stack.push(stack.pop() & 2147483646);
                break;
            }

            case IL_CONV_I8:
            {
                //stack.push(stack.pop() | 0);
                break;
            }

            case IL_CONV_R4:
            case IL_CONV_R8:
            case IL_CONV_R_UN:
                break;

            default:
            {
                kprintf("Unknown instruction %d (0x%x)\n", instructions[i], instructions[i]);
                break;
            }
        }
    }
}



void kmain(multiboot_t *multiboot, uint32_t magic) {

	extern void *end;
    heap_install((uintptr_t) &end);

    /* Init */
    stack_init();
    load();

    /* Entrypoint is the first method (ID 0) */
    execIL(0);


    /* Cleanup */
    stack_destroy();

    for(;;);
}