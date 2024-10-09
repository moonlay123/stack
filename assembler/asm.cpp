#include "asm.h"

int main(int argc, char **argv)
{
    argc--; argv++;
    code_to_stack(assembler(argv[0]), argv[1]);
}

void code_to_stack(code_t *code, const char *output_file)
{
    FILE *fp = NULL;
    int correctness = file_open(&fp, output_file, "r+");
    if (!correctness)
    {
        for(size_t i = 0; i < code->pointer - 1; i++)
        {
            fprintf(fp, "%lf ", code->data[i]);
        }
    }
    file_close(&fp);
}

code_t *assembler(const char *input_file)
{
    code_t *code = (code_t *) calloc(1, sizeof(code_t));
    code->data = (stack_unit_t *) calloc(code->size, sizeof(stack_unit_t));
    FILE *fp = NULL;
    int correctness = file_open(&fp, input_file, "r");
    if (!correctness)
    {
        bool run = true;
        char command[MAX_STRING_SIZE] = "";
        stack_unit_t to_push = 0;
        while(run)
        {
            //printf("%llu ", code->pointer);
            fscanf(fp, "%s", command);
            if (!strcmp(command, "push"))
            {
                fscanf(fp, "%lf", &to_push);
                code->data[code->pointer++] = PUSH;
                code->data[code->pointer++] = to_push;
                //printf("%llu", code->pointer);
                continue;
            }
            if (!strcmp(command, "sub"))
            {
                code->data[code->pointer++] = SUB;
                continue;
            }
            if (!strcmp(command, "div"))
            {
                code->data[code->pointer++] = DIV;
                continue;
            }
            if (!strcmp(command, "pls"))
            {
                code->data[code->pointer++] = PLS;
                continue;
            }
            if (!strcmp(command, "mul"))
            {
                code->data[code->pointer++] = MUL;
                continue;
            }
            if (!strcmp(command, "sin"))
            {
                code->data[code->pointer++] = SIN;
                continue;
            }
            if (!strcmp(command, "cos"))
            {
                code->data[code->pointer++] =COS;
                continue;
            }
            if (!strcmp(command, "sqrt"))
            {
                code->data[code->pointer++] = SQRT;
                continue;
            }
            if (!strcmp(command, "exp"))
            {
                code->data[code->pointer++] = EXP;
                continue;
            }
            if (!strcmp(command, "out"))
            {
                code->data[code->pointer++] = OUT;
                continue;
            }
            if (!strcmp(command, "hlt"))
            {
                code->data[code->pointer++] = HLT;
                run = false;
                continue;
            }
            if (!strcmp(command, "dump"))
            {
                code->data[code->pointer++] = DUMP;
                continue;
            }
        }
    }
    file_close(&fp);
    return code;
}
