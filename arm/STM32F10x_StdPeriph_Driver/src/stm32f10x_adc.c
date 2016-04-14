/**
  ******************************************************************************
  * @�ļ�    stm32f10x_adc.c
  * @����    MCD Ӧ�ó�����
  * @�汾    V3.1.0
  * @����    06/19/2009
  * @����    ����ļ� �ṩ���� ADC �̼�����.
  * @����    ANSON/sweet1985 Email:airanson110@126.com
  * @����    17/07/2009
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
#include "stm32f10x_adc.h"
#include "stm32f10x_rcc.h"

/* �������� --------------------------------------------------------------*/
/* ���ö��� --------------------------------------------------------------*/

/* ADC DISCNUM mask */
#define CR1_DISCNUM_Reset           ((uint32_t)0xFFFF1FFF)

/* ADC DISCEN mask */
#define CR1_DISCEN_Set              ((uint32_t)0x00000800)
#define CR1_DISCEN_Reset            ((uint32_t)0xFFFFF7FF)

/* ADC JAUTO mask */
#define CR1_JAUTO_Set               ((uint32_t)0x00000400)
#define CR1_JAUTO_Reset             ((uint32_t)0xFFFFFBFF)

/* ADC JDISCEN mask */
#define CR1_JDISCEN_Set             ((uint32_t)0x00001000)
#define CR1_JDISCEN_Reset           ((uint32_t)0xFFFFEFFF)

/* ADC AWDCH mask */
#define CR1_AWDCH_Reset             ((uint32_t)0xFFFFFFE0)

/* ADC Analog watchdog enable mode mask */
#define CR1_AWDMode_Reset           ((uint32_t)0xFF3FFDFF)

/* CR1 register Mask */
#define CR1_CLEAR_Mask              ((uint32_t)0xFFF0FEFF)

/* ADC ADON mask */
#define CR2_ADON_Set                ((uint32_t)0x00000001)
#define CR2_ADON_Reset              ((uint32_t)0xFFFFFFFE)

/* ADC DMA mask */
#define CR2_DMA_Set                 ((uint32_t)0x00000100)
#define CR2_DMA_Reset               ((uint32_t)0xFFFFFEFF)

/* ADC RSTCAL mask */
#define CR2_RSTCAL_Set              ((uint32_t)0x00000008)

/* ADC CAL mask */
#define CR2_CAL_Set                 ((uint32_t)0x00000004)

/* ADC SWSTART mask */
#define CR2_SWSTART_Set             ((uint32_t)0x00400000)

/* ADC EXTTRIG mask */
#define CR2_EXTTRIG_Set             ((uint32_t)0x00100000)
#define CR2_EXTTRIG_Reset           ((uint32_t)0xFFEFFFFF)

/* ADC Software start mask */
#define CR2_EXTTRIG_SWSTART_Set     ((uint32_t)0x00500000)
#define CR2_EXTTRIG_SWSTART_Reset   ((uint32_t)0xFFAFFFFF)

/* ADC JEXTSEL mask */
#define CR2_JEXTSEL_Reset           ((uint32_t)0xFFFF8FFF)

/* ADC JEXTTRIG mask */
#define CR2_JEXTTRIG_Set            ((uint32_t)0x00008000)
#define CR2_JEXTTRIG_Reset          ((uint32_t)0xFFFF7FFF)

/* ADC JSWSTART mask */
#define CR2_JSWSTART_Set            ((uint32_t)0x00200000)

/* ADC injected software start mask */
#define CR2_JEXTTRIG_JSWSTART_Set   ((uint32_t)0x00208000)
#define CR2_JEXTTRIG_JSWSTART_Reset ((uint32_t)0xFFDF7FFF)

/* ADC TSPD mask */
#define CR2_TSVREFE_Set             ((uint32_t)0x00800000)
#define CR2_TSVREFE_Reset           ((uint32_t)0xFF7FFFFF)

/* CR2 register Mask */
#define CR2_CLEAR_Mask              ((uint32_t)0xFFF1F7FD)

/* ADC SQx mask */
#define SQR3_SQ_Set                 ((uint32_t)0x0000001F)
#define SQR2_SQ_Set                 ((uint32_t)0x0000001F)
#define SQR1_SQ_Set                 ((uint32_t)0x0000001F)

/* SQR1 register Mask */
#define SQR1_CLEAR_Mask             ((uint32_t)0xFF0FFFFF)

/* ADC JSQx mask */
#define JSQR_JSQ_Set                ((uint32_t)0x0000001F)

/* ADC JL mask */
#define JSQR_JL_Set                 ((uint32_t)0x00300000)
#define JSQR_JL_Reset               ((uint32_t)0xFFCFFFFF)

/* ADC SMPx mask */
#define SMPR1_SMP_Set               ((uint32_t)0x00000007)
#define SMPR2_SMP_Set               ((uint32_t)0x00000007)

/* ADC JDRx registers offset */
#define JDR_Offset                  ((uint8_t)0x28)

/* ADC1 DR register base address */
#define DR_ADDRESS                  ((uint32_t)0x4001244C)

/* ���ú� -------------------------------------------------------------*/
/* ���ñ��� -----------------------------------------------------------*/
/* ���ú���ԭ�� -------------------------------------------------------*/
/* ���ú��� -----------------------------------------------------------*/

/**
  * @����  ������ ADCx ��ȫ���Ĵ�������Ϊȱʡֵ.
  * @����  ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
  * @����  û��
  */
void ADC_DeInit(ADC_TypeDef* ADCx)
{
  /* ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  
  if (ADCx == ADC1)
  {
    /* ʹ�� ADC1 ��λ״̬ */
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, ENABLE);
    /* �Ӹ�λ״̬�ͷ� ADC1  */
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1, DISABLE);
  }
  else if (ADCx == ADC2)
  {
    /* ʹ�� ADC2 ��λ״̬ */
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2, ENABLE);
    /* �Ӹ�λ״̬�ͷ� ADC2 */
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC2, DISABLE);
  }
  else
  {
    if (ADCx == ADC3)
    {
      /* ʹ�� ADC3 ��λ״̬ */
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, ENABLE);
      /* �Ӹ�λ״̬�ͷ� ADC3 */
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3, DISABLE);
    }
  }
}

/**
  * @����  ���� ADC_InitStruct ��ָ���Ĳ�����ʼ������ ADCx �ļĴ���.
  * @����  ADCx: ����x ������1 2��3������ѡ��ADC ��Χģ��.
  * @����  ADC_InitStruct: ָ��ṹ�� ADC_InitTypeDef ��ָ��,�ýṹ������ָ��ADC��Χģ���������Ϣ.
  * @����  û��
  */
