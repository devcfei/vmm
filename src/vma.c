#include <vmm.h>


#define MAX_VA_COUNT 0x1000 

static Size_t stackItemGlobal[MAX_VA_COUNT];
static vmaItem_t vmaItemGlobal[MAX_VA_COUNT];


static
vmaItem_t*
vmaItemAlloc(
    vmaHandle hVma
    )
{
    Result r = resOk;

    Size_t i;
    stackHandle hStack = &hVma->itemStack;
    r = stackPop(hStack, &i);

    return (vmaItem_t *)i;
}

static
Void
vmaItemFree(
    vmaHandle hVma,
    vmaItem_t* item
    )
{
    Result r = resOk;

    stackHandle hStack = &hVma->itemStack;
    r = stackPush(hStack, (Size_t)item);
    if (FAILED(r))
    {
        /*
         * stack overflow
         */
    }
}

Result
vmaInit(
	vmaHandle hVma,
	Size_t sizeTotal
	)
{
    Result r = resOk;
    stackHandle hStack = &hVma->itemStack;

    listInit(&hVma->listAllocated);
    listInit(&hVma->listFree);

    stackInit(hStack, stackItemGlobal, MAX_VA_COUNT);

    /*
     *  Push the items to the stack    
     */
    for (Size_t i = 0; i < MAX_VA_COUNT; ++i)
    {
        vmaItemFree(hVma, &vmaItemGlobal[i]);
    }

    vmaItem_t *item = vmaItemAlloc(hVma);
    item->va = 0;
    item->size = sizeTotal;

    listInsertTail(&hVma->listFree, &item->node);

    return r;
}

Result
vmaAlloc(
	vmaHandle hVma,
	Size_t size,
	Uint Alignment,
    Void** vaAllocated
	)
{
    Result r = resOk;
    listHead freeList = &hVma->listFree;

    Size_t vaAligned;
    Size_t sizeDrop;

    vmaItem_t *item;

    LIST_FOR_EACH(freeList, nextNode)
    {

        item = TYPE_BASE(nextNode, vmaItem_t, node);

        vaAligned = ALIGN_UP((Size_t)(item->va), Alignment);

        if (vaAligned + size <= (Size_t)item->va + item->size)
        {
            vmaItem_t *itemAllocated = vmaItemAlloc(hVma);
            itemAllocated->va = (Void *)vaAligned;
            itemAllocated->size = size;

            listInsertTail(&hVma->listAllocated, &itemAllocated->node);

            item->size -= itemAllocated->size;

            sizeDrop = vaAligned - (Size_t)item->va;

            if (sizeDrop)
            {
                vmaItem_t *itemDrop = vmaItemAlloc(hVma);
                itemDrop->va = item->va;
                itemDrop->size = sizeDrop;

                listInsertPrev(nextNode, &itemDrop->node);

                item->size -= sizeDrop;
            }

            item->va = itemAllocated->va + itemAllocated->size;

            *vaAllocated = itemAllocated->va;

            break;
        }
    }

    return r;
}

Result
vmaFree(
	vmaHandle hVma,
	Void* vaFree
	)
{
    Result r = resOk;

    listHead allocateList = &hVma->listAllocated;

    vmaItem_t *item;

    LIST_FOR_EACH(allocateList, nextNode)
    {
        item = TYPE_BASE(nextNode, vmaItem_t, node);

        if (item->va == vaFree)
        {
            listHead freeList = &hVma->listFree;
            vmaItem_t *itemFree;

            listRemoveNode(nextNode);

            LIST_FOR_EACH(freeList, nodeFree)
            {
                itemFree = TYPE_BASE(nodeFree, vmaItem_t, node);

                if ((vaFree < itemFree->va))
                {
                    listInsertPrev(nodeFree, nextNode);
                    break;
                }
            }

            break;
        }
    }

    return r;
}



Result
vmaDefrag(
	vmaHandle hVma
	)
{
    Result r = resOk;

    listHead freeList = &hVma->listFree;
    vmaItem_t *item;

    /*
	 *  Go through the free list and find the entries which are contiguous.
	 */

    LIST_FOR_EACH(freeList, nextNode)
    {
        item = TYPE_BASE(nextNode, vmaItem_t, node);

        listNode *backNode = nextNode->next;
        if (backNode)
        {
            vmaItem_t *itemBack;

            itemBack = TYPE_BASE(backNode, vmaItem_t, node);
            if (((Size_t)item->va + item->size) == (Size_t)itemBack->va)
            {
                itemBack->va = item->va;
                itemBack->size += item->size;

                listRemoveNode(nextNode);

                vmaItemFree(hVma, item);
            }
        }
    }

    return r;
}
