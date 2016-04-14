/**
  ******************************************************************************
  * @�ļ�    stm32f10x_exti.c
  * @����    MCD Ӧ�ó�����
  * @�汾    V3.1.0
  * @����    06/19/2009
  * @����    ����ļ��ṩȫ�� EXTI �̼�����.
  * @����    ANSON/sweet1985 Email:airanson110@126.com
  * @����    06/07/2009
  ******************************************************************************
  * @����
  *
  * ����̼��������ṩ���ͻ���Ϊ��Ʋ�Ʒ����д����Ĳο�Ŀ��ʹ�ͻ���Լʱ�䡣����
  * �ͻ�ʹ�ñ��̼��ڿ�����Ʒ����ϲ����Ľ���ⷨ�뵼�幫˾���е��κ�ֱ�ӵĺͼ��
  * �����Σ�Ҳ���е��κ��𺦶�������⳥��
  *
  * <h2><center>&����; ��Ȩ���� 2009 �ⷨ�뵼�幫˾</center></h2>
  * ����汾����ѧϰ������Ӣ��ԭ���г���Ӧ��Ӣ��ԭ��Ϊ׼��
  */ 

/* ���� ------------------------------------------------------------------*/
#include "stm32f10x_exti.h"

/* �������� ------------------------------------------------------------*/
/* ���ö��� ------------------------------------------------------------*/

#define EXTI_LineNone    ((uint32_t)0x00000)  /* No interrupt selected */

/* ���ú� --------------------------------------------------------------*/
/* ���ñ��� ------------------------------------------------------------*/
/* ���ú���ԭ�� --------------------------------------------------------*/
/* ���ú��� ------------------------------------------------------------*/

/**
  * @����  �� EXTI ����Ĵ�����λ��Ĭ��ֵ.
  * @����  û��
  * @����  û��
  */
void EXTI_DeInit(void)
{
  EXTI->IMR = 0x00000000;
  EXTI->EMR = 0x00000000;
  EXTI->RTSR = 0x00000000; 
  EXTI->FTSR = 0x00000000; 
  EXTI->PR = 0x000FFFFF;
}

/**
  * @����  ����EXIT_InitStruct��ָ���Ĳ�����ʼ�� EXTI ����.
  * @����  EXTI_InitStruct: ָ�� EXTI_InitTypeDef �Ľṹ���������ض�EXTI��������Ϣ.
  * @����  û��
  */
void EXTI_Init(EXTI_InitTypeDef* EXTI_InitStruct)
{
  uint32_t tmp = 0;

  /* ������ */
  assert_param(IS_EXTI_MODE(EXTI_InitStruct->EXTI_Mode));
  assert_param(IS_EXTI_TRIGGER(EXTI_InitStruct->EXTI_Trigger));
  assert_param(IS_EXTI_LINE(EXTI_InitStruct->EXTI_Line));  
  assert_param(IS_FUNCTIONAL_STATE(EXTI_InitStruct->EXTI_LineCmd));

  tmp = (uint32_t)EXTI_BASE;
     
  if (EXTI_InitStruct->EXTI_LineCmd != DISABLE)
  {
    /* ��� EXTI �ⲿ�ж������� */
    EXTI->IMR &= ~EXTI_InitStruct->EXTI_Line;
    EXTI->EMR &= ~EXTI_InitStruct->EXTI_Line;
    
    tmp += EXTI_InitStruct->EXTI_Mode;

    *(__IO uint32_t *) tmp |= EXTI_InitStruct->EXTI_Line;

    /* ����������½��ر������� */
    EXTI->RTSR &= ~EXTI_InitStruct->EXTI_Line;
    EXTI->FTSR &= ~EXTI_InitStruct->EXTI_Line;
    
    /* Ϊѡ�����ⲿ�ж�ѡ�񴥷�(�¼�) */
    if (EXTI_InitStruct->EXTI_Trigger == EXTI_Trigger_Rising_Falling)
    {
      /* �������½��� */
      EXTI->RTSR |= EXTI_InitStruct->EXTI_Line;
      EXTI->FTSR |= EXTI_InitStruct->EXTI_Line;
    }
    else
    {
      tmp = (uint32_t)EXTI_BASE;
      tmp += EXTI_InitStruct->EXTI_Trigger;

      *(__IO uint32_t *) tmp |= EXTI_InitStruct->EXTI_Line;
    }
  }
  else
  {
    tmp += EXTI_InitStruct->EXTI_Mode;

    /* ��ֹѡ�е��ⲿ�ж��� */
    *(__IO uint32_t *) tmp &= ~EXTI_InitStruct->EXTI_Line;
  }
}