void ADC_Init(ADC_TypeDef* ADCx, ADC_InitTypeDef* ADC_InitStruct)
{
  uint32_t tmpreg1 = 0;
  uint8_t tmpreg2 = 0;
  /* ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_MODE(ADC_InitStruct->ADC_Mode));
  assert_param(IS_FUNCTIONAL_STATE(ADC_InitStruct->ADC_ScanConvMode));
  assert_param(IS_FUNCTIONAL_STATE(ADC_InitStruct->ADC_ContinuousConvMode));
  assert_param(IS_ADC_EXT_TRIG(ADC_InitStruct->ADC_ExternalTrigConv));   
  assert_param(IS_ADC_DATA_ALIGN(ADC_InitStruct->ADC_DataAlign)); 
  assert_param(IS_ADC_REGULAR_LENGTH(ADC_InitStruct->ADC_NbrOfChannel));

  /*---------------------------- ADCx CR1 Configuration -----------------*/
  /* �õ� ADCx CR1 ��ֵ */
  tmpreg1 = ADCx->CR1;
  /* ��� DUALMOD �� SCAN λ */
  tmpreg1 &= CR1_CLEAR_Mask;
  /* ���� ADCx: ˫��ģʽ��ɨ��ת��ģʽ */
  /* ���� DUALMOD λ�� ���� ADC_Mode ��ֵ */
  /* ���� SCAN λ������ ADC_ScanConvMode ��ֵ */
  tmpreg1 |= (uint32_t)(ADC_InitStruct->ADC_Mode | ((uint32_t)ADC_InitStruct->ADC_ScanConvMode << 8));
  /* д ADCx CR1 */
  ADCx->CR1 = tmpreg1;

  /*---------------------------- ADCx CR2 Configuration -----------------*/
  /* �õ� ADCx CR2 ��ֵ */
  tmpreg1 = ADCx->CR2;
  /* ��� CONT, ALIGN �� EXTSEL λ */
  tmpreg1 &= CR2_CLEAR_Mask;
  /* ���� ADCx: �ⴥ���¼�������ת��ģʽ */
  /* ���� ALIGN λ������ ADC_DataAlign ��ֵ */
  /* ���� EXTSEL λ������ ADC_ExternalTrigConv ��ֵ */
  /* ���� CONT λ������ ADC_ContinuousConvMode ��ֵ */
  tmpreg1 |= (uint32_t)(ADC_InitStruct->ADC_DataAlign | ADC_InitStruct->ADC_ExternalTrigConv |
            ((uint32_t)ADC_InitStruct->ADC_ContinuousConvMode << 1));
  /* д ADCx CR2 */
  ADCx->CR2 = tmpreg1;

  /*---------------------------- ADCx SQR1 Configuration -----------------*/
  /* �õ� ADCx SQR1 ��ֵ */
  tmpreg1 = ADCx->SQR1;
  /* ��� L λ */
  tmpreg1 &= SQR1_CLEAR_Mask;
  /* ���� ADCx: �����ŵ����� */
  /* ���� L λ������ ADC_NbrOfChannel ��ֵ */
  tmpreg2 |= (uint8_t) (ADC_InitStruct->ADC_NbrOfChannel - (uint8_t)1);
  tmpreg1 |= (uint32_t)tmpreg2 << 20;
  /* д ADCx SQR1 */
  ADCx->SQR1 = tmpreg1;
}

/**
  * @����  �� ADC_InitStruct �е�ÿһ��������ȱʡֵ����.
  * @����  ADC_InitStruct : ָ��ṹ�� ADC_InitTypeDef ��ָ�룬������ʼ��.
  * @����  û��
  */
void ADC_StructInit(ADC_InitTypeDef* ADC_InitStruct)
{
  /* ��λ ADC ��ʼ�ṹ���� */
  /* ��ʼ�� ADC_Mode ��Ա */
  ADC_InitStruct->ADC_Mode = ADC_Mode_Independent;
  /* ��ʼ�� ADC_ScanConvMode ��Ա */
  ADC_InitStruct->ADC_ScanConvMode = DISABLE;
  /* ��ʼ�� ADC_ContinuousConvMode ��Ա */
  ADC_InitStruct->ADC_ContinuousConvMode = DISABLE;
  /* ��ʼ�� ADC_ExternalTrigConv ��Ա */
  ADC_InitStruct->ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  /* ��ʼ�� ADC_DataAlign ��Ա */
  ADC_InitStruct->ADC_DataAlign = ADC_DataAlign_Right;
  /* ��ʼ�� ADC_NbrOfChannel ��Ա */
  ADC_InitStruct->ADC_NbrOfChannel = 1;
}

/**
  * @����  ʹ�ܻ���ʧ��ָ���� ADC .
  * @����  ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
  * @����  NewState: ADCx �豸����״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void ADC_Cmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* ���� ADON λ����ʡ��ģʽ�»���ADC */
    ADCx->CR2 |= CR2_ADON_Set;
  }
  else
  {
    /* ʧ��ָ���� ADC �豸 */
    ADCx->CR2 &= CR2_ADON_Reset;
  }
}

/**
  * @����  ʹ�ܻ�ʧ��ָ���� ADC DMA ����.
  * @����  ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
  *        ע��: ADC2 û�� DMA ����.
  * @����  NewState: ѡ���� ADC DMA ת��������״̬.
  *        �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void ADC_DMACmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_ADC_DMA_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* ʹ��ѡ���� ADC DMA ���� */
    ADCx->CR2 |= CR2_DMA_Set;
  }
  else
  {
    /* ʧ��ѡ���� ADC DMA ���� */
    ADCx->CR2 &= CR2_DMA_Reset;
  }
}

/**
  * @����  ʹ�ܻ���ʧ��ָ����ADC���ж�.
  * @����  ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
  * @����  ADC_IT: ָ���� ADC �ж�Դ. 
  *          ����ʹ������ֵ��һ�����߼���ֵ�����:
  *          ADC_IT_EOC:  ��������ж�����
  *          ADC_IT_AWD:  ģ�⿴�Ź��ж�����
  *          ADC_IT_JEOC: ע��ͨ������ж�����
  * @����  NewState: ָ���� ADC �жϵ���״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void ADC_ITConfig(ADC_TypeDef* ADCx, uint16_t ADC_IT, FunctionalState NewState)
{
  uint8_t itmask = 0;
  /* ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_ADC_IT(ADC_IT));
  /* �õ� ADC IT �жϺ� */
  itmask = (uint8_t)ADC_IT;
  if (NewState != DISABLE)
  {
    /* ʹ��ѡ���� ADC �ж� */
    ADCx->CR1 |= itmask;
  }
  else
  {
    /* ʧ��ָ���� ADC �ж� */
    ADCx->CR1 &= (~(uint32_t)itmask);
  }
}

