/**
  ******************************************************************************
  * @�ļ�    stm32f10x_rcc.c
  * @����    MCD Ӧ�ó�����
  * @�汾    V3.1.0
  * @����    06/19/2009
  * @brief   ����ļ� �ṩ���� RCC �̼�����.
  * @����    ANSON/sweet1985 Email:airanson110@126.com
  * @����    03/07/2009
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
#include "stm32f10x_rcc.h"

/* �������� --------------------------------------------------------------*/
/* ���ö��� --------------------------------------------------------------*/

/* ------------ RCC registers bit address in the alias region ----------- */
#define RCC_OFFSET                (RCC_BASE - PERIPH_BASE)

/* --- CR �Ĵ��� ---*/

/* Alias word address of HSION bit */
#define CR_OFFSET                 (RCC_OFFSET + 0x00)
#define HSION_BitNumber           0x00
#define CR_HSION_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (HSION_BitNumber * 4))

/* Alias word address of PLLON bit */
#define PLLON_BitNumber           0x18
#define CR_PLLON_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (PLLON_BitNumber * 4))

#ifdef STM32F10X_CL
 /* Alias word address of PLL2ON bit */
 #define PLL2ON_BitNumber          0x1A
 #define CR_PLL2ON_BB              (PERIPH_BB_BASE + (CR_OFFSET * 32) + (PLL2ON_BitNumber * 4))

 /* Alias word address of PLL3ON bit */
 #define PLL3ON_BitNumber          0x1C
 #define CR_PLL3ON_BB              (PERIPH_BB_BASE + (CR_OFFSET * 32) + (PLL3ON_BitNumber * 4))
#endif /* STM32F10X_CL */ 

/* Alias word address of CSSON bit */
#define CSSON_BitNumber           0x13
#define CR_CSSON_BB               (PERIPH_BB_BASE + (CR_OFFSET * 32) + (CSSON_BitNumber * 4))

/* --- CFGR �Ĵ��� ---*/

/* Alias word address of USBPRE bit */
#define CFGR_OFFSET               (RCC_OFFSET + 0x04)

#ifndef STM32F10X_CL
 #define USBPRE_BitNumber          0x16
 #define CFGR_USBPRE_BB            (PERIPH_BB_BASE + (CFGR_OFFSET * 32) + (USBPRE_BitNumber * 4))
#else
 #define OTGFSPRE_BitNumber        0x16
 #define CFGR_OTGFSPRE_BB          (PERIPH_BB_BASE + (CFGR_OFFSET * 32) + (OTGFSPRE_BitNumber * 4))
#endif /* STM32F10X_CL */ 

/* --- BDCR �Ĵ��� ---*/

/* Alias word address of RTCEN bit */
#define BDCR_OFFSET               (RCC_OFFSET + 0x20)
#define RTCEN_BitNumber           0x0F
#define BDCR_RTCEN_BB             (PERIPH_BB_BASE + (BDCR_OFFSET * 32) + (RTCEN_BitNumber * 4))

/* Alias word address of BDRST bit */
#define BDRST_BitNumber           0x10
#define BDCR_BDRST_BB             (PERIPH_BB_BASE + (BDCR_OFFSET * 32) + (BDRST_BitNumber * 4))

/* --- CSR �Ĵ��� ---*/

/* Alias word address of LSION bit */
#define CSR_OFFSET                (RCC_OFFSET + 0x24)
#define LSION_BitNumber           0x00
#define CSR_LSION_BB              (PERIPH_BB_BASE + (CSR_OFFSET * 32) + (LSION_BitNumber * 4))

#ifdef STM32F10X_CL
/* --- CFGR2 �Ĵ��� ---*/

 /* Alias word address of I2S2SRC bit */
 #define CFGR2_OFFSET              (RCC_OFFSET + 0x2C)
 #define I2S2SRC_BitNumber         0x11
 #define CFGR2_I2S2SRC_BB          (PERIPH_BB_BASE + (CFGR2_OFFSET * 32) + (I2S2SRC_BitNumber * 4))

 /* Alias word address of I2S3SRC bit */
 #define I2S3SRC_BitNumber         0x12
 #define CFGR2_I2S3SRC_BB          (PERIPH_BB_BASE + (CFGR2_OFFSET * 32) + (I2S3SRC_BitNumber * 4))
#endif /* STM32F10X_CL */

/* ---------------------- RCC �Ĵ��� bit mask ------------------------ */

/* CR register bit mask */
#define CR_HSEBYP_Reset           ((uint32_t)0xFFFBFFFF)
#define CR_HSEBYP_Set             ((uint32_t)0x00040000)
#define CR_HSEON_Reset            ((uint32_t)0xFFFEFFFF)
#define CR_HSEON_Set              ((uint32_t)0x00010000)
#define CR_HSITRIM_Mask           ((uint32_t)0xFFFFFF07)

/* CFGR register bit mask */
#ifndef STM32F10X_CL
 #define CFGR_PLL_Mask            ((uint32_t)0xFFC0FFFF)
#else
 #define CFGR_PLL_Mask           ((uint32_t)0xFFC2FFFF)
#endif /* STM32F10X_CL */ 

#define CFGR_PLLMull_Mask         ((uint32_t)0x003C0000)
#define CFGR_PLLSRC_Mask          ((uint32_t)0x00010000)
#define CFGR_PLLXTPRE_Mask        ((uint32_t)0x00020000)
#define CFGR_SWS_Mask             ((uint32_t)0x0000000C)
#define CFGR_SW_Mask              ((uint32_t)0xFFFFFFFC)
#define CFGR_HPRE_Reset_Mask      ((uint32_t)0xFFFFFF0F)
#define CFGR_HPRE_Set_Mask        ((uint32_t)0x000000F0)
#define CFGR_PPRE1_Reset_Mask     ((uint32_t)0xFFFFF8FF)
#define CFGR_PPRE1_Set_Mask       ((uint32_t)0x00000700)
#define CFGR_PPRE2_Reset_Mask     ((uint32_t)0xFFFFC7FF)
#define CFGR_PPRE2_Set_Mask       ((uint32_t)0x00003800)
#define CFGR_ADCPRE_Reset_Mask    ((uint32_t)0xFFFF3FFF)
#define CFGR_ADCPRE_Set_Mask      ((uint32_t)0x0000C000)

/* CSR register bit mask */
#define CSR_RMVF_Set              ((uint32_t)0x01000000)

#ifdef STM32F10X_CL
/* CFGR2 register bit mask */
 #define CFGR2_PREDIV1SRC         ((uint32_t)0x00010000)
 #define CFGR2_PREDIV1            ((uint32_t)0x0000000F)
 #define CFGR2_PREDIV2            ((uint32_t)0x000000F0)
 #define CFGR2_PLL2MUL            ((uint32_t)0x00000F00)
 #define CFGR2_PLL3MUL            ((uint32_t)0x0000F000)
#endif /* STM32F10X_CL */ 

/* RCC Flag Mask */
#define FLAG_Mask                 ((uint8_t)0x1F)

#ifndef HSI_Value
/* Typical Value of the HSI in Hz */
 #define HSI_Value                 ((uint32_t)8000000)
#endif /* HSI_Value */

/* CIR register byte 2 (Bits[15:8]) base address */
#define CIR_BYTE2_ADDRESS         ((uint32_t)0x40021009)

/* CIR register byte 3 (Bits[23:16]) base address */
#define CIR_BYTE3_ADDRESS         ((uint32_t)0x4002100A)

/* CFGR register byte 4 (Bits[31:24]) base address */
#define CFGR_BYTE4_ADDRESS        ((uint32_t)0x40021007)

/* BDCR register base address */
#define BDCR_ADDRESS              (PERIPH_BASE + BDCR_OFFSET)

#ifndef HSEStartUp_TimeOut
/* Time out for HSE start up */
 #define HSEStartUp_TimeOut        ((uint16_t)0x0500)
#endif /* HSEStartUp_TimeOut */

/* ���ú� ------------------------------------------------------------*/
/* ���ñ��� ----------------------------------------------------------*/

static __I uint8_t APBAHBPrescTable[16] = {0, 0, 0, 0, 1, 2, 3, 4, 1, 2, 3, 4, 6, 7, 8, 9};
static __I uint8_t ADCPrescTable[4] = {2, 4, 6, 8};

