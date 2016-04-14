/**
  ******************************************************************************
  * @�ļ�    stm32f10x_wwdg.c
  * @����    MCD Ӧ�ó�����
  * @�汾    V3.1.0
  * @����    06/19/2009
  * @����    ����ļ� �ṩ���� WWDG �̼�����.
  * @����    ANSON/sweet1985 Email:airanson110@126.com
  * @����    09/07/2009
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_wwdg.h"
#include "stm32f10x_rcc.h"

/* �������� --------------------------------------------------------------*/
/* ���ö��� --------------------------------------------------------------*/

/* ----------- WWDG registers bit address in the alias region ----------- */
#define WWDG_OFFSET       (WWDG_BASE - PERIPH_BASE)

/* Alias word address of EWI bit */
#define CFR_OFFSET        (WWDG_OFFSET + 0x04)
#define EWI_BitNumber     0x09
#define CFR_EWI_BB        (PERIPH_BB_BASE + (CFR_OFFSET * 32) + (EWI_BitNumber * 4))

/* --------------------- WWDG registers bit mask ------------------------ */

/* CR register bit mask */
#define CR_WDGA_Set       ((uint32_t)0x00000080)

/* CFR register bit mask */
#define CFR_WDGTB_Mask    ((uint32_t)0xFFFFFE7F)
#define CFR_W_Mask        ((uint32_t)0xFFFFFF80)
#define BIT_Mask          ((uint8_t)0x7F)

/* ���ú� -------------------------------------------------------------*/
/* ���ñ��� -----------------------------------------------------------*/
/* ���ú���ԭ�� -------------------------------------------------------*/
/* ���ú��� -----------------------------------------------------------*/

/**
  * @����  ������ WWDG �Ĵ�������Ϊȱʡֵ.
  * @����  û��
  * @����  û��
  */
void WWDG_DeInit(void)
{
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_WWDG, ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_WWDG, DISABLE);
}

/**
  * @����  ���� WWDG Ԥ��Ƶֵ.
  * @����  WWDG_Prescaler: ָ�� WWDG Ԥ��Ƶֵ.
  *                        ������������������ֵ֮һ:
  *                        WWDG_Prescaler_1: WWDG ������ʱ�� = (PCLK1/4096)/1
  *                        WWDG_Prescaler_2: WWDG ������ʱ�� = (PCLK1/4096)/2
  *                        WWDG_Prescaler_4: WWDG ������ʱ�� = (PCLK1/4096)/4
  *                        WWDG_Prescaler_8: WWDG ������ʱ�� = (PCLK1/4096)/8
  * @����  û��
  */
void WWDG_SetPrescaler(uint32_t WWDG_Prescaler)
{
  uint32_t tmpreg = 0;
  /* ������ */
  assert_param(IS_WWDG_PRESCALER(WWDG_Prescaler));
  /* ��� WDGTB[1:0] λ */
  tmpreg = WWDG->CFR & CFR_WDGTB_Mask;
  /* ���� WDGTB[1:0] λ������ WWDG_Prescaler ��ֵ */
  tmpreg |= WWDG_Prescaler;
  /* �洢��ֵ */
  WWDG->CFR = tmpreg;
}

/**
  * @����  ���� WWDG ����ֵ.
  * @����  ָ��Ҫ�͵��������ȽϵĴ���ֵ.
  *        �����������ֵ����С��0x80.
  * @����  û��
  */
void WWDG_SetWindowValue(uint8_t WindowValue)
{
  __IO uint32_t tmpreg = 0;

  /* ������ */
  assert_param(IS_WWDG_WINDOW_VALUE(WindowValue));
  /* ��� W[6:0] λ */

  tmpreg = WWDG->CFR & CFR_W_Mask;

  /* ���մ���ֵ��ֵ���� W[6:0] λ */
  tmpreg |= WindowValue & (uint32_t) BIT_Mask;

  /* �洢��ֵ */
  WWDG->CFR = tmpreg;
}

/**
  * @����  ʹ�� WWDG ���ڻ����ж� (EWI).
  * @����  û��
  * @����  û��
  */
void WWDG_EnableIT(void)
{
  *(__IO uint32_t *) CFR_EWI_BB = (uint32_t)ENABLE;
}

/**
  * @����  ���� WWDG ������ֵ.
  * @����  Counter: ָ�����Ź�������ֵ.
  *                 �ò���ȡֵ������0x40��0x7F֮��.
  * @����  û��
  */
void WWDG_SetCounter(uint8_t Counter)
{
  /* ������ */
  assert_param(IS_WWDG_COUNTER(Counter));
  /* Ϊ���ü�������ֵд T[6:0]λ������д0��WDG Aλû������ */
  WWDG->CR = Counter & BIT_Mask;
}

/**
  * @����  ʹ�� WWDG �������������ֵ.                  
  * @����  Counter: ָ�����Ź������������ֵ.
  *                 �������������0x40��0x7F֮���һ����.
  * @����  û��
  */
void WWDG_Enable(uint8_t Counter)
{
  /* ������ */
  assert_param(IS_WWDG_COUNTER(Counter));
  WWDG->CR = CR_WDGA_Set | Counter;
}

/**
  * @����  ��� WWDG ���ڻ����жϱ�־λ���������.
  * @����  û��
  * @����  ���ڻ����жϱ�־λ����״̬ (SET �� RESET)
  */
FlagStatus WWDG_GetFlagStatus(void)
{
  return (FlagStatus)(WWDG->SR);
}

/**
  * @����  ������ڻ����жϱ�־λ.
  * @����  û��
  * @����  û��
  */
void WWDG_ClearFlag(void)
{
  WWDG->SR = (uint32_t)RESET;
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