/**
  * @����  ����ָ����ADC��У׼�Ĵ���.
  * @����  ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
  * @����  û��
  */
void ADC_ResetCalibration(ADC_TypeDef* ADCx)
{
  /* ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  /* ����ָ����ADC��У׼�Ĵ��� */  
  ADCx->CR2 |= CR2_RSTCAL_Set;
}

/**
  * @����  ��ȡADC����У׼�Ĵ�����״̬.
  * @����  ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
  * @����  ADC����У׼�Ĵ�������״̬ (SET �� RESET).
  */
FlagStatus ADC_GetResetCalibrationStatus(ADC_TypeDef* ADCx)
{
  FlagStatus bitstatus = RESET;
  /* ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  /* ��� RSTCAL λ��״̬ */
  if ((ADCx->CR2 & CR2_RSTCAL_Set) != (uint32_t)RESET)
  {
    /* ���� RSTCAL λ */
    bitstatus = SET;
  }
  else
  {
    /* ��λ RSTCAL λ */
    bitstatus = RESET;
  }
  /* ���� RSTCAL λ��״̬ */
  return  bitstatus;
}

/**
  * @����  ��ʼָ��ADC��У׼״̬.
  * @����  ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
  * @����  û��
  */
void ADC_StartCalibration(ADC_TypeDef* ADCx)
{
  /* ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  /* ʹ��ѡ���� ADC У׼���� */  
  ADCx->CR2 |= CR2_CAL_Set;
}

/**
  * @����  ��ȡָ��ADC��У׼״̬.
  * @����  ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
  * @����  ָ��ADC��У׼����״̬ (SET �� RESET).
  */
FlagStatus ADC_GetCalibrationStatus(ADC_TypeDef* ADCx)
{
  FlagStatus bitstatus = RESET;
  /* ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  /* ��� CAL λ��״̬ */
  if ((ADCx->CR2 & CR2_CAL_Set) != (uint32_t)RESET)
  {
    /* ���� CAL λ: ��׼������ */
    bitstatus = SET;
  }
  else
  {
    /* ��λ CAL λ: У׼���� */
    bitstatus = RESET;
  }
  /* ���� CAL λ��״̬ */
  return  bitstatus;
}

/**
  * @����  ʹ�ܻ���ʧ��ָ����ADC�����ת����������.
  * @����  ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
  * @����  NewState: ѡ�е������������ʼ�źŵ�ADC����״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void ADC_SoftwareStartConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* ����ѡ���ADCת���ⲿ�¼�������ѡ���ADCת�� */
    ADCx->CR2 |= CR2_EXTTRIG_SWSTART_Set;
  }
  else
  {
    /* ��ֹѡ���ADCת���ⲿ�¼���ֹͣѡ���ADCת�� */
    ADCx->CR2 &= CR2_EXTTRIG_SWSTART_Reset;
  }
}

/**
  * @����  ��ȡADC���ת������״̬.
  * @����  ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
  * @����  ADC�����ʼת������״̬ (SET �� RESET).
  */
FlagStatus ADC_GetSoftwareStartConvStatus(ADC_TypeDef* ADCx)
{
  FlagStatus bitstatus = RESET;
  /* ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  /* ��� SWSTART λ����״̬ */
  if ((ADCx->CR2 & CR2_SWSTART_Set) != (uint32_t)RESET)
  {
    /* ���� SWSTART λ */
    bitstatus = SET;
  }
  else
  {
    /* ��λ SWSTART λ */
    bitstatus = RESET;
  }
  /* ���� SWSTART λ��״̬ */
  return  bitstatus;
}

/**
  * @����  ��ADC������ͨ�����ü��ģʽ.
  * @����  ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
  * @����  Number: ģʽ�³����ŵ�����ֵ����ֵ��ΧΪ1��8. 
  * @����  û��
  */
void ADC_DiscModeChannelCountConfig(ADC_TypeDef* ADCx, uint8_t Number)
{
  uint32_t tmpreg1 = 0;
  uint32_t tmpreg2 = 0;
  /* ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_REGULAR_DISC_NUMBER(Number));
  /* �õ���ȥ�ļĴ���ֵ */
  tmpreg1 = ADCx->CR1;
  /* �����ȥ�Ĳ����ģʽͨ�������� */
  tmpreg1 &= CR1_DISCNUM_Reset;
  /* ���ü��ģʽͨ�������� */
  tmpreg2 = Number - 1;
  tmpreg1 |= tmpreg2 << 13;
  /* �洢�Ĵ�������ֵ */
  ADCx->CR1 = tmpreg1;
}

/**
  * @����  ʹ�ܻ���ʧ��ָ����ADC������ͨ���ļ��ģʽ.
  * @����  ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
  * @����  NewState: ָ����ADC������ͨ���ļ��ģʽ����״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void ADC_DiscModeCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* ʹ��ָ����ADC������ͨ���ļ��ģʽ */
    ADCx->CR1 |= CR1_DISCEN_Set;
  }
  else
  {
    /* ʧ��ָ����ADC������ͨ���ļ��ģʽ */
    ADCx->CR1 &= CR1_DISCEN_Reset;
  }
}