/* ���ú���ԭ�� ------------------------------------------------------*/
/* ���ú��� ----------------------------------------------------------*/


/**
  * @����  ������RCC�Ĵ�������Ϊȱʡֵ.
  * @����  û��
  * @����  û��
  */
void RCC_DeInit(void)
{
  /* ��λHSION (�ڲ�<8MHz>����ʱ��ʹ��) */
  RCC->CR |= (uint32_t)0x00000001;

  /* ��λ SW, HPRE, PPRE1, PPRE2, ADCPRE , MCO λ */
  /* (HIS��ѡ��ϵͳʱ�ӣ�HSI����������ϵͳʱ�ӣ�SYSCLKδ����Ƶ��HCLKδ����Ƶ��
     ADCԤ��Ƶ��<PLCK2>��2��Ƶ��HSI����ʱ��/2��ѡ��PLL����ʱ��)��΢��������ʱ����� */
#ifndef STM32F10X_CL
  RCC->CFGR &= (uint32_t)0xF8FF0000;   /* ?�ڱ�������1 */
#else
  RCC->CFGR &= (uint32_t)0xF0FF0000;   /* ?�ڱ�������0 */
#endif /* STM32F10X_CL */   
  
  /* ��λ HSEON, CSSON, PLLON λ (PLL���ã�ʱ�Ӽ�������ã�HSE��������) */
  RCC->CR &= (uint32_t)0xFEF6FFFF;

  /* ��λ HSEBYP λ (�ⲿ1~25MHz����δ����·) */
  RCC->CR &= (uint32_t)0xFFFBFFFF;

  /* ��λ PLLSRC, PLLXTPRE, PLLMUL , USBPRE/OTGFSPRE λ */
  /*(USBԤ��Ƶ��ΪPLLʱ��1.5��Ƶ��PLL��Ƶ����X2��HSEδ����Ƶ��ΪPLL���룬HSI����ʱ��/2������PLL����) */
  RCC->CFGR &= (uint32_t)0xFF80FFFF;

#ifndef STM32F10X_CL
  /* ʹ����ʱ���ж�ʧ�� */
  RCC->CIR = 0x009F0000;
#else
  /* ��λ PLL2ON , PLL3ON λ () */
  RCC->CR &= (uint32_t)0xEBFFFFFF;

  /* ʹ����ʱ���ж�ʧ��  */
  RCC->CIR = 0x00FF0000;

  /* ��λ CFGR2 �Ĵ��� */
  RCC->CFGR2 = 0x00000000;
#endif /* STM32F10X_CL */
}

/**
  * @����  �����ⲿ�������� (HSE).
  * @ע��  ��� HSE ��ֱ��ʹ�û���ͨ�� PLL ��Ϊϵͳʱ�ӣ���������ֹͣ.
  * @����  RCC_HSE: ָ�� HSE �µ�״̬.
  *                 �����������ѡ�������һ��:
  *             RCC_HSE_OFF: HSE ���� OFF
  *             RCC_HSE_ON : HSE ���� ON
  *             RCC_HSE_Bypass: HSE ������·ʹ���ⲿʱ��
  * @����  û��
  */
void RCC_HSEConfig(uint32_t RCC_HSE)
{
  /* ������ */
  assert_param(IS_RCC_HSE(RCC_HSE));
  /* ��λ HSEON , HSEBYP λ ������ HSE ��ǰ ------------------*/
  /* Reset HSEON bit (���� HSE ����)*/
  RCC->CR &= CR_HSEON_Reset;
  /* Reset HSEBYP bit (�ⲿ1~25MHz����δ����·)*/
  RCC->CR &= CR_HSEBYP_Reset;
  /* ���� HSE (RCC_HSE_OFF �Ѿ���code���Ĵ��븲����) */
  switch(RCC_HSE)
  {
    case RCC_HSE_ON:
      /* Set HSEON bit (�� HSE ����)*/
      RCC->CR |= CR_HSEON_Set;
      break;
      
    case RCC_HSE_Bypass:
      /* Set HSEBYP and HSEON bits (���� HSE �������ⲿ1~25MHz�������ⲿʱ����·)*/
      RCC->CR |= CR_HSEBYP_Set | CR_HSEON_Set;
      break;
      
    default:
      break;
  }
}

/**
  * @����  �ȴ� HSE ����.
  * @����  û��
  * @����  һ��ErrorStatusö��ֵ:
  *        SUCCESS: HSE �����Ѿ��ȶ����ҿ���ʹ��
  *        ERROR  : HSE ������δ׼����
  */
ErrorStatus RCC_WaitForHSEStartUp(void)
{
  __IO uint32_t StartUpCounter = 0;
  ErrorStatus status = ERROR;
  FlagStatus HSEStatus = RESET;
  
  /* �ȴ� HSE ׼���úͳ�ʱ��ʱ�䵽�� */
  do
  {
    HSEStatus = RCC_GetFlagStatus(RCC_FLAG_HSERDY);
    StartUpCounter++;  
  } while((StartUpCounter != HSEStartUp_TimeOut) && (HSEStatus == RESET));
  
  if (RCC_GetFlagStatus(RCC_FLAG_HSERDY) != RESET)
  {
    status = SUCCESS;
  }
  else
  {
    status = ERROR;
  }  
  return (status);
}

/**
  * @����  �����ڲ��������� (HSI) ���ֵ.
  * @����  HSICalibrationValue: У�����ֵ���������һ������0��0x1F֮���ֵ.
  * @����  û��
  */
void RCC_AdjustHSICalibrationValue(uint8_t HSICalibrationValue)
{
  uint32_t tmpreg = 0;
  /* ������ */
  assert_param(IS_RCC_CALIBRATION_VALUE(HSICalibrationValue));
  tmpreg = RCC->CR;
  /* ��� HSITRIM[4:0] λ (Ĭ��ֵ��16ʹ HIS �ϸ�����8MHz) */
  tmpreg &= CR_HSITRIM_Mask;
  /* ���� HSITRIM[4:0] λ������ HSICalibrationValue ��ֵ */
  tmpreg |= (uint32_t)HSICalibrationValue << 3;
  /* �洢�µ�ֵ */
  RCC->CR = tmpreg;
}

/**
  * @����  ʹ�ܻ�ʧ���ڲ��������� (HSI).
  * @ע��  ��� HSE ��ֱ��ʹ�û���ͨ�� PLL ��Ϊϵͳʱ�ӣ���������ֹͣ.
  * @����  NewState: HSI ����״̬. 
  *                 ���������ȡ: ENABLE �� DISABLE.
  * @����  û��
  */
void RCC_HSICmd(FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CR_HSION_BB = (uint32_t)NewState;
}

/**
  * @����  ����PLLʱ��Դ����Ƶϵ��.
  * @ע��  �����������Ӧ���� PLL ����ֹ��ʱ��.
  * @����  RCC_PLLSource: ָ������ PLL ��ʱ��Դ.
  *                       STM32_����ϵ��΢������, ���������ȡ���е�ֵ:
  *                                      RCC_PLLSource_HSI_Div2: PLL ʱ��������� HSI ����ʱ�Ӷ���Ƶ
  *                                      RCC_PLLSource_PREDIV1: PREDIV1 clock selected as PLL clock entry
  *                       ����_STM32_΢������, ���������ȡ���е�ֵ:
  *                                      RCC_PLLSource_HSI_Div2: PLL ʱ��������� HSI ����ʱ�Ӷ���Ƶ
  *                                      RCC_PLLSource_HSE_Div1: PLL ʱ��������� HSE ����ʱ��
  *                                      RCC_PLLSource_HSE_Div2: PLL ʱ��������� HSE ����ʱ�Ӷ���Ƶ
  * @����  RCC_PLLMul: ָ�� PLL ��Ƶϵ��.
  *                    STM32_Connectivity_line_devices, ������������� RCC_PLLMul_x x����ȡ{[4,9], 6_5}
  *                    other_STM32_devices, ������������� RCC_PLLMul_x x����ȡ:[2,16]
  * @����  û��
  */
