/**
  ******************************************************************************
  * @�ļ�    stm32f10x_tim.c
  * @����    MCD Ӧ�ó�����
  * @�汾    V3.1.0
  * @����    06/19/2009
  * @����    ����ļ� �ṩ���� TIM �̼�����.
  * @����    ANSON/sweet1985 Email:airanson110@126.com
  * @����    10/07/2009
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
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"

/* �������� --------------------------------------------------------------*/
/* ���ö��� --------------------------------------------------------------*/

/* ---------------------- TIM registers bit mask ------------------------ */
#define CR1_CEN_Set                 ((uint16_t)0x0001)
#define CR1_CEN_Reset               ((uint16_t)0x03FE)
#define CR1_UDIS_Set                ((uint16_t)0x0002)
#define CR1_UDIS_Reset              ((uint16_t)0x03FD)
#define CR1_URS_Set                 ((uint16_t)0x0004)
#define CR1_URS_Reset               ((uint16_t)0x03FB)
#define CR1_OPM_Reset               ((uint16_t)0x03F7)
#define CR1_CounterMode_Mask        ((uint16_t)0x038F)
#define CR1_ARPE_Set                ((uint16_t)0x0080)
#define CR1_ARPE_Reset              ((uint16_t)0x037F)
#define CR1_CKD_Mask                ((uint16_t)0x00FF)
#define CR2_CCPC_Set                ((uint16_t)0x0001)
#define CR2_CCPC_Reset              ((uint16_t)0xFFFE)
#define CR2_CCUS_Set                ((uint16_t)0x0004)
#define CR2_CCUS_Reset              ((uint16_t)0xFFFB)
#define CR2_CCDS_Set                ((uint16_t)0x0008)
#define CR2_CCDS_Reset              ((uint16_t)0xFFF7)
#define CR2_MMS_Mask                ((uint16_t)0xFF8F)
#define CR2_TI1S_Set                ((uint16_t)0x0080)
#define CR2_TI1S_Reset              ((uint16_t)0xFF7F)
#define CR2_OIS1_Reset              ((uint16_t)0x7EFF)
#define CR2_OIS1N_Reset             ((uint16_t)0x7DFF)
#define CR2_OIS2_Reset              ((uint16_t)0x7BFF)
#define CR2_OIS2N_Reset             ((uint16_t)0x77FF)
#define CR2_OIS3_Reset              ((uint16_t)0x6FFF)
#define CR2_OIS3N_Reset             ((uint16_t)0x5FFF)
#define CR2_OIS4_Reset              ((uint16_t)0x3FFF)
#define SMCR_SMS_Mask               ((uint16_t)0xFFF8)
#define SMCR_ETR_Mask               ((uint16_t)0x00FF)
#define SMCR_TS_Mask                ((uint16_t)0xFF8F)
#define SMCR_MSM_Reset              ((uint16_t)0xFF7F)
#define SMCR_ECE_Set                ((uint16_t)0x4000)
#define CCMR_CC13S_Mask             ((uint16_t)0xFFFC)
#define CCMR_CC24S_Mask             ((uint16_t)0xFCFF)
#define CCMR_TI13Direct_Set         ((uint16_t)0x0001)
#define CCMR_TI24Direct_Set         ((uint16_t)0x0100)
#define CCMR_OC13FE_Reset           ((uint16_t)0xFFFB)
#define CCMR_OC24FE_Reset           ((uint16_t)0xFBFF)
#define CCMR_OC13PE_Reset           ((uint16_t)0xFFF7)
#define CCMR_OC24PE_Reset           ((uint16_t)0xF7FF)
#define CCMR_OC13M_Mask             ((uint16_t)0xFF8F)
#define CCMR_OC24M_Mask             ((uint16_t)0x8FFF) 
#define CCMR_OC13CE_Reset           ((uint16_t)0xFF7F)
#define CCMR_OC24CE_Reset           ((uint16_t)0x7FFF)
#define CCMR_IC13PSC_Mask           ((uint16_t)0xFFF3)
#define CCMR_IC24PSC_Mask           ((uint16_t)0xF3FF)
#define CCMR_IC13F_Mask             ((uint16_t)0xFF0F)
#define CCMR_IC24F_Mask             ((uint16_t)0x0FFF)
#define CCMR_Offset                 ((uint16_t)0x0018)
#define CCER_CCE_Set                ((uint16_t)0x0001)
#define	CCER_CCNE_Set               ((uint16_t)0x0004)
#define CCER_CC1P_Reset             ((uint16_t)0xFFFD)
#define CCER_CC2P_Reset             ((uint16_t)0xFFDF)
#define CCER_CC3P_Reset             ((uint16_t)0xFDFF)
#define CCER_CC4P_Reset             ((uint16_t)0xDFFF)
#define CCER_CC1NP_Reset            ((uint16_t)0xFFF7)
#define CCER_CC2NP_Reset            ((uint16_t)0xFF7F)
#define CCER_CC3NP_Reset            ((uint16_t)0xF7FF)
#define CCER_CC1E_Set               ((uint16_t)0x0001)
#define CCER_CC1E_Reset             ((uint16_t)0xFFFE)
#define CCER_CC1NE_Reset            ((uint16_t)0xFFFB)
#define CCER_CC2E_Set               ((uint16_t)0x0010)
#define CCER_CC2E_Reset             ((uint16_t)0xFFEF)
#define CCER_CC2NE_Reset            ((uint16_t)0xFFBF)
#define CCER_CC3E_Set               ((uint16_t)0x0100)
#define CCER_CC3E_Reset             ((uint16_t)0xFEFF)
#define CCER_CC3NE_Reset            ((uint16_t)0xFBFF)
#define CCER_CC4E_Set               ((uint16_t)0x1000)
#define CCER_CC4E_Reset             ((uint16_t)0xEFFF)
#define BDTR_MOE_Set                ((uint16_t)0x8000)
#define BDTR_MOE_Reset              ((uint16_t)0x7FFF)

/* ���ú� -------------------------------------------------------------*/
/* ���ñ��� -----------------------------------------------------------*/
/* ���ú���ԭ�� -------------------------------------------------------*/

static void TI1_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter);
static void TI2_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter);
static void TI3_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter);
static void TI4_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter);

/* ���ú��� -----------------------------------------------------------*/

/**
  * @����  ������ TIMx �Ĵ�������Ϊȱʡֵ.
  * @����  TIMx: �˴�x������1��8��ѡ������.
  * @����  û��
  */
void TIM_DeInit(TIM_TypeDef* TIMx)
{
  /* ������ */
  assert_param(IS_TIM_ALL_PERIPH(TIMx)); 
 
  if (TIMx == TIM1)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM1, DISABLE);  
  }     
  else if (TIMx == TIM2)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM2, DISABLE);
  }
  else if (TIMx == TIM3)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM3, DISABLE);
  }
  else if (TIMx == TIM4)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM4, DISABLE);
  } 
  else if (TIMx == TIM5)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM5, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM5, DISABLE);
  } 
  else if (TIMx == TIM6)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM6, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM6, DISABLE);
  } 
  else if (TIMx == TIM7)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM7, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM7, DISABLE);
  } 
  else
  {
    if (TIMx == TIM8)
    {
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM8, ENABLE);
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_TIM8, DISABLE);
    }  
  }
}

/**
  * @����  ���� TIM_TimeBaseInitStruct ��ָ���Ĳ�����ʼ�� TIMx ��ʱ�������λ.
  * @����  TIMx: ���� x ������ 1, 2, 3, 4, 5 �� 8 �趨 TIM ����.
  * @����  TIM_TimeBaseInitStruct: ָ��һ������ TIM ʱ���������Ϣ�� TIM_BaseInitTypeDef �ṹ��ָ��.
  * @����  û��
  */
void TIM_TimeBaseInit(TIM_TypeDef* TIMx, TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct)
{
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx)); 
  assert_param(IS_TIM_COUNTER_MODE(TIM_TimeBaseInitStruct->TIM_CounterMode));
  assert_param(IS_TIM_CKD_DIV(TIM_TimeBaseInitStruct->TIM_ClockDivision));
  /* ѡ�������ģʽ ������ʱ��Ƶ�� */
  TIMx->CR1 &= CR1_CKD_Mask & CR1_CounterMode_Mask;
  TIMx->CR1 |= (uint32_t)TIM_TimeBaseInitStruct->TIM_ClockDivision |
                TIM_TimeBaseInitStruct->TIM_CounterMode;
  
  /* �����Զ�װ��ֵ */
  TIMx->ARR = TIM_TimeBaseInitStruct->TIM_Period ;
 
  /* ����Ԥ��Ƶ��ֵ */
  TIMx->PSC = TIM_TimeBaseInitStruct->TIM_Prescaler;
    
  if ((((uint32_t) TIMx) == TIM1_BASE) || (((uint32_t) TIMx) == TIM8_BASE))  
  {
    /* �����ظ�������ֵ */
    TIMx->RCR = TIM_TimeBaseInitStruct->TIM_RepetitionCounter;
  }

  /* ����һ�������¼���������װ��Ԥ��Ƶ��ֵ */
  TIMx->EGR = TIM_PSCReloadMode_Immediate;          
}

/**
  * @����  ���� TIM_OCInitStruct �е��ض�������ʼ��TIMxͨ��1.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_OCInitStruct: ָ��һ�������ض�TIMx�����������Ϣ�� TIM_OCInitTypeDef �ṹ��ָ��.
  * @����  û��
  */
void TIM_OC1Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  uint16_t tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;
   
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx)); 
  assert_param(IS_TIM_OC_MODE(TIM_OCInitStruct->TIM_OCMode));
  assert_param(IS_TIM_OUTPUT_STATE(TIM_OCInitStruct->TIM_OutputState));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCInitStruct->TIM_OCPolarity));   
  /* ʧ��ͨ�� 1: ��λ CC1E λ */
  TIMx->CCER &= CCER_CC1E_Reset;
  
  /* �õ� TIMx CCER(����/�Ƚ�ʹ��) �Ĵ�����ֵ */
  tmpccer = TIMx->CCER;
  /* �õ� TIMx CR2(���ƼĴ���2) �Ĵ�����ֵ */
  tmpcr2 =  TIMx->CR2;
  
  /* �õ� TIMx CCMR1(����/�Ƚ�ģʽ) �Ĵ�����ֵ */
  tmpccmrx = TIMx->CCMR1;
    
  /* ��λ����Ƚ�ģʽλ */
  tmpccmrx &= CCMR_OC13M_Mask;
  
  /* ѡ������Ƚ�ģʽ */
  tmpccmrx |= TIM_OCInitStruct->TIM_OCMode;
  
  /* ��λ������Եȼ� */
  tmpccer &= CCER_CC1P_Reset;
  /* ��������Ƚϼ��� */
  tmpccer |= TIM_OCInitStruct->TIM_OCPolarity;
  
  /* �������״̬ */
  tmpccer |= TIM_OCInitStruct->TIM_OutputState;
  
  /* ���ò���ȽϼĴ���ֵ */
  TIMx->CCR1 = TIM_OCInitStruct->TIM_Pulse;
  
  if(((uint32_t) TIMx == TIM1_BASE) || ((uint32_t) TIMx == TIM8_BASE))
  {
    assert_param(IS_TIM_OUTPUTN_STATE(TIM_OCInitStruct->TIM_OutputNState));
    assert_param(IS_TIM_OCN_POLARITY(TIM_OCInitStruct->TIM_OCNPolarity));
    assert_param(IS_TIM_OCNIDLE_STATE(TIM_OCInitStruct->TIM_OCNIdleState));
    assert_param(IS_TIM_OCIDLE_STATE(TIM_OCInitStruct->TIM_OCIdleState));
    
    /* ��λ��� N ���Եȼ� */
    tmpccer &= CCER_CC1NP_Reset;
    /* ������� N ���� */
    tmpccer |= TIM_OCInitStruct->TIM_OCNPolarity;
    /* ��λ��� N ״̬ */
    tmpccer &= CCER_CC1NE_Reset;
    
    /* ������� N ״̬ */
    tmpccer |= TIM_OCInitStruct->TIM_OutputNState;
    /* ��λ����Ƚϣ�����Ƚ� N ����״̬ */
    tmpcr2 &= CR2_OIS1_Reset;
    tmpcr2 &= CR2_OIS1N_Reset;
    /* �����������״̬ */
    tmpcr2 |= TIM_OCInitStruct->TIM_OCIdleState;
    /* ������� N ����״̬ */
    tmpcr2 |= TIM_OCInitStruct->TIM_OCNIdleState;
  }
  /* д TIMx CR2(���ƼĴ���2) */
  TIMx->CR2 = tmpcr2;
  
  /* д TIMx CCMR1(����/�Ƚ�ģʽ�Ĵ���1) */
  TIMx->CCMR1 = tmpccmrx;
  
  /* д TIMx CCER(����/�Ƚ�ʹ�ܼĴ���) */
  TIMx->CCER = tmpccer;
}

/**
  * @����  ���� TIM_OCInitStruct �е��ض�������ʼ��TIMxͨ��2.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_OCInitStruct: ָ��һ�������ض�TIMx�����������Ϣ�� TIM_OCInitTypeDef �ṹ��ָ��.
  * @����  û��
  */
