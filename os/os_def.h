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

#define portINITIAL_XPSR			( 0x01000000 )

#endif
