;After task has been created, call 
;this function to start OS
OSStartHighRdy
	LDR R0, =NVIC_SYSPRI14
	LDR R1, =NVIC_PENDSV_PRI
	STRB R1, [R0]
	MOV R0, #0
	MSR PSP, R0
	LDR R0, __OS_Running
	MOV R1, #1
	STRB R1, [R0]
	LDR R0, =NVIC_INT_CTRL
	LDR R1, =NVIC_PENDSVSET
	STR R1, [R0]
	CPSIE I

;Context switch,trigger PendSV
OSCtxSw
    LDR R0, =NVIC_INT_CTRL
    LDR R1, =NVIC_PENDSVSET
    STR R1, [R0]
    BX LR


;PendSV handler
OS_CPU_PendSVHandler
    CPSID I
    MRS R0,PSP
    CBZ R0,OSPendSV_nosave

    SUBS R0,R0, #0x20
    STM R0, {R4-R11}
    LDR R1, =OS_TCBCur
    LDR R1, [R1]
    STR R0, [R1]
OSPendSV_nosave
    LDR R0, =OSPrioCur
    LDR R1, =OSPrioHighRdy
    LDR R2,[R1]
    STRB R2, [R0]
    
    LDR R0, =OSTCBCur
    LDR R1, =OSTCBHighRdy
    LDR R2, [R1]
    STR R2, [R0]
    
    LDR R0, [R2]
    LDM R0, {R4-R11}
    ADDS R0, R0, #0x20
    MSR PSP, R0
    ORR LR, LR, #0x04
    CPSIE I
    BX LR

    END   