void TIM_OC2Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  uint16_t tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;
   
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx)); 
  assert_param(IS_TIM_OC_MODE(TIM_OCInitStruct->TIM_OCMode));
  assert_param(IS_TIM_OUTPUT_STATE(TIM_OCInitStruct->TIM_OutputState));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCInitStruct->TIM_OCPolarity));   
  /* ʧ��ͨ�� 2: ��λ CC2E λ */
  TIMx->CCER &= CCER_CC2E_Reset;
  
  /* �õ� TIMx CCER(����/�Ƚ�ʹ��) �Ĵ�����ֵ */  
  tmpccer = TIMx->CCER;
  /* �õ� TIMx CR2(���ƼĴ���2) �Ĵ�����ֵ */
  tmpcr2 =  TIMx->CR2;
  
  /* �õ� TIMx CCMR1(����/�Ƚ�ģʽ) �Ĵ�����ֵ */
  tmpccmrx = TIMx->CCMR1;
    
  /* ��λ����Ƚ�ģʽλ */
  tmpccmrx &= CCMR_OC24M_Mask;
  
  /* ѡ������Ƚ�ģʽ */
  tmpccmrx |= (uint16_t)(TIM_OCInitStruct->TIM_OCMode << 8);
  
  /* ��λ������Եȼ� */
  tmpccer &= CCER_CC2P_Reset;
  /* ��������Ƚϼ��� */
  tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OCPolarity << 4);
  
  /* �������״̬ */
  tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OutputState << 4);
  
  /* ���ò���ȽϼĴ���ֵ */
  TIMx->CCR2 = TIM_OCInitStruct->TIM_Pulse;
  
  if(((uint32_t) TIMx == TIM1_BASE) || ((uint32_t) TIMx == TIM8_BASE))
  {
    assert_param(IS_TIM_OUTPUTN_STATE(TIM_OCInitStruct->TIM_OutputNState));
    assert_param(IS_TIM_OCN_POLARITY(TIM_OCInitStruct->TIM_OCNPolarity));
    assert_param(IS_TIM_OCNIDLE_STATE(TIM_OCInitStruct->TIM_OCNIdleState));
    assert_param(IS_TIM_OCIDLE_STATE(TIM_OCInitStruct->TIM_OCIdleState));
    
    /* ��λ��� N ���Եȼ� */
    tmpccer &= CCER_CC2NP_Reset;
    /* ������� N ���� */
    tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OCNPolarity << 4);
    /* ��λ��� N ״̬ */
    tmpccer &= CCER_CC2NE_Reset;
    
    /* ������� N ״̬ */
    tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OutputNState << 4);
    /* ��λ����Ƚϣ�����Ƚ� N ����״̬ */
    tmpcr2 &= CR2_OIS2_Reset;
    tmpcr2 &= CR2_OIS2N_Reset;
    /* �����������״̬ */
    tmpcr2 |= (uint16_t)(TIM_OCInitStruct->TIM_OCIdleState << 2);
    /* ������� N ����״̬ */
    tmpcr2 |= (uint16_t)(TIM_OCInitStruct->TIM_OCNIdleState << 2);
  }
  /* д TIMx CR2(���ƼĴ���2) */
  TIMx->CR2 = tmpcr2;
  
  /* д TIMx CCMR1(����/�Ƚ�ģʽ�Ĵ���1) */
  TIMx->CCMR1 = tmpccmrx;
  
  /* д TIMx CCER(����/�Ƚ�ʹ�ܼĴ���) */
  TIMx->CCER = tmpccer;
}

/**
  * @����  ���� TIM_OCInitStruct �е��ض�������ʼ��TIMxͨ��3.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_OCInitStruct: ָ��һ�������ض�TIMx�����������Ϣ�� TIM_OCInitTypeDef �ṹ��ָ��.
  * @����  û��
  */
void TIM_OC3Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  uint16_t tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;
   
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx)); 
  assert_param(IS_TIM_OC_MODE(TIM_OCInitStruct->TIM_OCMode));
  assert_param(IS_TIM_OUTPUT_STATE(TIM_OCInitStruct->TIM_OutputState));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCInitStruct->TIM_OCPolarity));   
  /* ʧ�� 3: ��λ CC3E λ */
  TIMx->CCER &= CCER_CC3E_Reset;
  
  /* �õ� TIMx CCER(����/�Ƚ�ʹ��) �Ĵ�����ֵ */
  tmpccer = TIMx->CCER;
  /* �õ� TIMx CR2(���ƼĴ���2) �Ĵ�����ֵ */
  tmpcr2 =  TIMx->CR2;
  
  /* �õ� TIMx CCMR2(����/�Ƚ�ģʽ) �Ĵ�����ֵ */
  tmpccmrx = TIMx->CCMR2;
    
  /* ��λ����Ƚ�ģʽλ */
  tmpccmrx &= CCMR_OC13M_Mask;
  
  /* ѡ������Ƚ�ģʽ */
  tmpccmrx |= TIM_OCInitStruct->TIM_OCMode;
  
  /* ��λ������Եȼ� */
  tmpccer &= CCER_CC3P_Reset;
  /* ��������Ƚϼ��� */
  tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OCPolarity << 8);
  
  /* �������״̬ */
  tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OutputState << 8);
  
  /* ���ò���ȽϼĴ���ֵ */
  TIMx->CCR3 = TIM_OCInitStruct->TIM_Pulse;
  
  if(((uint32_t) TIMx == TIM1_BASE) || ((uint32_t) TIMx == TIM8_BASE))
  {
    assert_param(IS_TIM_OUTPUTN_STATE(TIM_OCInitStruct->TIM_OutputNState));
    assert_param(IS_TIM_OCN_POLARITY(TIM_OCInitStruct->TIM_OCNPolarity));
    assert_param(IS_TIM_OCNIDLE_STATE(TIM_OCInitStruct->TIM_OCNIdleState));
    assert_param(IS_TIM_OCIDLE_STATE(TIM_OCInitStruct->TIM_OCIdleState));
    
    /* ��λ��� N ���Եȼ� */
    tmpccer &= CCER_CC3NP_Reset;
    /* ������� N ���� */
    tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OCNPolarity << 8);
    /* ��λ��� N ״̬ */
    tmpccer &= CCER_CC3NE_Reset;
    
    /* ������� N ״̬ */
    tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OutputNState << 8);
    /* ��λ����Ƚϣ�����Ƚ� N ����״̬ */
    tmpcr2 &= CR2_OIS3_Reset;
    tmpcr2 &= CR2_OIS3N_Reset;
    /* �����������״̬ */
    tmpcr2 |= (uint16_t)(TIM_OCInitStruct->TIM_OCIdleState << 4);
    /* ������� N ����״̬ */
    tmpcr2 |= (uint16_t)(TIM_OCInitStruct->TIM_OCNIdleState << 4);
  }
  /* д TIMx CR2(���ƼĴ���2) */
  TIMx->CR2 = tmpcr2;
  
  /* д TIMx CCMR2(����/�Ƚ�ģʽ�Ĵ���2) */
  TIMx->CCMR2 = tmpccmrx;
  
  /* д TIMx CCER(����/�Ƚ�ʹ�ܼĴ���) */
  TIMx->CCER = tmpccer;
}

/**
  * @����  ���� TIM_OCInitStruct �е��ض�������ʼ��TIMxͨ��4.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_OCInitStruct: ָ��һ�������ض�TIMx�����������Ϣ�� TIM_OCInitTypeDef �ṹ��ָ��.
  * @����  û��
  */
void TIM_OC4Init(TIM_TypeDef* TIMx, TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  uint16_t tmpccmrx = 0, tmpccer = 0, tmpcr2 = 0;
   
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx)); 
  assert_param(IS_TIM_OC_MODE(TIM_OCInitStruct->TIM_OCMode));
  assert_param(IS_TIM_OUTPUT_STATE(TIM_OCInitStruct->TIM_OutputState));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCInitStruct->TIM_OCPolarity));   
  /* ʧ��ͨ�� 4: ��λ CC4E λ */
  TIMx->CCER &= CCER_CC4E_Reset;
  
  /* �õ� TIMx CCER(����/�Ƚ�ʹ��) �Ĵ�����ֵ */
  tmpccer = TIMx->CCER;
  /* �õ� TIMx CR2(���ƼĴ���2) �Ĵ�����ֵ */
  tmpcr2 =  TIMx->CR2;
  
  /* �õ� TIMx CCMR2(����/�Ƚ�ģʽ) �Ĵ�����ֵ */
  tmpccmrx = TIMx->CCMR2;
    
  /* ��λ����Ƚ�ģʽλ */
  tmpccmrx &= CCMR_OC24M_Mask;
  
  /* ѡ������Ƚ�ģʽ */
  tmpccmrx |= (uint16_t)(TIM_OCInitStruct->TIM_OCMode << 8);
  
  /* ��λ������Եȼ� */
  tmpccer &= CCER_CC4P_Reset;
  /* ��������Ƚϼ��� */
  tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OCPolarity << 12);
  
  /* �������״̬ */
  tmpccer |= (uint16_t)(TIM_OCInitStruct->TIM_OutputState << 12);
  
  /* ���ò���ȽϼĴ���ֵ */
  TIMx->CCR4 = TIM_OCInitStruct->TIM_Pulse;
  
  if(((uint32_t) TIMx == TIM1_BASE) || ((uint32_t) TIMx == TIM8_BASE))
  {
    assert_param(IS_TIM_OCIDLE_STATE(TIM_OCInitStruct->TIM_OCIdleState));
    /* ��λ����ȽϿ���״̬ */
    tmpcr2 &= CR2_OIS4_Reset;
    /* �����������״̬ */
    tmpcr2 |= (uint16_t)(TIM_OCInitStruct->TIM_OCIdleState << 6);
  }
  /* д TIMx CR2(���ƼĴ���2) */
  TIMx->CR2 = tmpcr2;
  
  /* д TIMx CCMR2(����/�Ƚ�ģʽ�Ĵ���2) */  
  TIMx->CCMR2 = tmpccmrx;
  
  /* д TIMx CCER(����/�Ƚ�ʹ�ܼĴ���) */
  TIMx->CCER = tmpccer;
}

/**
  * @����  ���� TIM_ ICInitStruct �е��ض�������ʼ�� TIMx.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_ICInitStruct: ָ��һ�������ض� TIMx �����������Ϣ�� TIM_OCInitTypeDef �ṹ��ָ��.
  * @����  û��
  */
void TIM_ICInit(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct)
{
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_CHANNEL(TIM_ICInitStruct->TIM_Channel));
  assert_param(IS_TIM_IC_POLARITY(TIM_ICInitStruct->TIM_ICPolarity));
  assert_param(IS_TIM_IC_SELECTION(TIM_ICInitStruct->TIM_ICSelection));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICInitStruct->TIM_ICPrescaler));
  assert_param(IS_TIM_IC_FILTER(TIM_ICInitStruct->TIM_ICFilter));
  
  if (TIM_ICInitStruct->TIM_Channel == TIM_Channel_1)
  {
    /* TI1 ���� */
    TI1_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity,
               TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);
    /* �������벶׽Ԥ��Ƶ��ֵ */
    TIM_SetIC1Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
  else if (TIM_ICInitStruct->TIM_Channel == TIM_Channel_2)
  {
    /* TI2 ���� */
    TI2_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity,
               TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);
    /* �������벶׽Ԥ��Ƶ��ֵ */
    TIM_SetIC2Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
  else if (TIM_ICInitStruct->TIM_Channel == TIM_Channel_3)
  {
    /* TI3 ���� */
    TI3_Config(TIMx,  TIM_ICInitStruct->TIM_ICPolarity,
               TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);
    /* �������벶׽Ԥ��Ƶ��ֵ */
    TIM_SetIC3Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
  else
  {
    /* TI4 ���� */
    TI4_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity,
               TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);
    /* �������벶׽Ԥ��Ƶ��ֵ */
    TIM_SetIC4Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
}

/**
  * @����  ����TIM_ICInitStructָ���Ĳ�������TIM�豸ȥ�����ⲿ��PWM�ź�.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_ICInitStruct: ָ��һ�������ض� TIMx �����������Ϣ�� TIM_ICInitTypeDef �ṹ��ָ��.
  * @����  û��
  */
void TIM_PWMIConfig(TIM_TypeDef* TIMx, TIM_ICInitTypeDef* TIM_ICInitStruct)
{
  uint16_t icoppositepolarity = TIM_ICPolarity_Rising;
  uint16_t icoppositeselection = TIM_ICSelection_DirectTI;
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  /* ѡ���෴�����뼫�� */
  if (TIM_ICInitStruct->TIM_ICPolarity == TIM_ICPolarity_Rising)
  {
    icoppositepolarity = TIM_ICPolarity_Falling;
  }
  else
  {
    icoppositepolarity = TIM_ICPolarity_Rising;
  }
  /* ѡ���෴������ */
  if (TIM_ICInitStruct->TIM_ICSelection == TIM_ICSelection_DirectTI)
  {
    icoppositeselection = TIM_ICSelection_IndirectTI;
  }
  else
  {
    icoppositeselection = TIM_ICSelection_DirectTI;
  }
  if (TIM_ICInitStruct->TIM_Channel == TIM_Channel_1)
  {
    /* TI1 ���� */
    TI1_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity, TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);
    /* �������벶׽Ԥ��Ƶ��ֵ */
    TIM_SetIC1Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
    /* TI2 ���� */
    TI2_Config(TIMx, icoppositepolarity, icoppositeselection, TIM_ICInitStruct->TIM_ICFilter);
    /* �������벶׽Ԥ��Ƶ��ֵ */
    TIM_SetIC2Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
  else
  { 
    /* TI2 ���� */
    TI2_Config(TIMx, TIM_ICInitStruct->TIM_ICPolarity, TIM_ICInitStruct->TIM_ICSelection,
               TIM_ICInitStruct->TIM_ICFilter);
    /* �������벶׽Ԥ��Ƶ��ֵ */
    TIM_SetIC2Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
    /* TI1 ���� */
    TI1_Config(TIMx, icoppositepolarity, icoppositeselection, TIM_ICInitStruct->TIM_ICFilter);
    /* �������벶׽Ԥ��Ƶ��ֵ */
    TIM_SetIC1Prescaler(TIMx, TIM_ICInitStruct->TIM_ICPrescaler);
  }
}

/**
  * @����  ����: ����ɲ�����ԣ�����ʱ�䣬����ƽ��OSSI��OSSR״̬��AOE(�Զ����ʹ��).
  * @����  TIMx: �˴�x������1��8��ѡ������
  * @����  TIM_BDTRInitStruct: ָ��һ�������ض� TIMx �����������Ϣ�� TIM_BDTRInitTypeDef �ṹ��ָ��.
  * @����  û��
  */