/**
  * @����  ����ָ��ADC�Ĺ�����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��.
  * @����  ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
  * @����  ADC_Channel: ���� ADC ͨ��. 
  *          �����������ʹ������ֵ��һ��:
  *          ADC_Channel_0:  ѡ��ADCͨ��0  
  *          ADC_Channel_1:  ѡ��ADCͨ��1  
  *          ADC_Channel_2:  ѡ��ADCͨ��2  
  *          ADC_Channel_3:  ѡ��ADCͨ��3  
  *          ADC_Channel_4:  ѡ��ADCͨ��4  
  *          ADC_Channel_5:  ѡ��ADCͨ��5  
  *          ADC_Channel_6:  ѡ��ADCͨ��6  
  *          ADC_Channel_7:  ѡ��ADCͨ��7  
  *          ADC_Channel_8:  ѡ��ADCͨ��8  
  *          ADC_Channel_9:  ѡ��ADCͨ��9  
  *          ADC_Channel_10: ѡ��ADCͨ��10  
  *          ADC_Channel_11: ѡ��ADCͨ��11  
  *          ADC_Channel_12: ѡ��ADCͨ��12  
  *          ADC_Channel_13: ѡ��ADCͨ��13  
  *          ADC_Channel_14: ѡ��ADCͨ��14  
  *          ADC_Channel_15: ѡ��ADCͨ��15  
  *          ADC_Channel_16: ѡ��ADCͨ��16  
  *          ADC_Channel_17: ѡ��ADCͨ��17  
  * @����  Rank: ���������˳��. ȡֵ��Χ1��16.
  * @����  ADC_SampleTime: ָ��ADCͨ���Ĳ���ʱ��ֵ. 
  *          �����������ʹ������ֵ��һ��:
  *          ADC_SampleTime_1Cycles5:   ����ʱ����� 1.5   ����
  *          ADC_SampleTime_7Cycles5:   ����ʱ����� 7.5   ����
  *          ADC_SampleTime_13Cycles5:  ����ʱ����� 13.5  ����
  *          ADC_SampleTime_28Cycles5:  ����ʱ����� 28.5  ����	
  *          ADC_SampleTime_41Cycles5:  ����ʱ����� 41.5  ����	
  *          ADC_SampleTime_55Cycles5:  ����ʱ����� 55.5  ����	
  *          ADC_SampleTime_71Cycles5:  ����ʱ����� 71.5  ����	
  *          ADC_SampleTime_239Cycles5: ����ʱ����� 239.5 ����	
  * @����  û��
  */
void ADC_RegularChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime)
{
  uint32_t tmpreg1 = 0, tmpreg2 = 0;
  /* ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_CHANNEL(ADC_Channel));
  assert_param(IS_ADC_REGULAR_RANK(Rank));
  assert_param(IS_ADC_SAMPLE_TIME(ADC_SampleTime));
  /* ���ѡ�� ADC_Channel_10 ... ADC_Channel_17 */
  if (ADC_Channel > ADC_Channel_9)
  {
    /* �õ��ɵļĴ�����ֵ */
    tmpreg1 = ADCx->SMPR1;
    /* ������Ҫ����ı�־ */
    tmpreg2 = SMPR1_SMP_Set << (3 * (ADC_Channel - 10));
    /* ����ɵ�ͨ������ʱ�� */
    tmpreg1 &= ~tmpreg2;
    /* ������Ҫ��λ�ı�־ */
    tmpreg2 = (uint32_t)ADC_SampleTime << (3 * (ADC_Channel - 10));
    /* �����µ�ͨ������ʱ�� */
    tmpreg1 |= tmpreg2;
    /* �洢�µļĴ�����ֵ */
    ADCx->SMPR1 = tmpreg1;
  }
  else /* ADC_Channel include in ADC_Channel_[0..9] */
  {
    /* �õ��ɵļĴ�����ֵ */
    tmpreg1 = ADCx->SMPR2;
    /* ������Ҫ����ı�־ */
    tmpreg2 = SMPR2_SMP_Set << (3 * ADC_Channel);
    /* ����ɵ�ͨ������ʱ�� */
    tmpreg1 &= ~tmpreg2;
    /* ������Ҫ��λ�ı�־ */
    tmpreg2 = (uint32_t)ADC_SampleTime << (3 * ADC_Channel);
    /* �����µ�ͨ������ʱ�� */
    tmpreg1 |= tmpreg2;
    /* �洢�µļĴ�����ֵ */
    ADCx->SMPR2 = tmpreg1;
  }
  /* For Rank 1 to 6 */
  if (Rank < 7)
  {
    /* �õ��ɵļĴ�����ֵ */
    tmpreg1 = ADCx->SQR3;
    /* ������Ҫ����ı�־ */
    tmpreg2 = SQR3_SQ_Set << (5 * (Rank - 1));
    /* ����ѡ�����������ɵ� SQx λ */
    tmpreg1 &= ~tmpreg2;
    /* ������Ҫ��λ�ı�־ */
    tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 1));
    /* ����ѡ������������µ� SQx λ */
    tmpreg1 |= tmpreg2;
    /* �洢�µļĴ�����ֵ */
    ADCx->SQR3 = tmpreg1;
  }
  /* For Rank 7 to 12 */
  else if (Rank < 13)
  {
    /* �õ��ɵļĴ�����ֵ */
    tmpreg1 = ADCx->SQR2;
    /* ������Ҫ����ı�־ */
    tmpreg2 = SQR2_SQ_Set << (5 * (Rank - 7));
    /* ����ѡ�����������ɵ� SQx λ */
    tmpreg1 &= ~tmpreg2;
    /* ������Ҫ��λ�ı�־ */
    tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 7));
    /* ����ѡ������������µ� SQx λ */
    tmpreg1 |= tmpreg2;
    /* �洢�µļĴ�����ֵ */
    ADCx->SQR2 = tmpreg1;
  }
  /* For Rank 13 to 16 */
  else
  {
    /* �õ��ɵļĴ�����ֵ */
    tmpreg1 = ADCx->SQR1;
    /* ������Ҫ����ı�־ */
    tmpreg2 = SQR1_SQ_Set << (5 * (Rank - 13));
    /* ����ѡ�����������ɵ� SQx λ */
    tmpreg1 &= ~tmpreg2;
    /* ������Ҫ��λ�ı�־ */
    tmpreg2 = (uint32_t)ADC_Channel << (5 * (Rank - 13));
    /* ����ѡ������������µ� SQx λ */
    tmpreg1 |= tmpreg2;
    /* �洢�µļĴ�����ֵ */
    ADCx->SQR1 = tmpreg1;
  }
}

/**
  * @����  ʹ�ܻ���ʧ�� ADCx �ľ��ⲿ��������ת������.
  * @����  ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
  * @����  NewState: ѡ���� ADCx �ľ��ⲿ��������ת�����ܵ���״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void ADC_ExternalTrigConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* ʹ�� ADCx �ľ��ⲿ��������ת������ */
    ADCx->CR2 |= CR2_EXTTRIG_Set;
  }
  else
  {
    /* ʧ�� ADCx �ľ��ⲿ��������ת������ */
    ADCx->CR2 &= CR2_EXTTRIG_Reset;
  }
}

/**
  * @����  �������һ�� ADCx �������ת�����.
  * @����  ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
  * @����  ת�����.
  */
uint16_t ADC_GetConversionValue(ADC_TypeDef* ADCx)
{
  /* ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  /* �������һ�� ADCx �������ת����� */
  return (uint16_t) ADCx->DR;
}

/**
  * @����  �������һ��˫ ADC ģʽ�µ� ADC1 �� ADC2 ת�����.
  * @retval ת�����.
  */
