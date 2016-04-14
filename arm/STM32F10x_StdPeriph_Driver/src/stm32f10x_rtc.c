/**
  ******************************************************************************
  * @�ļ�    stm32f10x_rtc.c
  * @����    MCD Ӧ�ó�����
  * @�汾    V3.1.0
  * @����    06/19/2009
  * @����    ����ļ� �ṩ���� RTC �̼�����.
  * @����    ANSON/sweet1985 Email:airanson110@126.com
  * @����    08/07/2009
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
#include "stm32f10x_rtc.h"

/* �������� --------------------------------------------------------------*/
/* ���ö��� --------------------------------------------------------------*/
#define CRL_CNF_Set      ((uint16_t)0x0010)      /*!< Configuration Flag Enable Mask */
#define CRL_CNF_Reset    ((uint16_t)0xFFEF)      /*!< Configuration Flag Disable Mask */
#define RTC_LSB_Mask     ((uint32_t)0x0000FFFF)  /*!< RTC LSB Mask */
#define PRLH_MSB_Mask    ((uint32_t)0x000F0000)  /*!< RTC Prescaler MSB Mask */

/* ���ú� -----------------------------------------------------------------*/
/* ���ñ��� ---------------------------------------------------------------*/
/* ���ú���ԭ�� -----------------------------------------------------------*/
/* ���ú��� ---------------------------------------------------------------*/

/**
  * @����  ʹ�ܻ�ʧ��ָ���� RTC �ж�.
  * @����  RTC_IT: Ҫʹ�ܻ�ʧ��ָ���� RTC �ж�.
  *                �����������������ֵ���������:
  *                RTC_IT_OW:  ����ж�
  *                RTC_IT_ALR: �����ж�
  *                RTC_IT_SEC: ���ж�
  * @����  NewState: ָ�� RTC �жϵ��µ�״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void RTC_ITConfig(uint16_t RTC_IT, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_RTC_IT(RTC_IT));  
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    RTC->CRH |= RTC_IT;
  }
  else
  {
    RTC->CRH &= (uint16_t)~RTC_IT;
  }
}

/**
  * @����  ���� RTC ����ģʽ.
  * @����  û��
  * @����  û��
  */
void RTC_EnterConfigMode(void)
{
  /* ���� CNF ��־��������ģʽ */
  RTC->CRL |= CRL_CNF_Set;
}

/**
  * @����  �˳� RTC ����ģʽ.
  * @����  û��
  * @����  û��
  */
void RTC_ExitConfigMode(void)
{
  /* ��λ CNF ��־�˳�����ģʽ */
  RTC->CRL &= CRL_CNF_Reset;
}

/**
  * @����  ��� RTC ������ֵ.
  * @����  û��
  * @����  RTC ������ֵ.
  */
uint32_t RTC_GetCounter(void)
{
  uint16_t tmp = 0;
  tmp = RTC->CNTL;
  return (((uint32_t)RTC->CNTH << 16 ) | tmp) ;
}

/**
  * @����  ���� RTC ������ֵ.
  * @����  CounterValue: RTC ����������ֵ.
  * @����  û��
  */
void RTC_SetCounter(uint32_t CounterValue)
{ 
  RTC_EnterConfigMode();
  /* ���� RTC ��������λ�� */
  RTC->CNTH = CounterValue >> 16;
  /* ���� RTC ��������λ�� */
  RTC->CNTL = (CounterValue & RTC_LSB_Mask);
  RTC_ExitConfigMode();
}

/**
  * @����  ���� RTC Ԥ��Ƶװ�ڼĴ�����ֵ.
  * @����  PrescalerValue: RTC Ԥ��Ƶװ�ڼĴ�������ֵ.
  * @����  û��
  */
void RTC_SetPrescaler(uint32_t PrescalerValue)
{
  /* ������ */
  assert_param(IS_RTC_PRESCALER(PrescalerValue));
  
  RTC_EnterConfigMode();
  /* ���� RTC Ԥ��Ƶװ�ڼĴ�����λ�� */
  RTC->PRLH = (PrescalerValue & PRLH_MSB_Mask) >> 16;
  /* ���� RTC Ԥ��Ƶװ�ڼĴ�����λ�� */
  RTC->PRLL = (PrescalerValue & RTC_LSB_Mask);
  RTC_ExitConfigMode();
}

/**
  * @����  ���� RTC �����Ĵ�����ֵ.
  * @����  AlarmValue: RTC �����Ĵ�������ֵ.
  * @����  û��
  */
void RTC_SetAlarm(uint32_t AlarmValue)
{  
  RTC_EnterConfigMode();
  /* ���ñ����Ĵ�����λ�� */
  RTC->ALRH = AlarmValue >> 16;
  /* ���ñ����Ĵ�����λ�� */
  RTC->ALRL = (AlarmValue & RTC_LSB_Mask);
  RTC_ExitConfigMode();
}