void TIM_BDTRConfig(TIM_TypeDef* TIMx, TIM_BDTRInitTypeDef *TIM_BDTRInitStruct)
{
  /* ������ */
  assert_param(IS_TIM_18_PERIPH(TIMx));
  assert_param(IS_TIM_OSSR_STATE(TIM_BDTRInitStruct->TIM_OSSRState));
  assert_param(IS_TIM_OSSI_STATE(TIM_BDTRInitStruct->TIM_OSSIState));
  assert_param(IS_TIM_LOCK_LEVEL(TIM_BDTRInitStruct->TIM_LOCKLevel));
  assert_param(IS_TIM_BREAK_STATE(TIM_BDTRInitStruct->TIM_Break));
  assert_param(IS_TIM_BREAK_POLARITY(TIM_BDTRInitStruct->TIM_BreakPolarity));
  assert_param(IS_TIM_AUTOMATIC_OUTPUT_STATE(TIM_BDTRInitStruct->TIM_AutomaticOutput));
  /* ���ã�����ƽ���𣬼������λ�ͼ��ԣ�OSSR״̬��OSSI״̬���ͺ�ʱ����Զ����ʹ��λ */
  TIMx->BDTR = (uint32_t)TIM_BDTRInitStruct->TIM_OSSRState | TIM_BDTRInitStruct->TIM_OSSIState |
             TIM_BDTRInitStruct->TIM_LOCKLevel | TIM_BDTRInitStruct->TIM_DeadTime |
             TIM_BDTRInitStruct->TIM_Break | TIM_BDTRInitStruct->TIM_BreakPolarity |
             TIM_BDTRInitStruct->TIM_AutomaticOutput;
}

/**
  * @����  �� TIM1_TimeBaseInitStruct �е�ÿһ��������ȱʡֵ����.
  * @����  TIM_TimeBaseInitStruct : ָ��һ��������ʼ���� TIM_TimeBaseInitTypeDef �ṹ��ָ��.
  * @����  û��
  */
void TIM_TimeBaseStructInit(TIM_TimeBaseInitTypeDef* TIM_TimeBaseInitStruct)
{
  /* ���ó�Ĭ������ */
  TIM_TimeBaseInitStruct->TIM_Period = 0xFFFF;
  TIM_TimeBaseInitStruct->TIM_Prescaler = 0x0000;
  TIM_TimeBaseInitStruct->TIM_ClockDivision = TIM_CKD_DIV1;
  TIM_TimeBaseInitStruct->TIM_CounterMode = TIM_CounterMode_Up;
  TIM_TimeBaseInitStruct->TIM_RepetitionCounter = 0x0000;
}

/**
  * @����  �� TIM1_OCInitStruct �е�ÿһ��������ȱʡֵ����.
  * @����  TIM_OCInitStruct : ָ��һ��������ʼ���� TIM_OCInitStruct �ṹ��ָ��.
  * @����  û��
  */
void TIM_OCStructInit(TIM_OCInitTypeDef* TIM_OCInitStruct)
{
  /* ���ó�Ĭ������ */
  TIM_OCInitStruct->TIM_OCMode = TIM_OCMode_Timing;
  TIM_OCInitStruct->TIM_OutputState = TIM_OutputState_Disable;
  TIM_OCInitStruct->TIM_OutputNState = TIM_OutputNState_Disable;
  TIM_OCInitStruct->TIM_Pulse = 0x0000;
  TIM_OCInitStruct->TIM_OCPolarity = TIM_OCPolarity_High;
  TIM_OCInitStruct->TIM_OCNPolarity = TIM_OCPolarity_High;
  TIM_OCInitStruct->TIM_OCIdleState = TIM_OCIdleState_Reset;
  TIM_OCInitStruct->TIM_OCNIdleState = TIM_OCNIdleState_Reset;
}

/**
  * @����  �� TIM1_ICInitStruct �е�ÿһ��������ȱʡֵ����.
  * @����  TIM_ICInitStruct : ָ��һ��������ʼ���� TIM_ICInitStruct �ṹ��ָ��.
  * @����  û��
  */
void TIM_ICStructInit(TIM_ICInitTypeDef* TIM_ICInitStruct)
{
  /* ���ó�Ĭ������ */
  TIM_ICInitStruct->TIM_Channel = TIM_Channel_1;
  TIM_ICInitStruct->TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStruct->TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStruct->TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStruct->TIM_ICFilter = 0x00;
}

/**
  * @����  �� TIM1_BDTRInitStruct �е�ÿһ��������ȱʡֵ����.
  * @����  TIM_BDTRInitStruct: ָ��һ��������ʼ���� TIM1_BDTRInitStruct �ṹ��.
  * @����  û��
  */
void TIM_BDTRStructInit(TIM_BDTRInitTypeDef* TIM_BDTRInitStruct)
{
  /* ���ó�Ĭ������ */
  TIM_BDTRInitStruct->TIM_OSSRState = TIM_OSSRState_Disable;
  TIM_BDTRInitStruct->TIM_OSSIState = TIM_OSSIState_Disable;
  TIM_BDTRInitStruct->TIM_LOCKLevel = TIM_LOCKLevel_OFF;
  TIM_BDTRInitStruct->TIM_DeadTime = 0x00;
  TIM_BDTRInitStruct->TIM_Break = TIM_Break_Disable;
  TIM_BDTRInitStruct->TIM_BreakPolarity = TIM_BreakPolarity_Low;
  TIM_BDTRInitStruct->TIM_AutomaticOutput = TIM_AutomaticOutput_Disable;
}

/**
  * @����  ʹ�ܻ���ʧ��ָ���� TIM ����.
  * @����  TIMx: �˴�x������1��8��ѡ������.
  * @����  NewState: TIMx �������״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void TIM_Cmd(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* ʹ�� TIM ������ */
    TIMx->CR1 |= CR1_CEN_Set;
  }
  else
  {
    /* ʧ�� TIM ������ */
    TIMx->CR1 &= CR1_CEN_Reset;
  }
}

/**
  * @����  ʹ�ܻ�ʧ�� TIM ��Χ�����.
  * @����  TIMx: �˴�x������1��8��ѡ������.
  * @����  NewState: TIM ��Χ���������״̬.
  *                  �������������: ENABLE or DISABLE.
  * @����  û��
  */
void TIM_CtrlPWMOutputs(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_TIM_18_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* ʹ�� TIM ����� */
    TIMx->BDTR |= BDTR_MOE_Set;
  }
  else
  {
    /* ʧ�� TIM ����� */
    TIMx->BDTR &= BDTR_MOE_Reset;
  }  
}

/**
  * @����  ʹ�ܻ�ʧ��ָ���� TIM �ж�.
  * @����  TIMx: �˴�x������1��8��ѡ������.
  * @����  TIM_IT: ����ʹ�ܻ�ʧ�ܵ� TIM �ж�Դ.
  *                �����������ȡ����ֵ֮һ:
  *                TIM_IT_Update:  TIM �����ж�Դ
  *                TIM_IT_CC1:     TIM ��/�Ƚ� 1 �ж�Դ
  *                TIM_IT_CC2:     TIM ��/�Ƚ� 2 �ж�Դ
  *                TIM_IT_CC3:     TIM ��/�Ƚ� 3 �ж�Դ
  *                TIM_IT_CC4:     TIM ��/�Ƚ� 4 �ж�Դ
  *                TIM_IT_COM:     TIM COM�ж�Դ
  *                TIM_IT_Trigger: TIM �����ж�Դ
  *                TIM_IT_Break:   TIM ɲ���ж�Դ
  * @ע�� 
  *   - TIM6 �� TIM7 ֻ�ܲ���У���ж�. 
  *   - TIM_IT_COM �� TIM_IT_Break ֻ�ܺ� TIM1 �� TIM8 һ��ʹ��.  
  * @����  NewState: TIM �жϵ���״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void TIM_ITConfig(TIM_TypeDef* TIMx, uint16_t TIM_IT, FunctionalState NewState)
{  
  /* ������s */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_IT(TIM_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* ʹ���ж�Դ */
    TIMx->DIER |= TIM_IT;
  }
  else
  {
    /* ʧ���ж�Դ */
    TIMx->DIER &= (uint16_t)~TIM_IT;
  }
}

/**
  * @����  ���ý������������ TIM �¼�.
  * @����  TIMx: �˴�x������1��8��ѡ������.
  * @����  TIM_EventSource: ָ���¼�Դ.
  *                         �����������ȡ����һ��������ֵ:	   
  *          TIM_EventSource_Update: ��ʱ�������¼�Դ
  *          TIM_EventSource_CC1:     ��ʱ����׽�Ƚ� 1 �¼�Դ
  *          TIM_EventSource_CC2:     ��ʱ����׽�Ƚ� 2 �¼�Դ
  *          TIM_EventSource_CC3:     ��ʱ����׽�Ƚ� 3 �¼�Դ
  *          TIM_EventSource_CC4:     ��ʱ����׽�Ƚ� 4 �¼�Դ
  *          TIM_EventSource_COM:     ��ʱ�� COM �¼�Դ  
  *          TIM_EventSource_Trigger: ��ʱ���������¼�Դ
  *          TIM_EventSource_Break:   ��ʱ��ɲ���¼�Դ
  * @ע�� 
  *   - TIM6 �� TIM7 ֻ�ܲ���У���ж�. 
  *   - TIM_IT_COM �� TIM_IT_Break ֻ�ܺ� TIM1 �� TIM8 һ��ʹ��. 
  * @����  û��
  */
void TIM_GenerateEvent(TIM_TypeDef* TIMx, uint16_t TIM_EventSource)
{ 
  /* ������ */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_EVENT_SOURCE(TIM_EventSource));
  
  /* �����¼�Դ */
  TIMx->EGR = TIM_EventSource;
}

/**
  * @����  ���� TIMx ��DMA�ӿ�.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_DMABase: DMA ����ַ.
  *                     �����������ȡ�����ֵ֮һ:
  *   TIM_DMABase_CR, TIM_DMABase_CR2, TIM_DMABase_SMCR,
  *   TIM_DMABase_DIER, TIM1_DMABase_SR, TIM_DMABase_EGR,
  *   TIM_DMABase_CCMR1, TIM_DMABase_CCMR2, TIM_DMABase_CCER,
  *   TIM_DMABase_CNT, TIM_DMABase_PSC, TIM_DMABase_ARR,
  *   TIM_DMABase_RCR, TIM_DMABase_CCR1, TIM_DMABase_CCR2,
  *   TIM_DMABase_CCR3, TIM_DMABase_CCR4, TIM_DMABase_BDTR,
  *   TIM_DMABase_DCR.
  * @����  TIM_DMABurstLength: DMA ���崮����.
  *                            �����������ȡ��������ֵ֮һ:
  *        TIM_DMABurstLength_1Byte �� TIM_DMABurstLength_18Bytes.
  * @����  û��
  */
void TIM_DMAConfig(TIM_TypeDef* TIMx, uint16_t TIM_DMABase, uint16_t TIM_DMABurstLength)
{
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_DMA_BASE(TIM_DMABase));
  assert_param(IS_TIM_DMA_LENGTH(TIM_DMABurstLength));
  /* ���� DMA ����ַ�� DMA ���崮���� */
  TIMx->DCR = TIM_DMABase | TIM_DMABurstLength;
}

/**
  * @����  ʹ�ܻ�ʧ�� TIMx �� DMA ����.
  * @����  TIMx: �˴�x������1��8��ѡ������. 
  * @����  TIM_DMASource: ָ�� DMA ������Դ.
  *                       ������������������κ�ֵ�����:
  *                       TIM_DMA_Update:  TIM �����ж�Դ
  *                       TIM_DMA_CC1:     TIM ����Ƚ� 1 DMA Դ
  *                       TIM_DMA_CC2:     TIM ����Ƚ� 2 DMA Դ
  *                       TIM_DMA_CC3:     TIM ����Ƚ� 3 DMA Դ
  *                       TIM_DMA_CC4:     TIM ����Ƚ� 4 DMA Դ
  *                       TIM_DMA_COM:     TIM COM DMA Դ
  *                       TIM_DMA_Trigger: TIM ���� DMA Դ
  * @����  NewState: DMA ������״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void TIM_DMACmd(TIM_TypeDef* TIMx, uint16_t TIM_DMASource, FunctionalState NewState)
{ 
  /* ������ */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_DMA_SOURCE(TIM_DMASource));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* ʹ�� DMA Դ */
    TIMx->DIER |= TIM_DMASource; 
  }
  else
  {
    /* ʧ�� DMA Դ */
    TIMx->DIER &= (uint16_t)~TIM_DMASource;
  }
}

/**
  * @����  ���� TIMx �ڲ�ʱ��
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  û��
  */
void TIM_InternalClockConfig(TIM_TypeDef* TIMx)
{
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  /* ʧ���ڲ�ʱ��Ԥ��Ƶ����ģʽ */
  TIMx->SMCR &=  SMCR_SMS_Mask;
}

/**
  * @����  ���� TIMx �ڲ�������Ϊ�ⲿʱ��ģʽ
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_ITRSource: ������.
  *                       �����������ȡ�����ֵ֮һ:
  *                       TIM_TS_ITR0: �ڲ������� 0
  *                       TIM_TS_ITR1: �ڲ������� 1
  *                       TIM_TS_ITR2: �ڲ������� 2
  *                       TIM_TS_ITR3: �ڲ������� 3
  * @����  û��
  */
void TIM_ITRxExternalClockConfig(TIM_TypeDef* TIMx, uint16_t TIM_InputTriggerSource)
{
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_INTERNAL_TRIGGER_SELECTION(TIM_InputTriggerSource));
  /* ѡ���ڲ������� */
  TIM_SelectInputTrigger(TIMx, TIM_InputTriggerSource);
  /* ѡ���ⲿʱ��ģʽ1 */
  TIMx->SMCR |= TIM_SlaveMode_External1;
}

/**
  * @����  ���� TIMx ����Ϊ�ⲿʱ��
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_TIxExternalCLKSource: ������.
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_TIxExternalCLK1Source_TI1ED: TI1 ��Ե�����
  *          TIM_TIxExternalCLK1Source_TI1: ��ɸѡ�Ķ�ʱ������ 1
  *          TIM_TIxExternalCLK1Source_TI2: ��ɸѡ�Ķ�ʱ������ 2
  * @����  TIM_ICPolarity: ָ�� TIx ����.
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_ICPolarity_Rising
  *          TIM_ICPolarity_Falling
  * @����  ICFilter : ָ�����벶�������.
  *                   �ò���������0x0��0xF֮���ֵ.
  * @����  û��
  */
