#ifndef __OS_DEF_H__
#define __OS_DEF_H__

/************************************/
/********Basictype define************/
/************************************/
typedef unsigned char   			BOOLEAN;
typedef unsigned char   			INT8U;
typedef signed char    		 		INT8S;
typedef unsigned short int    INT16U;
typedef signed short int      INT16S;
typedef unsigned int   				INT32U;
typedef signed int     				INT32S;

typedef INT32U OS_STK_t;
typedef struct TCB{
    OS_STK_t *OSTaskStackTop;
    INT32U OSWaitTick;
}TCB_t;


typedef struct os_mem{
		void *MemStartAddr;
		void *MemFreeList;
		INT32U MemBlkSize;
		INT32U MemN;
		INT32U MemNFree;
}OSMEM_t;

#define portINITIAL_XPSR			( 0x01000000 )

/*
*********************************************************************************************************
*                                             ERROR CODES
*********************************************************************************************************
*/
typedef INT8U													OS_ERR_t;

#define OS_ERR_NONE                   0u

#define OS_ERR_MEM_INVALID_MP					1u
#define OS_ERR_MEM_INVALID_ADDR				2u
#define OS_ERR_MEM_INVALID_BLKS				3u
#define OS_ERR_MEM_INVALID_SIZE				4u


#endif
