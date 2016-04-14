#include "seuos.h"
#include "stm32f10x.h"


void Init_LED(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;					//����һ��GPIO�ṹ�����

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD| RCC_APB2Periph_GPIOG |RCC_APB2Periph_AFIO,ENABLE);	
															//ʹ�ܸ����˿�ʱ�ӣ���Ҫ������
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_14;             //����LED D2�˿ڹҽӵ�PG14�˿�
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   	//ͨ���������
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//���ö˿��ٶ�Ϊ50M
  	GPIO_Init(GPIOG, &GPIO_InitStructure);				   	//���˿�GPIOD���г�ʼ������

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 ; 			//����LED D5�˿ڹҽӵ�13�˿�
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   	//ͨ���������
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   	//���ö˿��ٶ�Ϊ50M
  	GPIO_Init(GPIOD, &GPIO_InitStructure);				   	//���˿�GPIOD���г�ʼ������

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
