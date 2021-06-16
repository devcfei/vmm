#include <gtest/gtest.h>


extern "C"
{
#include <vmm.h>


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