void RCC_PLLConfig(uint32_t RCC_PLLSource, uint32_t RCC_PLLMul)
{
  uint32_t tmpreg = 0;

  /* ������ */
  assert_param(IS_RCC_PLL_SOURCE(RCC_PLLSource));
  assert_param(IS_RCC_PLL_MUL(RCC_PLLMul));

  tmpreg = RCC->CFGR;
  /* �� PLLSRC, PLLXTPRE , PLLMUL[3:0] λ */
  tmpreg &= CFGR_PLL_Mask;
  /* ���� PLL ����λ */
  tmpreg |= RCC_PLLSource | RCC_PLLMul;
  /* �洢��ֵ */
  RCC->CFGR = tmpreg;
}

/**
  * @����  ʹ�ܻ�ʧ�� PLL.
  * @ע��  ��� PLL ��Ϊϵͳʱ�ӣ���������ֹͣ.
  * @����  NewState: PLL ����״̬. �����������ȡ: ENABLE �� DISABLE.
  * @����  û��
  */
void RCC_PLLCmd(FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(__IO uint32_t *) CR_PLLON_BB = (uint32_t)NewState;
}

#ifdef STM32F10X_CL
/**
  * @����  ���� PREDIV1 ��Ƶ����.
  * @ע�� 
  *   - �����������Ӧ���� PLL ����ֹ��ʱ��.
  *   - �����������Ӧ���� STM32 ����ϵ��΢������.
  * @����  RCC_PREDIV1_Source: ָ�� PREDIV1 ʱ��Դ.
  *                            �����������ȡ����ֵ�е�һ��:
  *                            RCC_PREDIV1_Source_HSE: HSE ��Ϊ PREDIV1 ʱ��
  *                            RCC_PREDIV1_Source_PLL2: PLL2 ��Ϊ PREDIV1 ʱ��
  * @����  RCC_PREDIV1_Div: ѡ�� PREDIV1 ��Ƶϵ��.
  *                         ������� RCC_PREDIV1_Divx �� x����ȡ:[1,16]
  * @����  û��
  */
void RCC_PREDIV1Config(uint32_t RCC_PREDIV1_Source, uint32_t RCC_PREDIV1_Div)
{
  uint32_t tmpreg = 0;
  
  /* ������ */
  assert_param(IS_RCC_PREDIV1_SOURCE(RCC_PREDIV1_Source));
  assert_param(IS_RCC_PREDIV1(RCC_PREDIV1_Div));

  tmpreg = RCC->CFGR2;
  /* �� PREDIV1[3:0] �� PREDIV1SRC λ */
  tmpreg &= ~(CFGR2_PREDIV1 | CFGR2_PREDIV1SRC);
  /* Set the PREDIV1 clock source and division factor */
  tmpreg |= RCC_PREDIV1_Source | RCC_PREDIV1_Div ;
  /* �洢��ֵ */
  RCC->CFGR2 = tmpreg;
}


/**
  * @����  ���� PREDIV2 ��Ƶ����.
  * @ע�� 
  *   - �����������Ӧ���� PLL2,PLL3 ����ֹ��ʱ��.
  *   - �����������Ӧ���� STM32 ����ϵ��΢������.
  * @����  RCC_PREDIV2_Div: ѡ�� PREDIV2 ��Ƶϵ��.
  *                         ������� RCC_PREDIV2_Divx �� x����ȡ:[1,16]
  * @����  û��
  */
void RCC_PREDIV2Config(uint32_t RCC_PREDIV2_Div)
{
  uint32_t tmpreg = 0;

  /* Check the parameters */
  assert_param(IS_RCC_PREDIV2(RCC_PREDIV2_Div));

  tmpreg = RCC->CFGR2;
  /* Clear PREDIV2[3:0] bits */
  tmpreg &= ~CFGR2_PREDIV2;
  /* Set the PREDIV2 division factor */
  tmpreg |= RCC_PREDIV2_Div;
  /* Store the new value */
  RCC->CFGR2 = tmpreg;
}

/**
  * @����  ���� PLL2 ��Ƶϵ��.
  * @ע��
  *   - �����������Ӧ���� PLL2 ����ֹ��ʱ��.
  *   - �����������Ӧ���� STM32 ����ϵ��΢������.
  * @����  RCC_PLL2Mul: ָ�� PLL2 ��Ƶϵ��.
  *                     ������� RCC_PLL2Mul_x �� x����ȡ:{[8,14], 16, 20}
  * @����  û��
  */
void RCC_PLL2Config(uint32_t RCC_PLL2Mul)
{
  uint32_t tmpreg = 0;

  /* ������ */
  assert_param(IS_RCC_PLL2_MUL(RCC_PLL2Mul));

  tmpreg = RCC->CFGR2;
  /* �� PLL2Mul[3:0] λ */
  tmpreg &= ~CFGR2_PLL2MUL;
  /* ���� PLL2 ����λ */
  tmpreg |= RCC_PLL2Mul;
  /* �洢���� */
  RCC->CFGR2 = tmpreg;
}


/**
  * @����  ʹ�ܻ�ʧ�� PLL2.
  * @ע�� 
  *   - ��� PLL2 ������ϵͳʱ�ӣ����Ͳ��ܱ���ֹ
  *     (i.e.�������� PLL ʱ������Դ������˵������ϵͳʱ��).
  *   - �����������Ӧ���� STM32 ����ϵ��΢������.
  * @����  NewState: PLL2 �µ�״̬. �����������ȡ: ENABLE �� DISABLE.
  * @����  û��
  */
void RCC_PLL2Cmd(FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  *(__IO uint32_t *) CR_PLL2ON_BB = (uint32_t)NewState;
}


/**
  * @����  ���� PLL3 ��Ƶϵ��.
  * @ע��
  *   - �����������Ӧ���� PLL3 ����ֹ��ʱ��.
  *   - �����������Ӧ���� STM32 ����ϵ��΢������.
  * @����  RCC_PLL3Mul: ָ�� PLL3 ��Ƶϵ��.
  *                     ������� RCC_PLL3Mul_x �� x����ȡ:{[8,14], 16, 20}
  * @����  û��
  */
void RCC_PLL3Config(uint32_t RCC_PLL3Mul)
{
  uint32_t tmpreg = 0;

  /* ������ */
  assert_param(IS_RCC_PLL3_MUL(RCC_PLL3Mul));

  tmpreg = RCC->CFGR2;
  /* �� PLL3Mul[3:0] λ */
  tmpreg &= ~CFGR2_PLL3MUL;
  /* ���� PLL3 ����λ */
  tmpreg |= RCC_PLL3Mul;
  /* �洢��ֵ */
  RCC->CFGR2 = tmpreg;
}


/**
  * @����  ʹ�ܻ�ʧ�� PLL3.
  * @ע��  �����������Ӧ���� STM32 ����ϵ��΢������.
  * @����  NewState: PLL3 �µ�״̬. �����������ȡ: ENABLE �� DISABLE.
  * @����  û��
  */
void RCC_PLL3Cmd(FunctionalState NewState)
{
  /* Check the parameters */

  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CR_PLL3ON_BB = (uint32_t)NewState;
}
#endif /* STM32F10X_CL */

/**
  * @����  ����ϵͳʱ�� (SYSCLK).
  * @����  RCC_SYSCLKSource: ָ����Ϊϵͳʱ�ӵ�ʱ��Դ.
  *                          �����������ѡ�������е�һ��:
  *                          RCC_SYSCLKSource_HSI:   ѡ�� HSI ��Ϊϵͳʱ��
  *                          RCC_SYSCLKSource_HSE:   ѡ�� HSE ��Ϊϵͳʱ��
  *                          RCC_SYSCLKSource_PLLCLK:ѡ�� PLL ��Ϊϵͳʱ��
  * @����  û��
  */
void RCC_SYSCLKConfig(uint32_t RCC_SYSCLKSource)
{
  uint32_t tmpreg = 0;
  /* ������ */
  assert_param(IS_RCC_SYSCLK_SOURCE(RCC_SYSCLKSource));
  tmpreg = RCC->CFGR;
  /* �� SW[1:0] λ  */
  tmpreg &= CFGR_SW_Mask;
  /* ���� SW[1:0] λΪ RCC_SYSCLKSource ��ֵ */
  tmpreg |= RCC_SYSCLKSource;
  /* �洢��ֵ */
  RCC->CFGR = tmpreg;
}