/**
  * @����  ��EXTI_InitStruct��Ա��ΪĬ��ֵ.
  * @����  EXTI_InitStruct: ָ��EXTI_InitTypeDef�ṹ���ýṹ������ʼ��.
  * @����  û��
  */
void EXTI_StructInit(EXTI_InitTypeDef* EXTI_InitStruct)
{
  EXTI_InitStruct->EXTI_Line = EXTI_LineNone;
  EXTI_InitStruct->EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStruct->EXTI_Trigger = EXTI_Trigger_Falling;
  EXTI_InitStruct->EXTI_LineCmd = DISABLE;
}

/**
  * @����  ����һ������ж�.
  * @����  EXTI_Line: ָ�� EXTI �߼�����ֹ.
  *                   ������������������ EXTI_Linex �� x����ȡ(0..19).
  * @����  û��
  */
void EXTI_GenerateSWInterrupt(uint32_t EXTI_Line)
{
  /* ������ */
  assert_param(IS_EXTI_LINE(EXTI_Line));
  
  EXTI->SWIER |= EXTI_Line;
}

/**
  * @����  ����ض��� EXTI �߱�־�Ƿ���λ.
  * @����  EXTI_Line: �ض��� EXTI �߱�־.
  *                   �������������:
  *                                  EXTI_Linex: �ⲿ�ж��� x����ȡ(0..19)
  * @����  EXTI_Line ����״̬ (SET �� RESET).
  */
FlagStatus EXTI_GetFlagStatus(uint32_t EXTI_Line)
{
  FlagStatus bitstatus = RESET;
  /* ������ */
  assert_param(IS_GET_EXTI_LINE(EXTI_Line));
  
  if ((EXTI->PR & EXTI_Line) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

/**
  * @����  ��� EXTI �ߵĹ����־.
  * @����  EXTI_Line: ָ��Ҫ�����־�� EXTI ��.
  *                   �����������������EXTI_Linex��ϣ�x������(0..19).
  * @����  û��
  */
void EXTI_ClearFlag(uint32_t EXTI_Line)
{
  /* ������ */
  assert_param(IS_EXTI_LINE(EXTI_Line));
  
  EXTI->PR = EXTI_Line;
}

/**
  * @����  ���ָ���� EXTI ��·�������������.
  * @����  EXTI_Line: ����� EXTI ��·�Ĺ���λ.
  *                   �������������:
  *                                  EXTI_Linex: �ⲿ�ж��� x������(0..19)
  * @����  EXTI_Line ����״̬ (SET �� RESET).
  */
ITStatus EXTI_GetITStatus(uint32_t EXTI_Line)
{
  ITStatus bitstatus = RESET;
  uint32_t enablestatus = 0;
  /* ������ */
  assert_param(IS_GET_EXTI_LINE(EXTI_Line));
  
  enablestatus =  EXTI->IMR & EXTI_Line;
  if (((EXTI->PR & EXTI_Line) != (uint32_t)RESET) && (enablestatus != (uint32_t)RESET))
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  return bitstatus;
}

/**
  * @����  ���EXTI��·����λ.
  * @����  EXTI_Line: �����EXTI��·�Ĺ���λ.
  *                    ����������������� EXTI_Linex ��ϣ� x������(0..19).
  * @����  û��
  */
void EXTI_ClearITPendingBit(uint32_t EXTI_Line)
{
  /* ������ */
  assert_param(IS_EXTI_LINE(EXTI_Line));
  
  EXTI->PR = EXTI_Line;
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
