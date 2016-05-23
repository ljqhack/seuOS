       
   AREA globals,CODE,READONLY  
   
   IMPORT OSCurrentTCB
   EXPORT OSStartHighRdy
;After task has been created, call 
;this function to start OS
OSStartHighRdy
	LDR R3, =OSCurrentTCB
	LDR R1, [R3]
	LDR R0, [R1]
	adds R0, #32
	msr psp, R0
	movs R0, #2
	msr CONTROL, R0
	pop {R0-R5}
	mov LR, R5
	pop {pc}
	end