/**
  * @����  �������ϵͳʱ�ӵ�ʱ��Դ.
  * @����  û��
  * @����  ��Ϊϵͳʱ�ӵ�ʱ��Դ. 
  *        �������ֵ����������֮�е�һ��:
  *        0x00: HSI ������ϵͳʱ��
  *        0x04: HSE ������ϵͳʱ��
  *        0x08: PLL ������ϵͳʱ��
  */
uint8_t RCC_GetSYSCLKSource(void)
{
  return ((uint8_t)(RCC->CFGR & CFGR_SWS_Mask));
}

/**
  * @����  ���� AHB ʱ�� (HCLK).
  * @����  RCC_SYSCLK: ���� AHB ʱ�ӷ�Ƶ. ���ʱ����Դ��ϵͳʱ��
  *                    �����������ȡ����ֵ�е�һ��:
  *                    RCC_SYSCLK_Div1: AHB clock = SYSCLK
  *                    RCC_SYSCLK_Div2: AHB clock = SYSCLK/2
  *                    RCC_SYSCLK_Div4: AHB clock = SYSCLK/4
  *                    RCC_SYSCLK_Div8: AHB clock = SYSCLK/8
  *                    RCC_SYSCLK_Div16: AHB clock = SYSCLK/16
  *                    RCC_SYSCLK_Div64: AHB clock = SYSCLK/64
  *                    RCC_SYSCLK_Div128: AHB clock = SYSCLK/128
  *                    RCC_SYSCLK_Div256: AHB clock = SYSCLK/256
  *                    RCC_SYSCLK_Div512: AHB clock = SYSCLK/512
  * @����  û��
  */
void RCC_HCLKConfig(uint32_t RCC_SYSCLK)
{
  uint32_t tmpreg = 0;
  /* ������ */
  assert_param(IS_RCC_HCLK(RCC_SYSCLK));
  tmpreg = RCC->CFGR;
  /* �� HPRE[3:0] λ */
  tmpreg &= CFGR_HPRE_Reset_Mask;
  /* ���� HPRE[3:0] λΪ RCC_SYSCLK ��ֵ */
  tmpreg |= RCC_SYSCLK;
  /* �洢��ֵ */
  RCC->CFGR = tmpreg;
}

/**
  * @����  ���õ��� APB ʱ�� (PCLK1).
  * @����  RCC_HCLK: ���� APB1 ʱ�ӷ�Ƶ. ���ʱ����Դ�� AHB ʱ�� (HCLK).
  *                  �����������ȡ����ֵ�е�һ��:
  *                  RCC_HCLK_Div1: APB1 clock = HCLK
  *                  RCC_HCLK_Div2: APB1 clock = HCLK/2
  *                  RCC_HCLK_Div4: APB1 clock = HCLK/4
  *                  RCC_HCLK_Div8: APB1 clock = HCLK/8
  *                  RCC_HCLK_Div16: APB1 clock = HCLK/16
  * @����  û��
  */
void RCC_PCLK1Config(uint32_t RCC_HCLK)
{
  uint32_t tmpreg = 0;
  /* ������ */
  assert_param(IS_RCC_PCLK(RCC_HCLK));
  tmpreg = RCC->CFGR;
  /* �� PPRE1[2:0] λ */
  tmpreg &= CFGR_PPRE1_Reset_Mask;
  /* ���� PPRE1[2:0] λΪ RCC_HCLK ��ֵ */
  tmpreg |= RCC_HCLK;
  /* �洢��ֵ */
  RCC->CFGR = tmpreg;
}

/**
  * @����  ���ø��� APB ʱ�� (PCLK2).
  * @����  RCC_HCLK: ���� APB2 ʱ�ӷ�Ƶ. ���ʱ����Դ�� AHB ʱ�� (HCLK).
  *                  �����������ȡ����ֵ�е�һ��:
  *                  RCC_HCLK_Div1: APB1 clock = HCLK
  *                  RCC_HCLK_Div2: APB1 clock = HCLK/2
  *                  RCC_HCLK_Div4: APB1 clock = HCLK/4
  *                  RCC_HCLK_Div8: APB1 clock = HCLK/8
  *                  RCC_HCLK_Div16: APB1 clock = HCLK/16
  * @����  û��
  */
void RCC_PCLK2Config(uint32_t RCC_HCLK)
{
  uint32_t tmpreg = 0;
  /* ������ */
  assert_param(IS_RCC_PCLK(RCC_HCLK));
  tmpreg = RCC->CFGR;
  /* �� PPRE2[2:0] λ */
  tmpreg &= CFGR_PPRE2_Reset_Mask;
  /* ���� PPRE2[2:0] λΪ RCC_HCLK ��ֵ */
  tmpreg |= RCC_HCLK << 3;
  /* �洢��ֵ */
  RCC->CFGR = tmpreg;
}

/**
  * @����  ʹ�ܻ�ʧ��ָ���� RCC �ж�.
  * @����  RCC_IT: ��ʹ�ܻ���ʧ�ܵ�RCC�ж�Դ.
  * 
  *                 STM32_����ϵ��΢������, �����������ȡ����ֵ�е�һ��:      
  *                                         RCC_IT_LSIRDY: LSI ׼���ж�
  *                                         RCC_IT_LSERDY: LSE ׼���ж�
  *                                         RCC_IT_HSIRDY: HSI ׼���ж�
  *                                         RCC_IT_HSERDY: HSE ׼���ж�
  *                                         RCC_IT_PLLRDY: PLL ׼���ж�
  *                                         RCC_IT_PLL2RDY: PLL2 ׼���ж�
  *                                         RCC_IT_PLL3RDY: PLL3 ׼���ж�
  * 
  *                    ����_STM32_΢������, �����������ȡ����ֵ�е�һ��:      
  *                                         RCC_IT_LSIRDY: LSI ׼���ж�
  *                                         RCC_IT_LSERDY: LSE ׼���ж�
  *                                         RCC_IT_HSIRDY: HSI ׼���ж�
  *                                         RCC_IT_HSERDY: HSE ׼���ж�
  *                                         RCC_IT_PLLRDY: PLL ׼���ж�
  *       
  * @����  NewState: RCC �ж��µ�״̬.
  *                   �����������ȡ: ENABLE �� DISABLE.
  * @����  û��
  */
void RCC_ITConfig(uint8_t RCC_IT, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_RCC_IT(RCC_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* ִ��RCC_CIR[12:8]λ���ֽڴ�ȡ��ʹ��ѡ����ж�s */
    *(__IO uint8_t *) CIR_BYTE2_ADDRESS |= RCC_IT;
  }
  else
  {
    /* ִ��RCC_CIR[12:8]λ���ֽڴ�ȡ����ֹѡ����ж� */
    *(__IO uint8_t *) CIR_BYTE2_ADDRESS &= (uint8_t)~RCC_IT;
  }
}

#ifndef STM32F10X_CL
/**
  * @����  ���� USB ʱ�� (USBCLK).
  * @����  RCC_USBCLKSource: ָ�� USB ��ʱ��Դ. ���ʱ����Դ�� PLL ���.
  *                          �����������ȡ����ֵ�е�һ��:
  *                          RCC_USBCLKSource_PLLCLK_1Div5: PLL ʱ�ӵ�1.5��Ƶ���� USB ʱ��Դ
  *                          RCC_USBCLKSource_PLLCLK_Div1:  PLL ʱ������ USB ʱ��Դ
  * @����  û��
  */
void RCC_USBCLKConfig(uint32_t RCC_USBCLKSource)
{
  /* ������ */
  assert_param(IS_RCC_USBCLK_SOURCE(RCC_USBCLKSource));

  *(__IO uint32_t *) CFGR_USBPRE_BB = RCC_USBCLKSource;
}
#else
/**
  * @����  ���� USB OTG FS ʱ�� (OTGFSCLK).
  *        �����������Ӧ���� STM32 ����ϵ��΢������.
  * @����  RCC_OTGFSCLKSource: ָ�� USB OTG FS ʱ��Դ.
  *                            ���ʱ����Դ�� PLL �����.
  *                            �����������ȡ����ֵ�е�һ��:
  *                            RCC_OTGFSCLKSource_PLLVCO_Div3: PLL VCO clock divided by 2 selected as USB OTG FS clock source
  *                            RCC_OTGFSCLKSource_PLLVCO_Div2: PLL VCO clock divided by 2 selected as USB OTG FS clock source
  * @����  û��
  */
