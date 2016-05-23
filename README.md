Copyright (c) 2016 ljqhack@Southeast University

## Welcome to seuOS

本项目基于 LPC82x(cortex-m0+)开发的一款嵌入式调度内核，实现如下功能：
* 任务管理
* 时间管理
* 信息量管理
* 内存管理

### Step1.在一个全新的堆栈中运行函数

每个函数运行时都会有自己的堆栈，局部变量、参数、寄存器现场都是保存在堆栈中，我们通常写代码，调用一个函数时，堆栈实际都是由编译器负责帮你完成了所有工作。那么，我们有没有什么办法指定一个堆栈空间，让函数运行时使用这个堆栈呢？
首先要了解一下函数调用的过程，以函数delay()为例，函数原型如下：
```
void delay(uint32_t i)
{
	uint32_t j;
	uint32_t k;
	for(j = i; j > 0; j--)
	{
		for(k = 10000; k > 0; k--);
	}
}
```
该函数编译后，汇编代码如下：
```
0x00000A9E 0000      MOVS     r0,r0
0x00000AA0 4601      MOV      r1,r0
0x00000AA2 E005      B        0x00000AB0
0x00000AA4 4A04      LDR      r2,[pc,#16]  ; @0x00000AB8
0x00000AA6 E000      B        0x00000AAA
0x00000AA8 1E52      SUBS     r2,r2,#1
0x00000AAA 2A00      CMP      r2,#0x00
0x00000AAC D1FC      BNE      0x00000AA8
0x00000AAE 1E49      SUBS     r1,r1,#1
0x00000AB0 2900      CMP      r1,#0x00
0x00000AB2 D1F7      BNE      0x00000AA4
0x00000AB4 4770      BX       lr
```

操作系统开发笔记：
[https://ljqhack1.gitbooks.io/operate_system_note/content/index.html](https://ljqhack1.gitbooks.io/operate_system_note/content/index.html)
