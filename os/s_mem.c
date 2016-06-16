#include "seuos.h"

/**
 * @brief	Initialize a memory block
 * @return	if error occur, return error code
 */
OS_ERR_t OSMemInit(OSMEM_t *mp, void *addr, INT8U nblks, INT8U blksize)
{
	void **plink;
	INT8U *pblk;
	INT8U i;
	if(mp == (void *)0)
	{
		return OS_ERR_MEM_INVALID_MP;
	}
	if(addr == (void *)0)
	{
		return OS_ERR_MEM_INVALID_ADDR;
	}
	if( ((INT32U)addr & (sizeof(void *) - 1)) != 0u)
	{
		return OS_ERR_MEM_INVALID_ADDR;
	}
	if(nblks < 2u )
	{
		return OS_ERR_MEM_INVALID_BLKS;
	}
	if(blksize < sizeof(void *))
	{
		return OS_ERR_MEM_INVALID_SIZE;
	}
	plink = (void **) addr;
	pblk = (INT8U *)((INT32U)addr + blksize);
	for(i = 0u; i < (nblks - 1u); i++)
	{
		*plink = pblk;
		plink = (void **)pblk;
		pblk = (INT8U *)((INT32U)addr + blksize);
	}
	*plink = (void *) 0;
	mp->MemStartAddr = addr;
	mp->MemFreeList = addr;
	mp->MemN = nblks;
	mp->MemBlkSize = blksize;
	mp->MemNFree = nblks;
	return OS_ERR_NONE;
}

/**
 * @brief	Get a memory block from memory pool
 * @return if error occur,return error code, else return pointer
 */
void *OSMemAlloc(OSMEM_t *mp)
{
	void *blk;
	if(mp == (void *)0)
	{
		return (void*)0;
	}
	
	if(! mp->MemNFree)
	{
		return (void*)0;
	}
	
	blk = mp->MemFreeList;
	mp->MemFreeList = *(void **)blk;
	mp->MemNFree--;
	return blk;
}


/**
 * @brief	Free a memory
 * @return None
 */
void OSMemFree(OSMEM_t *mp, void *pblk)
{
	if( (mp == (void *)0) || (pblk == (void *)0) )
	{
		return;
	}
	if(mp->MemNFree >= mp->MemN)
	{
		return;
	}
	*(void **)pblk = mp->MemFreeList;
	mp->MemFreeList = pblk;
	mp->MemNFree++;
}