/**
  * @����  ��ȡ RTC Ԥ��Ƶ��Ƶ���ӵ�ֵ.
  * @����  û��
  * @����  RTC Ԥ��Ƶ��Ƶ���ӵ�ֵ.
  */
uint32_t RTC_GetDivider(void)
{
  uint32_t tmp = 0x00;
  tmp = ((uint32_t)RTC->DIVH & (uint32_t)0x000F) << 16;
  tmp |= RTC->DIVL;
  return tmp;
}

/**
  * @����  �ȴ����һ�ζ�RTC�Ĵ�����д�������.
  * @ע��  �������һ��Ҫ���κζ�RTC�Ĵ���д����֮ǰ����.
  * @����  û��
  * @����  û��
  */
void RTC_WaitForLastTask(void)
{
  /* ѭ��ֱ�� RTOFF ��־��λ */
  while ((RTC->CRL & RTC_FLAG_RTOFF) == (uint16_t)RESET)
  {
  }
}

/**
  * @����  �ȴ�RTC�Ĵ���(RTC_CNT,RTC_ALR��RTC_PRL)��RTC APBʱ��ͬ��.
  * @ע��  �������һ��Ҫ���κζ�APB��λ��APBʱ��ֹͣ���д����֮ǰ����.
  * @����  û��
  * @����  û��
  */
void RTC_WaitForSynchro(void)
{
  /* ��� RSF ��־ */
  RTC->CRL &= (uint16_t)~RTC_FLAG_RSF;
  /* ѭ��ֱ�� RSF ��־��λ */
  while ((RTC->CRL & RTC_FLAG_RSF) == (uint16_t)RESET)
  {
  }
}

/**
  * @����  ���ָ���� RTC ��־�������.
  * @����  RTC_FLAG: ָ��Ҫ���ı�־.
  *                  �����������ѡ�����ֵ֮һ:
  *                  RTC_FLAG_RTOFF: RTC �����رձ�־
  *                  RTC_FLAG_RSF:   �Ĵ���ͬ����־
  *                  RTC_FLAG_OW:    ����жϱ�־
  *                  RTC_FLAG_ALR:   �����жϱ�־
  *                  RTC_FLAG_SEC:   ���жϱ�־
  * @����  RTC_FLAG ����״̬ (SET �� RESET).
  */
FlagStatus RTC_GetFlagStatus(uint16_t RTC_FLAG)
{
  FlagStatus bitstatus = RESET;
  
  /* ������ */
  assert_param(IS_RTC_GET_FLAG(RTC_FLAG)); 
  
  if ((RTC->CRL & RTC_FLAG) != (uint16_t)RESET)
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
  * @����  ��� RTC �����־.
  * @����  RTC_FLAG: ָ��������ı�־.
  *                  �����������������ֵ���������:
  *                  RTC_FLAG_RSF: ͬ���Ĵ�����־. ֻ����SPB��λ��APBʱ��ֹ֮ͣ�����.
  *                  RTC_FLAG_OW:  ����жϱ�־
  *                  RTC_FLAG_ALR: �����жϱ�־
  *                  RTC_FLAG_SEC: ���жϱ�־
  * @����  û��
  */
void RTC_ClearFlag(uint16_t RTC_FLAG)
{
  /* ������ */
  assert_param(IS_RTC_CLEAR_FLAG(RTC_FLAG)); 
    
  /* ��� RTC �����־ */
  RTC->CRL &= (uint16_t)~RTC_FLAG;
}

/**
  * @����  ���ָ���� RTC �жϷ������.
  * @param  RTC_IT: Ҫ����RTC�ж�Դ.
  *                 �����������ѡ�����ֵ֮һ:
  *                 RTC_IT_OW:  ����ж�
  *                 RTC_IT_ALR: �����ж�
  *                 RTC_IT_SEC: ���ж�
  * @����  RTC_IT ����״̬ (SET �� RESET).
  */
ITStatus RTC_GetITStatus(uint16_t RTC_IT)
{
  ITStatus bitstatus = RESET;
  /* ������ */
  assert_param(IS_RTC_GET_IT(RTC_IT)); 
  
  bitstatus = (ITStatus)(RTC->CRL & RTC_IT);
  if (((RTC->CRH & RTC_IT) != (uint16_t)RESET) && (bitstatus != (uint16_t)RESET))
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
  * @����  ��� RTC ���жϴ�����λ.
  * @����  RTC_IT: ������� RTC �жϴ�����λ.
  *                �����������������ֵ���������:
  *                RTC_IT_OW:  ����ж�
  *                RTC_IT_ALR: �����ж�
  *                RTC_IT_SEC: ���ж�
  * @����  û��
  */
void RTC_ClearITPendingBit(uint16_t RTC_IT)
{
  /* ������ */
  assert_param(IS_RTC_IT(RTC_IT));  
  
  /* ��� RTC �����־ */
  RTC->CRL &= (uint16_t)~RTC_IT;
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
