/**
  ******************************************************************************
  * @�ļ�    stm32f10x_dac.c
  * @����    MCD Ӧ�ó�����
  * @�汾    V3.1.0
  * @����    06/19/2009
  * @����    ����ļ� �ṩ���� DAC �̼�����.
  * @����    ANSON/sweet1985 Email:airanson110@126.com
  * @����    18/07/2009
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
#include "stm32f10x_dac.h"
#include "stm32f10x_rcc.h"

/* �������� --------------------------------------------------------------*/
/* ���ö��� --------------------------------------------------------------*/

/* DAC EN mask */
#define CR_EN_Set                  ((uint32_t)0x00000001)

/* DAC DMAEN mask */
#define CR_DMAEN_Set               ((uint32_t)0x00001000)

/* CR register Mask */
#define CR_CLEAR_Mask              ((uint32_t)0x00000FFE)

/* DAC SWTRIG mask */
#define SWTRIGR_SWTRIG_Set         ((uint32_t)0x00000001)

/* DAC Dual Channels SWTRIG masks */
#define DUAL_SWTRIG_Set            ((uint32_t)0x00000003)
#define DUAL_SWTRIG_Reset          ((uint32_t)0xFFFFFFFC)

/* DHR registers offsets */
#define DHR12R1_Offset             ((uint32_t)0x00000008)
#define DHR12R2_Offset             ((uint32_t)0x00000014)
#define DHR12RD_Offset             ((uint32_t)0x00000020)

/* DOR register offset */
#define DOR_Offset                 ((uint32_t)0x0000002C)

/* ���ú� -------------------------------------------------------------*/
/* ���ñ��� -----------------------------------------------------------*/
/* ���ú���ԭ�� -------------------------------------------------------*/
/* ���ú��� -----------------------------------------------------------*/

/**
  * @����  �� DAC �����ļĴ�����λΪĬ��ֵ
  * @����  û��
  * @����  û��
  */
void DAC_DeInit(void)
{
  /* ʹ�� DAC ��λ״̬ */
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_DAC, ENABLE);
  /* �ͷ� DAC ��λ״̬ */
  RCC_APB1PeriphResetCmd(RCC_APB1Periph_DAC, DISABLE);
}

/**
  * @����  ���� DAC_InitStruct ָ���Ĳ�����ʼ�� DAC �ⲿ�豸.
  * @����  DAC_Channel: ѡ���� DAC ͨ��. 
  *          �����������������ֵ֮һ:
  *          DAC_Channel_1: ѡ�� DAC ͨ��1
  *          DAC_Channel_2: ѡ�� DAC ͨ��2
  * @����  DAC_InitStruct: ָ�������ָ�� DAC ͨ��������Ϣ�� DAC_InitTypeDef �ṹָ��.
  * @����  û��
  */
void DAC_Init(uint32_t DAC_Channel, DAC_InitTypeDef* DAC_InitStruct)
{
  uint32_t tmpreg1 = 0, tmpreg2 = 0;
  /* ������ */
  assert_param(IS_DAC_TRIGGER(DAC_InitStruct->DAC_Trigger));
  assert_param(IS_DAC_GENERATE_WAVE(DAC_InitStruct->DAC_WaveGeneration));
  assert_param(IS_DAC_LFSR_UNMASK_TRIANGLE_AMPLITUDE(DAC_InitStruct->DAC_LFSRUnmask_TriangleAmplitude));
  assert_param(IS_DAC_OUTPUT_BUFFER_STATE(DAC_InitStruct->DAC_OutputBuffer));
/*---------------------------- DAC CR Configuration --------------------------*/
  /* �õ� DAC CR ��ֵ */
  tmpreg1 = DAC->CR;
  /* ��� BOFFx, TENx, TSELx, WAVEx �� MAMPx λ */
  tmpreg1 &= ~(CR_CLEAR_Mask << DAC_Channel);
  /* ����ѡ���� DAC ͨ��: �������, ������, �����źŷ�����,�����źŷ�������ʱ��/��� */
  /* ���� TSELx �� TENx λ������ DAC_Trigger ��ֵ */
  /* ���� WAVEx λ������ DAC_WaveGeneration ��ֵ */
  /* ���� MAMPx λ������ DAC_LFSRUnmask_TriangleAmplitude ��ֵ */ 
  /* ���� BOFFx λ������ DAC_OutputBuffer ��ֵ */   
  tmpreg2 = (DAC_InitStruct->DAC_Trigger | DAC_InitStruct->DAC_WaveGeneration |
             DAC_InitStruct->DAC_LFSRUnmask_TriangleAmplitude | DAC_InitStruct->DAC_OutputBuffer);
  /* ����DAC_Channel����CR�Ĵ���ֵ */
  tmpreg1 |= tmpreg2 << DAC_Channel;
  /* д DAC CR */
  DAC->CR = tmpreg1;
}

/**
  * @����  ��Ĭ��ֵ��� DAC_InitStruct �ṹ��ÿһ����Ա.
  * @����  DAC_InitStruct : ָ��Ҫ����ʼ���� DAC_InitTypeDef �ṹָ��.
  * @����  û��
  */
