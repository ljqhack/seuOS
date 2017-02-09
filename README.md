Copyright (c) 2016 ljqhack@Southeast University


## Welcome to seuOS


本项目希望开发的一款嵌入式调度内核，实现如下功能：
* 任务管理
* 时间管理
* 信息量、消息队列管理
* 内存管理

为了能较直观的展现，本项目前面几个step在代码组织上没有过多考虑可移值性等一些问题，后面会慢慢整理为比较规范的代码。
开发过程中的代码是在LPC82x系列MCU上进行测试,LPC82x是Cortex-M0+的内核，下文中讲述的一些内容可能讲的是Cortex-M3，但实现的代码却是Cortex-M0+的代码，不必惊讶，望各位注意这个问题，因为基本思想都大同小异，有不同之处可以自行到ARM官网查阅相关文档。以下文字及代码大多本人一点一点码起来，部份也会参考相关文档内容，由于水平有限，难免会有一些错误，希望各位批评指教！
* 操作系统开发笔记：
[https://ljqhack1.gitbooks.io/operate_system_note/content/index.html](https://ljqhack1.gitbooks.io/operate_system_note/content/index.html)


### Step0.代码的组织

* seuOS/
* ├── common/                   lpc82x的common文件
* ├── os/                       本项目要写的os代码
* │     ├── seuos.c             os的核心代码
* │     ├── seuos.h             os头文件
* │     ├── s.asm               相关汇编代码
* │     └── os_def.h            相关宏定义及类型定义
* │ 
* ├── peri_example/             测试代码  
* ├── peri_example/             lpc82x的driver  
* ├── tool                      工具
* └── README.md                 你正在看的文件 -_-" 这不用介绍了吧


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

有了以上基础后，这样可以写一个函数，创建人工堆栈，模拟一个中断过程（Cortex-M0+），代码如下：

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

### Step2.上下文切换（Context Switch）

有了上面的成果，实际上这个系统中已经运行起来了一个任务，只不过这个任务一直强占CPU资源，还没有一种机制让这个任务在不想使用CPU的时候让出使用权，所以下面我们要做的事情就是如果让任务让出使用权，从而使其它任务开始运行。
上下文切换，实际含义是任务切换，或者说CPU寄存器内容的切换，当多任务内核决定运行其它任务时，它保存正在运行任务的当前状态，也即CPU寄存器全部内容，这些内容被保存在当前任务的栈区，入栈完后，就把下一个将要运行的任务的当前状况从栈区重新加载到CPU中，并开始下一个任务的运行，这个过程就叫做任务切换。

对于Cortex-M内核而言，硬件中断异常系统引入了PendSV中断，即可悬挂中断，不同于普通的软件中断，它可以被“缓期执行”，直到其它重要的任务完了后才执行。
PendSV典型的使用场合就是上下文切换（不同任务间切换）。例如，一个系统中有两个就绪任务，上下文切换被触发的场合可以是：
* 执行一个系统调用
* 系统滴答定时器（SYSTICK）中断（轮转调度需要）

PendSV中断处理函数中需要做些什么事情呢？下面的就是pendsv中的处理流程：
```
OS_CPU_PendSVHandler:
if (PSP != NULL) { (1)
Save R4-R11 onto task stack; (2)
OSTCBCur->OSTCBStkPtr = SP; (3)
}
OSTaskSwHook(); (4)
OSPrioCur = OSPrioHighRdy; (5)
OSTCBCur = OSTCBHighRdy; (6)
PSP = OSTCBHighRdy->OSTCBStkPtr; (7)
Restore R4-R11 from new task stack; (8)
Return from exception; (9)
```
参考该过程，可以写出CM0+中对应代码如下：
```
PendSV_Handler  PROC
  EXPORT PendSV_Handler
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
  ENDP
```
上面的代码可以完成任务运行过程中所需要的上下文切换，OSPrioHighRdy和OSPrioHighTCB可以看成两个PendSV的函数，这两个参数在调试器中计算完成，并在调度器中调用PendSV触发函数以完成上下文切换。

另外，step2中写了一个延时管理函数OSTimeDly,该函数只是简单的使任务进入非就绪态，然后进行调度。但是到目前为止，任务还不能自由来回切换，step2中的代码运行后，只能从task0被调度到task1，但是还不能重新回到task0运行，这个的实现请看step3。


### Step3.系统的心脏（时钟节拍）

下面这一节来完善OSTimeDly的相关功能，完成这一节后，我们写的这个os将能够运转起来。

时钟节拍（clock tick）是指特定的周期性中断。这个中断可以看做是系统心脏的脉动。中断之前的时间间隔取决于不同的应用，一般为10-200ms。时钟节拍使得内核可以将任务延时若十个整数时钟节拍，以及当任务等待事件发生时，提供等待超时的依据。时钟节拍越快，系统的额外开销就越大。

首先要配置一下MCU的Systick定时器(1ms)：
```
void OSCPUSystickInit( void )
{
	*(SYSTICK_LOAD) = ( configCPU_CLOCK_HZ / configTICK_RATE_HZ ) - 1UL;
	*(SYSTICK_CTRL) = portNVIC_SYSTICK_CLK | portNVIC_SYSTICK_INT | portNVIC_SYSTICK_ENABLE;
}
```

紧接着，系统每隔1ms进入一次中断，在中断里面需要完成如下一事情：
* 遍历任务控制块（OSTCB），将OSWaitTick不为0的控制块都减1
* 如果发现某个OSTCB的OSWaitTick刚好减到0,则将该任务置为就绪态
* 退出中断前，计算就绪表中最高优先级
* 如果当前最高优先级不等于当前运行的优先级，就进行一次调度（触发一次PendSV）

SysTick_Handler的相当代码如下,具体实现请查看step3的相关代码：
```
void SysTick_Handler(void)
{
	OSTimeTick();
	OSIntExit();
}
```

完成上述工作后，任务就可以调度运转起来了

