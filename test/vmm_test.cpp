#include <gtest/gtest.h>

extern "C"
{
#include <vmm.h>
#include <stdio.h>



typedef struct testObject_s
{
    Uint a;
    Uint b;
    Uint c;

} testObject_t;

typedef struct testListItem_s
{

    testObject_t obj;
    listNode ent;

} testListItem_t;


}


TEST(Vmm, Basic) 
{
    int r;
    r = vmm_init();
    EXPECT_EQ(0,r);
}


TEST(List, listIsEmpty)
{

    testListItem_t items[10];
    Uint arr1[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    Uint arr2[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    listNode list;

    listInit(&list);

    EXPECT_EQ(True, listIsEmpty(&list));

    listInsertTail(&list, &items[0].ent);

    EXPECT_EQ(False, listIsEmpty(&list));
}

TEST(List, listRemoveNode)
{

    testListItem_t items[10];
    Uint arr1[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    Uint arr2[9] = {0, 1, 2, 3, 4, 6, 7, 8, 9};

    listNode list;

    listInit(&list);

    for (int i = 0; i < sizeof(items) / sizeof(items[0]); ++i)
    {
        items[i].obj.a = arr1[i];
        listInsertTail(&list, &items[i].ent);
    }

    listNode *node = static_cast<listNode *>(NULL_PTR);
    LIST_FOR_EACH(&list, iter)
    {
        testListItem_t *item = TYPE_BASE(iter, testListItem_t, ent);
        if (item->obj.a == 5)
        {
            node = &item->ent;
            break;
        }
    }

    EXPECT_NE(NULL_PTR, node);

    listRemoveNode(node);

    int i = 0;
    LIST_FOR_EACH(&list, iter)
    {
        testListItem_t *item = TYPE_BASE(iter, testListItem_t, ent);
        EXPECT_EQ(arr2[i++], item->obj.a);
    }
}

TEST(List, listInsertTail)
{

    testListItem_t items[10];
    Uint arr1[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    Uint arr2[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

    listNode list;

    listInit(&list);

    for (int i = 0; i < sizeof(items) / sizeof(items[0]); ++i)
    {
        items[i].obj.a = arr1[i];
        listInsertTail(&list, &items[i].ent);
    }

    int i = 0;
    LIST_FOR_EACH(&list, iter)
    {
        testListItem_t *item = TYPE_BASE(iter, testListItem_t, ent);
        arr2[i++] = item->obj.a;
    }

    for (int i = 0; i < sizeof(items) / sizeof(items[0]); ++i)
    {
        EXPECT_EQ(arr1[i], arr2[i]);
    }
}


TEST(Stack, stackBasic)
{
    Result r;

    stackObject_t stack;
    stackHandle hStack = &stack;

    Size_t *stackData = new Size_t[5];

    memset(stackData, 0, sizeof(Size_t) * 5);

    Size_t data;

    stackInit(hStack, stackData, 5);

    stackPush(hStack, 0xa0);
    stackPush(hStack, 0xa1);
    stackPush(hStack, 0xa2);
    stackPush(hStack, 0xa3);
    stackPush(hStack, 0xa4);

    r = stackPush(hStack, 0xa5);
    EXPECT_EQ(r, resNoMore);

    stackPop(hStack, &data);
    EXPECT_EQ(data, 0xa4);

    stackPop(hStack, &data);
    EXPECT_EQ(data, 0xa3);

    stackPop(hStack, &data);
    EXPECT_EQ(data, 0xa2);

    stackPop(hStack, &data);
    EXPECT_EQ(data, 0xa1);

    stackPop(hStack, &data);
    EXPECT_EQ(data, 0xa0);

    r = stackPop(hStack, &data);

    EXPECT_EQ(r, resNoMore);
}


TEST(VmaTest, Allocate)
{
    Result r;
    vmaObject_t vma;
    vmaHandle hVma = &vma;

    /*
     *  allocated : na
     *  free: 0x00000-0xFFFFF
     */
    vmaInit(hVma,0x100000);
    Void* va;

    /*
     *  allocated : 0x00000 - 0x000FF
     *  free: 0x00100-0xFFFFF
     */
    r = vmaAlloc(hVma,0x100,0x100,&va);
    EXPECT_EQ(r, resOk);
    EXPECT_EQ(va,(Void*)0x0);


    /*
     *  allocated : 0x00000 - 0x000FF
     *  allocated : 0x01000 - 0x010FF
     *  free: 0x00100-0x00FFF
     *  free: 0x01100-0x01FFF
     *  free: 0x02000-0xFFFFF
     */

    r = vmaAlloc(hVma,0x100,0x1000,&va);
    EXPECT_EQ(r, resOk);
    EXPECT_EQ(va,(Void*)0x1000);

    /*
     *  allocated : 0x00000 - 0x000FF
     *  allocated : 0x01000 - 0x01FFF
     *  allocated : 0x02000 - 0x020FF
     *  free: 0x00100-0x00FFF
     *  free: 0x01100-0x01FFF
     *  free: 0x02100-0xFFFFF
     */
    r = vmaAlloc(hVma,0x100,0x1000,&va);
    EXPECT_EQ(r, resOk);
    EXPECT_EQ(va,(Void*)0x2000);

    /*
     *  allocated : 0x00000 - 0x000FF
     *  allocated : 0x01000 - 0x01FFF
     *  allocated : 0x02000 - 0x020FF
     *  allocated : 0x02100 - 0x030FF
     *  free: 0x00100-0x00FFF
     *  free: 0x01100-0x01FFF
     *  free: 0x03100-0xFFFFF
     */
    r = vmaAlloc(hVma,0x1000,0x100,&va);
    EXPECT_EQ(r, resOk);
    EXPECT_EQ(va,(Void*)0x2100);

    /*
     *  allocated : 0x00000 - 0x000FF
     *  allocated : 0x01000 - 0x01FFF
     *  allocated : 0x02000 - 0x020FF
     *  allocated : 0x02100 - 0x030FF
     *  allocated : 0x00100 - 0x001FF
     *  free: 0x00200-0x00FFF
     *  free: 0x01100-0x01FFF
     *  free: 0x03100-0xFFFFF
     */

    r = vmaAlloc(hVma,0x100,0x100,&va);
    EXPECT_EQ(r, resOk);
    EXPECT_EQ(va,(Void*)0x0100);

    /*
     *  allocated : 0x00000 - 0x000FF
     *  allocated : 0x01000 - 0x01FFF
     *  allocated : 0x02000 - 0x020FF
     *  allocated : 0x02100 - 0x030FF
     *  allocated : 0x00100 - 0x001FF
     *  allocated : 0x00200 - 0x002FF
     *  free: 0x00300-0x00FFF
     *  free: 0x01100-0x01FFF
     *  free: 0x03100-0xFFFFF
     */

    r = vmaAlloc(hVma,0x100,0x100,&va);
    EXPECT_EQ(r, resOk);
    EXPECT_EQ(va,(Void*)0x0200);

    /*
     *  allocated : 0x00000 - 0x000FF
     *  allocated : 0x01000 - 0x01FFF
     *  allocated : 0x02000 - 0x020FF
     *  allocated : 0x02100 - 0x030FF
     *  allocated : 0x00100 - 0x001FF
     *  allocated : 0x00200 - 0x002FF
     *  allocated : 0x04000 - 0x000FF
     *  free: 0x00300-0x00FFF
     *  free: 0x01100-0x01FFF
     *  free: 0x03100-0x03FFF
     *  free: 0x04100-0xFFFFF
     */
    r = vmaAlloc(hVma,0x100,0x1000,&va);
    EXPECT_EQ(r, resOk);
    EXPECT_EQ(va,(Void*)0x4000);

    /*
     *  allocated : 0x00000 - 0x000FF
     *  allocated : 0x01000 - 0x01FFF
     *  allocated : 0x02000 - 0x020FF
     *  allocated : 0x02100 - 0x030FF
     *  allocated : 0x00100 - 0x001FF
     *  allocated : 0x00200 - 0x002FF
     *  allocated : 0x04000 - 0x000FF
     *  allocated : 0x00300 - 0x006FF
     *  free: 0x00700-0x00FFF
     *  free: 0x01100-0x01FFF
     *  free: 0x03100-0x03FFF
     *  free: 0x04100-0xFFFFF
     */
    r = vmaAlloc(hVma,0x400,0x100,&va);
    EXPECT_EQ(r, resOk);
    EXPECT_EQ(va,(Void*)0x300);

    /*
     *  allocated : 0x00000 - 0x000FF
     *  allocated : 0x01000 - 0x01FFF
     *  allocated : 0x02000 - 0x020FF
     *  allocated : 0x02100 - 0x030FF
     *  allocated : 0x00100 - 0x001FF
     *  allocated : 0x00200 - 0x002FF
     *  allocated : 0x04000 - 0x000FF
     *  allocated : 0x00300 - 0x006FF
     *  allocated : 0x00700 - 0x007FF
     *  free: 0x00800-0x00FFF
     *  free: 0x01100-0x01FFF
     *  free: 0x03100-0x03FFF
     *  free: 0x04100-0xFFFFF
     */
    r = vmaAlloc(hVma,0x100,0x100,&va);
    EXPECT_EQ(r, resOk);
    EXPECT_EQ(va,(Void*)0x700);

    /*
     *  allocated : 0x00000 - 0x000FF
     *  allocated : 0x01000 - 0x01FFF
     *  allocated : 0x02000 - 0x020FF
     *  allocated : 0x02100 - 0x030FF
     *  allocated : 0x00100 - 0x001FF
     *  allocated : 0x00200 - 0x002FF
     *  allocated : 0x04000 - 0x000FF
     *  allocated : 0x00300 - 0x006FF
     *  allocated : 0x00700 - 0x007FF
     *  free: 0x00800-0x00FFF
     *  free: 0x03100-0x03FFF
     *  free: 0x04100-0xFFFFF
     */
    r = vmaAlloc(hVma,0x900,0x100,&va);
    EXPECT_EQ(r, resOk);
    EXPECT_EQ(va,(Void*)0x01100);
}



TEST(VmaTest, Free)
{
    Result r;
    vmaObject_t vma;
    vmaHandle hVma = &vma;

    /*
     *  allocated : na
     *  free: 0x00000-0xFFFFF
     */
    vmaInit(hVma,0x100000);
    Void* va;


    /*
     *  allocated : 0x00000 - 0x000FF
     *  free: 0x00100 - 0xFFFFF
     */
    r = vmaAlloc(hVma,0x100,0x100,&va);
    EXPECT_EQ(r, resOk);
    EXPECT_EQ(va,(Void*)0x0);


    /*
     *  allocated : na
     *  free: 0x00000 - 0x000FF
     *  free: 0x00100 - 0xFFFFF
     */
    r = vmaFree(hVma,va);
    EXPECT_EQ(r, resOk);

    /*
     *  allocated : 0x00100 - 0x010FF
     *  free: 0x00000 - 0x000FF
     *  free: 0x01100 - 0xFFFFF
     */
    r = vmaAlloc(hVma,0x1000,0x100,&va);    
    EXPECT_EQ(r, resOk);
    EXPECT_EQ(va,(Void*)0x100);



}


TEST(VmaTest, Defrag)
{
    Result r;
    vmaObject_t vma;
    vmaHandle hVma = &vma;

    /*
     *  allocated : na
     *  free: 0x00000-0xFFFFF
     */
    vmaInit(hVma,0x100000);
    Void* va;


    /*
     *  allocated : 0x00000 - 0x000FF
     *  free: 0x00100 - 0xFFFFF
     */
    r = vmaAlloc(hVma,0x100,0x100,&va);
    EXPECT_EQ(r, resOk);
    EXPECT_EQ(va,(Void*)0x0);


    /*
     *  allocated : na
     *  free: 0x00000 - 0x000FF
     *  free: 0x00100 - 0xFFFFF
     */
    r = vmaFree(hVma,va);
    EXPECT_EQ(r, resOk);

    /*
     *  allocated : 0x00100 - 0x010FF
     *  free: 0x00000 - 0x000FF
     *  free: 0x01100 - 0xFFFFF
     */
    r = vmaAlloc(hVma,0x1000,0x100,&va);    
    EXPECT_EQ(r, resOk);
    EXPECT_EQ(va,(Void*)0x100);

    /*
     *  allocated : na
     *  free: 0x00000 - 0x000FF
     *  free: 0x01100 - 0xFFFFF
     *  free: 0x00100 - 0x010FF
     */
    r = vmaFree(hVma,va);
    EXPECT_EQ(r, resOk);

    /*
     *  allocated : na
     *  free: 0x00000-0xFFFFF
     */
    r = vmaDefrag(hVma);


    r = vmaAlloc(hVma,0x1000,0x1000,&va);
    EXPECT_EQ(r, resOk);
    EXPECT_EQ(va,(Void*)0x0);   
}