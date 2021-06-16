#ifndef __LIST_H__
#define __LIST_H__

typedef struct listNode_s
{
	struct listNode_s *next;
	struct listNode_s *prev;
} listNode;

typedef listNode* listHead;

static 
inline 
Void
listInit(
	listHead list
	)
{
	list->next = list->prev = list;
	return;
}

static 
inline 
Bool
listIsEmpty(
	const listHead list
	)
{
	return (Bool)(list->next == list);
}

static 
inline 
Bool
listRemoveNode(
	listNode *node
	)
{

	listNode *prev;
	listNode *next;

	next = node->next;
	prev = node->prev;
	if ((next->prev != node) || (prev->next != node))
	{
		/*fatal error*/
	}

	prev->next = next;
	next->prev = prev;
	return (Bool)(prev == next);
}

static 
inline 
listNode *
listRemoveHead(
	listHead list
	)
{
	listNode *node;
	listNode *next;

	node = list->next;

	next = node->next;
	if ((node->prev != list) || (next->prev != node))
	{
		/*fatal error*/
	}
	list->next = next;
	next->prev = list;
	return node;
}

static 
inline 
listNode *
listRemoveTail(
	listHead list
	)
{
	listNode *node;
	listNode *prev;

	node = list->prev;
	prev = node->prev;
	if ((node->next != list) || (prev->next != node))
	{
		/*fatal error*/
	}

	list->prev = prev;
	prev->next = list;
	return node;
}

static 
inline 
Void
listInsertTail(
	listHead list,
	listNode *node
	)
{
	listNode *prev;
	prev = list->prev;
	if (prev->next != list)
	{
		/*fatal error*/
	}

	node->next = list;
	node->prev = prev;
	prev->next = node;
	list->prev = node;
	return;
}

static 
inline 
Void
listInsertHead(
	listHead list,
	listNode *node
	)
{
	listNode *next;
	next = list->next;
	if (next->prev != list)
	{
		/*fatal error*/
	}

	node->next = next;
	node->prev = list;
	next->prev = node;
	list->next = node;
	return;
}

static 
inline 
Void
listInsertPrev(
	listNode *current,
	listNode *node
	)
{
	current->prev->next = node;
	node->next = current;
	node->prev = current->prev;
	current->prev = node;

	return;
}

static 
inline 
Void
listAppend(
	listHead list,
	listNode *append
	)
{
	listNode *end = list->prev;
	list->prev->next = append;
	list->prev = append->prev;
	append->prev->next = list;
	append->prev = end;
	return;
}

#define LIST_FOR_EACH(_LIST_, _NODE_NAME_)                   \
	for (listNode *_NODE_NAME_ = (listNode *)(_LIST_)->next; \
		 _NODE_NAME_ != (_LIST_);                            \
		 _NODE_NAME_ = _NODE_NAME_->next)

#endif /* __LIST_H__ */