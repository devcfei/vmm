#ifndef __STACK_H__
#define __STACK_H__



typedef struct stackObject_s
{
    Size_t* base;
    Size_t top;
    Size_t max;
}stackObject_t;

typedef stackObject_t* stackHandle;


static
inline
Result
stackInit(
    stackHandle hStack,
    Size_t* base,
    Size_t max
    )
{
    hStack->base = base;
    hStack->top = 0;
    hStack->max = max;
    return resOk;
}



static
inline
Result
stackPush(
    stackHandle hStack,
    Size_t item
    )
{
    if(hStack->top< hStack->max )
    {
        hStack->base[hStack->top] = item;
        hStack->top++;

        return resOk;
    }
    return resNoMore;
}

static
inline
Result
stackPop(
    stackHandle hStack,
    Size_t* item
    )
{

    if(hStack->top == 0 )
    {
        return resNoMore;
    }

    *item =  hStack->base[hStack->top-1];
    hStack->top--;

    return resOk;
}


#endif /*__STACK_H__*/

