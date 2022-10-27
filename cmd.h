#define PUSH(arg) StackPush(&stk, arg);
#define POP StackPop(&stk)
#define OPERATION(operation) int x = POP;\
                             int y = POP;\
                             PUSH (y operation x)


DEF_CMD(PUSH, 1, 1, {
    ip++;
    PUSH (GetArg (text.strings[ip].ptr))
})

DEF_CMD(SUB, 2, 0,{
    OPERATION(-)
})

DEF_CMD(DIV, 3, 0,{
    OPERATION(/)
})

DEF_CMD(ADD, 4, 0,{
    OPERATION(+)

})

DEF_CMD(MUL, 5, 0,{
    OPERATION(*)
})

DEF_CMD(POP, 6, 0,{
    POP;
})

DEF_CMD(OUT, 7, 0,{
    printf("%lg\n", POP);
})

DEF_CMD(HLT, 0, 0,{
    return 0;
})

DEF_CMD(DUP, 8, 0,{
    int x = POP;
    PUSH(x)
    PUSH(x)
})

DEF_CMD(JMP, 9, 1, {
    int x = 0;
})


#undef PUSH
#undef POP
#undef OPERATION