void RCC_OTGFSCLKConfig(uint32_t RCC_OTGFSCLKSource)
{
  /* ������ */
  assert_param(IS_RCC_OTGFSCLK_SOURCE(RCC_OTGFSCLKSource));

  *(__IO uint32_t *) CFGR_OTGFSPRE_BB = RCC_OTGFSCLKSource;
}
#endif /* STM32F10X_CL */ 

/**
  * @����  ���� ADC ʱ�� (ADCCLK).
  * @����  RCC_PCLK2: ���� ADC ʱ�ӷ�Ƶ. ���ʱ����Դ�� APB2 ʱ�� (PCLK2).
  *                                      �����������ȡ����ֵ�е�һ��:
  *                                      RCC_PCLK2_Div2: ADC clock = PCLK2/2
  *                                      RCC_PCLK2_Div4: ADC clock = PCLK2/4
  *                                      RCC_PCLK2_Div6: ADC clock = PCLK2/6
  *                                      RCC_PCLK2_Div8: ADC clock = PCLK2/8
  * @����  û��
  */
void RCC_ADCCLKConfig(uint32_t RCC_PCLK2)
{
  uint32_t tmpreg = 0;
  /* ������ */
  assert_param(IS_RCC_ADCCLK(RCC_PCLK2));
  tmpreg = RCC->CFGR;
  /* �� ADCPRE[1:0] λ */
  tmpreg &= CFGR_ADCPRE_Reset_Mask;
  /* ���� ADCPRE[1:0] λȡ���� RCC_PCLK2 ��ֵ */
  tmpreg |= RCC_PCLK2;
  /* �洢��ֵ */
  RCC->CFGR = tmpreg;
}

#ifdef STM32F10X_CL
/**
  * @����  ���� I2S2 ʱ��Դ(I2S2CLK).
  * @ע��
  *   - �������������ʹ��I2S2 APBʱ��֮ǰ����.
  *   - �����������Ӧ���� STM32 ����ϵ��΢������.
  * @����  RCC_I2S2CLKSource: ָ�� I2S2 ʱ��Դ.
  *                           �����������ȡ����ֵ�е�һ��:
  *                           RCC_I2S2CLKSource_SYSCLK: ϵͳʱ����Ϊ I2S2 ʱ��Դ
  *                           RCC_I2S2CLKSource_PLL3_VCO: PLL3 VCO ��Ϊ I2S2 ʱ��Դ
  * @����  û��
  */
void RCC_I2S2CLKConfig(uint32_t RCC_I2S2CLKSource)
{
  /* ������ */
  assert_param(IS_RCC_I2S2CLK_SOURCE(RCC_I2S2CLKSource));

  *(__IO uint32_t *) CFGR2_I2S2SRC_BB = RCC_I2S2CLKSource;
}

/**
  * @����  ���� I2S3 ʱ��Դ(I2S2CLK).
  * @ע��
  *   - �������������ʹ��I2S3 APBʱ��֮ǰ����.
  *   - �����������Ӧ���� STM32 ����ϵ��΢������.
  * @����  RCC_I2S3CLKSource: ָ�� I2S3 ʱ��Դ.
  *                           �����������ȡ����ֵ�е�һ��:
  *                           RCC_I2S2CLKSource_SYSCLK: ϵͳʱ����Ϊ I2S3 ʱ��Դ
  *                           RCC_I2S2CLKSource_PLL3_VCO: PLL3 VCO ��Ϊ I2S3 ʱ��Դ
  * @����  û��
  */
void RCC_I2S3CLKConfig(uint32_t RCC_I2S3CLKSource)
{
  /* ָ������ */
  assert_param(IS_RCC_I2S3CLK_SOURCE(RCC_I2S3CLKSource));

  *(__IO uint32_t *) CFGR2_I2S3SRC_BB = RCC_I2S3CLKSource;
}
#endif /* STM32F10X_CL */

/**
  * @����  �����ⲿ�������� (LSE).
  * @����  RCC_LSE: ָ�� LSE ����״̬.
  *                 �����������ȡ����ֵ�е�һ��:
  *                 RCC_LSE_OFF: LSE ���� OFF
  *                 RCC_LSE_ON: LSE ���� ON
  *                 RCC_LSE_Bypass: LSE �������ⲿʱ����·
  * @����  û��
  */
void RCC_LSEConfig(uint8_t RCC_LSE)
{
  /* ������ */
  assert_param(IS_RCC_LSE(RCC_LSE));
  /* Reset LSEON and LSEBYP bits before configuring the LSE ------------------*/
  /* ��λ LSEON λ */
  *(__IO uint8_t *) BDCR_ADDRESS = RCC_LSE_OFF;
  /* ��λ LSEBYP λ */
  *(__IO uint8_t *) BDCR_ADDRESS = RCC_LSE_OFF;
  /* Configure LSE (RCC_LSE_OFF �Ѿ��� code ���벿�ֱ�����) */
  switch(RCC_LSE)
  {
    case RCC_LSE_ON:
      /* ���� LSEON λ */
      *(__IO uint8_t *) BDCR_ADDRESS = RCC_LSE_ON;
      break;
      
    case RCC_LSE_Bypass:
      /* ���� LSEBYP , LSEON λ */
      *(__IO uint8_t *) BDCR_ADDRESS = RCC_LSE_Bypass | RCC_LSE_ON;
      break;            
      
    default:
      break;      
  }
}

/**
  * @����  ʹ�ܻ�ʧ���ڲ��������� (LSI).
  * @ע��  ��� IWDG �������У�LSI �Ͳ��ܱ�����.
  * @����  NewState: LSI �µ�״̬. �����������ȡ: ENABLE �� DISABLE.
  * @����  û��
  */
void RCC_LSICmd(FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CSR_LSION_BB = (uint32_t)NewState;
}

/**
  * @����  ���� RTC ʱ�� (RTCCLK).
  * @ע��  ֻҪѡ���� RTC ʱ��,RTCʱ�ӾͲ��ܸı�ֱ��������λ.
  * @����  RCC_RTCCLKSource: ָ�� RTC ʱ��Դ.
  *                          �����������ȡ����ֵ�е�һ��:
  *                          RCC_RTCCLKSource_LSE: LSE ��Ϊ RTC ʱ��
  *                          RCC_RTCCLKSource_LSI: LSI ��Ϊ RTC ʱ��
  *                          RCC_RTCCLKSource_HSE_Div128: HSE ʱ�ӵ� 128 ��Ƶ��Ϊ RTC ʱ��
  * @����  û��
  */
void RCC_RTCCLKConfig(uint32_t RCC_RTCCLKSource)
{
  /* ������ */
  assert_param(IS_RCC_RTCCLK_SOURCE(RCC_RTCCLKSource));
  /* ָ�� RTC ʱ��Դ */
  RCC->BDCR |= RCC_RTCCLKSource;
}

/**
  * @����  ʹ�ܻ�ʧ�� RTC ʱ��.
  * @ע��  ����ʹ�� RCC_RTCCLKConfig ����ѡ�� RTC ʱ�Ӻ�,��ʹ���������.
  * @����  NewState: RTC ʱ�ӵ���״̬. �����������ȡ: ENABLE �� DISABLE.
  * @����  û��
  */
void RCC_RTCCLKCmd(FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) BDCR_RTCEN_BB = (uint32_t)NewState;
}

/**
  * @����  ����Ƭ��ʱ�ӵĲ�ͬƵ��.
  * @����  RCC_Clocks: ָ�����ʱ��Ƶ�ʵ� RCC_ClocksTypeDef �ṹ��ָ��.
  * @����  û��
  */