void TIM_TIxExternalClockConfig(TIM_TypeDef* TIMx, uint16_t TIM_TIxExternalCLKSource,
                                uint16_t TIM_ICPolarity, uint16_t ICFilter)
{
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_TIXCLK_SOURCE(TIM_TIxExternalCLKSource));
  assert_param(IS_TIM_IC_POLARITY(TIM_ICPolarity));
  assert_param(IS_TIM_IC_FILTER(ICFilter));
  /* ���ö�ʱ������ʱ��Դ */
  if (TIM_TIxExternalCLKSource == TIM_TIxExternalCLK1Source_TI2)
  {
    TI2_Config(TIMx, TIM_ICPolarity, TIM_ICSelection_DirectTI, ICFilter);
  }
  else
  {
    TI1_Config(TIMx, TIM_ICPolarity, TIM_ICSelection_DirectTI, ICFilter);
  }
  /* ѡ�񴥷��� */
  TIM_SelectInputTrigger(TIMx, TIM_TIxExternalCLKSource);
  /* ѡ���ⲿʱ��ģʽ1 */
  TIMx->SMCR |= TIM_SlaveMode_External1;
}

/**
  * @����  �����ⲿʱ��ģʽ1
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_ExtTRGPrescaler: �ⲿ������Ԥ��Ƶ��.
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_ExtTRGPSC_OFF:  ETRP Ԥ��Ƶ�� ��.
  *          TIM_ExtTRGPSC_DIV2: ETRP Ƶ�ʳ��� 2.
  *          TIM_ExtTRGPSC_DIV4: ETRP Ƶ�ʳ��� 4.
  *          TIM_ExtTRGPSC_DIV8: ETRP Ƶ�ʳ��� 8.
  * @����  TIM_ExtTRGPolarity: �ⲿ����������.
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_ExtTRGPolarity_Inverted:    �͵�ƽ���½���.
  *          TIM_ExtTRGPolarity_NonInverted: �ߵ�ƽ��������.
  * @����  ExtTRGFilter: �ⲿ������������.
  *          �ò���������0x0��0xF֮���ֵ
  * @����  û��
  */ 
void TIM_ETRClockMode1Config(TIM_TypeDef* TIMx, uint16_t TIM_ExtTRGPrescaler, uint16_t TIM_ExtTRGPolarity,
                             uint16_t ExtTRGFilter)
{
  uint16_t tmpsmcr = 0;
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_EXT_PRESCALER(TIM_ExtTRGPrescaler));
  assert_param(IS_TIM_EXT_POLARITY(TIM_ExtTRGPolarity));
  assert_param(IS_TIM_EXT_FILTER(ExtTRGFilter));
  /* ���� ETR ʱ��Դ */
  TIM_ETRConfig(TIMx, TIM_ExtTRGPrescaler, TIM_ExtTRGPolarity, ExtTRGFilter);
  
  /* �õ� TIMx SMCR(��ģʽ����) �Ĵ�����ֵ */
  tmpsmcr = TIMx->SMCR;
  /* ��λ SMS(��ģʽѡ��) λ */
  tmpsmcr &= SMCR_SMS_Mask;
  /* ѡ���ⲿʱ��ģʽ1 */
  tmpsmcr |= TIM_SlaveMode_External1;
  /* ѡ�񴥷��� ��ѡ�� : ETRF */
  tmpsmcr &= SMCR_TS_Mask;
  tmpsmcr |= TIM_TS_ETRF;
  /* д TIMx SMCR */
  TIMx->SMCR = tmpsmcr;
}

/**
  * @����  �����ⲿʱ��ģʽ2
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_ExtTRGPrescaler: �ⲿ������Ԥ��Ƶ��.
  *          �������������ֵ֮һ:
  *          TIM_ExtTRGPSC_OFF:  ETRP Ԥ��Ƶ�� ��.
  *          TIM_ExtTRGPSC_DIV2: ETRP Ƶ�ʳ��� 2.
  *          TIM_ExtTRGPSC_DIV4: ETRP Ƶ�ʳ��� 4.
  *          TIM_ExtTRGPSC_DIV8: ETRP Ƶ�ʳ��� 8.
  * @����  TIM_ExtTRGPolarity: �ⲿ����������.
  *          �������������ֵ֮һ:
  *          TIM_ExtTRGPolarity_Inverted:    �͵�ƽ���½���.
  *          TIM_ExtTRGPolarity_NonInverted: �ߵ�ƽ��������.
  * @����  ExtTRGFilter: �ⲿ������������.
  *          �ò���������0x0��0xF֮���ֵ
  * @����  û��
  */
void TIM_ETRClockMode2Config(TIM_TypeDef* TIMx, uint16_t TIM_ExtTRGPrescaler, 
                             uint16_t TIM_ExtTRGPolarity, uint16_t ExtTRGFilter)
{
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_EXT_PRESCALER(TIM_ExtTRGPrescaler));
  assert_param(IS_TIM_EXT_POLARITY(TIM_ExtTRGPolarity));
  assert_param(IS_TIM_EXT_FILTER(ExtTRGFilter));
  /* ���� ETR ʱ��Դ */
  TIM_ETRConfig(TIMx, TIM_ExtTRGPrescaler, TIM_ExtTRGPolarity, ExtTRGFilter);
  /* ʹ���ⲿʱ��ģʽ2 */
  TIMx->SMCR |= SMCR_ECE_Set;
}

/**
  * @����  ���� TIMx �ⲿ���� (ETR).
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_ExtTRGPrescaler: �ⲿ������Ԥ��Ƶ��.
  *          �������������ֵ֮һ:
  *          TIM_ExtTRGPSC_OFF:  ETRP Ԥ��Ƶ�� ��.
  *          TIM_ExtTRGPSC_DIV2: ETRP Ƶ�ʳ��� 2.
  *          TIM_ExtTRGPSC_DIV4: ETRP Ƶ�ʳ��� 4.
  *          TIM_ExtTRGPSC_DIV8: ETRP Ƶ�ʳ��� 8.
  * @����  TIM_ExtTRGPolarity: �ⲿ����������.
  *          �������������ֵ֮һ:
  *          TIM_ExtTRGPolarity_Inverted:    �͵�ƽ���½���.
  *          TIM_ExtTRGPolarity_NonInverted: �ߵ�ƽ��������.
  * @����  ExtTRGFilter: �ⲿ������������.
  *          �ò���������0x0��0xF֮���ֵ
  * @����  û��
  */
void TIM_ETRConfig(TIM_TypeDef* TIMx, uint16_t TIM_ExtTRGPrescaler, uint16_t TIM_ExtTRGPolarity,
                   uint16_t ExtTRGFilter)
{
  uint16_t tmpsmcr = 0;
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_EXT_PRESCALER(TIM_ExtTRGPrescaler));
  assert_param(IS_TIM_EXT_POLARITY(TIM_ExtTRGPolarity));
  assert_param(IS_TIM_EXT_FILTER(ExtTRGFilter));
  tmpsmcr = TIMx->SMCR;
  /* ��λ ETR λ */
  tmpsmcr &= SMCR_ETR_Mask;
  /* ����Ԥ��Ƶ����������ֵ�ͼ��� */
  tmpsmcr |= (uint16_t)(TIM_ExtTRGPrescaler | (uint16_t)(TIM_ExtTRGPolarity | (uint16_t)(ExtTRGFilter << (uint16_t)8)));
  /* дTIMx SMCR */
  TIMx->SMCR = tmpsmcr;
}

/**
  * @����  ���� TIMx Ԥ��Ƶ��.
  * @����  TIMx: �˴�x������1��8��ѡ������.
  * @����  Prescaler: ָ��Ԥ��Ƶ������ֵ
  * @����  TIM_PSCReloadMode: ָ��Ԥ��Ƶ������ģʽ
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_PSCReloadMode_Update:    TIM Ԥ��Ƶֵ�ڸ����¼�װ��.
  *          TIM_PSCReloadMode_Immediate: TIM Ԥ��Ƶֵ��ʱװ��.
  * @����  û��
  */
void TIM_PrescalerConfig(TIM_TypeDef* TIMx, uint16_t Prescaler, uint16_t TIM_PSCReloadMode)
{
  /* ������ */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_PRESCALER_RELOAD(TIM_PSCReloadMode));
  /* ����Ԥ��Ƶ��ֵ */
  TIMx->PSC = Prescaler;
  /* ���� �� ��λ UG λ */
  TIMx->EGR = TIM_PSCReloadMode;
}

/**
  * @����  ָ�� TIMx ������ģʽ.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_CounterMode: ��ʹ�õļ�����ģʽ
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_CounterMode_Up:             TIM ���ϼ���ģʽ
  *          TIM_CounterMode_Down:           TIM ���¼���ģʽ
  *          TIM_CounterMode_CenterAligned1: TIM ���Ķ���ģʽ1
  *          TIM_CounterMode_CenterAligned2: TIM ���Ķ���ģʽ2
  *          TIM_CounterMode_CenterAligned3: TIM ���Ķ���ģʽ3
  * @����  û��
  */
void TIM_CounterModeConfig(TIM_TypeDef* TIMx, uint16_t TIM_CounterMode)
{
  uint16_t tmpcr1 = 0;
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_COUNTER_MODE(TIM_CounterMode));
  tmpcr1 = TIMx->CR1;
  /* ��λ CMS �� DIR λ */
  tmpcr1 &= CR1_CounterMode_Mask;
  /* ���ü���ģʽ */
  tmpcr1 |= TIM_CounterMode;
  /* д TIMx CR1 �Ĵ��� */
  TIMx->CR1 = tmpcr1;
}

/**
  * @����  ѡ�����봥��Դ
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_InputTriggerSource: ���봥��Դ.
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_TS_ITR0:    �ڲ������� 0
  *          TIM_TS_ITR1:    �ڲ������� 1
  *          TIM_TS_ITR2:    �ڲ������� 2
  *          TIM_TS_ITR3:    �ڲ������� 3
  *          TIM_TS_TI1F_ED: TI1 ��Ե�����
  *          TIM_TS_TI1FP1:  ������ʱ������ 1
  *          TIM_TS_TI2FP2:  ������ʱ������ 2
  *          TIM_TS_ETRF:    �ⲿ��������
  * @����  û��
  */
void TIM_SelectInputTrigger(TIM_TypeDef* TIMx, uint16_t TIM_InputTriggerSource)
{
  uint16_t tmpsmcr = 0;
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_TRIGGER_SELECTION(TIM_InputTriggerSource));
  /* �õ� TIMx SMCR �Ĵ���ֵ */
  tmpsmcr = TIMx->SMCR;
  /* ��λ TS λ */
  tmpsmcr &= SMCR_TS_Mask;
  /* �������봥��Դ */
  tmpsmcr |= TIM_InputTriggerSource;
  /* д TIMx SMCR */
  TIMx->SMCR = tmpsmcr;
}

/**
  * @����  ���� TIMx ����������.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_EncoderMode: ָ�� TIMx ������ģʽ.
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_EncoderMode_TI1:  TIM������ʹ��ģʽ1.
  *          TIM_EncoderMode_TI2:  TIM������ʹ��ģʽ2.
  *          TIM_EncoderMode_TI12: TIM������ʹ��ģʽ3.
  * @����  TIM_IC1Polarity: ָ�� IC1 ����
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_ICPolarity_Falling: IC �½���.
  *          TIM_ICPolarity_Rising:  IC ������.
  * @����  TIM_IC2Polarity: ָ�� IC2 ����
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_ICPolarity_Falling: IC �½���.
  *          TIM_ICPolarity_Rising:  IC ������.
  * @����  û��
  */
void TIM_EncoderInterfaceConfig(TIM_TypeDef* TIMx, uint16_t TIM_EncoderMode,
                                uint16_t TIM_IC1Polarity, uint16_t TIM_IC2Polarity)
{
  uint16_t tmpsmcr = 0;
  uint16_t tmpccmr1 = 0;
  uint16_t tmpccer = 0;
    
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_ENCODER_MODE(TIM_EncoderMode));
  assert_param(IS_TIM_IC_POLARITY(TIM_IC1Polarity));
  assert_param(IS_TIM_IC_POLARITY(TIM_IC2Polarity));

  /* �õ� TIMx SMCR �Ĵ�����ֵ */
  tmpsmcr = TIMx->SMCR;

  /* �õ� TIMx CCMR1 �Ĵ�����ֵ */
  tmpccmr1 = TIMx->CCMR1;

  /* �õ� TIMx CCER �Ĵ�����ֵ */
  tmpccer = TIMx->CCER;

  /* ���ñ�����ģʽ */
  tmpsmcr &= SMCR_SMS_Mask;
  tmpsmcr |= TIM_EncoderMode;

  /* ѡ�񲶻�Ƚ� 1 �Ͳ���Ƚ� 2 ��Ϊ���� */
  tmpccmr1 &= CCMR_CC13S_Mask & CCMR_CC24S_Mask;
  tmpccmr1 |= CCMR_TI13Direct_Set | CCMR_TI24Direct_Set;

  /* ���� TI1 �� TI2 ���� */
  tmpccer &= CCER_CC1P_Reset & CCER_CC2P_Reset;
  tmpccer |= (uint16_t)(TIM_IC1Polarity | (uint16_t)(TIM_IC2Polarity << (uint16_t)4));

  /* д TIMx SMCR */
  TIMx->SMCR = tmpsmcr;

  /* д TIMx CCMR1 */
  TIMx->CCMR1 = tmpccmr1;

  /* д TIMx CCER */
  TIMx->CCER = tmpccer;
}

/**
  * @����  �� TIMx ���1Ϊ����߷ǻ��ƽ.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_ForcedAction: ����źŵ����ö���.
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_ForcedAction_Active:   ǿ��OCxREFΪ��Ч��ƽ
  *          TIM_ForcedAction_InActive: ǿ��OCxREFΪ��Ч��ƽ
  * @����  û��
  */
void TIM_ForcedOC1Config(TIM_TypeDef* TIMx, uint16_t TIM_ForcedAction)
{
  uint16_t tmpccmr1 = 0;
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_FORCED_ACTION(TIM_ForcedAction));
  tmpccmr1 = TIMx->CCMR1;
  /* ��λ OC1M λ */
  tmpccmr1 &= CCMR_OC13M_Mask;
  /* ����ǿ�����ģʽ */
  tmpccmr1 |= TIM_ForcedAction;
  /* д TIMx CCMR1 �Ĵ��� */
  TIMx->CCMR1 = tmpccmr1;
}

