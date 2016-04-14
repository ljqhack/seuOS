/**
  ******************************************************************************
  * @file    stm32f10x_pwr.c
  * @author  MCD Ӧ�ó�����
  * @version V3.1.0
  * @date    06/19/2009
  * @brief   ����ļ� �ṩ���� PWR �̼�����.
  * @����    ANSON/sweet1985 Email:airanson110@126.com
  * @����    07/07/2009
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
#include "stm32f10x_pwr.h"
#include "stm32f10x_rcc.h"

/* �������� ------------------------------------------------------------*/
/* ���ö��� ------------------------------------------------------------*/

/* --------- PWR registers bit address in the alias region ------------ */
#define PWR_OFFSET               (PWR_BASE - PERIPH_BASE)

/* --- CR �Ĵ��� ---*/

/* Alias word address of DBP bit */
#define CR_OFFSET                (PWR_OFFSET + 0x00)
#define DBP_BitNumber            0x08
#define CR_DBP_BB                (PERIPH_BB_BASE + (CR_OFFSET * 32) + (DBP_BitNumber * 4))

/* Alias word address of PVDE bit */
#define PVDE_BitNumber           0x04
#define CR_PVDE_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (PVDE_BitNumber * 4))

/* --- CSR �Ĵ��� ---*/

/* Alias word address of EWUP bit */
#define CSR_OFFSET               (PWR_OFFSET + 0x04)
#define EWUP_BitNumber           0x08
#define CSR_EWUP_BB              (PERIPH_BB_BASE + (CSR_OFFSET * 32) + (EWUP_BitNumber * 4))

/* ------------------ PWR �Ĵ���λ���� ------------------------ */

/* CR register bit mask */
#define CR_PDDS_Set              ((uint32_t)0x00000002)
#define CR_DS_Mask               ((uint32_t)0xFFFFFFFC)
#define CR_CWUF_Set              ((uint32_t)0x00000004)
#define CR_PLS_Mask              ((uint32_t)0xFFFFFF1F)

/* --------- Cortex ϵͳ���ƼĴ���λ���� ---------------------- */

/* Cortex ϵͳ���ƼĴ�����ַ */
#define SCB_SysCtrl              ((uint32_t)0xE000ED10)

/* SLEEPDEEP λ���� */
#define SysCtrl_SLEEPDEEP_Set    ((uint32_t)0x00000004)

/* ���ú� -------------------------------------------------------------*/
/* ���ñ��� -----------------------------------------------------------*/
/* ���ú���ԭ�� -------------------------------------------------------*/
/* ���ú��� -----------------------------------------------------------*/

/**
  * @����  ��λ PWR ��Χ�Ĵ���ΪĬ�ϸ�λֵ.
  * @����  û��
  * @����  û��
  */
void PWR_DeInit(void)
{
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_PWR, ENABLE);
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_PWR, DISABLE);
}

/**
  * @����  ʹ�ܻ���ʧ�� RTC �ͺ󱸼Ĵ�������.
  * @����  NewState: ���� RTC �ͱ��ݼĴ�������״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void PWR_BackupAccessCmd(FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CR_DBP_BB = (uint32_t)NewState;
}

/**
  * @����  ʹ�ܻ���ʧ�ܿɱ�̵�ѹ̽���� (PVD).
  * @����  NewState: PVD ����״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void PWR_PVDCmd(FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CR_PVDE_BB = (uint32_t)NewState;
}

/**
  * @����  ���� PVD ��̽���ѹ��ֵ (PVD).
  * @����  PWR_PVDLevel: PVD ��̽���ѹ��ֵ
  *                      �����������������ֵ�е�һ��:
  *          PWR_PVDLevel_2V2: PVD ̽���ѹ��ֵ 2.2V
  *          PWR_PVDLevel_2V3: PVD ̽���ѹ��ֵ 2.3V
  *          PWR_PVDLevel_2V4: PVD ̽���ѹ��ֵ 2.4V
  *          PWR_PVDLevel_2V5: PVD ̽���ѹ��ֵ 2.5V
  *          PWR_PVDLevel_2V6: PVD ̽���ѹ��ֵ 2.6V
  *          PWR_PVDLevel_2V7: PVD ̽���ѹ��ֵ 2.7V
  *          PWR_PVDLevel_2V8: PVD ̽���ѹ��ֵ 2.8V
  *          PWR_PVDLevel_2V9: PVD ̽���ѹ��ֵ 2.9V
  * @����  û��
  */
void PWR_PVDLevelConfig(uint32_t PWR_PVDLevel)
{
  uint32_t tmpreg = 0;
  /* ������ */
  assert_param(IS_PWR_PVD_LEVEL(PWR_PVDLevel));
  tmpreg = PWR->CR;
  /* ��� PLS[7:5] λ */
  tmpreg &= CR_PLS_Mask;
  /* ���� PLS[7:5] λ ���� PWR_PVDLevel ��ֵ */
  tmpreg |= PWR_PVDLevel;
  /* �洢��ֵ */
  PWR->CR = tmpreg;
}