void RCC_GetClocksFreq(RCC_ClocksTypeDef* RCC_Clocks)
{
  uint32_t tmp = 0, pllmull = 0, pllsource = 0, presc = 0;

#ifdef  STM32F10X_CL
  uint32_t prediv1source = 0, prediv1factor = 0, prediv2factor = 0, pll2mull = 0;
#endif /* STM32F10X_CL */
    
  /* �õ�ϵͳʱ��Դ -------------------------------------------------------*/
  tmp = RCC->CFGR & CFGR_SWS_Mask;
  
  switch (tmp)
  {
    case 0x00:  /* HSI used as system clock */
      RCC_Clocks->SYSCLK_Frequency = HSI_Value;
      break;
    case 0x04:  /* HSE used as system clock */
      RCC_Clocks->SYSCLK_Frequency = HSE_Value;
      break;
    case 0x08:  /* PLL used as system clock */

      /* �õ� PLL ʱ��Դ�ͱ�Ƶ���� ----------------------*/
      pllmull = RCC->CFGR & CFGR_PLLMull_Mask;
      pllsource = RCC->CFGR & CFGR_PLLSRC_Mask;
      
#ifndef STM32F10X_CL      
      pllmull = ( pllmull >> 18) + 2;
      
      if (pllsource == 0x00)
      {/* HSI ��ʱ�ӵ�2��Ƶ��Ϊ PLL ʱ��Դ */
        RCC_Clocks->SYSCLK_Frequency = (HSI_Value >> 1) * pllmull;
      }
      else
      {/* HSE ��Ϊ PLL ʱ��Դ */
        if ((RCC->CFGR & CFGR_PLLXTPRE_Mask) != (uint32_t)RESET)
        {/* HSE ��ʱ�ӱ�2��Ƶ */
          RCC_Clocks->SYSCLK_Frequency = (HSE_Value >> 1) * pllmull;
        }
        else
        {
          RCC_Clocks->SYSCLK_Frequency = HSE_Value * pllmull;
        }
      }
#else
      pllmull = pllmull >> 18;
      
      if (pllmull != 0x0D)
      {
         pllmull += 2;
      }
      else
      { /* PLL ��Ƶ���� = PLL ����ʱ�� * 6.5 */
        pllmull = 13 / 2; 
      }
            
      if (pllsource == 0x00)
      {/* HSI ��ʱ�ӱ�2��Ƶ��Ϊ PLL ʱ��Դ */
        RCC_Clocks->SYSCLK_Frequency = (HSI_Value >> 1) * pllmull;
      }
      else
      {/* PREDIV1 ��Ϊ PLL ʱ��Դ */
        
        /* ָ�� PREDIV1 ʱ��Դ�ͷ�Ƶϵ�� */
        prediv1source = RCC->CFGR2 & CFGR2_PREDIV1SRC;
        prediv1factor = (RCC->CFGR2 & CFGR2_PREDIV1) + 1;
        
        if (prediv1source == 0)
        { /* HSE ��ʱ�ӱ�ѡ����Ϊ PREDIV1 ʱ��Դ */
          RCC_Clocks->SYSCLK_Frequency = (HSE_Value / prediv1factor) * pllmull;          
        }
        else
        {/* PLL2 ʱ����Ϊ PREDIV1 ʱ��Դ */
          
          /* ָ�� PREDIV2 ��Ƶϵ�� �� PLL2 ��Ƶϵ�� */
          prediv2factor = ((RCC->CFGR2 & CFGR2_PREDIV2) >> 4) + 1;
          pll2mull = ((RCC->CFGR2 & CFGR2_PLL2MUL) >> 8 ) + 2; 
          RCC_Clocks->SYSCLK_Frequency = (((HSE_Value / prediv2factor) * pll2mull) / prediv1factor) * pllmull;                         
        }
      }
#endif /* STM32F10X_CL */ 
      break;

    default:
      RCC_Clocks->SYSCLK_Frequency = HSI_Value;
      break;
  }

  /* ���� HCLK, PCLK1, PCLK2 , ADCCLK ʱ��Ƶ�� ----------------*/
  /* �õ� HCLK Ԥ��Ƶ�� */
  tmp = RCC->CFGR & CFGR_HPRE_Set_Mask;
  tmp = tmp >> 4;
  presc = APBAHBPrescTable[tmp];
  /* HCLK ʱ��Ƶ�� */
  RCC_Clocks->HCLK_Frequency = RCC_Clocks->SYSCLK_Frequency >> presc;
  /* �õ� PCLK1 Ԥ��Ƶ�� */
  tmp = RCC->CFGR & CFGR_PPRE1_Set_Mask;
  tmp = tmp >> 8;
  presc = APBAHBPrescTable[tmp];
  /* PCLK1 ʱ��Ƶ�� */
  RCC_Clocks->PCLK1_Frequency = RCC_Clocks->HCLK_Frequency >> presc;
  /* �õ� PCLK2 Ԥ��Ƶ�� */
  tmp = RCC->CFGR & CFGR_PPRE2_Set_Mask;
  tmp = tmp >> 11;
  presc = APBAHBPrescTable[tmp];
  /* PCLK2 ʱ��Ƶ�� */
  RCC_Clocks->PCLK2_Frequency = RCC_Clocks->HCLK_Frequency >> presc;
  /* �õ� ADCCLK Ԥ��Ƶ�� */
  tmp = RCC->CFGR & CFGR_ADCPRE_Set_Mask;
  tmp = tmp >> 14;
  presc = ADCPrescTable[tmp];
  /* ADCCLK ʱ��Ƶ�� */
  RCC_Clocks->ADCCLK_Frequency = RCC_Clocks->PCLK2_Frequency / presc;
}

/**
  * @����  ʹ�ܻ�ʧ�� AHB ��Χ�豸ʱ��.
  * @����  RCC_AHBPeriph: �����ſ�ʱ�ӵ� AHB ��Χ�豸.
  *   
  *        STM32_����ϵ��΢������, �����������ȡ����ֵ�е�һ��:      
  *                                RCC_AHBPeriph_DMA1
  *                                RCC_AHBPeriph_DMA2
  *                                RCC_AHBPeriph_SRAM
  *                                RCC_AHBPeriph_FLITF
  *                                RCC_AHBPeriph_CRC
  *                                RCC_AHBPeriph_OTG_FS    
  *                                RCC_AHBPeriph_ETH_MAC   
  *                                RCC_AHBPeriph_ETH_MAC_Tx
  *                                RCC_AHBPeriph_ETH_MAC_Rx
  * 
  *           ����_STM32_΢������, �����������ȡ����ֵ�е�һ��:         
  *                                RCC_AHBPeriph_DMA1
  *                                RCC_AHBPeriph_DMA2
  *                                RCC_AHBPeriph_SRAM
  *                                RCC_AHBPeriph_FLITF
  *                                RCC_AHBPeriph_CRC
  *                                RCC_AHBPeriph_FSMC
  *                                RCC_AHBPeriph_SDIO
  *   
  * @ע��  SRAM �� FLITF ʱ��ֻ����˯�ߵ�ģʽ�¿�����Ч.
  * @����  NewState: ָ����Χ�豸ʱ���µ�״̬.
  *                  �����������ȡ: ENABLE �� DISABLE.
  * @����  û��
  */
void RCC_AHBPeriphClockCmd(uint32_t RCC_AHBPeriph, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_RCC_AHB_PERIPH(RCC_AHBPeriph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    RCC->AHBENR |= RCC_AHBPeriph;
  }
  else
  {
    RCC->AHBENR &= ~RCC_AHBPeriph;
  }
}

/**
  * @����  ʹ�ܻ�ʧ�ܸ��� APB (APB2) ��Χ�豸ʱ��.
  * @����  RCC_APB2Periph: �����ſ�ʱ�ӵ� AHB2 ��Χ�豸.
  *                        ���������������������ֵ�����:
  *          RCC_APB2Periph_AFIO, RCC_APB2Periph_GPIOA, RCC_APB2Periph_GPIOB,
  *          RCC_APB2Periph_GPIOC, RCC_APB2Periph_GPIOD, RCC_APB2Periph_GPIOE,
  *          RCC_APB2Periph_GPIOF, RCC_APB2Periph_GPIOG, RCC_APB2Periph_ADC1,
  *          RCC_APB2Periph_ADC2, RCC_APB2Periph_TIM1, RCC_APB2Periph_SPI1,
  *          RCC_APB2Periph_TIM8, RCC_APB2Periph_USART1, RCC_APB2Periph_ADC3
  * @����  NewState: ָ����Χ�豸ʱ�ӵ���״̬.
  *                  �����������ȡ: ENABLE �� DISABLE.
  * @����  û��
  */