/**
  * @����  �� TIMx ���2Ϊ����߷ǻ��ƽ.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_ForcedAction: ����źŵ����ö���.
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_ForcedAction_Active:   ǿ��OCxREFΪ��Ч��ƽ
  *          TIM_ForcedAction_InActive: ǿ��OCxREFΪ��Ч��ƽ
  * @����  û��
  */
void TIM_ForcedOC2Config(TIM_TypeDef* TIMx, uint16_t TIM_ForcedAction)
{
  uint16_t tmpccmr1 = 0;
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_FORCED_ACTION(TIM_ForcedAction));
  tmpccmr1 = TIMx->CCMR1;
  /* ��λ OC2M λ */
  tmpccmr1 &= CCMR_OC24M_Mask;
  /* ����ǿ�����ģʽ */
  tmpccmr1 |= (uint16_t)(TIM_ForcedAction << 8);
  /* д TIMx CCMR1 �Ĵ��� */
  TIMx->CCMR1 = tmpccmr1;
}

/**
  * @����  �� TIMx ���3Ϊ����߷ǻ��ƽ.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_ForcedAction: ����źŵ����ö���.
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_ForcedAction_Active:   ǿ��OCxREFΪ��Ч��ƽ
  *          TIM_ForcedAction_InActive: ǿ��OCxREFΪ��Ч��ƽ
  * @����  û��
  */
void TIM_ForcedOC3Config(TIM_TypeDef* TIMx, uint16_t TIM_ForcedAction)
{
  uint16_t tmpccmr2 = 0;
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_FORCED_ACTION(TIM_ForcedAction));
  tmpccmr2 = TIMx->CCMR2;
  /* ��λ OC1M λ */
  tmpccmr2 &= CCMR_OC13M_Mask;
  /* ����ǿ�����ģʽ */
  tmpccmr2 |= TIM_ForcedAction;
  /* д TIMx CCMR2 �Ĵ��� */
  TIMx->CCMR2 = tmpccmr2;
}

/**
  * @����  �� TIMx ���4Ϊ����߷ǻ��ƽ.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_ForcedAction: ����źŵ����ö���.
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_ForcedAction_Active:   ǿ��OCxREFΪ��Ч��ƽ
  *          TIM_ForcedAction_InActive: ǿ��OCxREFΪ��Ч��ƽ
  * @����  û��
  */
void TIM_ForcedOC4Config(TIM_TypeDef* TIMx, uint16_t TIM_ForcedAction)
{
  uint16_t tmpccmr2 = 0;
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_FORCED_ACTION(TIM_ForcedAction));
  tmpccmr2 = TIMx->CCMR2;
  /* ��λ OC2M λ */
  tmpccmr2 &= CCMR_OC24M_Mask;
  /* ����ǿ�����ģʽ */
  tmpccmr2 |= (uint16_t)(TIM_ForcedAction << 8);
  /* д TIMx CCMR2 �Ĵ��� */
  TIMx->CCMR2 = tmpccmr2;
}

/**
  * @����  ʹ�ܻ���ʧ�� TIMx �� ARR �ϵ�Ԥװ�ؼĴ���.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  NewState: ��TIM1_CR1�Ĵ����е�ARPEλ����״̬
  *                  �������������: ENABLE or DISABLE.
  * @����  û��
  */
void TIM_ARRPreloadConfig(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* ���� ARR ��Ԥװ��λ */
    TIMx->CR1 |= CR1_ARPE_Set;
  }
  else
  {
    /* ��λ ARR ��Ԥװ��λ */
    TIMx->CR1 &= CR1_ARPE_Reset;
  }
}

/**
  * @����  ѡ�� TIM �����ͨѶ�¼�.
  * @����  TIMx: �˴�x������1��8��ѡ������
  * @����  NewState: ͨѶ�¼�����״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void TIM_SelectCOM(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_TIM_18_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* ���� COM λ */
    TIMx->CR2 |= CR2_CCUS_Set;
  }
  else
  {
    /* ��λ COM λ */
    TIMx->CR2 &= CR2_CCUS_Reset;
  }
}

/**
  * @����  ѡ�� TIMx ����Ĳ���Ƚ� DMA Դ.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  NewState: ����Ƚ� DMA Դ����״̬
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void TIM_SelectCCDMA(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* ���� CCDS λ */
    TIMx->CR2 |= CR2_CCDS_Set;
  }
  else
  {
    /* ��λ CCDS λ */
    TIMx->CR2 &= CR2_CCDS_Reset;
  }
}

/**
  * @����  ���û��߸�λ TIMx ����ȽϿ���λ.
  * @����  TIMx: �˴�x������1��8��ѡ������
  * @����  NewState: ����Ƚ�Ԥ�ؿ���λ����״̬
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void TIM_CCPreloadControl(TIM_TypeDef* TIMx, FunctionalState NewState)
{ 
  /* ������ */
  assert_param(IS_TIM_18_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* ���� CCPC λ */
    TIMx->CR2 |= CR2_CCPC_Set;
  }
  else
  {
    /* ��λ CCPC λ */
    TIMx->CR2 &= CR2_CCPC_Reset;
  }
}

/**
  * @����  ʹ�ܻ���ʧ�� TIMx �� CCR1 �ϵ�Ԥװ�ؼĴ���.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_OCPreload: ����Ƚ�Ԥװ��״̬
  *                       �����������ȡ�����ֵ֮һ:
  *                       TIM_OCPreload_Enable
  *                       TIM_OCPreload_Disable
  * @����  û��
  */
void TIM_OC1PreloadConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload)
{
  uint16_t tmpccmr1 = 0;
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCPreload));
  tmpccmr1 = TIMx->CCMR1;
  /* ��λ OC1PE λ */
  tmpccmr1 &= CCMR_OC13PE_Reset;
  /* ʹ�ܻ���ʧ��Ԥװ�ؼĴ��� */
  tmpccmr1 |= TIM_OCPreload;
  /* д TIMx CCMR1 �Ĵ��� */
  TIMx->CCMR1 = tmpccmr1;
}

/**
  * @����  ʹ�ܻ���ʧ�� TIMx �� CCR2 �ϵ�Ԥװ�ؼĴ���.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_OCPreload: ����Ƚ�Ԥװ��״̬
  *                       �����������ȡ�����ֵ֮һ:
  *                       TIM_OCPreload_Enable
  *                       TIM_OCPreload_Disable
  * @����  û��
  */
void TIM_OC2PreloadConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload)
{
  uint16_t tmpccmr1 = 0;
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCPreload));
  tmpccmr1 = TIMx->CCMR1;
  /* ��λ OC2PE λ */
  tmpccmr1 &= CCMR_OC24PE_Reset;
  /* ʹ�ܻ���ʧ��Ԥװ�ؼĴ��� */
  tmpccmr1 |= (uint16_t)(TIM_OCPreload << 8);
  /* д TIMx CCMR1 �Ĵ��� */
  TIMx->CCMR1 = tmpccmr1;
}

/**
  * @����  ʹ�ܻ���ʧ�� TIMx �� CCR3 �ϵ�Ԥװ�ؼĴ���.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_OCPreload: ����Ƚ�Ԥװ��״̬
  *                       �����������ȡ�����ֵ֮һ:
  *                       TIM_OCPreload_Enable
  *                       TIM_OCPreload_Disable
  * @����  û��
  */
void TIM_OC3PreloadConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload)
{
  uint16_t tmpccmr2 = 0;
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCPreload));
  tmpccmr2 = TIMx->CCMR2;
  /* ��λ OC3PE λ */
  tmpccmr2 &= CCMR_OC13PE_Reset;
  /* ʹ�ܻ���ʧ��Ԥװ�ؼĴ��� */
  tmpccmr2 |= TIM_OCPreload;
  /* д TIMx CCMR2 �Ĵ��� */
  TIMx->CCMR2 = tmpccmr2;
}

/**
  * @����  ʹ�ܻ���ʧ�� TIMx �� CCR4 �ϵ�Ԥװ�ؼĴ���.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_OCPreload: ����Ƚ�Ԥװ��״̬
  *                       �����������ȡ�����ֵ֮һ:
  *                       TIM_OCPreload_Enable
  *                       TIM_OCPreload_Disable
  * @����  û��
  */
void TIM_OC4PreloadConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPreload)
{
  uint16_t tmpccmr2 = 0;
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCPRELOAD_STATE(TIM_OCPreload));
  tmpccmr2 = TIMx->CCMR2;
  /* ��λ OC4PE λ */
  tmpccmr2 &= CCMR_OC24PE_Reset;
  /* ʹ�ܻ���ʧ��Ԥװ�ؼĴ��� */
  tmpccmr2 |= (uint16_t)(TIM_OCPreload << 8);
  /* д TIMx CCMR2 �Ĵ��� */
  TIMx->CCMR2 = tmpccmr2;
}

/**
  * @����  ���� TIMx ����Ƚ�1��������.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_OCFast: ����ȽϿ���������״̬.
  *                    �����������ȡ�����ֵ֮һ:
  *                    TIM_OCFast_Enable:  TIM ����ȽϿ�������ʹ��
  *                    TIM_OCFast_Disable: TIM ����ȽϿ�������ʧ��
  * @����  û��
  */
void TIM_OC1FastConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCFast)
{
  uint16_t tmpccmr1 = 0;
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCFAST_STATE(TIM_OCFast));
  /* �õ� TIMx CCMR1 �Ĵ�����ֵ */
  tmpccmr1 = TIMx->CCMR1;
  /* ��λ OC1FE λ */
  tmpccmr1 &= CCMR_OC13FE_Reset;
  /* ʹ�ܻ�ʧ�ܲ���ȽϿ������� */
  tmpccmr1 |= TIM_OCFast;
  /* д TIMx CCMR1 */
  TIMx->CCMR1 = tmpccmr1;
}

/**
  * @����  ���� TIMx ����Ƚ�2��������.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_OCFast: ����ȽϿ���������״̬.
  *                    �����������ȡ�����ֵ֮һ:
  *                    TIM_OCFast_Enable:  TIM ����ȽϿ�������ʹ��
  *                    TIM_OCFast_Disable: TIM ����ȽϿ�������ʧ��
  * @����  û��
  */
void TIM_OC2FastConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCFast)
{
  uint16_t tmpccmr1 = 0;
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCFAST_STATE(TIM_OCFast));
  /* �õ� TIMx CCMR1 �Ĵ�����ֵ */
  tmpccmr1 = TIMx->CCMR1;
  /* ��λ OC2FE λ */
  tmpccmr1 &= CCMR_OC24FE_Reset;
  /* ʹ�ܻ�ʧ�ܲ���ȽϿ������� */
  tmpccmr1 |= (uint16_t)(TIM_OCFast << 8);
  /* д TIMx CCMR1 */
  TIMx->CCMR1 = tmpccmr1;
}

/**
  * @����  ���� TIMx ����Ƚ�3��������.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_OCFast: ����ȽϿ���������״̬.
  *                    �����������ȡ�����ֵ֮һ:
  *                    TIM_OCFast_Enable:  TIM ����ȽϿ�������ʹ��
  *                    TIM_OCFast_Disable: TIM ����ȽϿ�������ʧ��
  * @����  û��
  */
void TIM_OC3FastConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCFast)
{
  uint16_t tmpccmr2 = 0;
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCFAST_STATE(TIM_OCFast));
  /* �õ� TIMx CCMR2 �Ĵ�����ֵ */
  tmpccmr2 = TIMx->CCMR2;
  /* ��λ OC3FE λ */
  tmpccmr2 &= CCMR_OC13FE_Reset;
  /* ʹ�ܻ�ʧ�ܲ���ȽϿ������� */
  tmpccmr2 |= TIM_OCFast;
  /* д TIMx CCMR2 */
  TIMx->CCMR2 = tmpccmr2;
}

/**
  * @����  ���� TIMx ����Ƚ�4��������.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_OCFast: ����ȽϿ���������״̬.
  *                    �����������ȡ�����ֵ֮һ:
  *                    TIM_OCFast_Enable:  TIM ����ȽϿ�������ʹ��
  *                    TIM_OCFast_Disable: TIM ����ȽϿ�������ʧ��
  * @����  û��
  */
void TIM_OC4FastConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCFast)
{
  uint16_t tmpccmr2 = 0;
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCFAST_STATE(TIM_OCFast));
  /* �õ� TIMx CCMR2 �Ĵ�����ֵ */
  tmpccmr2 = TIMx->CCMR2;
  /* ��λ OC4FE λ */
  tmpccmr2 &= CCMR_OC24FE_Reset;
  /* ʹ�ܻ�ʧ�ܲ���ȽϿ������� */
  tmpccmr2 |= (uint16_t)(TIM_OCFast << 8);
  /* д TIMx CCMR2 */
  TIMx->CCMR2 = tmpccmr2;
}

/**
  * @����  ��һ���ⲿ�¼�ʱ������߱��� OCREF1 �ź�
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_OCClear: ����Ƚ����ʹ��λ����״̬.
  *                     �����������ȡ�����ֵ֮һ:
  *                     TIM_OCClear_Enable:  TIM ����Ƚ����ʹ��
  *                     TIM_OCClear_Disable: TIM ����Ƚ����ʧ��
  * @����  û��
  */
void TIM_ClearOC1Ref(TIM_TypeDef* TIMx, uint16_t TIM_OCClear)
{
  uint16_t tmpccmr1 = 0;
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCCLEAR_STATE(TIM_OCClear));
  tmpccmr1 = TIMx->CCMR1;
  /* ��λ OC1CE λ */
  tmpccmr1 &= CCMR_OC13CE_Reset;
  /* ʹ�ܻ�ʧ������Ƚ����λ */
  tmpccmr1 |= TIM_OCClear;
  /* д TIMx CCMR1 �Ĵ��� */
  TIMx->CCMR1 = tmpccmr1;
}

/**
  * @����  ��һ���ⲿ�¼�ʱ������߱��� OCREF2 �ź�
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_OCClear: ����Ƚ����ʹ��λ����״̬.
  *                     �����������ȡ�����ֵ֮һ:
  *                     TIM_OCClear_Enable:  TIM ����Ƚ����ʹ��
  *                     TIM_OCClear_Disable: TIM ����Ƚ����ʧ��
  * @����  û��
  */
