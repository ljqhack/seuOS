/**
  ******************************************************************************
  * @�ļ�    stm32f10x_iwdg.c
  * @����    MCD Ӧ�ó�����
  * @�汾    V3.1.0
  * @����    06/19/2009
  * @����    ����ļ� �ṩ���� IWDG �̼�����.
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

/* ���� ------------------------------------------------------------------*/
#include "stm32f10x_iwdg.h"

/* �������� --------------------------------------------------------------*/
/* ���ö��� --------------------------------------------------------------*/

/* ---------------------- IWDG registers bit mask ----------------------------*/

/* KR register bit mask */
#define KR_KEY_Reload    ((uint16_t)0xAAAA)
#define KR_KEY_Enable    ((uint16_t)0xCCCC)

/* ���ú� -------------------------------------------------------------*/
/* ���ñ��� -----------------------------------------------------------*/
/* ���ú���ԭ�� -------------------------------------------------------*/
/* ���ú��� -----------------------------------------------------------*/


/**
  * @����  ʹ�ܻ���ʧ�ܶԼĴ��� IWDG_PR �� IWDG_RLR ��д����.
  * @����  IWDG_WriteAccess: �� IWDG_PR �� IWDG_RLR �Ĵ�����д���ʵ��µ�״̬.
  *                          ������������������ֵ֮һ:
  *                          IWDG_WriteAccess_Enable:  ʹ�ܶԼĴ��� IWDG_PR �� IWDG_RLR ��д����
  *                          IWDG_WriteAccess_Disable: ʧ�ܶԼĴ��� IWDG_PR �� IWDG_RLR ��д����
  * @����  û��
  */
void IWDG_WriteAccessCmd(uint16_t IWDG_WriteAccess)
{
  /* ������ */
  assert_param(IS_IWDG_WRITE_ACCESS(IWDG_WriteAccess));
  IWDG->KR = IWDG_WriteAccess;
}

/**
  * @����  ���� IWDG Ԥ��Ƶֵ.
  * @����  IWDG_Prescaler: ָ�� IWDG Ԥ��Ƶֵ.
  *                        ������������������ֵ֮һ:
  *                        IWDG_Prescaler_4:   ����IWDGԤ��ƵֵΪ 4
  *                        IWDG_Prescaler_8:   ����IWDGԤ��ƵֵΪ 8
  *                        IWDG_Prescaler_16:  ����IWDGԤ��ƵֵΪ 16
  *                        IWDG_Prescaler_32:  ����IWDGԤ��ƵֵΪ 32
  *                        IWDG_Prescaler_64:  ����IWDGԤ��ƵֵΪ 64
  *                        IWDG_Prescaler_128: ����IWDGԤ��ƵֵΪ 128
  *                        IWDG_Prescaler_256: ����IWDGԤ��ƵֵΪ 256
  * @����  û��
  */
void IWDG_SetPrescaler(uint8_t IWDG_Prescaler)
{
  /* ������ */
  assert_param(IS_IWDG_PRESCALER(IWDG_Prescaler));
  IWDG->PR = IWDG_Prescaler;
}

/**
  * @����  ���� IWDG ��װ��ֵ.
  * @����  Reload: ָ�� IWDG ��װ��ֵ.
  *                �������һ������'0'��'0x0FFF'֮�����.
  * @����  û��
  */
void IWDG_SetReload(uint16_t Reload)
{
  /* ������ */
  assert_param(IS_IWDG_RELOAD(Reload));
  IWDG->RLR = Reload;
}

/**
  * @����  ���� IWDG ��װ�ؼĴ�����ֵ��װ�� IWDG ������
  *        (�� IWDG_PR �� IWDG_RLR �Ĵ�����д���ʱ�����).
  * @����  û��
  * @����  û��
  */
void IWDG_ReloadCounter(void)
{
  IWDG->KR = KR_KEY_Reload;
}

/**
  * @����  ʹ�� IWDG (�� IWDG_PR �� IWDG_RLR �Ĵ�����д���ʱ�����).
  * @����  û��
  * @����  û��
  */
void IWDG_Enable(void)
{
  IWDG->KR = KR_KEY_Enable;
}

/**
  * @����  ���ָ���� IWDG ��־λ���������.
  * @����  IWDG_FLAG: Ҫ���ı�־.
  *                   ������������������ֵ֮һ:
  *                   IWDG_FLAG_PVU: Ԥ��Ƶ����ֵ������
  *                   IWDG_FLAG_RVU: ����ֵ������
  * @����  IWDG_FLAG ����״̬ (SET �� RESET).
  */
FlagStatus IWDG_GetFlagStatus(uint16_t IWDG_FLAG)
{
  FlagStatus bitstatus = RESET;
  /* ������ */
  assert_param(IS_IWDG_FLAG(IWDG_FLAG));
  if ((IWDG->SR & IWDG_FLAG) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  /* ���ر�־״̬ */
  return bitstatus;
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
