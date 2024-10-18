#ifndef STACK
#define STACK

#include <stdio.h>
#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <string.h>
#include <math.h>

#define DUMP(stack) //dump(stack, NULL, __FILE__, __func__, __LINE__);

typedef enum
{
    NO_ERROR = 0,
    NULL_DATA = 1,
    OVERFLOWS = 1 << 1,
    NO_STACK = 1 << 2,
    DESTROYED = 1 << 3,
    EMPTY_CAPACITY = 1 << 4,
    EMPTY_SIZE = 1 << 5,
    HASH_ERROR = 1 << 6,
    CNR_ERROR = 1 << 7
} err_t;
typedef int stack_unit_t;

const size_t DEFAULT = 0xAAAAAAAAAAAAAAAA;
const size_t MOD = 1e9 + 7;
const size_t SIMPLE = 101;
const size_t MAX_STRING_SIZE = 50;

struct stack_t
{
    const size_t CNR_start = DEFAULT;
    size_t errors = NO_ERROR;
    size_t capacity = 1024;
    size_t size = 0;
    stack_unit_t *data = NULL;
    size_t hash = 0;
    const size_t CNR_end   = DEFAULT;
};

/*
Считает хэш стека
*/
size_t get_hash(stack_t *stack);

/*
Ставит hash
*/
void set_hash(stack_t *stack);

/*
Проверяет хэш и канарейку
*/
err_t verify_hash_cnr(stack_t *stack);

/*
Создает стек
*/
void init(stack_t *stack, size_t capacity);

/*
Увеличивает/уменьшает размер стека, если надо
*/
void stack_balancer(stack_t *stack);

/*
уничтожает стек
*/
void Dtor(stack_t *stack);

/*
Пушит объект в стек
*/
void push(stack_t *stack, stack_unit_t to_push);

/*
Удаляет элемент из верхушки стека
*/
stack_unit_t pop(stack_t *stack);

/*
Выводит всю инфу о стеке в указываемый файл
*/
size_t dump(stack_t *stack, FILE *fp, const char *file, const char *func, int line);

/*
Проверяет стек на возможные или уже возникнувшие ошибки
*/
size_t verify(stack_t *stack, const char *func);

/*
Генерирует текст в зависимости от ошибки
*/
void error_logger(size_t error, FILE *fp);

/*
открывает файл
*/
int file_open(FILE **fp, const char *file_name, const char *type);

/*
Закрывает файл
*/
int file_close(FILE **fp);

/*
Запускает работу над стеком из файла
*/
void run(char *file_name);

#endif