void TIM_ClearOC2Ref(TIM_TypeDef* TIMx, uint16_t TIM_OCClear)
{
  uint16_t tmpccmr1 = 0;
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCCLEAR_STATE(TIM_OCClear));
  tmpccmr1 = TIMx->CCMR1;
  /* ��λ OC2CE λ */
  tmpccmr1 &= CCMR_OC24CE_Reset;
  /* ʹ�ܻ�ʧ������Ƚ����λ */
  tmpccmr1 |= (uint16_t)(TIM_OCClear << 8);
  /* д TIMx CCMR1 �Ĵ��� */
  TIMx->CCMR1 = tmpccmr1;
}

/**
  * @����  ��һ���ⲿ�¼�ʱ������߱��� OCREF3 �ź�
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_OCClear: ����Ƚ����ʹ��λ����״̬.
  *                     �����������ȡ�����ֵ֮һ:
  *                     TIM_OCClear_Enable:  TIM ����Ƚ����ʹ��
  *                     TIM_OCClear_Disable: TIM ����Ƚ����ʧ��
  * @����  û��
  */
void TIM_ClearOC3Ref(TIM_TypeDef* TIMx, uint16_t TIM_OCClear)
{
  uint16_t tmpccmr2 = 0;
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCCLEAR_STATE(TIM_OCClear));
  tmpccmr2 = TIMx->CCMR2;
  /* ��λ OC3CE λ */
  tmpccmr2 &= CCMR_OC13CE_Reset;
  /* ʹ�ܻ�ʧ������Ƚ����λ */
  tmpccmr2 |= TIM_OCClear;
  /* д TIMx CCMR2 �Ĵ��� */
  TIMx->CCMR2 = tmpccmr2;
}

/**
  * @����  ��һ���ⲿ�¼�ʱ������߱��� OCREF4 �ź�
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_OCClear: ����Ƚ����ʹ��λ����״̬.
  *                     �����������ȡ�����ֵ֮һ:
  *                     TIM_OCClear_Enable:  TIM ����Ƚ����ʹ��
  *                     TIM_OCClear_Disable: TIM ����Ƚ����ʧ��
  * @����  û��
  */
void TIM_ClearOC4Ref(TIM_TypeDef* TIMx, uint16_t TIM_OCClear)
{
  uint16_t tmpccmr2 = 0;
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OCCLEAR_STATE(TIM_OCClear));
  tmpccmr2 = TIMx->CCMR2;
  /* ��λ OC4CE λ */
  tmpccmr2 &= CCMR_OC24CE_Reset;
  /* ʹ�ܻ�ʧ������Ƚ����λ */
  tmpccmr2 |= (uint16_t)(TIM_OCClear << 8);
  /* д TIMx CCMR2 �Ĵ��� */
  TIMx->CCMR2 = tmpccmr2;
}

/**
  * @����  ���� TIMx ͨ��1����.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_OCPolarity: ָ���� OC1 ����
  *                        �����������ȡ�����ֵ֮һ:
  *                        TIM_OCPolarity_High: ����Ƚϼ��Ը�
  *                        TIM_OCPolarity_Low:  ����Ƚϼ��Ե�
  * @����  û��
  */
void TIM_OC1PolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPolarity)
{
  uint16_t tmpccer = 0;
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCPolarity));
  tmpccer = TIMx->CCER;
  /* ���û�λ CC1P λ */
  tmpccer &= CCER_CC1P_Reset;
  tmpccer |= TIM_OCPolarity;
  /* д TIMx CCER �Ĵ��� */
  TIMx->CCER = tmpccer;
}

/**
  * @����  ���� TIMx ͨ�� 1N ����.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_OCNPolarity: ָ�� OC1N ����
  *                         �����������ȡ�����ֵ֮һ:
  *                         TIM_OCNPolarity_High: ����Ƚϼ��Ը�
  *                         TIM_OCNPolarity_Low:  ����Ƚϼ��Ե�
  * @����  û��
  */
void TIM_OC1NPolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCNPolarity)
{
  uint16_t tmpccer = 0;
  /* ������ */
  assert_param(IS_TIM_18_PERIPH(TIMx));
  assert_param(IS_TIM_OCN_POLARITY(TIM_OCNPolarity));
   
  tmpccer = TIMx->CCER;
  /* ���û�λ CC1NP λ */
  tmpccer &= CCER_CC1NP_Reset;
  tmpccer |= TIM_OCNPolarity;
  /* д TIMx CCER �Ĵ��� */
  TIMx->CCER = tmpccer;
}

/**
  * @����  ���� TIMx ͨ��2����.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_OCPolarity: ָ���� OC2 ����
  *                        �����������ȡ�����ֵ֮һ:
  *                        TIM_OCPolarity_High: ����Ƚϼ��Ը�
  *                        TIM_OCPolarity_Low:  ����Ƚϼ��Ե�
  * @����  û��
  */
void TIM_OC2PolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPolarity)
{
  uint16_t tmpccer = 0;
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCPolarity));
  tmpccer = TIMx->CCER;
  /* ���û�λ CC2P λ */
  tmpccer &= CCER_CC2P_Reset;
  tmpccer |= (uint16_t)(TIM_OCPolarity << 4);
  /* д TIMx CCER �Ĵ��� */
  TIMx->CCER = tmpccer;
}

/**
  * @����  ���� TIMx ͨ�� 2N ����.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_OCNPolarity: ָ�� OC2N ����
  *                         �����������ȡ�����ֵ֮һ:
  *                         TIM_OCNPolarity_High: ����Ƚϼ��Ը�
  *                         TIM_OCNPolarity_Low:  ����Ƚϼ��Ե�
  * @����  û��
  */
void TIM_OC2NPolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCNPolarity)
{
  uint16_t tmpccer = 0;
  /* ������ */
  assert_param(IS_TIM_18_PERIPH(TIMx));
  assert_param(IS_TIM_OCN_POLARITY(TIM_OCNPolarity));
  
  tmpccer = TIMx->CCER;
  /* ���û�λ CC2NP λ */
  tmpccer &= CCER_CC2NP_Reset;
  tmpccer |= (uint16_t)(TIM_OCNPolarity << 4);
  /* д TIMx CCER �Ĵ��� */
  TIMx->CCER = tmpccer;
}

/**
  * @����  ���� TIMx ͨ��3����.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_OCPolarity: ָ���� OC3 ����
  *                        �����������ȡ�����ֵ֮һ:
  *                        TIM_OCPolarity_High: ����Ƚϼ��Ը�
  *                        TIM_OCPolarity_Low:  ����Ƚϼ��Ե�
  * @����  û��
  */
void TIM_OC3PolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPolarity)
{
  uint16_t tmpccer = 0;
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCPolarity));
  tmpccer = TIMx->CCER;
  /* ���û�λ CC3P λ */
  tmpccer &= CCER_CC3P_Reset;
  tmpccer |= (uint16_t)(TIM_OCPolarity << 8);
  /* д TIMx CCER �Ĵ��� */
  TIMx->CCER = tmpccer;
}

/**
  * @����  ���� TIMx ͨ�� 3N ����.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_OCNPolarity: ָ�� OC3N ����
  *                         �����������ȡ�����ֵ֮һ:
  *                         TIM_OCNPolarity_High: ����Ƚϼ��Ը�
  *                         TIM_OCNPolarity_Low:  ����Ƚϼ��Ե�
  * @����  û��
  */
void TIM_OC3NPolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCNPolarity)
{
  uint16_t tmpccer = 0;
 
  /* ������ */
  assert_param(IS_TIM_18_PERIPH(TIMx));
  assert_param(IS_TIM_OCN_POLARITY(TIM_OCNPolarity));
    
  tmpccer = TIMx->CCER;
  /* ���û�λ CC3NP λ */
  tmpccer &= CCER_CC3NP_Reset;
  tmpccer |= (uint16_t)(TIM_OCNPolarity << 8);
  /* д TIMx CCER �Ĵ��� */
  TIMx->CCER = tmpccer;
}

/**
  * @����  ���� TIMx ͨ��4����.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_OCPolarity: ָ���� OC4 ����
  *                        �����������ȡ�����ֵ֮һ:
  *                        TIM_OCPolarity_High: ����Ƚϼ��Ը�
  *                        TIM_OCPolarity_Low:  ����Ƚϼ��Ե�
  * @����  û��
  */
void TIM_OC4PolarityConfig(TIM_TypeDef* TIMx, uint16_t TIM_OCPolarity)
{
  uint16_t tmpccer = 0;
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_OC_POLARITY(TIM_OCPolarity));
  tmpccer = TIMx->CCER;
  /* ���û�λ CC4P λ */
  tmpccer &= CCER_CC4P_Reset;
  tmpccer |= (uint16_t)(TIM_OCPolarity << 12);
  /* д TIMx CCER �Ĵ��� */
  TIMx->CCER = tmpccer;
}

/**
  * @����  ʹ�ܻ�ʧ�� TIM ����Ƚ�ͨ�� x.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_Channel: ָ���� TIM ͨ��
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_Channel_1: TIM ͨ�� 1
  *          TIM_Channel_2: TIM ͨ�� 2
  *          TIM_Channel_3: TIM ͨ�� 3
  *          TIM_Channel_4: TIM ͨ�� 4
  * @����  TIM_CCx: ָ�� TIM ͨ�� CCxE λ����״̬.
  *                 �������������: TIM_CCx_Enable �� TIM_CCx_Disable. 
  * @����  û��
  */
void TIM_CCxCmd(TIM_TypeDef* TIMx, uint16_t TIM_Channel, uint16_t TIM_CCx)
{
  uint16_t tmp = 0;

  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_CHANNEL(TIM_Channel));
  assert_param(IS_TIM_CCX(TIM_CCx));

  tmp = CCER_CCE_Set << TIM_Channel;

  /* ��λ CCxE λ */
  TIMx->CCER &= (uint16_t)~ tmp;

  /* ���û�λ CCxE λ */ 
  TIMx->CCER |=  (uint16_t)(TIM_CCx << TIM_Channel);
}

/**
  * @����  ʹ�ܻ�ʧ�� TIM ����Ƚ�ͨ�� xN.
  * @����  TIMx: �˴�x������1��8��ѡ������.
  * @����  TIM_Channel: ָ�� TIM ͨ��
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_Channel_1: TIM ͨ�� 1
  *          TIM_Channel_2: TIM ͨ�� 2
  *          TIM_Channel_3: TIM ͨ�� 3
  * @����  TIM_CCxN: ָ�� TIM ͨ�� CCxNE λ����״̬.
  *                  �������������: TIM_CCxN_Enable �� TIM_CCxN_Disable. 
  * @����  û��
  */
void TIM_CCxNCmd(TIM_TypeDef* TIMx, uint16_t TIM_Channel, uint16_t TIM_CCxN)
{
  uint16_t tmp = 0;

  /* ������ */
  assert_param(IS_TIM_18_PERIPH(TIMx));
  assert_param(IS_TIM_COMPLEMENTARY_CHANNEL(TIM_Channel));
  assert_param(IS_TIM_CCXN(TIM_CCxN));

  tmp = CCER_CCNE_Set << TIM_Channel;

  /* ��λ CCxNE λ */
  TIMx->CCER &= (uint16_t) ~tmp;

  /* ���û�λ CCxNE λ */ 
  TIMx->CCER |=  (uint16_t)(TIM_CCxN << TIM_Channel);
}

/**
  * @����  ѡ�� TIM ����Ƚ�ģʽ.
  * @ע��   �ڸı�����Ƚ�ģʽ֮ǰ���ú���ʧ��ѡ���ͨ��.
  *         �û���Ҫʹ�� TIM1_CCxCmd �� TIM1_CCxNCmd ����ʹ��ͨ��.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_Channel: ָ���� TIM ͨ��
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_Channel_1: TIM ͨ�� 1
  *          TIM_Channel_2: TIM ͨ�� 2
  *          TIM_Channel_3: TIM ͨ�� 3
  *          TIM_Channel_4: TIM ͨ�� 4
  * @����  TIM_OCMode: ָ�� TIM ����Ƚ�ģʽ.
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_OCMode_Timing
  *          TIM_OCMode_Active
  *          TIM_OCMode_Toggle
  *          TIM_OCMode_PWM1
  *          TIM_OCMode_PWM2
  *          TIM_ForcedAction_Active
  *          TIM_ForcedAction_InActive
  * @����  û��
  */
void TIM_SelectOCxM(TIM_TypeDef* TIMx, uint16_t TIM_Channel, uint16_t TIM_OCMode)
{
  uint32_t tmp = 0;
  uint16_t tmp1 = 0;

  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_CHANNEL(TIM_Channel));
  assert_param(IS_TIM_OCM(TIM_OCMode));

  tmp = (uint32_t) TIMx;
  tmp += CCMR_Offset;

  tmp1 = CCER_CCE_Set << (uint16_t)TIM_Channel;

  /* ʧ��ͨ��: ��λ CCxE λ */
  TIMx->CCER &= (uint16_t) ~tmp1;

  if((TIM_Channel == TIM_Channel_1) ||(TIM_Channel == TIM_Channel_3))
  {
    tmp += (TIM_Channel>>1);

    /* ��λ OCxM λ���� CCMRx �Ĵ����� */
    *(__IO uint32_t *) tmp &= CCMR_OC13M_Mask;
   
    /* ���� OCxM λ���� CCMRx �Ĵ����� */
    *(__IO uint32_t *) tmp |= TIM_OCMode;
  }
  else
  {
    tmp += (uint16_t)(TIM_Channel - (uint16_t)4)>> (uint16_t)1;

    /* ��λ OCxM λ���� CCMRx �Ĵ����� */
    *(__IO uint32_t *) tmp &= CCMR_OC24M_Mask;
    
    /* ���� OCxM λ���� CCMRx �Ĵ����� */
    *(__IO uint32_t *) tmp |= (uint16_t)(TIM_OCMode << 8);
  }
}

/**
  * @����  ʹ�ܻ���ʧ�� TIM �����¼�.
  * @����  TIMx: �˴�x������1��8��ѡ������.
  * @����  NewState: TIM1_CR1�Ĵ���UDISλ����״̬
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void TIM_UpdateDisableConfig(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* ���ø���ʧ��λ */
    TIMx->CR1 |= CR1_UDIS_Set;
  }
  else
  {
    /* ��λ����ʧ��λ */
    TIMx->CR1 &= CR1_UDIS_Reset;
  }
}

