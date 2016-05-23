Copyright (c) 2016 ljqhack@Southeast University

## Welcome to seuOS

本项目基于 sep6210(cortex-m3)开发的一款嵌入式调度内核，实现如下功能：
* 任务管理
* 时间管理
* 信息量管理
* 内存管理

### Step1.在一个全新的堆栈中运行函数

每个函数运行时都会有自己的堆栈，局部变量、参数、寄存器现场都是保存在堆栈中，我们通常写代码，调用一个函数时，堆栈实际都是由编译器负责帮你完成了所有工作。那么，我们有没有什么办法指定一个堆栈空间，让函数运行时使用这个堆栈呢？
事实上，要完成这件事情，可以模拟一个函数被中断后堆栈的情况，这样再强制将sp指针指向人工堆栈的栈顶，最后模拟中断返回，这样函数就好像刚中断一样。
下面以cortex-m3为例，介绍一下Cortex-M3异常响应过程，分为三部曲(这三个步骤都是由硬件自动完成，不需要软件插手)：
* 入栈：将8个寄存器的值压入栈
* 取中断向量：从向量表找出服务程序入口地址
* 选择堆栈指针MSP/PSP，更新堆栈指针SP，更新连接寄存器LR，更新程序计数器PC

首先**入栈**的8个寄存器按空间顺序依次为（注意并非时间顺序）：xPSR、PC、LR、R12、R3、R2、R1、R0；注意这里为啥只压入R0-R3以及R12呢？原来，在ARM上，有一套的C函数调用标准约定（《C/C++ Procedure Call Standard for the ARM Architecture》， AAPCS, Ref5 ） 中原因就在它上面：它使得中断服务例程能用C语言编写，编译器优先使用入栈了的寄存器来保存中间结果（当然，如果程序过大也可能要用到R4-R11，此时编译器负责生成代码来push它们，但是ISR应该短小精悍，不要让系统如此操心）。

**取指**并非在入栈完成才进行,当数据总线（系统总线）正在为入栈操作而忙得风风火火时，指令总线（I-Code总线）可不是凉快地坐那儿看热闹——它正在为响应中断紧张有序地执行另一项重要的任务：从向量表中找出正确的异常向量，然后在服务程序的入口处预取指。

最后是**更新寄存器**，在入栈和取向量操作完成之后，执行服务例程之前，还要更新一系列的寄存器：
* SP：在入栈后会把堆栈指针（PSP或MSP）更新到新的位置。在执行服务例程时，将由MSP负责对堆栈的访问；
* PSR：更新IPSR位段（地处PSR的最低部分）的值为新响应的异常编号；
* PC：在取向量完成后，PC将指向服务例程的入口地址；
* LR：在出入ISR的时候，LR的值将得到重新的诠释，这种特殊的值称为“EXC_RETURN”，在异常进入时由系统计算并赋给LR，并在异常返回时使用它。

有了以上基础后，这样可以写一个函数，创建人工堆栈，模拟一个中断过程，代码如下：

```
void OSTaskCreate(void(*task)(void), OS_STK_t *top, INT8U prio)
{
    OS_STK_t *stk;
    if( ( OSTaskTbl & (0x00000001L<<prio)) ) 
    {return;}                                             //Task priority has been created, fail
    stk = top;
    --stk;
    *stk = 0x01000000;
    --stk;
    *stk = (INT32U) task;
    --stk;
    *stk = 0x00000000;
    stk -= 5;
    *stk = 0x00000000;
    stk -= 8;                                             //monitor a interrupt stack
    
    OSTCB[prio].OSTaskStackTop = stk;                     //save stack top to task control bank
    OSTaskTbl |= (0x00000001L)<<prio;                     //task priotiry set bit
    OSRdyTbl |= (0x00000001)<<prio;                       //task ready table
		
    OSPrioHighRdy = prio;				
    OSCurrentTCB = OSTCB + prio;
}
```

创建完人工堆栈后，调用如下代码可以使任务在其中运行：
```
void OSStartTask(void)
{
	if(false == OSRunning)
	{
		OSPrioHighRdy = 0;
		while( !(OSRdyTbl & ((0x00000001L)<<OSPrioHighRdy)) )
		{
			OSPrioHighRdy++;
		}
		OSTaskRunningPrio = OSPrioHighRdy;
		OSStartHighRdy();
	}
}

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
```
其中，OSStartHighRdy可以申明它的原型为 void OSStartHighRdy(void)。具体代码可见step1分支。

操作系统开发笔记：
[https://ljqhack1.gitbooks.io/operate_system_note/content/index.html](https://ljqhack1.gitbooks.io/operate_system_note/content/index.html)