void DAC_StructInit(DAC_InitTypeDef* DAC_InitStruct)
{
/*--------------- Reset DAC init structure parameters values -----------------*/
  /* ��ʼ�� DAC_Trigger ��Ա */
  DAC_InitStruct->DAC_Trigger = DAC_Trigger_None;
  /* ��ʼ�� DAC_WaveGeneration ��Ա */
  DAC_InitStruct->DAC_WaveGeneration = DAC_WaveGeneration_None;
  /* ��ʼ�� DAC_LFSRUnmask_TriangleAmplitude ��Ա */
  DAC_InitStruct->DAC_LFSRUnmask_TriangleAmplitude = DAC_LFSRUnmask_Bit0;
  /* ��ʼ�� DAC_OutputBuffer ��Ա */
  DAC_InitStruct->DAC_OutputBuffer = DAC_OutputBuffer_Enable;
}

/**
  * @����  ʹ�ܻ�ʧ��ָ���� DAC ͨ��.
  * @����  DAC_Channel: ѡ�� DAC ͨ��. 
  *          �����������������ֵ֮һ:
  *          DAC_Channel_1: ѡ�� DAC ͨ��1 
  *          DAC_Channel_2: ѡ�� DAC ͨ��2  
  * @����  NewState: DAC ͨ������״̬. 
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void DAC_Cmd(uint32_t DAC_Channel, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_DAC_CHANNEL(DAC_Channel));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* ʹ��ָ���� DAC ͨ�� */
    DAC->CR |= CR_EN_Set << DAC_Channel;
  }
  else
  {
    /* ʧ��ָ���� DAC ͨ�� */
    DAC->CR &= ~(CR_EN_Set << DAC_Channel);
  }
}

/**
  * @����  ʹ�ܻ�ʧ��ָ���� DAC ͨ�� DMA ����.
  * @����  DAC_Channel: ѡ���� DAC ͨ��. 
  *          �����������������ֵ֮һ:
  *          DAC_Channel_1: ѡ�� DAC ͨ��1 
  *          DAC_Channel_2: ѡ�� DAC ͨ��2  
  * @����  NewState: ָ���� DAC ͨ�� DMA �������״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void DAC_DMACmd(uint32_t DAC_Channel, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_DAC_CHANNEL(DAC_Channel));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* ʹ��ָ���� DAC ͨ�� DMA ���� */
    DAC->CR |= CR_DMAEN_Set << DAC_Channel;
  }
  else
  {
    /* ʧ��ָ���� DAC ͨ�� DMA ���� */
    DAC->CR &= ~(CR_DMAEN_Set << DAC_Channel);
  }
}

/**
  * @����  ʹ�ܻ�ʧ��ѡ��� DAC ͨ���������.
  * @����  DAC_Channel: ѡ���� DAC ͨ��. 
  *          �����������������ֵ֮һ:
  *          DAC_Channel_1: ѡ�� DAC ͨ��1 
  *          DAC_Channel_2: ѡ�� DAC ͨ��2  
  * @����  NewState: ѡ��� DAC ͨ�������������״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void DAC_SoftwareTriggerCmd(uint32_t DAC_Channel, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_DAC_CHANNEL(DAC_Channel));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* ʹ��ѡ��� DAC ͨ��������� */
    DAC->SWTRIGR |= SWTRIGR_SWTRIG_Set << (DAC_Channel >> 4);
  }
  else
  {
    /* ʧ��ѡ��� DAC ͨ��������� */
    DAC->SWTRIGR &= ~(SWTRIGR_SWTRIG_Set << (DAC_Channel >> 4));
  }
}

/**
  * @����  ʹ�ܻ�ʧ������ DAC ͨ��ͬ���������.
  * @����  NewState: ���� DAC ͨ��ͬ�������������״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void DAC_DualSoftwareTriggerCmd(FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* ʹ������ DAC ͨ��ͬ��������� */
    DAC->SWTRIGR |= DUAL_SWTRIG_Set ;
  }
  else
  {
    /* ʧ������ DAC ͨ��ͬ��������� */
    DAC->SWTRIGR &= DUAL_SWTRIG_Reset;
  }
}

/**
  * @����  ʹ�ܻ�ʧ��ѡ��� DAC ͨ�����η���.
  * @����  DAC_Channel: ѡ���� DAC ͨ��. 
  *          �����������������ֵ֮һ:
  *          DAC_Channel_1: ѡ�� DAC ͨ��1 
  *          DAC_Channel_2: ѡ�� DAC ͨ��2 
  * @����  DAC_Wave: ָ���Ĳ�������.
  *          �����������������ֵ֮һ:
  *          DAC_Wave_Noise:    �������η���
  *          DAC_Wave_Triangle: ���ǲ��η���
  * @����  NewState: ѡ��� DAC ͨ�����η�������״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void DAC_WaveGenerationCmd(uint32_t DAC_Channel, uint32_t DAC_Wave, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_DAC_CHANNEL(DAC_Channel));
  assert_param(IS_DAC_WAVE(DAC_Wave)); 
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* ʹ��ѡ��� DAC ͨ��ѡ��Ĳ��� */
    DAC->CR |= DAC_Wave << DAC_Channel;
  }
  else
  {
    /* ʧ��ѡ��� DAC ͨ��ѡ��Ĳ��� */
    DAC->CR &= ~(DAC_Wave << DAC_Channel);
  }
}

