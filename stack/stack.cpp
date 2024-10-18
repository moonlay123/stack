#include "stack.h"

void init(stack_t *stack, size_t capacity)
{
    if (capacity == 0)
    {
        stack->capacity = 0;
        stack->errors |= EMPTY_CAPACITY;
        stack->hash = get_hash(stack);
        return;
    } else
    {
        stack->capacity = capacity;
        stack->errors |= NO_ERROR;
        stack->size = 0;
        //printf("553532325dfhd");
        //printf("%d", capacity);
        stack->data = (stack_unit_t *) calloc(capacity, sizeof(stack_unit_t));
        //printf("553532325dfhd");
        if (stack->data == NULL)
        {
            stack->errors |= NULL_DATA;
        }
        stack->hash = get_hash(stack);
    }
    DUMP(stack);
}

size_t get_hash(stack_t *stack) // canary
{
    const char *start = (const char *) stack;

    size_t hash = 0;
    for (size_t now = sizeof(size_t) + sizeof(err_t); now * sizeof(char) < sizeof(stack_t) - 2 * sizeof(size_t); now += 1)
    {
        hash = (hash * SIMPLE + *(start + now)) % MOD;
    }

    DUMP(stack);
    return hash;
}

void stack_balancer(stack_t *stack)
{
    stack->hash = get_hash(stack);
    DUMP(stack);
    if (stack->errors == NO_ERROR)
    {
        if (stack->size >= stack->capacity)
        {
            stack->capacity *= 2;
        }
        if (stack->size * 4 <= stack->capacity and stack->size != 0)
        {
            stack->capacity /= 2;
        }
        stack_unit_t *temp = stack->data;
        stack->data = (stack_unit_t *) _recalloc(stack->data, stack->capacity, sizeof(stack_unit_t));
        if (stack->data == NULL and temp != NULL)
        {
            stack->errors |= NULL_DATA;
            free(temp);
        }
        stack->hash = get_hash(stack);
        DUMP(stack); // macros dump( stk);
    }
}

void Dtor(stack_t *stack)
{
    DUMP(stack);
    if (verify(stack, __func__) == NO_ERROR)
    {
        free(stack->data);
        stack->data = NULL;
        stack->errors = DESTROYED;
        stack->capacity = 0;
        stack->size = 0;
        stack->hash = 0;
    }
    DUMP(stack);
}

void push(stack_t *stack, stack_unit_t to_push)
{
    DUMP(stack);
    verify(stack, __func__);
    if (stack->errors & HASH_ERROR or stack->errors & CNR_ERROR or stack->errors & EMPTY_SIZE or stack->errors == NO_ERROR)
    {
        stack->errors = NO_ERROR;
        if ( stack->size >= stack->capacity )
            stack_balancer(stack);
        stack->data[stack->size++] = to_push;
    }
}

stack_unit_t pop(stack_t *stack)
{
    DUMP(stack);
    verify(stack, __func__);
    if (stack->errors & HASH_ERROR or stack->errors & CNR_ERROR or stack->errors == NO_ERROR)
    {
        if (stack->size == 0)
        {
            stack->errors = stack->errors | EMPTY_SIZE;
            return 0;
        } else
        {
            //printf("1 %d %d \n", stack->data[stack->size - 1], stack->size);
            return stack->data[--stack->size];
        }
    }
    return 0;
}

size_t dump(stack_t *stack, FILE *fp, const char *file, const char *func, int line)
{
    if (stack == NULL)
    {
        error_logger(NO_STACK, NULL);
        return NO_STACK;
    }
    if (stack->errors & DESTROYED)
    {
        error_logger(DESTROYED, NULL);
        return DESTROYED;
    }
    if (fp == NULL)
    {
        int correctness = file_open(&fp, "logger_2.log", "a+");
        if (correctness)
        {
            fp = stdin;
        } else
            fseek(fp, 0, SEEK_END);
    }
    verify(stack, func);
    fprintf(fp,
            "In file %s, func %s, line %d\n"
            "Your stack parameters:\n"
            "Capacity: %llu \n"
            "Size: %llu \n"
            "Data pointer: %p \n"
            "Hash: %llu \n"
            "Canaries: \n %llu \n %llu \n", file, func, line,
           stack->capacity, stack->size, stack->data, stack->hash, stack->CNR_start, stack->CNR_end);
    for(size_t i = 0; i < stack->size; i++)
    {
        fprintf(fp, "%d ", stack->data[i]);
    }
    fprintf(fp, "\n");
    error_logger(stack->errors, NULL);

    file_close(&fp);
    return stack->errors;
}

err_t verify_hash_cnr(stack_t *stack)
{
    size_t hash = get_hash(stack);
    if (stack->hash != hash)
    {
        if (stack->hash == 0)
            stack->hash = hash;
        else
            stack->errors = (err_t) (stack->errors | HASH_ERROR);
    }
    if (stack->CNR_start != 0xAAAAAAAAAAAAAAAA or stack->CNR_end != 0xAAAAAAAAAAAAAAAA)
    {
        stack->errors = (err_t) (stack->errors | CNR_ERROR);
    }
    return NO_ERROR;
}

