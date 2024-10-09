#include "stack/stack.h"

int main()
{
    srand((unsigned int) time(NULL));
    stack_t a = {};
    init(&a, 15);
    push(&a, 3);
    push(&a, 5);
    printf("%lf", pop(&a));
}
