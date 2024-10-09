#ifndef ASM
#define ASM

#include "../stack/stack.h"

const size_t MAX_CODE = 10000;

typedef enum
{
    HLT,
    PUSH,
    SUB,
    DIV,
    PLS,
    MUL,
    SIN,
    COS,
    SQRT,
    EXP,
    DUMP,
    OUT
} operation_t ;

struct code_t
{
    size_t size = MAX_CODE;
    size_t pointer = 0;
    stack_unit_t *data = (stack_unit_t *) calloc(size, sizeof(stack_unit_t));
};

/*
Просто ассемблер, ничего необычного
*/
code_t *assembler(const char *input_file);

/*
Просто полученный машинный код перетыкивается в файл
*/
void code_to_stack(code_t *code, const char *output_file);

#endif