/**
  * @����  ���� DAC ͨ��1 ѡ�������ݱ��ּĴ���ֵ.
  * @����  DAC_Align: DAC ͨ��1 ָ�������ݶ���.
  *          �����������������ֵ֮һ:
  *          DAC_Align_8b_R:  ѡ��8λ�����Ҷ���
  *          DAC_Align_12b_L: ѡ��12λ���������
  *          DAC_Align_12b_R: ѡ��12λ�����Ҷ���
  * @����  Data : װ��ѡ������ݱ��ּĴ���������.
  * @����  û��
  */
void DAC_SetChannel1Data(uint32_t DAC_Align, uint16_t Data)
{  
  __IO uint32_t tmp = 0;
  
  /* ������ */
  assert_param(IS_DAC_ALIGN(DAC_Align));
  assert_param(IS_DAC_DATA(Data));
  
  tmp = (uint32_t)DAC_BASE; 
  tmp += DHR12R1_Offset + DAC_Align;

  /* ���� DAC ͨ��1 ѡ�������ݱ��ּĴ���ֵ */
  *(__IO uint32_t *) tmp = Data;
}

/**
  * @����  ���� DAC ͨ��2 ѡ�������ݱ��ּĴ���ֵ.
  * @����  DAC_Align: DAC ͨ��2 ָ�������ݶ���.
  *          �����������������ֵ֮һ:
  *          DAC_Align_8b_R:  ѡ��8λ�����Ҷ���
  *          DAC_Align_12b_L: ѡ��12λ���������
  *          DAC_Align_12b_R: ѡ��12λ�����Ҷ���
  * @����  Data : װ��ѡ������ݱ��ּĴ���������.
  * @����  û��
  */
void DAC_SetChannel2Data(uint32_t DAC_Align, uint16_t Data)
{
  __IO uint32_t tmp = 0;

  /* ������ */
  assert_param(IS_DAC_ALIGN(DAC_Align));
  assert_param(IS_DAC_DATA(Data));
  
  tmp = (uint32_t)DAC_BASE;
  tmp += DHR12R2_Offset + DAC_Align;

  /* ���� DAC ͨ��2 ѡ�������ݱ��ּĴ���ֵ */
  *(__IO uint32_t *)tmp = Data;
}

/**
  * @����  ����˫ͨ�� DAC ָ�������ݱ��ּĴ���ֵ.
  * @����  DAC_Align: ˫ͨ�� DAC ָ�����ݶ���.
  *          �����������������ֵ֮һ:
  *          DAC_Align_8b_R:  ѡ��8λ�����Ҷ���
  *          DAC_Align_12b_L: ѡ��12λ���������
  *          DAC_Align_12b_R: ѡ��12λ�����Ҷ���
  * @����  Data2: DACͨ��2 װ�ص�ѡ������ݱ��ּĴ���������.
  * @����  Data1: DACͨ��1 װ�ص�ѡ������ݱ��ּĴ���������.
  * @����  û��
  */
void DAC_SetDualChannelData(uint32_t DAC_Align, uint16_t Data2, uint16_t Data1)
{
  uint32_t data = 0, tmp = 0;
  
  /* ������ */
  assert_param(IS_DAC_ALIGN(DAC_Align));
  assert_param(IS_DAC_DATA(Data1));
  assert_param(IS_DAC_DATA(Data2));
  
  /* ���㲢����˫ DAC ���ݱ��ּĴ�����ֵ */
  if (DAC_Align == DAC_Align_8b_R)
  {
    data = ((uint32_t)Data2 << 8) | Data1; 
  }
  else
  {
    data = ((uint32_t)Data2 << 16) | Data1;
  }
  
  tmp = (uint32_t)DAC_BASE;
  tmp += DHR12RD_Offset + DAC_Align;

  /* ����˫ DAC ѡ������ݱ��ּĴ��� */
  *(__IO uint32_t *)tmp = data;
}

/**
  * @����  �������µ� DAC ͨ�����ݼĴ������ֵ.
  * @����  DAC_Channel: ѡ���� DAC ͨ��. 
  *          �����������������ֵ֮һ:
  *          DAC_Channel_1: ѡ�� DAC ͨ��1 
  *          DAC_Channel_2: ѡ�� DAC ͨ��2 
  * @����  ѡ���� DAC �������ֵ.
  */
uint16_t DAC_GetDataOutputValue(uint32_t DAC_Channel)
{
  __IO uint32_t tmp = 0;
  
  /* ������ */
  assert_param(IS_DAC_CHANNEL(DAC_Channel));
  
  tmp = (uint32_t) DAC_BASE ;
  tmp += DOR_Offset + ((uint32_t)DAC_Channel >> 2);
  
  /* ����ѡ���� DAC ���ݼĴ������ֵ */
  return (uint16_t) (*(__IO uint32_t*) tmp);
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
