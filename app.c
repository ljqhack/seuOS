#include "seuos.h"
#include "stm32f10x.h"


OS_STK_t Stack[200];

void Init_LED(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;					

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD| RCC_APB2Periph_GPIOG |RCC_APB2Periph_AFIO,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14;            
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	
  	GPIO_Init(GPIOG, &GPIO_InitStructure);				  

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ; 			
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   	
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	
  	GPIO_Init(GPIOD, &GPIO_InitStructure);	
 
    //GPIO_SetBits(GPIOG, GPIO_Pin_14);
    //GPIO_SetBits(GPIOD, GPIO_Pin_13);
    GPIO_ResetBits(GPIOG, GPIO_Pin_14);
    GPIO_ResetBits(GPIOD, GPIO_Pin_13);
}

void task0()
{
    INT32U i = 0;
    while(1)
    {
        i++;
        OSTimeDly(4);
    }
}
void task1()
{
    INT32U i = 0;
    while(1)
    {
        i++;
        OSTimeDly(4);
    }
}
void task2()
{
    INT32U i = 0;
    while(1)
    {
        i++;
        OSTimeDly(4);
    }
}
void task3()
{
    INT32U i = 0;
    while(1)
    {
        i++;
        OSTimeDly(4);
    }
}


int main(int argc, char *argv[])
{
    Init_LED();
    OSTaskCreate(Task0, &Stack[49], 0);
    OSTaskCreate(Task1, &Stack[99], 1);
    OSTaskCreate(Task2, &Stack[149], 2);
    OSTaskCreate(Task3, &Stack[199], 3);
    OSStart();
    return 0;
}
