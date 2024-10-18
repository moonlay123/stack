#include "executor/execute.h"

int main(int argc, char **argv)
{
    argc--; argv++;
    spu code = {};
    int correctness = assembler(argv[0], &code);
    //printf("1");
    //spu_dump(code);
    code_to_file(&code, "output.asm");
    //for(int i = 0; i < code.pointer; i++)
    //{
    //    printf("%d ", code.data[i]);
    //}
    //printf("1");
    execute(&code);
}
