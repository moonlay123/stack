#include "asm.h"

/*int main(int argc, char **argv)
{
    argc--; argv++;
    spu *code = assembler(argv[0]);
    code_to_stack(code, argv[1]);
}*/

void spu_dump(spu *code)
{
    if (code == NULL)
    {
        printf("No code");
        return;
    }
    printf("Max size of code: %lld\n", code->size);
    printf("Current pointer: %lld\n", code->pointer);
    printf("Data pointer: %p\n", code->data);
    printf("Register pointer: %p\n", code->regs);
    printf("Stack pointer: %p\n", code->stack);
}

void code_to_file(spu *code, const char *output_file)
{
    FILE *fp_w = NULL;
    int correctness = file_open(&fp_w, output_file, "w");
    //printf("%p %d", code->data, code->pointer);
    if (!correctness)
    {
       // printf("%p %d", code->data, code->pointer);
        for(size_t i = 0; i < code->pointer; i++)
        {
            //printf("%d ", code->data[i]);
            fprintf(fp_w, "%d ", code->data[i]);
        }
    }
    file_close(&fp_w);
}

void bufer_clean(FILE *fp_r)
{
    int file_c = ' ';
    while((file_c = getc(fp_r)) != '\n') {};
    getc(fp_r);
}

int label_check(spu *code, const char *label)
{
    for(size_t i = 0; i < code->labels_size; i++)
    {
        if (!strcmp(code->labels[i].label, label))
        {
            return (int)code->labels[i].position;
        }
    }
    return 0;
}

void label_pass(spu *code, FILE *fp_r)
{
    if (code == NULL or fp_r == NULL)
    {
        printf("DEBIL?");
        return;
    }
    bool run = true;
    size_t now_pos = 0;
    char *colon = NULL;
    char command[MAX_STRING_SIZE] = "";
    while (run)
    {
        fscanf(fp_r, "%s", command);
        if (feof(fp_r))
        {
            run = false;
            continue;
        }
        if (( colon = strchr(command, ':')) != NULL)
        {
            *colon = '\0';
            strcpy(code->labels[code->labels_size].label, command);
            printf("%s %lld\n", code->labels[code->labels_size].label, now_pos);
            code->labels[code->labels_size++].position = now_pos;

            continue;
        } else
        {
            ++now_pos;
            continue;
        }
        if (!strcmp(command, "push") or !strcmp(command, "pop") or
            !strcmp(command, "ja") or !strcmp(command, "jb") or !strcmp(command, "jae")
            or !strcmp(command, "jbe") or !strcmp(command, "je") or !strcmp(command, "jne"))
        {
            ++now_pos;
            continue;
        }
    }
    fseek(fp_r, 0, SEEK_SET);
}

int assembler(const char *input_file, spu *code)
{
    code->data = (stack_unit_t *) calloc(code->size, sizeof(stack_unit_t));
    FILE *fp_r = NULL;
    int correctness = file_open(&fp_r, input_file, "r");
    if (!correctness)
    {
        bool run = true;
        char command[MAX_STRING_SIZE] = "";
        char arg[MAX_STRING_SIZE] = "";
        char goto_j[MAX_STRING_SIZE] = "";
        label_pass(code, fp_r);
        while(run)
        {
            //printf("%llu ", code->pointer);
            fscanf(fp_r, "%s", command);
            //printf("%s ", command);
            //printf("%s\n", command);

            if (feof(fp_r))
            {
                run = false;
                continue;
            }
            if (strchr(command, ':') != NULL)
            {
                continue;
            }
            if (!strcmp(command, "push"))
            {
                fscanf(fp_r, "%s", arg);
                int str_int = atoi(arg);
                //printf("%s \n", arg);
                if (str_int != 0 or !strcmp(arg, "0"))
                {
                    code->data[code->pointer++] = PUSH;
                    code->data[code->pointer++] = str_int;
                } else
                {
                    code->data[code->pointer++] = PUSH_REG;
                    code->data[code->pointer++] = to_registr(arg);
                }
                //printf("%llu", code->pointer);
                continue;
            }
            if (!strcmp(command, "pop"))
            {
                fscanf(fp_r, "%s", arg);
                code->data[code->pointer++] = POP;
                code->data[code->pointer++] = to_registr(arg);
                //printf("%s \n", arg);
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
            if (!strcmp(command, "add"))
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
                code->data[code->pointer++] = COS;
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
            if (!strcmp(command, "dump"))
            {
                code->data[code->pointer++] = DUMP;
                continue;
            }
            if (!strcmp(command, "ja"))
            {
                fscanf(fp_r, "%s", goto_j);
                code->data[code->pointer++] = JA;
                code->data[code->pointer++] = label_check(code, goto_j);
                continue;
            }
            if (!strcmp(command, "jb"))
            {
                fscanf(fp_r, "%s", goto_j);
                code->data[code->pointer++] = JB;
                code->data[code->pointer++] = label_check(code, goto_j);
                //printf("%d \n", goto_j);
                continue;
            }
            if (!strcmp(command, "jae"))
            {
                fscanf(fp_r, "%s", goto_j);
                code->data[code->pointer++] = JAE;
                code->data[code->pointer++] = label_check(code, goto_j);
                continue;
            }
            if (!strcmp(command, "jbe"))
            {
                fscanf(fp_r, "%s", goto_j);
                code->data[code->pointer++] = JBE;
                code->data[code->pointer++] = label_check(code, goto_j);
                continue;
            }
            if (!strcmp(command, "je"))
            {
                fscanf(fp_r, "%s", goto_j);
                code->data[code->pointer++] = JE;
                code->data[code->pointer++] = label_check(code, goto_j);
                continue;
            }
            if (!strcmp(command, "jne"))
            {
                fscanf(fp_r, "%s", goto_j);
                code->data[code->pointer++] = JNE;
                code->data[code->pointer++] = label_check(code, goto_j);
                continue;
            }
            if (!strcmp(command, "hlt"))
            {
                code->data[code->pointer++] = HLT;
                continue;
            }
            printf("Nu ti i dolbaeb");
        }
    }
    file_close(&fp_r);
    return 0;
}

register_t to_registr(const char *reg)
{
    if (reg)
    {
        if (!strcmp(reg, "ax"))
        {
            return ax;
        }
        if (!strcmp(reg, "bx"))
        {
            return bx;
        }
        if (!strcmp(reg, "cx"))
        {
            return cx;
        }
        if (!strcmp(reg, "dx"))
        {
            return dx;
        }
    }
    return ax;
}