void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    RCC->APB2ENR |= RCC_APB2Periph;
  }
  else
  {
    RCC->APB2ENR &= ~RCC_APB2Periph;
  }
}

/**
  * @����  ʹ�ܻ�ʧ�ܵ��� APB (APB1) ��Χ�豸ʱ��.
  * @����  RCC_APB1Periph: �����ſ�ʱ�ӵ� APB1 ��Χ�豸.
  *                        ���������������������ֵ�����:
  *          RCC_APB1Periph_TIM2, RCC_APB1Periph_TIM3, RCC_APB1Periph_TIM4,
  *          RCC_APB1Periph_TIM5, RCC_APB1Periph_TIM6, RCC_APB1Periph_TIM7,
  *          RCC_APB1Periph_WWDG, RCC_APB1Periph_SPI2, RCC_APB1Periph_SPI3,
  *          RCC_APB1Periph_USART2, RCC_APB1Periph_USART3, RCC_APB1Periph_USART4, 
  *          RCC_APB1Periph_USART5, RCC_APB1Periph_I2C1, RCC_APB1Periph_I2C2,
  *          RCC_APB1Periph_USB, RCC_APB1Periph_CAN1, RCC_APB1Periph_BKP,
  *          RCC_APB1Periph_PWR, RCC_APB1Periph_DAC
  * @����  NewState: ָ����Χ�豸ʱ�ӵ���״̬.
  *                  �����������ȡ: ENABLE �� DISABLE.
  * @����  û��
  */
void RCC_APB1PeriphClockCmd(uint32_t RCC_APB1Periph, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_RCC_APB1_PERIPH(RCC_APB1Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    RCC->APB1ENR |= RCC_APB1Periph;
  }
  else
  {
    RCC->APB1ENR &= ~RCC_APB1Periph;
  }
}

#ifdef STM32F10X_CL
/**
  * @����  ���ƻ��ͷ� AHB ��Χ�豸��λ.
  * @ע��  �����������Ӧ���� STM32 ����ϵ��΢������.
  * @����  RCC_AHBPeriph: ָ����λ����Χ�豸.
  *                       ���������������������ֵ�����:
  *                       RCC_AHBPeriph_OTG_FS 
  *                       RCC_AHBPeriph_ETH_MAC
  * @����  NewState: ָ����Χ�豸��λ����״̬.
  *                  �����������ȡ: ENABLE �� DISABLE.
  * @����  û��
  */
void RCC_AHBPeriphResetCmd(uint32_t RCC_AHBPeriph, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_RCC_AHB_PERIPH_RESET(RCC_AHBPeriph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    RCC->AHBRSTR |= RCC_AHBPeriph;
  }
  else
  {
    RCC->AHBRSTR &= ~RCC_AHBPeriph;
  }
}
#endif /* STM32F10X_CL */ 

/**
  * @����  ���ƻ��ͷŸ��� APB(APB2) ��Χ�豸��λ.
  * @����  RCC_APB2Periph: ָ����λ�� APB2 ��Χ�豸.
  *                        ���������������������ֵ�����:
  *          RCC_APB2Periph_AFIO, RCC_APB2Periph_GPIOA, RCC_APB2Periph_GPIOB,
  *          RCC_APB2Periph_GPIOC, RCC_APB2Periph_GPIOD, RCC_APB2Periph_GPIOE,
  *          RCC_APB2Periph_GPIOF, RCC_APB2Periph_GPIOG, RCC_APB2Periph_ADC1,
  *          RCC_APB2Periph_ADC2, RCC_APB2Periph_TIM1, RCC_APB2Periph_SPI1,
  *          RCC_APB2Periph_TIM8, RCC_APB2Periph_USART1, RCC_APB2Periph_ADC3
  * @����  NewState: ָ����Χ�豸��λ����״̬.
  *                  �����������ȡ: ENABLE �� DISABLE.
  * @����  û��
  */
void RCC_APB2PeriphResetCmd(uint32_t RCC_APB2Periph, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_RCC_APB2_PERIPH(RCC_APB2Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    RCC->APB2RSTR |= RCC_APB2Periph;
  }
  else
  {
    RCC->APB2RSTR &= ~RCC_APB2Periph;
  }
}

/**
  * @����  ���ƻ��ͷŵ��� APB(APB1) ��Χ�豸��λ.
  * @����  RCC_APB1Periph: ָ����λ�� APB1 ��Χ�豸.
  *                        ���������������������ֵ�����:
  *          RCC_APB1Periph_TIM2, RCC_APB1Periph_TIM3, RCC_APB1Periph_TIM4,
  *          RCC_APB1Periph_TIM5, RCC_APB1Periph_TIM6, RCC_APB1Periph_TIM7,
  *          RCC_APB1Periph_WWDG, RCC_APB1Periph_SPI2, RCC_APB1Periph_SPI3,
  *          RCC_APB1Periph_USART2, RCC_APB1Periph_USART3, RCC_APB1Periph_USART4, 
  *          RCC_APB1Periph_USART5, RCC_APB1Periph_I2C1, RCC_APB1Periph_I2C2,
  *          RCC_APB1Periph_USB, RCC_APB1Periph_CAN1, RCC_APB1Periph_BKP,
  *          RCC_APB1Periph_PWR, RCC_APB1Periph_DAC
  * @����  NewState: ָ����Χ�豸��λ����״̬.
  *                  �����������ȡ: ENABLE �� DISABLE.
  * @����  û��
  */
void RCC_APB1PeriphResetCmd(uint32_t RCC_APB1Periph, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_RCC_APB1_PERIPH(RCC_APB1Periph));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    RCC->APB1RSTR |= RCC_APB1Periph;
  }
  else
  {
    RCC->APB1RSTR &= ~RCC_APB1Periph;
  }
}

/**
  * @����  ���ƻ��ͷű�����λ.
  * @����  NewState: ������λ����״̬.
  *                  �����������ȡ: ENABLE �� DISABLE.
  * @����  û��
  */
void RCC_BackupResetCmd(FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) BDCR_BDRST_BB = (uint32_t)NewState;
}

/**
  * @����  ʹ�ܻ�ʧ��ʱ�Ӱ�ȫϵͳ.
  * @����  NewState: ʱ�Ӱ�ȫϵͳ����״̬.
  *                  �����������ȡ: ENABLE �� DISABLE.
  * @����  û��
  */
void RCC_ClockSecuritySystemCmd(FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  *(__IO uint32_t *) CR_CSSON_BB = (uint32_t)NewState;
}

/**
  * @����  ѡ�� MCO �����ϵ����ʱ��Դ.
  * @����  RCC_MCO: ָ��Ҫ�����ʱ��Դ.
  *   
  *        STM32_����ϵ��΢������, �����������ȡ����ֵ�е�һ��:      
  *                                RCC_MCO_NoClock:      û��ѡ��ʱ��
  *                                RCC_MCO_SYSCLK:       ѡ��ϵͳʱ��
  *                                RCC_MCO_HSI:          ѡ��HIS��ʱ��
  *                                RCC_MCO_HSE:          ѡ��HSE��ʱ��
  *                                RCC_MCO_PLLCLK_Div2:  ѡ��PLLʱ������Ƶ
  *                                RCC_MCO_PLL2CLK:      ѡ��PLL2��ʱ��                    
  *                                RCC_MCO_PLL3CLK_Div2: ѡ��PLL3ʱ������Ƶ   
  *                                RCC_MCO_XT1:          ѡ���ⲿ 3-25 MHz ��ʱ��  
  *                                RCC_MCO_PLL3CLK:      ѡ��PLL3��ʱ�� 
  * 
  *           ����_STM32_΢������, �����������ȡ����ֵ�е�һ��:        
  *                                RCC_MCO_NoClock:      û��ѡ��ʱ��
  *                                RCC_MCO_SYSCLK:       ѡ��ϵͳʱ��
  *                                RCC_MCO_HSI:          ѡ��HIS��ʱ��
  *                                RCC_MCO_HSE:          ѡ��HSE��ʱ��
  *                                RCC_MCO_PLLCLK_Div2:  ѡ��PLLʱ������Ƶ
  *   
  * @����  û��
  */