/**
  * @����  ���� TIMx ��������Դ.
  * @����  TIMx: �˴�x������1��8��ѡ������.
  * @����  TIM_UpdateSource: ָ��������Դ.
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_UpdateSource_Regular: �����ظ������壺�ڸ����¼�ʱ��������ֹͣ.
  *          TIM_UpdateSource_Global:  ���ɵ�һ�����壺����������һ�������¼�ֹͣ.
  * @����  û��
  */
void TIM_UpdateRequestConfig(TIM_TypeDef* TIMx, uint16_t TIM_UpdateSource)
{
  /* ������ */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_UPDATE_SOURCE(TIM_UpdateSource));
  if (TIM_UpdateSource != TIM_UpdateSource_Global)
  {
    /* ���� URS λ */
    TIMx->CR1 |= CR1_URS_Set;
  }
  else
  {
    /* ��λ URS λ */
    TIMx->CR1 &= CR1_URS_Reset;
  }
}

/**
  * @����  ʹ�ܻ���ʧ�� TIM �����������ӿ�.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  NewState: ������Ӧ������״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void TIM_SelectHallSensor(TIM_TypeDef* TIMx, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* ���� TI1S λ */
    TIMx->CR2 |= CR2_TI1S_Set;
  }
  else
  {
    /* ��λ TI1S λ */
    TIMx->CR2 &= CR2_TI1S_Reset;
  }
}

/**
  * @����  ���� TIM ������ģʽ.
  * @����  TIMx: �˴�x������1��8��ѡ������.
  * @����  TIM_OPMode: ָ�� OPM ģʽ.
  *                    �����������ȡ�����ֵ֮һ:
  *                    TIM_OPMode_Single
  *                    TIM_OPMode_Repetitive
  * @����  û��
  */
void TIM_SelectOnePulseMode(TIM_TypeDef* TIMx, uint16_t TIM_OPMode)
{
  /* ������ */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_OPM_MODE(TIM_OPMode));
  /* ��λ OPM λ */
  TIMx->CR1 &= CR1_OPM_Reset;
  /* ���� OPM ģʽ */
  TIMx->CR1 |= TIM_OPMode;
}

/**
  * @����  ѡ�� TIM �������ģʽ.
  * @����  TIMx: �˴�x������1��8��ѡ������.
  * @����  TIM_TRGOSource: ָ���Ĵ������ģʽ.
  *                        ������������������ֵ:
  *
  *  - �������е� TIMx
  *          TIM_TRGOSource_Reset:  ʹ�üĴ���TIM1_EGR��UGλ��Ϊ������� (TRGO).
  *          TIM_TRGOSource_Enable: ʹ�ü�����ʹ��CEN��Ϊ������� (TRGO).
  *          TIM_TRGOSource_Update: ʹ�ø����¼���Ϊ������� (TRGO).
  *
  *  - �������е� TIMx ���� TIM6 �� TIM7
  *          TIM_TRGOSource_OC1:    һ��������߱Ƚ�ƥ�䷢��������־λCC1F������ʱ�����������һ���϶����� (TRGO).
  *          TIM_TRGOSource_OC1Ref: ʹ�� OC1REF ��Ϊ������� (TRGO).
  *          TIM_TRGOSource_OC2Ref: ʹ�� OC2REF ��Ϊ������� (TRGO).
  *          TIM_TRGOSource_OC3Ref: ʹ�� OC3REF ��Ϊ������� (TRGO).
  *          TIM_TRGOSource_OC4Ref: ʹ�� OC4REF ��Ϊ������� (TRGO).
  *
  * @����  û��
  */
void TIM_SelectOutputTrigger(TIM_TypeDef* TIMx, uint16_t TIM_TRGOSource)
{
  /* ������ */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_TRGO_SOURCE(TIM_TRGOSource));
  /* ��λ MMS λ */
  TIMx->CR2 &= CR2_MMS_Mask;
  /* ѡ�� TRGO ����Դ */
  TIMx->CR2 |=  TIM_TRGOSource;
}

/**
  * @����  ѡ�� TIMx ��ģʽ.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_SlaveMode: ָ����ʱ����ģʽ.
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_SlaveMode_Reset:     ѡ�д����ź� (TRGI) ���������س�ʼ���������������Ĵ����ĸ���.
  *          TIM_SlaveMode_Gated:     �������ź� (TRGI) Ϊ�ߵ�ƽ������ʱ��ʹ��.
  *          TIM_SlaveMode_Trigger:   �������ڴ��� (TRGI) �������ؿ�ʼ.
  *          TIM_SlaveMode_External1: ѡ�д��� (TRGI) ����������Ϊ������ʱ��.
  * @����  û��
  */
void TIM_SelectSlaveMode(TIM_TypeDef* TIMx, uint16_t TIM_SlaveMode)
{
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_SLAVE_MODE(TIM_SlaveMode));
  /* ��λ SMS λ */
  TIMx->SMCR &= SMCR_SMS_Mask;
  /* ѡ���ģʽ */
  TIMx->SMCR |= TIM_SlaveMode;
}

/**
  * @����  ���û�λ TIMx ��/��ģʽ.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_MasterSlaveMode: ��ʱ����/��ģʽ.
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_MasterSlaveMode_Enable:  ��/��ģʽʹ��.
  *          TIM_MasterSlaveMode_Disable: ��/��ģʽʧ��
  * @����  û��
  */
void TIM_SelectMasterSlaveMode(TIM_TypeDef* TIMx, uint16_t TIM_MasterSlaveMode)
{
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_MSM_STATE(TIM_MasterSlaveMode));
  /* ��λ MSM λ */
  TIMx->SMCR &= SMCR_MSM_Reset;
  
  /* ���û�λ MSM λ */
  TIMx->SMCR |= TIM_MasterSlaveMode;
}

/**
  * @����  ���� TIMx �����Ĵ�����ֵ
  * @����  TIMx: �˴�x������1��8��ѡ������.
  * @����  Counter: ָ�������Ĵ�������ֵ.
  * @����  û��
  */
void TIM_SetCounter(TIM_TypeDef* TIMx, uint16_t Counter)
{
  /* ������ */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  /* ���ü����Ĵ�����ֵ */
  TIMx->CNT = Counter;
}

/**
  * @����  ���� TIMx �Զ���װ�ؼĴ���ֵ
  * @����  TIMx: �˴�x������1��8��ѡ������.
  * @����  Autoreload: ָ���Զ���װ�ؼĴ�����ֵ.
  * @����  û��
  */
void TIM_SetAutoreload(TIM_TypeDef* TIMx, uint16_t Autoreload)
{
  /* ������ */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  /* �����Զ���װ�ؼĴ���ֵ */
  TIMx->ARR = Autoreload;
}

/**
  * @����  ���� TIMx ����Ƚ�1�Ĵ���ֵ
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  Compare1: ����Ƚ�1�Ĵ�����ֵ.
  * @����  û��
  */
void TIM_SetCompare1(TIM_TypeDef* TIMx, uint16_t Compare1)
{
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  /* ���ò���Ƚ�1�Ĵ���ֵ */
  TIMx->CCR1 = Compare1;
}

/**
  * @����  ���� TIMx ����Ƚ�2�Ĵ���ֵ
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  Compare2: ����Ƚ�2�Ĵ�����ֵ.
  * @����  û��
  */
void TIM_SetCompare2(TIM_TypeDef* TIMx, uint16_t Compare2)
{
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  /* ���ò���Ƚ�2�Ĵ���ֵ */
  TIMx->CCR2 = Compare2;
}

/**
  * @����  ���� TIMx ����Ƚ�3�Ĵ���ֵ
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  Compare3: ����Ƚ�3�Ĵ�����ֵ.
  * @����  û��
  */
void TIM_SetCompare3(TIM_TypeDef* TIMx, uint16_t Compare3)
{
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  /* ���ò���Ƚ�3�Ĵ���ֵ */
  TIMx->CCR3 = Compare3;
}

/**
  * @����  ���� TIMx ����Ƚ�4�Ĵ���ֵ
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  Compare4: ����Ƚ�4�Ĵ�����ֵ.
  * @����  û��
  */
void TIM_SetCompare4(TIM_TypeDef* TIMx, uint16_t Compare4)
{
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  /* ���ò���Ƚ�4�Ĵ���ֵ */
  TIMx->CCR4 = Compare4;
}

/**
  * @����  ���� TIMx ���벶��1Ԥ��Ƶ.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_ICPSC: ���벶��1Ԥ��Ƶ����ֵ.
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_ICPSC_DIV1: û��Ԥ��Ƶ��
  *          TIM_ICPSC_DIV2: ÿ2���¼�TIM����ִ��һ��
  *          TIM_ICPSC_DIV4: ÿ4���¼�TIM����ִ��һ��
  *          TIM_ICPSC_DIV8: ÿ8���¼�TIM����ִ��һ��
  * @����  û��
  */
void TIM_SetIC1Prescaler(TIM_TypeDef* TIMx, uint16_t TIM_ICPSC)
{
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICPSC));
  /* ��λ IC1PSC λ */
  TIMx->CCMR1 &= CCMR_IC13PSC_Mask;
  /* ���� IC1PSC ��ֵ */
  TIMx->CCMR1 |= TIM_ICPSC;
}

/**
  * @����  ���� TIMx ���벶��2Ԥ��Ƶ.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_ICPSC: ���벶��2Ԥ��Ƶ����ֵ.
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_ICPSC_DIV1: û��Ԥ��Ƶ��
  *          TIM_ICPSC_DIV2: ÿ2���¼�TIM����ִ��һ��
  *          TIM_ICPSC_DIV4: ÿ4���¼�TIM����ִ��һ��
  *          TIM_ICPSC_DIV8: ÿ8���¼�TIM����ִ��һ��
  * @����  û��
  */
void TIM_SetIC2Prescaler(TIM_TypeDef* TIMx, uint16_t TIM_ICPSC)
{
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICPSC));
  /* ��λ IC2PSC λ */
  TIMx->CCMR1 &= CCMR_IC24PSC_Mask;
  /* ���� IC2PSC ��ֵ */
  TIMx->CCMR1 |= (uint16_t)(TIM_ICPSC << 8);
}

/**
  * @����  ���� TIMx ���벶��3Ԥ��Ƶ.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_ICPSC: ���벶��3Ԥ��Ƶ����ֵ.
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_ICPSC_DIV1: û��Ԥ��Ƶ��
  *          TIM_ICPSC_DIV2: ÿ2���¼�TIM����ִ��һ��
  *          TIM_ICPSC_DIV4: ÿ4���¼�TIM����ִ��һ��
  *          TIM_ICPSC_DIV8: ÿ8���¼�TIM����ִ��һ��
  * @����  û��
  */
void TIM_SetIC3Prescaler(TIM_TypeDef* TIMx, uint16_t TIM_ICPSC)
{
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICPSC));
  /* ��λ IC3PSC λ */
  TIMx->CCMR2 &= CCMR_IC13PSC_Mask;
  /* ���� IC3PSC ��ֵ */
  TIMx->CCMR2 |= TIM_ICPSC;
}

/**
  * @����  ���� TIMx ���벶��4Ԥ��Ƶ.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_ICPSC: ���벶��4Ԥ��Ƶ����ֵ.
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_ICPSC_DIV1: û��Ԥ��Ƶ��
  *          TIM_ICPSC_DIV2: ÿ2���¼�TIM����ִ��һ��
  *          TIM_ICPSC_DIV4: ÿ4���¼�TIM����ִ��һ��
  *          TIM_ICPSC_DIV8: ÿ8���¼�TIM����ִ��һ��
  * @����  û��
  */
void TIM_SetIC4Prescaler(TIM_TypeDef* TIMx, uint16_t TIM_ICPSC)
{  
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_IC_PRESCALER(TIM_ICPSC));
  /* ��λ IC4PSC λ */
  TIMx->CCMR2 &= CCMR_IC24PSC_Mask;
  /* ���� IC4PSC ��ֵ */
  TIMx->CCMR2 |= (uint16_t)(TIM_ICPSC << 8);
}

/**
  * @����  ���� TIMx ��ʱ�ӷָ�ֵ.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_CKD: ʱ�ӷָ�ֵ.
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_CKD_DIV1: TDTS = Tck_tim
  *          TIM_CKD_DIV2: TDTS = 2*Tck_tim
  *          TIM_CKD_DIV4: TDTS = 4*Tck_tim
  * @����  û��
  */
void TIM_SetClockDivision(TIM_TypeDef* TIMx, uint16_t TIM_CKD)
{
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  assert_param(IS_TIM_CKD_DIV(TIM_CKD));
  /* ��λ CKD λ */
  TIMx->CR1 &= CR1_CKD_Mask;
  /* ���� CKD ��ֵ */
  TIMx->CR1 |= TIM_CKD;
}

/**
  * @����  ��� TIMx ���벶��1��ֵ.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  ������벶��1��ֵ.
  */
uint16_t TIM_GetCapture1(TIM_TypeDef* TIMx)
{
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  /* ������벶��1��ֵ */
  return TIMx->CCR1;
}

/**
  * @����  ��� TIMx ���벶��2��ֵ.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  ������벶��2��ֵ.
  */
uint16_t TIM_GetCapture2(TIM_TypeDef* TIMx)
{
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  /* ������벶��2��ֵ */
  return TIMx->CCR2;
}

/**
  * @����  ��� TIMx ���벶��3��ֵ.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  ������벶��3��ֵ.
  */
uint16_t TIM_GetCapture3(TIM_TypeDef* TIMx)
{
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx)); 
  /* ������벶��3��ֵ */
  return TIMx->CCR3;
}

/**
  * @����  ��� TIMx ���벶��4��ֵ.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  ������벶��4��ֵ.
  */
uint16_t TIM_GetCapture4(TIM_TypeDef* TIMx)
{
  /* ������ */
  assert_param(IS_TIM_123458_PERIPH(TIMx));
  /* ������벶��4��ֵ */
  return TIMx->CCR4;
}

/**
  * @����  ��� TIMx ��������ֵ.
  * @����  TIMx: �˴�x������1��8��ѡ������.
  * @����  ��������ֵ.
  */
uint16_t TIM_GetCounter(TIM_TypeDef* TIMx)
{
  /* ������ */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  /* ��ü�������ֵ */
  return TIMx->CNT;
}

