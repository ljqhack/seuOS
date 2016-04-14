/**
  ******************************************************************************
  * @�ļ�    stm32f10x_bkp.c
  * @����    MCD Ӧ�ó�����
  * @�汾    V3.1.0
  * @����    06/19/2009
  * @����    ����ļ��ṩ������ BKP �̼�����.
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
#include "stm32f10x_bkp.h"
#include "stm32f10x_rcc.h"

/* �������� --------------------------------------------------------------*/
/* ���ö��� --------------------------------------------------------------*/

/* ------------ BKP registers bit address in the alias region --------------- */
#define BKP_OFFSET        (BKP_BASE - PERIPH_BASE)

/* --- CR Register ----*/

/* Alias word address of TPAL bit */
#define CR_OFFSET         (BKP_OFFSET + 0x30)
#define TPAL_BitNumber    0x01
#define CR_TPAL_BB        (PERIPH_BB_BASE + (CR_OFFSET * 32) + (TPAL_BitNumber * 4))

/* Alias word address of TPE bit */
#define TPE_BitNumber     0x00
#define CR_TPE_BB         (PERIPH_BB_BASE + (CR_OFFSET * 32) + (TPE_BitNumber * 4))

/* --- CSR Register ---*/

/* Alias word address of TPIE bit */
#define CSR_OFFSET        (BKP_OFFSET + 0x34)
#define TPIE_BitNumber    0x02
#define CSR_TPIE_BB       (PERIPH_BB_BASE + (CSR_OFFSET * 32) + (TPIE_BitNumber * 4))

/* Alias word address of TIF bit */
#define TIF_BitNumber     0x09
#define CSR_TIF_BB        (PERIPH_BB_BASE + (CSR_OFFSET * 32) + (TIF_BitNumber * 4))

/* Alias word address of TEF bit */
#define TEF_BitNumber     0x08
#define CSR_TEF_BB        (PERIPH_BB_BASE + (CSR_OFFSET * 32) + (TEF_BitNumber * 4))

/* ---------------------- BKP registers bit mask ------------------------ */

/* RTCCR register bit mask */
#define RTCCR_CAL_Mask    ((uint16_t)0xFF80)
#define RTCCR_Mask        ((uint16_t)0xFC7F)

/* CSR register bit mask */
#define CSR_CTE_Set       ((uint16_t)0x0001)
#define CSR_CTI_Set       ((uint16_t)0x0002)

/* ���ú� -------------------------------------------------------------*/
/* ���ñ��� -----------------------------------------------------------*/
/* ���ú���ԭ�� -------------------------------------------------------*/
/* ���ú��� -----------------------------------------------------------*/

/**
  * @����  �� BKP ����Ĵ�����λ�����ǵ�Ĭ��ֵ.
  * @����  û��
  * @����  û��
  */
void BKP_DeInit(void)
{
  RCC_BackupResetCmd(ENABLE);
  RCC_BackupResetCmd(DISABLE);
}

/**
  * @����  ����������ܽŵ���Ч��ƽ.
  * @����  BKP_TamperPinLevel: ������ܽŵ���Ч��ƽ.
  *                            �����������ȡ�����е�һ��:
  *                            BKP_TamperPinLevel_High: ������ܽŸߵ�ƽ��Ч
  *                            BKP_TamperPinLevel_Low:  ������ܽŵ͵�ƽ��Ч
  * @����  û��
  */
void BKP_TamperPinLevelConfig(uint16_t BKP_TamperPinLevel)
{
  /* ������ */
  assert_param(IS_BKP_TAMPER_PIN_LEVEL(BKP_TamperPinLevel));
  *(__IO uint32_t *) CR_TPAL_BB = BKP_TamperPinLevel;
}

/**
  * @����  ʹ�ܻ�ʧ��������ܽ�.
  * @����  NewState: ������ܽŵ���״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void BKP_TamperPinCmd(FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CR_TPE_BB = (uint32_t)NewState;
}

/**
  * @����  ʹ�ܻ�ʧ��������ܽŵ��ж�.
  * @����  NewState: ������ܽŵ��жϵ���״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void BKP_ITConfig(FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CSR_TPIE_BB = (uint32_t)NewState;
}

/**
  * @����  ѡ����������ܽ�������� RTC ʱ��Դ.
  * @����  BKP_RTCOutputSource: ָ�� RTC �������Դ.
  *                             �����������ȡ�����е�һ��:
  *            BKP_RTCOutputSource_None: ������ܽ���û��ʵʱʱ�����.
  *            BKP_RTCOutputSource_CalibClock: ������ܽ������ʵʱʱ�Ӹ澯�����������ʵʱʱ�ӵ���Ƶ�ʳ���64.
  *            BKP_RTCOutputSource_Alarm: ������ܽ������ʵʱʱ�Ӹ澯����.
  *            BKP_RTCOutputSource_Second: ������ܽ������ʵʱʱ��������.  
  * @����  û��
  */