void RCC_MCOConfig(uint8_t RCC_MCO)
{
  /* ������ */
  assert_param(IS_RCC_MCO(RCC_MCO));

  /* ִ��MCO[2:0]λ���ֽڴ�ȡ��ѡ��MCOԴ */
  *(__IO uint8_t *) CFGR_BYTE4_ADDRESS = RCC_MCO;
}

/**
  * @����  ���ָ���� RCC ��־�������.
  * @����  RCC_FLAG: ָ��Ҫ���ı�־.
  *   
  *        STM32_����ϵ��΢������, �����������ȡ����ֵ�е�һ��: 
  *                                RCC_FLAG_HSIRDY:  HSI ��ʱ�Ӿ���
  *                                RCC_FLAG_HSERDY:  HSE ��ʱ�Ӿ���
  *                                RCC_FLAG_PLLRDY:  PLL ʱ�Ӿ���
  *                                RCC_FLAG_PLL2RDY: PLL2 ʱ�Ӿ���    
  *                                RCC_FLAG_PLL3RDY: PLL3 ʱ�Ӿ���                          
  *                                RCC_FLAG_LSERDY:  LSE ��ʱ�Ӿ���
  *                                RCC_FLAG_LSIRDY:  LSI ��ʱ�Ӿ���
  *                                RCC_FLAG_PINRST:  ���Ÿ�λ
  *                                RCC_FLAG_PORRST:  POR/PDR��λ
  *                                RCC_FLAG_SFTRST:  �����λ
  *                                RCC_FLAG_IWDGRST: �������Ź���λ
  *                                RCC_FLAG_WWDGRST: ���ڿ��Ź���λ
  *                                RCC_FLAG_LPWRRST: �͵�����λ
  * 
  *           ����_STM32_΢������, �����������ȡ����ֵ�е�һ��:            
  *                                RCC_FLAG_HSIRDY:  HSI ��ʱ�Ӿ���
  *                                RCC_FLAG_HSERDY:  HSE ��ʱ�Ӿ���
  *                                RCC_FLAG_PLLRDY:  PLL ʱ�Ӿ���
  *                                RCC_FLAG_LSERDY:  LSE ��ʱ�Ӿ���
  *                                RCC_FLAG_LSIRDY:  LSI ��ʱ�Ӿ���
  *                                RCC_FLAG_PINRST:  ���Ÿ�λ
  *                                RCC_FLAG_PORRST:  POR/PDR��λ
  *                                RCC_FLAG_SFTRST:  �����λ
  *                                RCC_FLAG_IWDGRST: �������Ź���λ
  *                                RCC_FLAG_WWDGRST: ���ڿ��Ź���λ
  *                                RCC_FLAG_LPWRRST: �͵�����λ
  *   
  * @����  RCC_FLAG ����״̬(SET��RESET).
  */
FlagStatus RCC_GetFlagStatus(uint8_t RCC_FLAG)
{
  uint32_t tmp = 0;
  uint32_t statusreg = 0;
  FlagStatus bitstatus = RESET;
  /* ������ */
  assert_param(IS_RCC_FLAG(RCC_FLAG));

  /* ȡ�� RCC �Ĵ������� */
  tmp = RCC_FLAG >> 5;
  if (tmp == 1)               /* CR �Ĵ����еļ���־ */
  {
    statusreg = RCC->CR;
  }
  else if (tmp == 2)          /* BDCR �Ĵ����еļ���־ */
  {
    statusreg = RCC->BDCR;
  }
  else                       /* CSR �Ĵ����еļ���־ */
  {
    statusreg = RCC->CSR;
  }

  /* ȡ�ñ�־��λ�� */
  tmp = RCC_FLAG & FLAG_Mask;
  if ((statusreg & ((uint32_t)1 << tmp)) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }

  /* ���ر�־��״̬ */
  return bitstatus;
}

/**
  * @����  ��� RCC ��λ��־.
  * @ע��   ��Щ��λ��־��: RCC_FLAG_PINRST, RCC_FLAG_PORRST, RCC_FLAG_SFTRST,
  *                         RCC_FLAG_IWDGRST, RCC_FLAG_WWDGRST, RCC_FLAG_LPWRRST
  * @����  û��
  * @����  û��
  */
void RCC_ClearFlag(void)
{
  /* ͨ����λ RMVF λ�������λ��־ */
  RCC->CSR |= CSR_RMVF_Set;
}

/**
  * @����  ���ָ���� RCC �ж��Ƿ���.
  * @����  RCC_IT: ָ����� RCC �ж�Դ.
  *   
  *        STM32_����ϵ��΢������, �����������ȡ����ֵ�е�һ��: 
  *                                RCC_IT_LSIRDY:  LSI �����ж�
  *                                RCC_IT_LSERDY:  LSE �����ж�
  *                                RCC_IT_HSIRDY:  HSI �����ж�
  *                                RCC_IT_HSERDY:  HSE �����ж�
  *                                RCC_IT_PLLRDY:  PLL �����ж�
  *                                RCC_IT_PLL2RDY: PLL2 �����ж� 
  *                                RCC_IT_PLL3RDY: PLL3 �����ж�                    
  *                                RCC_IT_CSS:     ʱ�Ӱ�ȫϵͳ�ж�
  * 
  *           ����_STM32_΢������, �����������ȡ����ֵ�е�һ��:      
  *                                RCC_IT_LSIRDY: LSI �����ж�
  *                                RCC_IT_LSERDY: LSE �����ж�
  *                                RCC_IT_HSIRDY: HSI �����ж�
  *                                RCC_IT_HSERDY: HSE �����ж�
  *                                RCC_IT_PLLRDY: PLL �����ж�
  *                                RCC_IT_CSS:    ʱ�Ӱ�ȫϵͳ�ж�
  *   
  * @����  RCC_IT ����״̬(SETorRESET).
  */
ITStatus RCC_GetITStatus(uint8_t RCC_IT)
{
  ITStatus bitstatus = RESET;
  /* ������ */
  assert_param(IS_RCC_GET_IT(RCC_IT));

  /* ���ָ���� RCC �ж�״̬ */
  if ((RCC->CIR & RCC_IT) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }

  /* ���� RCC_IT ״̬ */
  return  bitstatus;
}

/**
  * @����  ��� RCC �жϹ���λ.
  * @����  RCC_IT: ָ��Ҫ������жϹ���λ.
  *   
  *        STM32_����ϵ��΢������, �����������ȡ����ֵ�е�һ��: 
  *                                RCC_IT_LSIRDY:  LSI �����ж�
  *                                RCC_IT_LSERDY:  LSE �����ж�
  *                                RCC_IT_HSIRDY:  HSI �����ж�
  *                                RCC_IT_HSERDY:  HSE �����ж�
  *                                RCC_IT_PLLRDY:  PLL �����ж�
  *                                RCC_IT_PLL2RDY: PLL2 �����ж�
  *                                RCC_IT_PLL3RDY: PLL3 �����ж�                      
  *                                RCC_IT_CSS:     ʱ�Ӱ�ȫϵͳ�ж�
  * 
  *           ����_STM32_΢������, �����������ȡ����ֵ�е�һ��:           
  *                                RCC_IT_LSIRDY:  LSI �����ж�
  *                                RCC_IT_LSERDY:  LSE �����ж�
  *                                RCC_IT_HSIRDY:  HSI �����ж�
  *                                RCC_IT_HSERDY:  HSE �����ж�
  *                                RCC_IT_PLLRDY:  PLL �����ж�
  *                                RCC_IT_CSS:     ʱ�Ӱ�ȫϵͳ�ж�
  * @����  û��
  */
void RCC_ClearITPendingBit(uint8_t RCC_IT)
{
  /* ������ */
  assert_param(IS_RCC_CLEAR_IT(RCC_IT));

  /* ִ�� RCC_CIR[23:16] λ���ֽڴ�ȡ�����ѡ����жϹ���λ*/
  *(__IO uint8_t *) CIR_BYTE3_ADDRESS = RCC_IT;
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
