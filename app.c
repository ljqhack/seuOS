#include "seuos.h"
#include "stm32f10x.h"


void Init_LED(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;					//定义一个GPIO结构体变量

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD| RCC_APB2Periph_GPIOG |RCC_APB2Periph_AFIO,ENABLE);	
															//使能各个端口时钟，重要！！！
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14;             //配置LED D2端口挂接到PG14端口
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   	//通用输出推挽
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//配置端口速度为50M
  	GPIO_Init(GPIOG, &GPIO_InitStructure);				   	//将端口GPIOD进行初始化配置

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ; 			//配置LED D5端口挂接到13端口
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   	//通用输出推挽
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//配置端口速度为50M
  	GPIO_Init(GPIOD, &GPIO_InitStructure);				   	//将端口GPIOD进行初始化配置

    //GPIO_SetBits(GPIOG, GPIO_Pin_14);
    //GPIO_SetBits(GPIOD, GPIO_Pin_13);
    GPIO_ResetBits(GPIOG, GPIO_Pin_14);
    GPIO_ResetBits(GPIOD, GPIO_Pin_13);
}

int main(int argc, char *argv[])
{
    Init_LED();
    return 0;
}
