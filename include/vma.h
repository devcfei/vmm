#ifndef __VMA_H__
#define __VMA_H__


typedef struct vmaItem_s
{
	Void* va;
    Size_t size;
	listNode node;
}vmaItem_t;


typedef struct vmaObject_s
{
	listNode listFree;
	listNode listAllocated;


    stackObject_t itemStack;

}vmaObject_t;

typedef vmaObject_t* vmaHandle;



Result
vmaInit(
	vmaHandle hVma,
	Size_t sizeTotal
	);

Result
vmaAlloc(
	vmaHandle hVma,
	Size_t Size,
	Uint Alignment,
	Void** vaAllocated
	);

Result
vmaFree(
	vmaHandle hVma,
	Void* vaFree
	);


#endif /*__VMA_H__*/