/**
  * @����  ʹ�ܻ���ʧ�ܻ��ѹܽŹ���.
  * @����  NewState: ���ѹܽŹ��ܵ���״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void PWR_WakeUpPinCmd(FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CSR_EWUP_BB = (uint32_t)NewState;
}

/**
  * @����  ����ֹͣ(STOP)ģʽ.
  * @����  PWR_Regulator: ��ѹת������ֹͣģʽ�µ�״̬.
  *                       �����������������ֵ�е�һ��:
  *                       PWR_Regulator_ON: ֹͣģʽ�µ�ѹת���� ON
  *                       PWR_Regulator_LowPower: ֹͣģʽ�µ�ѹת��������͹���ģʽ
  * @����  PWR_STOPEntry: ѡ��ʹ��ָ�� WFE ���� WFI ������ֹͣģʽ.
  *                       �����������������ֵ�е�һ��:
  *                       PWR_STOPEntry_WFI: ʹ��ָ�� WFI ������ֹͣģʽ
  *                       PWR_STOPEntry_WFE: ʹ��ָ�� WFE ������ֹͣģʽ
  * @����  û��
  */
void PWR_EnterSTOPMode(uint32_t PWR_Regulator, uint8_t PWR_STOPEntry)
{
  uint32_t tmpreg = 0;
  /* ������ */
  assert_param(IS_PWR_REGULATOR(PWR_Regulator));
  assert_param(IS_PWR_STOP_ENTRY(PWR_STOPEntry));
  
  /* ��ֹͣģʽѡ��У׼״̬ ---------------------------------*/
  tmpreg = PWR->CR;
  /* �� PDDS �� LPDS λ */
  tmpreg &= CR_DS_Mask;
  /* ���� PWR_Regulator ֵ��λ LPDS */
  tmpreg |= PWR_Regulator;
  /* �����µ�ֵ */
  PWR->CR = tmpreg;
  /* ���� SLEEPDEEP λ���� Cortex ϵͳ���ƼĴ��� */
  *(__IO uint32_t *) SCB_SysCtrl |= SysCtrl_SLEEPDEEP_Set;
  
  /* ѡ��ֹͣģʽ���� --------------------------------------------------*/
  if(PWR_STOPEntry == PWR_STOPEntry_WFI)
  {   
    /* ����ȴ��ж� */
    __WFI();
  }
  else
  {
    /* ����ȴ��¼� */
    __WFE();
  }
}

/**
  * @����  �������(STANDBY)ģʽ.
  * @����  û��
  * @����  û��
  */
void PWR_EnterSTANDBYMode(void)
{
  /* ��� Wake-up ��־ */
  PWR->CR |= CR_CWUF_Set;
  /* ѡ�� STANDBY ģʽ */
  PWR->CR |= CR_PDDS_Set;
  /* ��λ Cortex ϵͳ�Ŀ��ƼĴ��� SLEEPDEEP λ */
  *(__IO uint32_t *) SCB_SysCtrl |= SysCtrl_SLEEPDEEP_Set;
/* ���ѡ�񳣳���ȷ���洢��������� */
#if defined ( __CC_ARM   )
  __force_stores();
#endif
  /* ����ȴ��ж� */
  __WFI();
}

/**
  * @����  ���ָ���� PWR ��־λ�������.
  * @����  PWR_FLAG: Ҫ���ı�־.
  *                  ������������������ֵ֮һ:
  *                  PWR_FLAG_WU: ���ѱ�־
  *                  PWR_FLAG_SB: ������־
  *                  PWR_FLAG_PVDO: PVD ���
  * @����  PWR_FLAG ����״̬ (SET or RESET).
  */
FlagStatus PWR_GetFlagStatus(uint32_t PWR_FLAG)
{
  FlagStatus bitstatus = RESET;
  /* ������ */
  assert_param(IS_PWR_GET_FLAG(PWR_FLAG));
  
  if ((PWR->CSR & PWR_FLAG) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  /* ����״̬��־ */
  return bitstatus;
}

/**
  * @����  ��� PWR �����־λ.
  * @����  PWR_FLAG: Ҫ��յı�־.
  *                  ������������������ֵ֮һ:
  *                  PWR_FLAG_WU: ���ѱ�־
  *                  PWR_FLAG_SB: ������־
  * @����  û��
  */
void PWR_ClearFlag(uint32_t PWR_FLAG)
{
  /* ������ */
  assert_param(IS_PWR_CLEAR_FLAG(PWR_FLAG));
         
  PWR->CR |=  PWR_FLAG << 2;
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