uint32_t ADC_GetDualModeConversionValue(void)
{
  /* ����˫ת���µ�ת����� */
  return (*(__IO uint32_t *) DR_ADDRESS);
}

/**
  * @����  ʹ�ܻ���ʧ��ָ�� ADC �ڹ�����ת�����Զ���ʼע����ת��.
  * @����  ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
  * @����  NewState: ѡ�еĽ����Զ�ע��ת����ADC��״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void ADC_AutoInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* ʹ��ָ�� ADC �ڹ�����ת�����Զ���ʼע����ת�� */
    ADCx->CR1 |= CR1_JAUTO_Set;
  }
  else
  {
    /* ʧ��ָ�� ADC �ڹ�����ת�����Զ���ʼע����ת�� */
    ADCx->CR1 &= CR1_JAUTO_Reset;
  }
}

/**
  * @����  ʹ�ܻ���ʧ��ָ�� ADC ��ע������ģʽ
  * @����  ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
  * @����  NewState: ADC ע����ͨ���ϼ��ģʽ����״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void ADC_InjectedDiscModeCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* ʧ��ָ�� ADC ��ע������ģʽ */
    ADCx->CR1 |= CR1_JDISCEN_Set;
  }
  else
  {
    /* ʧ��ָ�� ADC ��ע������ģʽ */
    ADCx->CR1 &= CR1_JDISCEN_Reset;
  }
}

/**
  * @����  ���� ADCx ���ⲿ��������ע����ת������.
  * @����  ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
  * @����  ADC_ExternalTrigInjecConv: ��ʼע��ת����ADC������. 
  *          �����������ȡ����ֵ֮һ:
  *          ADC_ExternalTrigInjecConv_T1_TRGO: ѡ��ʱ��1��TRGO��Ϊע��ת���ⲿ���� (for ADC1, ADC2 and ADC3)
  *          ADC_ExternalTrigInjecConv_T1_CC4:  ѡ��ʱ��1�Ĳ���Ƚ�4��Ϊע��ת���ⲿ���� (for ADC1, ADC2 and ADC3)
  *          ADC_ExternalTrigInjecConv_T2_TRGO: ѡ��ʱ��2�� TRGO ��Ϊע��ת���ⲿ���� (for ADC1 and ADC2)
  *          ADC_ExternalTrigInjecConv_T2_CC1:  ѡ��ʱ��2�Ĳ���Ƚ�1��Ϊע��ת���ⲿ���� (for ADC1 and ADC2)
  *          ADC_ExternalTrigInjecConv_T3_CC4:  ѡ��ʱ��3�Ĳ���Ƚ�4��Ϊע��ת���ⲿ���� (for ADC1 and ADC2)
  *          ADC_ExternalTrigInjecConv_T4_TRGO: ѡ��ʱ��4��TRGO��Ϊע��ת���ⲿ���� (for ADC1 and ADC2)
  *          ADC_ExternalTrigInjecConv_Ext_IT15_TIM8_CC4: �ⲿ�ж�15����timer8�Ƚ��¼���Ϊע��ת�����ⲿ������ (for ADC1 and ADC2)                       
  *          ADC_ExternalTrigInjecConv_T4_CC3:  ѡ��ʱ��4����Ƚ�3 (for ADC3 only)
  *          ADC_ExternalTrigInjecConv_T8_CC2:  ѡ��ʱ��8����Ƚ�2 (for ADC3 only)                         
  *          ADC_ExternalTrigInjecConv_T8_CC4:  ѡ��ʱ��8����Ƚ�4 (for ADC3 only)
  *          ADC_ExternalTrigInjecConv_T5_TRGO: ѡ��ʱ��5�����¼� (for ADC3 only)                         
  *          ADC_ExternalTrigInjecConv_T5_CC4:  ѡ��ʱ��5����Ƚ�4 (for ADC3 only)                        
  *          ADC_ExternalTrigInjecConv_None:    ע��ת����������������������ⲿ���������� (for ADC1, ADC2 and ADC3)
  * @����  û��
  */
void ADC_ExternalTrigInjectedConvConfig(ADC_TypeDef* ADCx, uint32_t ADC_ExternalTrigInjecConv)
{
  uint32_t tmpreg = 0;
  /* ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_EXT_INJEC_TRIG(ADC_ExternalTrigInjecConv));
  /* �õ��ɵļĴ�����ֵ */
  tmpreg = ADCx->CR2;
  /* ���ѡ���ע��ͨ��Ⱥ���ⲿ�����¼� */
  tmpreg &= CR2_JEXTSEL_Reset;
  /* ����ѡ���ע��ͨ��Ⱥ���ⲿ�����¼� */
  tmpreg |= ADC_ExternalTrigInjecConv;
  /* �洢�µļĴ�����ֵ */
  ADCx->CR2 = tmpreg;
}

/**
  * @����  ʹ�ܻ���ʧ�� ADCx �ľ��ⲿ��������ע����ת������.
  * @����  ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
  * @����  NewState: ָ��ADC�ⲿ��������ע��ת������״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void ADC_ExternalTrigInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* ʹ�� ADCx �ľ��ⲿ��������ע����ת������ */
    ADCx->CR2 |= CR2_JEXTTRIG_Set;
  }
  else
  {
    /* ʧ�� ADCx �ľ��ⲿ��������ע����ת������ */
    ADCx->CR2 &= CR2_JEXTTRIG_Reset;
  }
}

/**
  * @����  ʹ�ܻ���ʧ�� ADCx �������ע����ת������.
  * @����  ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
  * @����  NewState: ָ�� ADC �����������ע��ת������״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void ADC_SoftwareStartInjectedConvCmd(ADC_TypeDef* ADCx, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* ʹ�� ADCx �������ע����ת�����ܲ�������ע��ѡ�� */
    ADCx->CR2 |= CR2_JEXTTRIG_JSWSTART_Set;
  }
  else
  {
    /* ʧ�� ADCx �������ע����ת�����ܲ�������ע��ѡ�� */
    ADCx->CR2 &= CR2_JEXTTRIG_JSWSTART_Reset;
  }
}

/**
  * @����  ��ȡָ�� ADC ���������ע����ת��״̬.
  * @����  ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
  * @����  ָ���� ADC �����������ע��ת������״̬ (SET �� RESET).
  */
FlagStatus ADC_GetSoftwareStartInjectedConvCmdStatus(ADC_TypeDef* ADCx)
{
  FlagStatus bitstatus = RESET;
  /* ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  /* ��� JSWSTART λ��״̬ */
  if ((ADCx->CR2 & CR2_JSWSTART_Set) != (uint32_t)RESET)
  {
    /* ���� JSWSTART λ */
    bitstatus = SET;
  }
  else
  {
    /* ��λ JSWSTART λ */
    bitstatus = RESET;
  }
  /* ���� JSWSTART λ��״̬ */
  return  bitstatus;
}