/**
  * @����  ��� TIMx Ԥ��Ƶֵ.
  * @����  TIMx: �˴�x������1��8��ѡ������.
  * @����  Ԥ��Ƶ��ֵ.
  */
uint16_t TIM_GetPrescaler(TIM_TypeDef* TIMx)
{
  /* ������ */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  /* ���Ԥ��Ƶ��ֵ */
  return TIMx->PSC;
}

/**
  * @����  ���ָ���� TIM ��־λ�������.
  * @����  TIMx: �˴�x������1��8��ѡ������.
  * @����  TIM_FLAG: ������ TIM ��־λ.
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_FLAG_Update: TIM ���±�־
  *          TIM_FLAG_CC1: TIM ����/�Ƚ� 1 ��־λ
  *          TIM_FLAG_CC2: TIM ����/�Ƚ� 2 ��־λ
  *          TIM_FLAG_CC3: TIM ����/�Ƚ� 3 ��־λ
  *          TIM_FLAG_CC4: TIM ����/�Ƚ� 4 ��־λ
  *          TIM_FLAG_COM: TIM Com��־
  *          TIM_FLAG_Trigger: TIM ������־λ
  *          TIM_FLAG_Break: TIM ��Ϣ��־
  *          TIM_FLAG_CC1OF: TIM ����/�Ƚ� 1 �����־λ
  *          TIM_FLAG_CC2OF: TIM ����/�Ƚ� 2 �����־λ
  *          TIM_FLAG_CC3OF: TIM ����/�Ƚ� 3 �����־λ
  *          TIM_FLAG_CC4OF: TIM ����/�Ƚ� 4 �����־λ
  * @ע��
  *   - TIM6 �� TIM7 ������һ�������ж�. 
  *   - TIM_FLAG_COM �� TIM_FLAG_Break �������Ժ� TIM1 �� TIM8 һ��ʹ��.    
  * @����  TIM_FLAG ����״̬(SET �� RESET).
  */
FlagStatus TIM_GetFlagStatus(TIM_TypeDef* TIMx, uint16_t TIM_FLAG)
{ 
  ITStatus bitstatus = RESET;  
  /* ������ */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_GET_FLAG(TIM_FLAG));
  
  if ((TIMx->SR & TIM_FLAG) != (uint16_t)RESET)
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
  * @����  ��� TIMx �Ĵ������־λ.
  * @����  TIMx: �˴�x������1��8��ѡ������.
  * @����  TIM_FLAG: ��������ı��.
  *          �����������������ֵ���������:
  *          TIM_FLAG_Update: TIM ���±�־
  *          TIM_FLAG_CC1: TIM ����/�Ƚ� 1 ��־λ
  *          TIM_FLAG_CC2: TIM ����/�Ƚ� 2 ��־λ
  *          TIM_FLAG_CC3: TIM ����/�Ƚ� 3 ��־λ
  *          TIM_FLAG_CC4: TIM ����/�Ƚ� 4 ��־λ
  *          TIM_FLAG_COM: TIM Com��־
  *          TIM_FLAG_Trigger: TIM ������־λ
  *          TIM_FLAG_Break: TIM �жϱ�־
  *          TIM_FLAG_CC1OF: TIM ����/�Ƚ� 1 �����־λ
  *          TIM_FLAG_CC2OF: TIM ����/�Ƚ� 2 �����־λ
  *          TIM_FLAG_CC3OF: TIM ����/�Ƚ� 3 �����־λ
  *          TIM_FLAG_CC4OF: TIM ����/�Ƚ� 4 �����־λ
  * @ע��
  *   - TIM6 �� TIM7 ������һ�������ж�. 
  *   - TIM_FLAG_COM �� TIM_FLAG_Break �������Ժ� TIM1 �� TIM8 һ��ʹ��.    
  * @����  û��.
  */
void TIM_ClearFlag(TIM_TypeDef* TIMx, uint16_t TIM_FLAG)
{  
  /* ������ */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_CLEAR_FLAG(TIM_FLAG));
   
  /* �����־λ */
  TIMx->SR = (uint16_t)~TIM_FLAG;
}

/**
  * @����  ���ָ���� TIM �жϷ������.
  * @����  TIMx: �˴�x������1��8��ѡ������.
  * @����  TIM_IT: ������ TIM �ж�Դ.
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_IT_Update: TIM �����ж�Դ
  *          TIM_IT_CC1: TIM ����/�Ƚ� 1 �ж�Դ
  *          TIM_IT_CC2: TIM ����/�Ƚ� 2 �ж�Դ
  *          TIM_IT_CC3: TIM ����/�Ƚ� 3 �ж�Դ
  *          TIM_IT_CC4: TIM ����/�Ƚ� 4 �ж�Դ
  *          TIM_IT_COM: TIM Com �ж�Դ
  *          TIM_IT_Trigger: TIM �����ж�Դ
  *          TIM_IT_Break: TIM ��Ϣ�ж�Դ
  * @ע��
  *   - TIM6 �� TIM7 ������һ�������ж�. 
  *   - TIM_FLAG_COM �� TIM_FLAG_Break �������Ժ� TIM1 �� TIM8 һ��ʹ��.   
  * @����  TIM_IT ����״̬(SET �� RESET).
  */
ITStatus TIM_GetITStatus(TIM_TypeDef* TIMx, uint16_t TIM_IT)
{
  ITStatus bitstatus = RESET;  
  uint16_t itstatus = 0x0, itenable = 0x0;
  /* ������ */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_GET_IT(TIM_IT));
   
  itstatus = TIMx->SR & TIM_IT;
  
  itenable = TIMx->DIER & TIM_IT;
  if ((itstatus != (uint16_t)RESET) && (itenable != (uint16_t)RESET))
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
  * @����  ��� TIMx ���жϴ�����λ.
  * @����  TIMx: �˴�x������1��8��ѡ������.
  * @����  TIM_IT: ָ���жϴ�����λ.
  *          �����������������ֵ���������:
  *          TIM_IT_Update: TIM �����ж�Դ
  *          TIM_IT_CC1: TIM ����/�Ƚ� 1 �ж�Դ
  *          TIM_IT_CC2: TIM ����/�Ƚ� 2 �ж�Դ
  *          TIM_IT_CC3: TIM ����/�Ƚ� 3 �ж�Դ
  *          TIM_IT_CC4: TIM ����/�Ƚ� 4 �ж�Դ
  *          TIM_IT_COM: TIM Com �ж�Դ
  *          TIM_IT_Trigger: TIM �����ж�Դ
  *          TIM_IT_Break: TIM ��Ϣ�ж�Դ
  * @ע��
  *   - TIM6 �� TIM7 ������һ�������ж�. 
  *   - TIM_FLAG_COM �� TIM_FLAG_Break �������Ժ� TIM1 �� TIM8 һ��ʹ��.  
  * @����  û��
  */
void TIM_ClearITPendingBit(TIM_TypeDef* TIMx, uint16_t TIM_IT)
{
  /* ������ */
  assert_param(IS_TIM_ALL_PERIPH(TIMx));
  assert_param(IS_TIM_IT(TIM_IT));
  /* ��� TIMx ���жϴ�����λ */
  TIMx->SR = (uint16_t)~TIM_IT;
}

/**
  * @����  ���� TI1 ��Ϊ����.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_ICPolarity : ���뼫��.
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_ICPolarity_Rising
  *          TIM_ICPolarity_Falling
  * @����  TIM_ICSelection: ʹ��ָ��������.
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_ICSelection_DirectTI:   ѡ��TIM ����1����IC1.
  *          TIM_ICSelection_IndirectTI: ѡ��TIM ����1����IC2.
  *          TIM_ICSelection_TRC:        ѡ��TIM ����1����TRC.
  * @����  TIM_ICFilter: ָ�������벶���˲���.
  *                      ���������ֵ������ 0x00 �� 0x0F֮��.
  * @����  û��
  */
static void TI1_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter)
{
  uint16_t tmpccmr1 = 0, tmpccer = 0;
  /* ʧ��ͨ�� 1: ��λ CC1E λ */
  TIMx->CCER &= CCER_CC1E_Reset;
  tmpccmr1 = TIMx->CCMR1;
  tmpccer = TIMx->CCER;
  /* ѡ��������˲��� */
  tmpccmr1 &= CCMR_CC13S_Mask & CCMR_IC13F_Mask;
  tmpccmr1 |= (uint16_t)(TIM_ICSelection | (uint16_t)(TIM_ICFilter << (uint16_t)4));
  /* ѡ���Ժ���λCC1E */
  tmpccer &= CCER_CC1P_Reset;
  tmpccer |= (uint16_t)(TIM_ICPolarity | (uint16_t)CCER_CC1E_Set);
  /* дTIMx CCMR1��CCER�Ĵ��� */
  TIMx->CCMR1 = tmpccmr1;
  TIMx->CCER = tmpccer;
}

/**
  * @����  ���� TI2 ��Ϊ����.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_ICPolarity : ���뼫��.
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_ICPolarity_Rising
  *          TIM_ICPolarity_Falling
  * @����  TIM_ICSelection: ʹ��ָ��������.
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_ICSelection_DirectTI:   ѡ��TIM ����2����IC2.
  *          TIM_ICSelection_IndirectTI: ѡ��TIM ����2����IC1.
  *          TIM_ICSelection_TRC:        ѡ��TIM ����2����TRC.
  * @����  TIM_ICFilter: ָ�������벶���˲���.
  *                      ���������ֵ������ 0x00 �� 0x0F֮��.
  * @����  û��
  */
static void TI2_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter)
{
  uint16_t tmpccmr1 = 0, tmpccer = 0, tmp = 0;
  /* ʧ��ͨ�� 2: ��λ CC2E λ */
  TIMx->CCER &= CCER_CC2E_Reset;
  tmpccmr1 = TIMx->CCMR1;
  tmpccer = TIMx->CCER;
  tmp = (uint16_t)(TIM_ICPolarity << 4);
  /* ѡ��������˲��� */
  tmpccmr1 &= CCMR_CC24S_Mask & CCMR_IC24F_Mask;
  tmpccmr1 |= (uint16_t)(TIM_ICFilter << 12);
  tmpccmr1 |= (uint16_t)(TIM_ICSelection << 8);
  /* ѡ���Ժ���λCC2E */
  tmpccer &= CCER_CC2P_Reset;
  tmpccer |=  (uint16_t)(tmp | (uint16_t)CCER_CC2E_Set);
  /* дTIMx CCMR1��CCER�Ĵ��� */
  TIMx->CCMR1 = tmpccmr1 ;
  TIMx->CCER = tmpccer;
}

/**
  * @����  ���� TI3 ��Ϊ����.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_ICPolarity : ���뼫��.
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_ICPolarity_Rising
  *          TIM_ICPolarity_Falling
  * @����  TIM_ICSelection: ʹ��ָ��������.
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_ICSelection_DirectTI:   ѡ��TIM ����3����IC3.
  *          TIM_ICSelection_IndirectTI: ѡ��TIM ����3����IC4.
  *          TIM_ICSelection_TRC:        ѡ��TIM ����3����TRC.
  * @����  TIM_ICFilter: ָ�������벶���˲���.
  *                      ���������ֵ������ 0x00 �� 0x0F֮��.
  * @����  û��
  */
static void TI3_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter)
{
  uint16_t tmpccmr2 = 0, tmpccer = 0, tmp = 0;
  /* ʧ��ͨ�� 3: ��λ CC3E λ */
  TIMx->CCER &= CCER_CC3E_Reset;
  tmpccmr2 = TIMx->CCMR2;
  tmpccer = TIMx->CCER;
  tmp = (uint16_t)(TIM_ICPolarity << 8);
  /* ѡ��������˲��� */
  tmpccmr2 &= CCMR_CC13S_Mask & CCMR_IC13F_Mask;
  tmpccmr2 |= (uint16_t)(TIM_ICSelection | (uint16_t)(TIM_ICFilter << (uint16_t)4));
  /* ѡ���Ժ���λCC3E */
  tmpccer &= CCER_CC3P_Reset;
  tmpccer |= (uint16_t)(tmp | (uint16_t)CCER_CC3E_Set);
  /* дTIMx CCMR2��CCER�Ĵ��� */
  TIMx->CCMR2 = tmpccmr2;
  TIMx->CCER = tmpccer;
}

/**
  * @����  ���� TI4 ��Ϊ����.
  * @����  TIMx: �˴�x������1,2,3,4,5��8��ѡ������.
  * @����  TIM_ICPolarity : ���뼫��.
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_ICPolarity_Rising
  *          TIM_ICPolarity_Falling
  * @����  TIM_ICSelection: ʹ��ָ��������.
  *          �����������ȡ�����ֵ֮һ:
  *          TIM_ICSelection_DirectTI:   ѡ��TIM ����3����IC4.
  *          TIM_ICSelection_IndirectTI: ѡ��TIM ����3����IC3.
  *          TIM_ICSelection_TRC:        ѡ��TIM ����3����TRC.
  * @����  TIM_ICFilter: ָ�������벶���˲���.
  *                      ���������ֵ������ 0x00 �� 0x0F֮��.
  * @����  û��
  */
static void TI4_Config(TIM_TypeDef* TIMx, uint16_t TIM_ICPolarity, uint16_t TIM_ICSelection,
                       uint16_t TIM_ICFilter)
{
  uint16_t tmpccmr2 = 0, tmpccer = 0, tmp = 0;

  /* ʧ��ͨ�� 4: ��λ CC4E λ */
  TIMx->CCER &= CCER_CC4E_Reset;
  tmpccmr2 = TIMx->CCMR2;
  tmpccer = TIMx->CCER;
  tmp = (uint16_t)(TIM_ICPolarity << 12);

  /* ѡ����������ù����� */
  tmpccmr2 &= CCMR_CC24S_Mask & CCMR_IC24F_Mask;
  tmpccmr2 |= (uint16_t)(TIM_ICSelection << 8);
  tmpccmr2 |= (uint16_t)(TIM_ICFilter << 12);

  /* ѡ���Ժ���λCC4E */
  tmpccer &= CCER_CC4P_Reset;
  tmpccer |= (uint16_t)(tmp | (uint16_t)CCER_CC4E_Set);
  /* дTIMx CCMR2��CCER�Ĵ��� */
  TIMx->CCMR2 = tmpccmr2;
  TIMx->CCER = tmpccer ;
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