void BKP_RTCOutputConfig(uint16_t BKP_RTCOutputSource)
{
  uint16_t tmpreg = 0;
  /* ������ */
  assert_param(IS_BKP_RTC_OUTPUT_SOURCE(BKP_RTCOutputSource));
  tmpreg = BKP->RTCCR;
  /* ��� CCO, ASOE �� ASOS λ */
  tmpreg &= RTCCR_Mask;
  
  /* ���� CCO, ASOE �� ASOS λ������ BKP_RTCOutputSource ��ֵ */
  tmpreg |= BKP_RTCOutputSource;
  /* �洢��ֵ */
  BKP->RTCCR = tmpreg;
}

/**
  * @����  ���� RTC ʱ��У׼ֵ.
  * @����  CalibrationValue: ָ�� RTC ʱ�ӵ�У׼ֵ.
  *                          ������������� 0 �� 0x7F ֮��.
  * @����  û��
  */
void BKP_SetRTCCalibrationValue(uint8_t CalibrationValue)
{
  uint16_t tmpreg = 0;
  /* ������ */
  assert_param(IS_BKP_CALIBRATION_VALUE(CalibrationValue));
  tmpreg = BKP->RTCCR;
  /* ��� CAL[6:0] λ */
  tmpreg &= RTCCR_CAL_Mask;
  /* ���� CAL[6:0] λ������ CalibrationValue ��ֵ */
  tmpreg |= CalibrationValue;
  /* �洢��ֵ */
  BKP->RTCCR = tmpreg;
}

/**
  * @����  ��ָ���ĺ󱸼Ĵ�����д���û���������.
  * @����  BKP_DR: ָ���ĺ󱸼Ĵ���.
  *                ������� BKP_DRx ��x����ȡ[1, 42]
  * @����  Data: ��Ҫд�������
  * @����  û��
  */
void BKP_WriteBackupRegister(uint16_t BKP_DR, uint16_t Data)
{
  __IO uint32_t tmp = 0;

  /* ������ */
  assert_param(IS_BKP_DR(BKP_DR));

  tmp = (uint32_t)BKP_BASE; 
  tmp += BKP_DR;

  *(__IO uint32_t *) tmp = Data;
}

/**
  * @����  ��ָ�������ݱ��ݼĴ����ж�������.
  * @����  BKP_DR: ָ�������ݱ��ݼĴ���.
  *                ������� BKP_DRx ��x����ȡ[1, 42]
  * @����  ָ�������ݱ��ݼĴ���������
  */
uint16_t BKP_ReadBackupRegister(uint16_t BKP_DR)
{
  __IO uint32_t tmp = 0;

  /* ������ */
  assert_param(IS_BKP_DR(BKP_DR));

  tmp = (uint32_t)BKP_BASE; 
  tmp += BKP_DR;

  return (*(__IO uint16_t *) tmp);
}

/**
  * @����  ���������ܽ��¼��ı�־λ��������� ��.
  * @����  û��
  * @����  ������ܽ��¼��ı�־����״̬ (SET �� RESET).
  */
FlagStatus BKP_GetFlagStatus(void)
{
  return (FlagStatus)(*(__IO uint32_t *) CSR_TEF_BB);
}

/**
  * @����  ���������ܽ��¼��Ĵ������־λ.
  * @����  û��
  * @����  û��
  */
void BKP_ClearFlag(void)
{
  /* ���� CTE λ�����������ܽ��¼��Ĵ������־λ */
  BKP->CSR |= CSR_CTE_Set;
}

/**
  * @����  ����������жϷ������.
  * @����  û��
  * @����  �������жϱ�־λ����״̬ (SET �� RESET).
  */
ITStatus BKP_GetITStatus(void)
{
  return (ITStatus)(*(__IO uint32_t *) CSR_TIF_BB);
}

/**
  * @����  ������������жϵĴ�����λ.
  * @����  û��
  * @����  û��
  */
void BKP_ClearITPendingBit(void)
{
  /* ���� CTI λ�� ������������жϵĴ�����λ */
  BKP->CSR |= CSR_CTI_Set;
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