/**
  * @����  ����ָ�� ADC ��ע����ͨ�����������ǵ�ת��˳��Ͳ���ʱ��.
  * @����  ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
  * @����  ADC_Channel: �����õ�ADCͨ��. 
  *          �����������ȡ����ֵ֮һ:
  *          ADC_Channel_0:  ѡ��ADCͨ��0  
  *          ADC_Channel_1:  ѡ��ADCͨ��1  
  *          ADC_Channel_2:  ѡ��ADCͨ��2  
  *          ADC_Channel_3:  ѡ��ADCͨ��3  
  *          ADC_Channel_4:  ѡ��ADCͨ��4  
  *          ADC_Channel_5:  ѡ��ADCͨ��5  
  *          ADC_Channel_6:  ѡ��ADCͨ��6  
  *          ADC_Channel_7:  ѡ��ADCͨ��7  
  *          ADC_Channel_8:  ѡ��ADCͨ��8  
  *          ADC_Channel_9:  ѡ��ADCͨ��9  
  *          ADC_Channel_10: ѡ��ADCͨ��10  
  *          ADC_Channel_11: ѡ��ADCͨ��11  
  *          ADC_Channel_12: ѡ��ADCͨ��12  
  *          ADC_Channel_13: ѡ��ADCͨ��13  
  *          ADC_Channel_14: ѡ��ADCͨ��14  
  *          ADC_Channel_15: ѡ��ADCͨ��15  
  *          ADC_Channel_16: ѡ��ADCͨ��16  
  *          ADC_Channel_17: ѡ��ADCͨ��17 
  * @����  Rank: ���������˳��.ȡֵ��Χ1��4.
  * @����  ADC_SampleTime: ָ��ADCͨ���Ĳ���ʱ��ֵ. 
  *          �����������ʹ������ֵ��һ��:
  *          ADC_SampleTime_1Cycles5:   ����ʱ����� 1.5   ����
  *          ADC_SampleTime_7Cycles5:   ����ʱ����� 7.5   ����
  *          ADC_SampleTime_13Cycles5:  ����ʱ����� 13.5  ����
  *          ADC_SampleTime_28Cycles5:  ����ʱ����� 28.5  ����	
  *          ADC_SampleTime_41Cycles5:  ����ʱ����� 41.5  ����	
  *          ADC_SampleTime_55Cycles5:  ����ʱ����� 55.5  ����	
  *          ADC_SampleTime_71Cycles5:  ����ʱ����� 71.5  ����	
  *          ADC_SampleTime_239Cycles5: ����ʱ����� 239.5 ����		
  * @����  û��
  */
void ADC_InjectedChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel, uint8_t Rank, uint8_t ADC_SampleTime)
{
  uint32_t tmpreg1 = 0, tmpreg2 = 0, tmpreg3 = 0;
  /* ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_CHANNEL(ADC_Channel));
  assert_param(IS_ADC_INJECTED_RANK(Rank));
  assert_param(IS_ADC_SAMPLE_TIME(ADC_SampleTime));
  /* ���ѡ�� ADC_Channel_10 ... ADC_Channel_17 */
  if (ADC_Channel > ADC_Channel_9)
  {
    /* �õ��ɵļĴ�����ֵ */
    tmpreg1 = ADCx->SMPR1;
    /* ������Ҫ����ı�־ */
    tmpreg2 = SMPR1_SMP_Set << (3*(ADC_Channel - 10));
    /* ����ɵ�ͨ������ʱ�� */
    tmpreg1 &= ~tmpreg2;
    /* ������Ҫ��λ�ı�־ */
    tmpreg2 = (uint32_t)ADC_SampleTime << (3*(ADC_Channel - 10));
    /* �����µ�ͨ������ʱ�� */
    tmpreg1 |= tmpreg2;
    /* �洢�µļĴ�����ֵ */
    ADCx->SMPR1 = tmpreg1;
  }
  else /* ADC_Channel include in ADC_Channel_[0..9] */
  {
    /* �õ��ɵļĴ�����ֵ */
    tmpreg1 = ADCx->SMPR2;
    /* ������Ҫ����ı�־ */
    tmpreg2 = SMPR2_SMP_Set << (3 * ADC_Channel);
    /* ����ɵ�ͨ������ʱ�� */
    tmpreg1 &= ~tmpreg2;
    /* ������Ҫ��λ�ı�־ */
    tmpreg2 = (uint32_t)ADC_SampleTime << (3 * ADC_Channel);
    /* �����µ�ͨ������ʱ�� */
    tmpreg1 |= tmpreg2;
    /* �洢�µļĴ�����ֵ */
    ADCx->SMPR2 = tmpreg1;
  }
  /* ���� Rank ���������˳�� */
  /* �õ��ɵļĴ�����ֵ */
  tmpreg1 = ADCx->JSQR;
  /* �õ� JL ��ֵ: Number = JL+1 */
  tmpreg3 =  (tmpreg1 & JSQR_JL_Set)>> 20;
  /* ������Ҫ����ı�־: ((Rank-1)+(4-JL-1)) */
  tmpreg2 = JSQR_JSQ_Set << (5 * (uint8_t)((Rank + 3) - (tmpreg3 + 1)));
  /* ���ѡ������ JSQx λ */
  tmpreg1 &= ~tmpreg2;
  /* ������Ҫ��λ�ı�־: ((Rank-1)+(4-JL-1)) */
  tmpreg2 = (uint32_t)ADC_Channel << (5 * (uint8_t)((Rank + 3) - (tmpreg3 + 1)));
  /* ��λѡ�����оɵ� JSQx λ */
  tmpreg1 |= tmpreg2;
  /* �洢�µļĴ�����ֵ */
  ADCx->JSQR = tmpreg1;
}

/**
  * @����  ����ע����ͨ����ת�����г���
  * @����  ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
  * @����  Length: ת�����г���. 
  *                ������������� 1 �� 4 ֮���ֵ.
  * @����  û��
  */
void ADC_InjectedSequencerLengthConfig(ADC_TypeDef* ADCx, uint8_t Length)
{
  uint32_t tmpreg1 = 0;
  uint32_t tmpreg2 = 0;
  /* ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_INJECTED_LENGTH(Length));
  
  /* �õ��ɵļĴ�����ֵ */
  tmpreg1 = ADCx->JSQR;
  /* ���ע���ŵ��������ĳ��� JL λ */
  tmpreg1 &= JSQR_JL_Reset;
  /* ��λע���ŵ��������ĳ��� JL λ */
  tmpreg2 = Length - 1; 
  tmpreg1 |= tmpreg2 << 20;
  /* �洢�µļĴ�����ֵ */
  ADCx->JSQR = tmpreg1;
}

