/**
  ******************************************************************************
  * @�ļ�    misc.c
  * @����    MCD Ӧ�ó�����
  * @�汾    V3.1.0
  * @����    06/19/2009
  * @����    ����ļ��ṩ�����������Ĺ̼����� (׷�� CMSIS<΢����������ӿڱ�׼> ����).
  * @����    ANSON/sweet1985 Email:airanson110@126.com
  * @����    02/07/2009
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
#include "misc.h"

/* �������� --------------------------------------------------------------*/
/* ���ö��� --------------------------------------------------------------*/

#define AIRCR_VECTKEY_MASK    ((uint32_t)0x05FA0000)

/* ���ú� ------------------------------------------------------------*/
/* ���ñ��� ----------------------------------------------------------*/

/* ���ú���ԭ�� ------------------------------------------------------*/
/* ���ú��� ----------------------------------------------------------*/


/**
  * @����  �������ȼ����飺��ռ���ȼ��ʹ����ȼ�.
  * @����  NVIC_PriorityGroup: ���ȼ�����λ����. 
  *                            �����������ȡ����ֵ�е�һ��:
  *          NVIC_PriorityGroup_0: ��ռ���ȼ�0λ
  *                                �����ȼ�4λ
  *          NVIC_PriorityGroup_1: ��ռ���ȼ�1λ
  *                                �����ȼ�3λ
  *          NVIC_PriorityGroup_2: ��ռ���ȼ�2λ
  *                                �����ȼ�2λ
  *          NVIC_PriorityGroup_3: ��ռ���ȼ�3λ
  *                                �����ȼ�1λ
  *          NVIC_PriorityGroup_4: ��ռ���ȼ�4λ
  *                                �����ȼ�0λ
  * @����  û��
  */
void NVIC_PriorityGroupConfig(uint32_t NVIC_PriorityGroup)
{
  /* ������ */
  assert_param(IS_NVIC_PRIORITY_GROUP(NVIC_PriorityGroup));
  
  /* ����NVIC_PriorityGroup��ֵ����PRIGROUP[10:8]��λ */
  SCB->AIRCR = AIRCR_VECTKEY_MASK | NVIC_PriorityGroup;
}

/**
  * @����  ͨ��NVIC_InitStruct�е�ָ����������ʼ����Χ�豸.
  * @����  NVIC_InitStruct: һ��ָ�����ָ�� NVIC ��Χ�豸������Ϣ�� NVIC_InitTypeDef �ṹ��ָ��.
  * @����  û��
  */
void NVIC_Init(NVIC_InitTypeDef* NVIC_InitStruct)
{
  uint32_t tmppriority = 0x00, tmppre = 0x00, tmpsub = 0x0F;
  
  /* ������ */
  assert_param(IS_FUNCTIONAL_STATE(NVIC_InitStruct->NVIC_IRQChannelCmd));
  assert_param(IS_NVIC_PREEMPTION_PRIORITY(NVIC_InitStruct->NVIC_IRQChannelPreemptionPriority));  
  assert_param(IS_NVIC_SUB_PRIORITY(NVIC_InitStruct->NVIC_IRQChannelSubPriority));
    
  if (NVIC_InitStruct->NVIC_IRQChannelCmd != DISABLE)
  {
    /* ������� IRQ(�ж�����) ����Ȩ --------------------------------*/    
    tmppriority = (0x700 - ((SCB->AIRCR) & (uint32_t)0x700))>> 0x08;
    tmppre = (0x4 - tmppriority);
    tmpsub = tmpsub >> tmppriority;

    tmppriority = (uint32_t)NVIC_InitStruct->NVIC_IRQChannelPreemptionPriority << tmppre;
    tmppriority |=  NVIC_InitStruct->NVIC_IRQChannelSubPriority & tmpsub;
    tmppriority = tmppriority << 0x04;
        
    NVIC->IP[NVIC_InitStruct->NVIC_IRQChannel] = tmppriority;
    
    /* ʹ��ѡ��� IRQ(�ж�����) ͨ�� --------------------------------------*/
    NVIC->ISER[NVIC_InitStruct->NVIC_IRQChannel >> 0x05] =
      (uint32_t)0x01 << (NVIC_InitStruct->NVIC_IRQChannel & (uint8_t)0x1F);
  }
  else
  {
    /* ʧ��ѡ��� IRQ(�ж�����) ͨ�� -------------------------------------*/
    NVIC->ICER[NVIC_InitStruct->NVIC_IRQChannel >> 0x05] =
      (uint32_t)0x01 << (NVIC_InitStruct->NVIC_IRQChannel & (uint8_t)0x1F);
  }
}

/**
  * @����  �����������λ�ú�ƫ����.
  * @����  NVIC_VectTab: ָ���������� RAM �� FLASH .
  *                      �����������ȡ����ֵ�е�һ��:
  *                      NVIC_VectTab_RAM
  *                      NVIC_VectTab_FLASH
  * @����  Offset: ���������ַ��ƫ����. ���ֵ������ 0x100 ��������.
  * @����  û��
  */
void NVIC_SetVectorTable(uint32_t NVIC_VectTab, uint32_t Offset)
{ 
  /* ������ */
  assert_param(IS_NVIC_VECTTAB(NVIC_VectTab));
  assert_param(IS_NVIC_OFFSET(Offset));  
   
  SCB->VTOR = NVIC_VectTab | (Offset & (uint32_t)0x1FFFFF80);
}

/**
  * @����  ѡ��ϵͳ����͹���ģʽ������.
  * @����  LowPowerMode: ϵͳΪ����͹���ģʽ����ģʽ.
  *                      �����������ȡ�����ֵ֮һ:
  *                      NVIC_LP_SEVONPEND
  *                      NVIC_LP_SLEEPDEEP
  *                      NVIC_LP_SLEEPONEXIT
  * @����  NewState: LP ��������״̬.�������������: ENABLE �� DISABLE.
  * @����  û��
  */
void NVIC_SystemLPConfig(uint8_t LowPowerMode, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_NVIC_LP(LowPowerMode));
  assert_param(IS_FUNCTIONAL_STATE(NewState));  
  
  if (NewState != DISABLE)
  {
    SCB->SCR |= LowPowerMode;
  }
  else
  {
    SCB->SCR &= (uint32_t)(~(uint32_t)LowPowerMode);
  }
}

/**
  * @����  ���� SysTick ʱ��Դ.
  * @����  SysTick_CLKSource: ָ�� SysTick ʱ��Դ.
  *                           �����������ȡ�����ֵ֮һ:
  *          SysTick_CLKSource_HCLK_Div8: SysTick ʱ��ԴΪAHBʱ�ӵ�1/8.
  *          SysTick_CLKSource_HCLK:      SysTick ʱ��ԴΪAHBʱ��.
  * @����  û��
  */
void SysTick_CLKSourceConfig(uint32_t SysTick_CLKSource)
{
  /* ������ */
  assert_param(IS_SYSTICK_CLK_SOURCE(SysTick_CLKSource));
  if (SysTick_CLKSource == SysTick_CLKSource_HCLK)
  {
    SysTick->CTRL |= SysTick_CLKSource_HCLK;
  }
  else
  {
    SysTick->CTRL &= SysTick_CLKSource_HCLK_Div8;
  }
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
