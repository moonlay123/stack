#include "execute.h"

void execute(spu *code)
{
    if (code == NULL)
    {
        printf("SOSI");
        return;
    }
    size_t now_ip = 0;
    int extra_arg = 0;
    code->stack = (stack_t *) calloc(1, sizeof(stack_t));
    init(code->stack, 50);
    //printf("\n");
    while(now_ip < code->pointer)
    {
        //printf(" 2 ");
        //printf("data: %d pointer: %d max: %d \n", code->data[now_ip], now_ip, code->pointer);
        //printf("%d\n", code->data[now_ip]);
        //printf(" 3 ");
        switch(code->data[now_ip])
        {
            case (PUSH):
            {
                extra_arg = code->data[++now_ip];
                //printf("%d ", code->data[now_ip]);
                now_ip++;
                push(code->stack, extra_arg);
                break;
            }
            case (PUSH_REG):
            {
                extra_arg = code->data[++now_ip];
                //printf("%d ", code->data[now_ip]);
                now_ip++;
                push(code->stack, code->regs[extra_arg]);
                break;
            }
            case (POP):
            {
                extra_arg = code->data[++now_ip];
                //printf("%d ", code->data[now_ip]);
                now_ip++;
                code->regs[extra_arg] = pop(code->stack);
                break;
            }
            case (SUB):
            {
                stack_unit_t a = pop(code->stack);
                stack_unit_t b = pop(code->stack);
                now_ip++;
                push(code->stack, b - a);
                break;
            }
            case (DIV):
            {
                stack_unit_t a = pop(code->stack);
                stack_unit_t b = pop(code->stack);
                now_ip++;
                push(code->stack, b - a);
                break;
            }
            case (PLS):
            {
                stack_unit_t a = pop(code->stack);
                stack_unit_t b = pop(code->stack);
                now_ip++;
                push(code->stack, b + a);
                break;
            }
            case (MUL):
            {
                stack_unit_t a = pop(code->stack);
                stack_unit_t b = pop(code->stack);
                now_ip++;
                push(code->stack, b * a);
                break;
            }
            case (SIN):
            {
                stack_unit_t a = pop(code->stack);
                push(code->stack, (int)sin((double)a));
                now_ip++;
                break;
            }
            case (COS):
            {
                stack_unit_t a = pop(code->stack);
                push(code->stack, (int)cos((double)a));
                now_ip++;
                break;
            }
            case (SQRT):
            {
                stack_unit_t a = pop(code->stack);
                push(code->stack, (int)sqrt((double)a));
                now_ip++;
                break;
            }
            case (EXP):
            {
                stack_unit_t a = pop(code->stack);
                push(code->stack, (int)exp((double)a));
                now_ip++;
                break;
            }
            case (OUT):
            {
                printf("%d ", pop(code->stack));
                //printf("ax: %d\n", code->regs[1]);
                now_ip++;
                break;
            }
            case (HLT):
            {
                now_ip++;
                return;
            }
            case (DUMP):
            {
                now_ip++;
                break;
            }
            case (JA):
            {
                stack_unit_t a = pop(code->stack);
                stack_unit_t b = pop(code->stack);
                extra_arg = code->data[++now_ip];
                if (b > a)
                {
                    now_ip = extra_arg;
                } else
                {
                    now_ip++;
                }
                break;
            }
            case (JB):
            {
                stack_unit_t a = pop(code->stack);
                stack_unit_t b = pop(code->stack);
                extra_arg = code->data[++now_ip];
                if (b < a)
                {
                    now_ip = extra_arg;
                } else
                {
                    now_ip++;
                }
                break;
            }
            case (JAE):
            {
                stack_unit_t a = pop(code->stack);
                stack_unit_t b = pop(code->stack);
                extra_arg = code->data[++now_ip];
                if (b >= a)
                {
                    now_ip = extra_arg;
                } else
                {
                    now_ip++;
                }
                break;
            }
            case (JBE):
            {
                stack_unit_t a = pop(code->stack);
                stack_unit_t b = pop(code->stack);
                extra_arg = code->data[++now_ip];
                if (b <= a)
                {
                    now_ip = extra_arg;
                } else
                {
                    now_ip++;
                }
                break;
            }
            case (JE):
            {
                stack_unit_t a = pop(code->stack);
                stack_unit_t b = pop(code->stack);
                extra_arg = code->data[++now_ip];
                if (b == a)
                {
                    now_ip = extra_arg;
                } else
                {
                    now_ip++;
                }
                break;
            }
            case (JNE):
            {
                stack_unit_t a = pop(code->stack);
                stack_unit_t b = pop(code->stack);
                extra_arg = code->data[++now_ip];
                if (b != a)
                {
                    now_ip = extra_arg;
                }
                break;
            }
            default:
                //printf("Doesn't have this function %d\n", code->data[now_ip]);
                break;
        }
        //printf("%d ", now_ip);
    }
}
