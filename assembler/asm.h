#ifndef ASM
#define ASM

#include "../stack/stack.h"

const size_t MAX_CODE = 10000;
const size_t MAX_REG = 32;
const size_t MAX_LABEL_SIZE = 32;
const size_t MAX_LABEL_COUNT = 32;

typedef enum
{
    HLT      = 0,
    PUSH     = 1,
    PUSH_REG = 2,
    POP      = 3,
    SUB      = 4,
    DIV      = 5,
    PLS      = 6,
    MUL      = 7,
    SIN      = 8,
    COS      = 9,
    SQRT     = 10,
    EXP      = 11,
    DUMP     = 12,
    OUT      = 13,
    JA       = 14,
    JB       = 15,
    JAE      = 16,
    JBE      = 17,
    JE       = 18,
    JNE      = 19
} operation_t;

typedef enum
{
    ax  = 1,
    bx  = 2,
    cx  = 3,
    dx  = 4,
    MLX = 0
} register_t;

struct label_t
{
    char label[MAX_LABEL_SIZE] = "";
    size_t position = 0;
};

struct spu
{
    size_t size = MAX_CODE;
    size_t pointer = 0;
    stack_unit_t *data = NULL;
    int regs[MAX_REG] = {};
    stack_t *stack = NULL;
    label_t labels[MAX_LABEL_COUNT] = {};
    size_t labels_size = 0;
};

/*
Очистить буфер
*/
void bufer_clean(FILE *fp_r);

/*
Проверяет label
*/
int label_check(spu *code, const char *label);

/*
Сверка всех лейблов
*/
void label_pass(spu *code, FILE *fp_r);

void spu_dump(spu *code);

/*
Просто ассемблер, ничего необычного
*/
int assembler(const char *input_file, spu *code);

/*
Просто полученный машинный код перетыкивается в файл
*/
void code_to_file(spu *code, const char *output_file);

/*
Текст в регистр
*/
register_t to_registr(const char *reg);

#endif