/**
  * @����  ����ע����ͨ����ת��ƫ��ֵ
  * @����  ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
  * @����  ADC_InjectedChannel: ��Ҫ����ƫ��ֵ��ADCע���ŵ�. 
  *          �����������ʹ������ֵ��һ��:
  *          ADC_InjectedChannel_1: ѡ��ע��ͨ��1 
  *          ADC_InjectedChannel_2: ѡ��ע��ͨ��2 
  *          ADC_InjectedChannel_3: ѡ��ע��ͨ��3 
  *          ADC_InjectedChannel_4: ѡ��ע��ͨ��4 
  * @����  Offset: ѡ�е�ADCע���ŵ���ƫ��ֵ���ò�����һ��12λֵ.
  * @����  û��
  */
void ADC_SetInjectedOffset(ADC_TypeDef* ADCx, uint8_t ADC_InjectedChannel, uint16_t Offset)
{
  __IO uint32_t tmp = 0;
  
  /* ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_INJECTED_CHANNEL(ADC_InjectedChannel));
  assert_param(IS_ADC_OFFSET(Offset));  
  
  tmp = (uint32_t)ADCx;
  tmp += ADC_InjectedChannel;
  
  /* ����ѡ����ע��ͨ��������ƫ���� */
  *(__IO uint32_t *) tmp = (uint32_t)Offset;
}

/**
  * @����  ���� ADC ָ��ע��ͨ����ת�����
  * @����  ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
  * @����  ADC_InjectedChannel: ָ��ע��ͨ��.
  *          �����������ʹ������ֵ��һ��:
  *          ADC_InjectedChannel_1: ѡ��ע��ͨ��1 
  *          ADC_InjectedChannel_2: ѡ��ע��ͨ��2
  *          ADC_InjectedChannel_3: ѡ��ע��ͨ��3
  *          ADC_InjectedChannel_4: ѡ��ע��ͨ��4
  * @����  ת�����.
  */
uint16_t ADC_GetInjectedConversionValue(ADC_TypeDef* ADCx, uint8_t ADC_InjectedChannel)
{
  __IO uint32_t tmp = 0;
  
  /* ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_INJECTED_CHANNEL(ADC_InjectedChannel));

  tmp = (uint32_t)ADCx;
  tmp += ADC_InjectedChannel + JDR_Offset;
  
  /* ���� ADC ָ��ע��ͨ����ת����� */
  return (uint16_t) (*(__IO uint32_t*)  tmp);   
}

/**
  * @����  ʹ�ܻ���ʧ��ָ������/ȫ�壬����/ע����ͨ���ϵ�ģ�⿴�Ź�
  * @����  ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
  * @����  ADC_AnalogWatchdog: ADCģ�⿴�Ź�����.
  *          �����������ʹ������ֵ��һ��:
  *          ADC_AnalogWatchdog_SingleRegEnable:        ��������ͨ��������ģ�⿴�Ź�
  *          ADC_AnalogWatchdog_SingleInjecEnable:      ����ע��ͨ��������ģ�⿴�Ź�
  *          ADC_AnalogWatchdog_SingleRegOrInjecEnable: ��������ͨ������ע��ͨ��������ģ�⿴�Ź�
  *          ADC_AnalogWatchdog_AllRegEnable:           ���й���ͨ��������ģ�⿴�Ź�
  *          ADC_AnalogWatchdog_AllInjecEnable:         ����ע��ͨ��������ģ�⿴�Ź�
  *          ADC_AnalogWatchdog_AllRegAllInjecEnable:   ���й���ͨ��������ע��ͨ����������ģ�⿴�Ź�
  *          ADC_AnalogWatchdog_None:                   ������ģ�⿴�Ź�
  * @����  û��	  
  */
void ADC_AnalogWatchdogCmd(ADC_TypeDef* ADCx, uint32_t ADC_AnalogWatchdog)
{
  uint32_t tmpreg = 0;
  /* ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_ANALOG_WATCHDOG(ADC_AnalogWatchdog));
  /* �õ��ɵļĴ�����ֵ */
  tmpreg = ADCx->CR1;
  /* ��� AWDEN, AWDENJ �� AWDSGL λ */
  tmpreg &= CR1_AWDMode_Reset;
  /* ����ģ�⿴�Ź�����ģʽ */
  tmpreg |= ADC_AnalogWatchdog;
  /* �洢�µļĴ�����ֵ */
  ADCx->CR1 = tmpreg;
}

/**
  * @����  ����ģ�⿴�Ź��ĸ�/����ֵ.
  * @����  ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
  * @����  HighThreshold: ģ�⿴�Ź��ĸ���ֵ.
  *                       ���������һ��12λ��ֵ.
  * @����  LowThreshold:  ģ�⿴�Ź��ĵ���ֵ.
  *                       ���������һ��12λ��ֵ.
  * @����  û��
  */
void ADC_AnalogWatchdogThresholdsConfig(ADC_TypeDef* ADCx, uint16_t HighThreshold,
                                        uint16_t LowThreshold)
{
  /* ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_THRESHOLD(HighThreshold));
  assert_param(IS_ADC_THRESHOLD(LowThreshold));
  /* ����ģ�⿴�Ź��ĸ���ֵ */
  ADCx->HTR = HighThreshold;
  /* ����ģ�⿴�Ź��ĵ���ֵ */
  ADCx->LTR = LowThreshold;
}

/**
  * @����  �Ե���ADCͨ������ģ�⿴�Ź�
  * @����  ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
  * @����  ADC_Channel: ��ҪΪ������ģ�⿴�Ź����ŵ�. 
  *          �����������ʹ������ֵ��һ��:
  *          ADC_Channel_0:  ѡ��ADCͨ��0  
  *          ADC_Channel_1:  ѡ��ADCͨ��1  
  *          ADC_Channel_2:  ѡ��ADCͨ��2  
  *          ADC_Channel_3:  ѡ��ADCͨ��3  
  *          ADC_Channel_4:  ѡ��ADCͨ��4  
  *          ADC_Channel_5:  ѡ��ADCͨ��5  
  *          ADC_Channel_6:  ѡ��ADCͨ��6  
  *          ADC_Channel_7:  ѡ��ADCͨ��7  
  *          ADC_Channel_8:  ѡ��ADCͨ��8  
  *          ADC_Channel_9:  ѡ��ADCͨ��9  
  *          ADC_Channel_10: ѡ��ADCͨ��10  
  *          ADC_Channel_11: ѡ��ADCͨ��11  
  *          ADC_Channel_12: ѡ��ADCͨ��12  
  *          ADC_Channel_13: ѡ��ADCͨ��13  
  *          ADC_Channel_14: ѡ��ADCͨ��14  
  *          ADC_Channel_15: ѡ��ADCͨ��15  
  *          ADC_Channel_16: ѡ��ADCͨ��16  
  *          ADC_Channel_17: ѡ��ADCͨ��17 
  * @����  û��
  */