size_t verify(stack_t *stack, const char *func)
{
    if (!stack)
    {
        return NO_STACK;
    }
    if (stack->errors != NO_ERROR)
    {
        return stack->errors;
    } else
    {
        if (stack->data == NULL)
        {
            stack->errors = stack->errors | NULL_DATA;
        } else if (stack->capacity == 0)
        {
            stack->errors = stack->errors | EMPTY_CAPACITY;
        } else if (stack->size > stack->capacity)
        {
            stack->errors = stack->errors | OVERFLOWS;
        }
        if (strcmp(func, "get_hash"))
        {
            verify_hash_cnr(stack);
        }
        return stack->errors;
    }
}

void error_logger(size_t error, FILE *fp)
{
    if (fp == NULL)
    {
        int correctness = file_open(&fp, "logger_2.log", "a+"); // check, cringeeeeee
        if (correctness)
        {
            fp = stdin;
        } else
            fseek(fp, 0, SEEK_END);
    }

    if (error == NO_ERROR)
        fprintf(fp, "No errors\n");
    if (error & NULL_DATA)
        fprintf(fp, "WARNING: Data pointer is NULL\n");
    if (error & OVERFLOWS)
        fprintf(fp, "ERROR: Your stack size is bigger, than capacity\n");
    if (error & NO_STACK)
        fprintf(fp, "ERROR: No stack\n");
    if (error & DESTROYED)
        fprintf(fp, "WARNING: Stack already destroyed\n");
    if (error & EMPTY_CAPACITY)
        fprintf(fp, "ERROR: Your stack have zero capacity\n");
    if (error & EMPTY_SIZE)
        fprintf(fp, "WARNING: Your stack if empty\n");
    if (error & HASH_ERROR)
        fprintf(fp, "WARNING: Hashes are not similar\n");
    if (error & CNR_ERROR)
        fprintf(fp, "WARNING: Canary was changed\n");
    file_close(&fp);
}

void run(char *file_name)
{
    stack_t stack = {};

    init(&stack, 100);

    FILE *fp = NULL;
    int correctness = file_open(&fp, file_name, "r");
    if (!correctness)
    {
        bool run = true;
        char command[MAX_STRING_SIZE] = "";
        stack_unit_t to_push = 0;
        while(run)
        {
            fscanf(fp, "%s", command);
            if (!strcmp(command, "push"))
            {
                fscanf(fp, "%d", &to_push);
                //printf("%d", to_push);
                push(&stack, to_push);
                to_push = 0;
                continue;
            }
            if (!strcmp(command, "sub"))
            {
                stack_unit_t a = pop(&stack);
                stack_unit_t b = pop(&stack);
                push(&stack, b - a);
                //printf("%d %d ", b, a);
                continue;
            }
            if (!strcmp(command, "div"))
            {
                stack_unit_t a = pop(&stack);
                stack_unit_t b = pop(&stack);
                push(&stack, b / a);
                continue;
            }
            if (!strcmp(command, "pls"))
            {
                stack_unit_t a = pop(&stack);
                stack_unit_t b = pop(&stack);
                //printf("%d %d ", a, b);
                push(&stack, b + a);
                continue;
            }
            if (!strcmp(command, "mul"))
            {
                stack_unit_t a = pop(&stack);
                stack_unit_t b = pop(&stack);
                push(&stack, b * a);
                continue;
            }
            if (!strcmp(command, "sin"))
            {
                stack_unit_t a = pop(&stack);
                push(&stack, (int)sin((double)a));
                continue;
            }
            if (!strcmp(command, "cos"))
            {
                stack_unit_t a = pop(&stack);
                push(&stack, (int)cos((double)a));
                continue;
            }
            if (!strcmp(command, "sqrt"))
            {
                stack_unit_t a = pop(&stack);
                push(&stack, (int)sqrt((double)a));
                continue;
            }
            if (!strcmp(command, "exp"))
            {
                stack_unit_t a = pop(&stack);
                push(&stack, (int)exp((double)a));
                continue;
            }
            if (!strcmp(command, "out"))
            {
                stack_unit_t a = pop(&stack);
                printf("%d", a);
            }
            if (!strcmp(command, "hlt"))
            {
                run = false;
                continue;
            }
            if (!strcmp(command, "dump"))
            {
                DUMP(&stack);
                continue;
            }
        }
    }
}

int file_open(FILE **fp, const char *file_name, const char *type)
{
    if (fp == NULL or file_name == NULL)
        return 1;
    if ((*fp = fopen(file_name, type)) == NULL)
    {
        perror("File open error");
    }
    return 0;
}

int file_close(FILE **fp)
{
    if (fp == NULL)
        return 1;
    if (fclose(*fp) != 0)
        perror("File close error");
    *fp = NULL;

    return 0;
}
