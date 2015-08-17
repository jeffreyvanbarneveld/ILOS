#ifndef __OPS_H
#define __OPS_H

/* IL codes */
#define IL_CEQ -511
#define IL_CGT -510
#define IL_CLT -508
#define IL_LDARG -503
#define IL_LDARG_0 2
#define IL_LDARG_1 3
#define IL_LDARG_2 4
#define IL_LDARG_3 5
#define IL_LDLOC_0 6
#define IL_LDLOC_1 7
#define IL_LDLOC_2 8
#define IL_LDLOC_3 9
#define IL_STLOC_0 10
#define IL_STLOC_1 11
#define IL_STLOC_2 12
#define IL_STLOC_3 13
#define IL_LDARG_S 14
#define IL_LDLOC_S 17
#define IL_STLOC_S 19
#define IL_LDC_I4_m1 21
#define IL_LDC_I4_0 22
#define IL_LDC_I4_1 23
#define IL_LDC_I4_2 24
#define IL_LDC_I4_3 25
#define IL_LDC_I4_4 26
#define IL_LDC_I4_5 27
#define IL_LDC_I4_6 28
#define IL_LDC_I4_7 29
#define IL_LDC_I4_8 30
#define IL_LDC_I4_S 31
#define IL_LDC_I4 32
#define IL_LDC_I8 33
#define IL_LDC_R4 34
#define IL_LDC_R8 35
#define IL_CALL 40
#define IL_RET 42
#define IL_BR_S 43
#define IL_BRFALSE_S 44
#define IL_BRTRUE_S 45
#define IL_BEQ_S 46
#define IL_BGE_S 47
#define IL_BGT_S 48
#define IL_BLE_S 49
#define IL_BLT_S 50
#define IL_BNE_UN_S 51
#define IL_BGE_UN_S 52
#define IL_BGT_UN_S 53
#define IL_BLE_UN_S 54
#define IL_BLT_UN_S 55
#define IL_BR 56
#define IL_BEQ 59
#define IL_BGE 60
#define IL_BGT 61
#define IL_BLE 62
#define IL_BLT 63
#define IL_BNE_UN 64
#define IL_BGE_UN 65
#define IL_BGT_UN 66
#define IL_BLE_UN 67
#define IL_BLT_UN 68
#define IL_STIND_REF 81
#define IL_STIND_I1 82
#define IL_STIND_I2 83
#define IL_STIND_I4 84
#define IL_STIND_I8 85
#define IL_STIND_R4 86
#define IL_STIND_R8 87
#define IL_ADD 88
#define IL_SUB 89
#define IL_MUL 90
#define IL_DIV 91
#define IL_REM 93
#define IL_AND 95
#define IL_OR 96
#define IL_XOR 97
#define IL_SHL 98
#define IL_SHR 99
#define IL_NEG 101
#define IL_NOT 102
#define IL_CONV_I1 103
#define IL_CONV_I2 104
#define IL_CONV_I4 105
#define IL_CONV_I8 106
#define IL_CONV_R4 107
#define IL_CONV_R8 108
#define IL_CONV_U4 109
#define IL_CONV_U8 110
#define IL_CALLVIRT 111
#define IL_LDSTR 114
#define IL_NEWOBJ 115
#define IL_CONV_R_UN 118
#define IL_LDFLD 123
#define IL_STFLD 125
#define IL_LDSFLD 126
#define IL_STSFLD 128
#define IL_CONV_U2 209
#define IL_CONV_U1 210
#define IL_CONV_I 211
#define IL_STIND_I 223
#define IL_CONV_U 224

/* Internal opcodes */
#define OP_RUNTIME_CALL -1

#endif