void ADC_AnalogWatchdogSingleChannelConfig(ADC_TypeDef* ADCx, uint8_t ADC_Channel)
{
  uint32_t tmpreg = 0;
  /* ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_CHANNEL(ADC_Channel));
  /* �õ��ɵļĴ�����ֵ */
  tmpreg = ADCx->CR1;
  /* ���ģ�⿴�Ź�ͨ��ѡ��λ */
  tmpreg &= CR1_AWDCH_Reset;
  /* ����ģ�⿴�Ź�ͨ��ѡ��λ */
  tmpreg |= ADC_Channel;
  /* �洢�Ĵ�������ֵ */
  ADCx->CR1 = tmpreg;
}

/**
  * @����  ʹ�ܻ���ʧ���¶ȴ��������ڲ��ο���ѹͨ��.
  * @����  NewState: �¶ȴ��������ڲ��ο���ѹͨ������״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void ADC_TempSensorVrefintCmd(FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* ʹ���¶ȴ��������ڲ��ο���ѹͨ�� */
    ADC1->CR2 |= CR2_TSVREFE_Set;
  }
  else
  {
    /* ʧ���¶ȴ��������ڲ��ο���ѹͨ�� */
    ADC1->CR2 &= CR2_TSVREFE_Reset;
  }
}

/**
  * @����  ����ƶ� ADC ��־λ��1���.
  * @����  ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
  * @����  ADC_FLAG: ָ����Ҫ���ı�־. 
  *          �����������ʹ������ֵ��һ��:
  *          ADC_FLAG_AWD:   ģ�⿴�Ź���־λ
  *          ADC_FLAG_EOC:   ת��������־λ
  *          ADC_FLAG_JEOC:  ע����ת��������־λ
  *          ADC_FLAG_JSTRT: ע����ת����ʼ��־λ
  *          ADC_FLAG_STRT:  ������ת����ʼ��־λ
  * @����  ADC_FLAG ����״̬ (SET �� RESET).
  */
FlagStatus ADC_GetFlagStatus(ADC_TypeDef* ADCx, uint8_t ADC_FLAG)
{
  FlagStatus bitstatus = RESET;
  /* ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_GET_FLAG(ADC_FLAG));
  /* ���ָ��ADC״̬λ */
  if ((ADCx->SR & ADC_FLAG) != (uint8_t)RESET)
  {
    /* ���� ADC_FLAG λ */
    bitstatus = SET;
  }
  else
  {
    /* ��λ ADC_FLAG λ */
    bitstatus = RESET;
  }
  /* ���� ADC_FLAG ��״̬ */
  return  bitstatus;
}

/**
  * @����  ��� ADCx �Ĵ������־λ.
  * @����  ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
  * @����  ADC_FLAG: ������ı�־λ. 
  *          �����������������ֵ�����:
  *          ADC_FLAG_AWD:   ģ�⿴�Ź���־λ
  *          ADC_FLAG_EOC:   ת��������־λ
  *          ADC_FLAG_JEOC:  ע����ת��������־λ
  *          ADC_FLAG_JSTRT: ע����ת����ʼ��־λ
  *          ADC_FLAG_STRT:  ������ת����ʼ��־λ
  * @����  û��
  */
void ADC_ClearFlag(ADC_TypeDef* ADCx, uint8_t ADC_FLAG)
{
  /* ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_CLEAR_FLAG(ADC_FLAG));
  /* ���ָ���� ADC ��־ */
  ADCx->SR = ~(uint32_t)ADC_FLAG;
}

/**
  * @����  ���ָ����ADC�ж��Ƿ���.
  * @����  ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
  * @����  ADC_IT: ��Ҫ����ADC�ж�Դ. 
  *          �����������ʹ������ֵ��һ��:
  *          ADC_IT_EOC:  ת�������ж�����
  *          ADC_IT_AWD:  ģ�⿴�Ź��ж�����
  *          ADC_IT_JEOC: ע����ת�������ж�����
  * @����  ADC_IT ����״̬ (SET �� RESET).
  */
ITStatus ADC_GetITStatus(ADC_TypeDef* ADCx, uint16_t ADC_IT)
{
  ITStatus bitstatus = RESET;
  uint32_t itmask = 0, enablestatus = 0;
  /* ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_GET_IT(ADC_IT));
  /* �õ� ADC IT �жϺ� */
  itmask = ADC_IT >> 8;
  /* ���� ADC_IT ����λ״̬ */
  enablestatus = (ADCx->CR1 & (uint8_t)ADC_IT) ;
  /* ���ָ���� ADC �ж� */
  if (((ADCx->SR & itmask) != (uint32_t)RESET) && enablestatus)
  {
    /* ���� ADC_IT */
    bitstatus = SET;
  }
  else
  {
    /* ��λ ADC_IT */
    bitstatus = RESET;
  }
  /* ���� ADC_IT ��״̬ */
  return  bitstatus;
}

/**
  * @����  ��� ADCx ���жϴ�����λ.
  * @����  ADCx: ����x������1��2��3������ѡ��ADC��Χģ��.
  * @����  ADC_IT: ������� ADC �жϴ�����λ.
  *          ����ʹ������ֵ��һ�����߼���ֵ�����:
  *          ADC_IT_EOC:  ת�������ж�
  *          ADC_IT_AWD:  ģ�⿴�Ź��ж�
  *          ADC_IT_JEOC: ע����ת�������ж�
  * @����  û��
  */
void ADC_ClearITPendingBit(ADC_TypeDef* ADCx, uint16_t ADC_IT)
{
  uint8_t itmask = 0;
  /* ������ */
  assert_param(IS_ADC_ALL_PERIPH(ADCx));
  assert_param(IS_ADC_IT(ADC_IT));
  /* �õ� ADC IT �жϺ� */
  itmask = (uint8_t)(ADC_IT >> 8);
  /* ���ѡ���ADC�жϹ���λ */
  ADCx->SR = ~(uint32_t)itmask;
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
