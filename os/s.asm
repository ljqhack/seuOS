
OS_NVIC_INT_CTRL      EQU     0xe000ed04
OS_NVIC_PENDSVSET	  EQU	  0x10000000	
   AREA globals,CODE,READONLY  
   IMPORT OSCurrentTCB
   IMPORT OSPrioHighRdy
   IMPORT OSTaskRunningPrio
   IMPORT OSPrioHighTCB
	      
;After task has been created, call 
;this function to start OS
OSStartHighRdy  PROC
	EXPORT OSStartHighRdy
	LDR R3, =OSCurrentTCB
	LDR R1, [R3]
	LDR R0, [R1]
	ADDS R0, #32
	MSR psp, R0
	MOVS R0, #2
	MSR CONTROL, R0
	POP {R0-R5}
	MOV LR, R5
	POP {pc}
	ENDP
		
OSCtxSw  PROC
	EXPORT OSCtxSw
	LDR R0, =OS_NVIC_INT_CTRL
	LDR R1, =OS_NVIC_PENDSVSET
	STR R1, [R0]
	BX LR
	ENDP



PendSV_Handler  PROC
	EXPORT PendSV_Handler
	;MOV R0, R0
	;BX LR
	MRS R0, psp
	LDR R3, =OSCurrentTCB
	LDR R2, [R3]
	SUBS R0, R0, #32
	STR R0, [R2]                 ;save psp to tcb
	STMIA R0!, {R4-R7}
	MOV R4, R8
	MOV R5, R9
	MOV R6, R10
	MOV R7, R11
	STMIA R0!, {R4-R7}
	
	LDR R0, =OSPrioHighRdy
	LDR R1, =OSTaskRunningPrio
	LDRB R2, [R0]
	STRB R2, [R1]                ;OSTaskRunningPrio=OSPrioHighRdy
	
	LDR R0, =OSPrioHighTCB
	LDR R1, =OSCurrentTCB
	LDR R2, [R0]
	STR R2, [R1]                ;OSCurrentTCB=OSPrioHighTCB
	
	LDR R0,[R2]                  ;R0 is sp
	ADDS R0, R0, #16
	LDMIA R0!, {R4-R7}
	MOV R8, R4
	MOV R9, R5
	MOV R10, R6
	MOV R11, R7

	MSR PSP, R0
	
	SUBS R0, #32
	LDMIA R0!, {R4-R7}
	
	BX LR
	
	;MOVS R0, #2
	;MSR CONTROL, R0
	;POP {R0-R5}
	;MOV LR, R5
	;POP {pc}
	ENDP
	end