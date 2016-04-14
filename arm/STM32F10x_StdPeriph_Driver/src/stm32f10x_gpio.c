/**
  ******************************************************************************
  * @�ļ�    stm32f10x_gpio.c
  * @����    MCD Ӧ�ó�����
  * @�汾    V3.1.0
  * @����    06/19/2009
  * @����    ����ļ� �ṩ���� GPIO �̼�����.
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
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

/* �������� -----------------------------------------------------------*/
/* ���ö��� ------------------------------------------------------------*/

/* ------------ RCC registers bit address in the alias region ----------------*/
#define AFIO_OFFSET                 (AFIO_BASE - PERIPH_BASE)

/* --- EVENTCR Register -----*/

/* Alias word address of EVOE bit */
#define EVCR_OFFSET                 (AFIO_OFFSET + 0x00)
#define EVOE_BitNumber              ((uint8_t)0x07)
#define EVCR_EVOE_BB                (PERIPH_BB_BASE + (EVCR_OFFSET * 32) + (EVOE_BitNumber * 4))


/* ---  MAPR Register ---*/ 
/* Alias word address of MII_RMII_SEL bit */ 
#define MAPR_OFFSET                 (AFIO_OFFSET + 0x04) 
#define MII_RMII_SEL_BitNumber      ((u8)0x17) 
#define MAPR_MII_RMII_SEL_BB        (PERIPH_BB_BASE + (MAPR_OFFSET * 32) + (MII_RMII_SEL_BitNumber * 4))


#define EVCR_PORTPINCONFIG_MASK     ((uint16_t)0xFF80)
#define LSB_MASK                    ((uint16_t)0xFFFF)
#define DBGAFR_POSITION_MASK        ((uint32_t)0x000F0000)
#define DBGAFR_SWJCFG_MASK          ((uint32_t)0xF0FFFFFF)
#define DBGAFR_LOCATION_MASK        ((uint32_t)0x00200000)
#define DBGAFR_NUMBITS_MASK         ((uint32_t)0x00100000)

/* ���ú� -------------------------------------------------------------*/
/* ���ñ��� ---------------------------------------------------------*/
/* ���ú���ԭ�� -----------------------------------------------*/
/* ���ú��� ---------------------------------------------------------*/

/**
  * @����   ��GPIO�����ļĴ�����λΪĬ��ֵ
  * @����   GPIOx��x��ΪA��G��ѡ���ض���GPIO�豸
  * @����   û��
  */
void GPIO_DeInit(GPIO_TypeDef* GPIOx)
{
  /* ���˶Բ���*/
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  
  if (GPIOx == GPIOA)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOA, DISABLE);
  }
  else if (GPIOx == GPIOB)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOB, DISABLE);
  }
  else if (GPIOx == GPIOC)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOC, DISABLE);
  }
  else if (GPIOx == GPIOD)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOD, DISABLE);
  }    
  else if (GPIOx == GPIOE)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOE, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOE, DISABLE);
  } 
  else if (GPIOx == GPIOF)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOF, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOF, DISABLE);
  }
  else
  {
    if (GPIOx == GPIOG)
    {
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOG, ENABLE);
      RCC_APB2PeriphResetCmd(RCC_APB2Periph_GPIOG, DISABLE);
    }
  }
}

/**
  * @����  �����ù��ܣ���ӳ�䣬�¼����Ƽ�EXTI���ã��Ĵ�����λΪĬ��ֵ
  * @����  û��
  * @����  û��
  */
void GPIO_AFIODeInit(void)
{
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO, ENABLE);
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_AFIO, DISABLE);
}

/**
  * @����  ����GPIO_InitStruct(�ṹ��)���ض�������ʼ��GPIO����
  * @����  GPIOx��x��ΪA��G��ѡ���ض���GPIO�豸��
  *        GPIO_InitStruct: ָ��GPIO_InitTypeDef�ṹ��ָ�룬�������ض�GPIO������������Ϣ.
  * @����  û��
  */
void GPIO_Init(GPIO_TypeDef* GPIOx, GPIO_InitTypeDef* GPIO_InitStruct)
{
  uint32_t currentmode = 0x00, currentpin = 0x00, pinpos = 0x00, pos = 0x00;
  uint32_t tmpreg = 0x00, pinmask = 0x00;
  /* ������ */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_MODE(GPIO_InitStruct->GPIO_Mode));
  assert_param(IS_GPIO_PIN(GPIO_InitStruct->GPIO_Pin));  
  
/*---------------------------- GPIO ģʽ���� --------------------------------*/
  currentmode = ((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x0F);
  if ((((uint32_t)GPIO_InitStruct->GPIO_Mode) & ((uint32_t)0x10)) != 0x00)
  { 
    /* Check the parameters */
    assert_param(IS_GPIO_SPEED(GPIO_InitStruct->GPIO_Speed));
    /* Output mode */
    currentmode |= (uint32_t)GPIO_InitStruct->GPIO_Speed;
  }
/*-------------------- GPIO CRL(�˿ڿ��ƼĴ������ֽ�) ���� -------------------*/
  /* ���õ�8λ�˿ڹܽ� */
  if (((uint32_t)GPIO_InitStruct->GPIO_Pin & ((uint32_t)0x00FF)) != 0x00)
  {
    tmpreg = GPIOx->CRL;
    for (pinpos = 0x00; pinpos < 0x08; pinpos++)
    {
      pos = ((uint32_t)0x01) << pinpos;
      /* ȡ�ö˿����ŵ�λ�� */
      currentpin = (GPIO_InitStruct->GPIO_Pin) & pos;
      if (currentpin == pos)
      {
        pos = pinpos << 2;
        /* �����Ӧ�ĵ�λ���ƼĴ��� */
        pinmask = ((uint32_t)0x0F) << pos;
        tmpreg &= ~pinmask;
        /* д��Ӧ��ģʽ����λ */
        tmpreg |= (currentmode << pos);
        /* ��λ��Ӧ��������ݼĴ��� (ODR) λ */
        if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPD)
        {
          GPIOx->BRR = (((uint32_t)0x01) << pinpos);
        }
        else
        {
          /* ��λ��Ӧ��������ݼĴ��� (ODR) λ */
          if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPU)
          {
            GPIOx->BSRR = (((uint32_t)0x01) << pinpos);
          }
        }
      }
    }
    GPIOx->CRL = tmpreg;
  }
/*-------------------- GPIO CRH(�˿ڿ��ƼĴ������ֽ�) ���� -------------------*/
  /* ���ø�8λ�˿����� */
  if (GPIO_InitStruct->GPIO_Pin > 0x00FF)
  {
    tmpreg = GPIOx->CRH;
    for (pinpos = 0x00; pinpos < 0x08; pinpos++)
    {
      pos = (((uint32_t)0x01) << (pinpos + 0x08));
      /* ȡ�ö˿����ŵ�λ�� */
      currentpin = ((GPIO_InitStruct->GPIO_Pin) & pos);
      if (currentpin == pos)
      {
        pos = pinpos << 2;
        /* �����Ӧ�ĵ�λ���ƼĴ��� */
        pinmask = ((uint32_t)0x0F) << pos;
        tmpreg &= ~pinmask;
        /* д��Ӧ��ģʽ����λ */
        tmpreg |= (currentmode << pos);
        /* ��λ��Ӧ��������ݼĴ��� (ODR) λ */
        if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPD)
        {
          GPIOx->BRR = (((uint32_t)0x01) << (pinpos + 0x08));
        }
        /* ��λ��Ӧ��������ݼĴ��� (ODR) λ */
        if (GPIO_InitStruct->GPIO_Mode == GPIO_Mode_IPU)
        {
          GPIOx->BSRR = (((uint32_t)0x01) << (pinpos + 0x08));
        }
      }
    }
    GPIOx->CRH = tmpreg;
  }
}

/**
  * @����  ��GPIO_InitStruct�е�ÿһ��������ȱʡֵ����.
  * @����  GPIO_InitStruct : ָ��GPIO_InitTypeDef�ṹ���ýṹ������ʼ��
  * @����  û��
  */
void GPIO_StructInit(GPIO_InitTypeDef* GPIO_InitStruct)
{
  /* ��λGPIO��ʼ���ṹ����ֵ */
  GPIO_InitStruct->GPIO_Pin  = GPIO_Pin_All;
  GPIO_InitStruct->GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStruct->GPIO_Mode = GPIO_Mode_IN_FLOATING;
}

/**
  * @����  ��ȡָ���˿ڹܽŵ�����.
  * @����  GPIOx: x��ΪA��G��ѡ���ض���GPIO�豸.
  *        GPIO_Pin:  ����ȡ������λ.
  *                   �������GPIO_Pin_x����ѡ��0��15.
  * @����  ָ���˿ڹܽŵ�����ֵ.
  */
uint8_t GPIO_ReadInputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  uint8_t bitstatus = 0x00;
  
  /* ������ */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GET_GPIO_PIN(GPIO_Pin)); 
  
  if ((GPIOx->IDR & GPIO_Pin) != (uint32_t)Bit_RESET)
  {
    bitstatus = (uint8_t)Bit_SET;
  }
  else
  {
    bitstatus = (uint8_t)Bit_RESET;
  }
  return bitstatus;
}

/**
  * @����  ���ض���GPIO�������ݶ˿ڶ�ȡ.
  * @����  GPIOx: x��ΪA��G��ѡ���ض���GPIO�豸.
  * @����  ָ���� GPIO �˿ڹܽŵ�����ֵ.
  */
uint16_t GPIO_ReadInputData(GPIO_TypeDef* GPIOx)
{
  /* ������ */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  
  return ((uint16_t)GPIOx->IDR);
}

/**
  * @����  ��ȡָ���˿ڹܽŵ����(λ).
  * @����  GPIOx: x��ΪA��G��ѡ���ض���GPIO�豸.
  *        GPIO_Pin: ��Ҫ��ȡ�Ķ˿�λ.
  *                  �������GPIO_Pin_x����ѡ��0��15.
  * @����  ����˿ڵ����ֵ.
  */
uint8_t GPIO_ReadOutputDataBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  uint8_t bitstatus = 0x00;
  /* ������ */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GET_GPIO_PIN(GPIO_Pin)); 
  
  if ((GPIOx->ODR & GPIO_Pin) != (uint32_t)Bit_RESET)
  {
    bitstatus = (uint8_t)Bit_SET;
  }
  else
  {
    bitstatus = (uint8_t)Bit_RESET;
  }
  return bitstatus;
}

/**
  * @����  ��ȡָ����GPIO�˿����.
  * @����  GPIOx: x��ΪA��G��ѡ���ض���GPIO�豸.
  * @����  ָ�� GPIO ������ݶ˿�ֵ.
  */
uint16_t GPIO_ReadOutputData(GPIO_TypeDef* GPIOx)
{
  /* Check the parameters */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
    
  return ((uint16_t)GPIOx->ODR);
}

/**
  * @����  ����ָ�������ݶ˿�λ.
  * @����  x��ΪA��G��ѡ���ض���GPIO�豸.
  *        GPIO_Pin: ȷ���˿�λ.
  *                  �ò�����ΪGPIO_Pin_x���������,����xΪ0��15.
  * @����  û��
  */
void GPIO_SetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  /* ������ */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  
  GPIOx->BSRR = GPIO_Pin;
}

/**
  * @����  ���ָ�������ݶ˿�λ.
  * @����  x��ΪA��G��ѡ���ض���GPIO�豸.
  *        GPIO_Pin: ȷ���˿�λ.
  *                  �ò�����ΪGPIO_Pin_x���������,����xΪ0��15.
  * @����  û��
  */
void GPIO_ResetBits(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  /* ������ */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  
  GPIOx->BRR = GPIO_Pin;
}

/**
  * @����  ���û������ָ�������ݶ˿�λ.
  * @����  GPIOx: x��ΪA��G��ѡ���ض���GPIO�豸.
  *        GPIO_Pin: ָ����д��Ķ˿�λ.
  *                  �ò�����ΪGPIO_Pin_x���������,����xΪ0��15.
  *        BitVal:   �ò���ָ����д�뵽ѡ��λ.
  *                  BitVal����ΪBitActionö������ֵ: 
  *                  @arg Bit_RESET:����˿����ţ�
  *                  @arg Bit_SET:��λ�˿�����
  * @����  û��
  */
void GPIO_WriteBit(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, BitAction BitVal)
{
  /* ������ */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GET_GPIO_PIN(GPIO_Pin));
  assert_param(IS_GPIO_BIT_ACTION(BitVal)); 
  
  if (BitVal != Bit_RESET)
  {
    GPIOx->BSRR = GPIO_Pin;
  }
  else
  {
    GPIOx->BRR = GPIO_Pin;
  }
}

/**
  * @����  ��ָ��GPIO���ݶ˿�д������.
  * @����  GPIOx: x��ΪA��G��ѡ���ض���GPIO�豸.
  *        PortVal: д�뵽���ݶ˿ڼĴ�����ֵ.
  * @����  û��
  */
void GPIO_Write(GPIO_TypeDef* GPIOx, uint16_t PortVal)
{
  /* ������ */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  
  GPIOx->ODR = PortVal;
}

/**
  * @����  ����GPIO�������üĴ���.
  * @����  GPIOx: x��ΪA��G��ѡ���ض���GPIO�豸.
  *        GPIO_Pin:ָ����д��Ķ˿�λ.
  *                 �ò�����ΪGPIO_Pin_x���������,����xΪ0��15.
  * @����  û��
  */
void GPIO_PinLockConfig(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
  uint32_t tmp = 0x00010000;
  
  /* ������ */
  assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
  assert_param(IS_GPIO_PIN(GPIO_Pin));
  
  tmp |= GPIO_Pin;
  /* Set LCKK bit */
  GPIOx->LCKR = tmp;
  /* Reset LCKK bit */
  GPIOx->LCKR =  GPIO_Pin;
  /* Set LCKK bit */
  GPIOx->LCKR = tmp;
  /* Read LCKK bit*/
  tmp = GPIOx->LCKR;
  /* Read LCKK bit*/
  tmp = GPIOx->LCKR;
}

/**
  * @����  ѡ�� GPIO �ܽ������¼����.
  * @����  GPIO_PortSource: ѡ�������¼������GPIO�˿�
  *                         GPIOx:x��ΪA��E��ѡ���ض���GPIO�豸.
  *        GPIO_PinSource: �¼�����Ĺܽ�.
  *                        �ò�����ΪGPIO_PinSourcex,����x��Ϊ0��15.
  * @����  û��
  */
void GPIO_EventOutputConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource)
{
  uint32_t tmpreg = 0x00;
  /* ������ */
  assert_param(IS_GPIO_EVENTOUT_PORT_SOURCE(GPIO_PortSource));
  assert_param(IS_GPIO_PIN_SOURCE(GPIO_PinSource));
    
  tmpreg = AFIO->EVCR;
  /* Clear the PORT[6:4] and PIN[3:0] bits */
  tmpreg &= EVCR_PORTPINCONFIG_MASK;
  tmpreg |= (uint32_t)GPIO_PortSource << 0x04;
  tmpreg |= GPIO_PinSource;
  AFIO->EVCR = tmpreg;
}

/**
  * @����  ʹ�ܻ���ʧ���¼����.
  * @����  NewState: �¼��������״̬.
  *                  �����������ȡ��ENABLE����DISABLE.
  * @����  û��
  */
void GPIO_EventOutputCmd(FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  *(__IO uint32_t *) EVCR_EVOE_BB = (uint32_t)NewState;
}

/**
  * @����  �ı�ָ���ܽŵ�ӳ��.
  * @����  GPIO_Remap: ѡ����ӳ��Ĺܽ�.
  *                    GPIO_Remap �����ı䱸�ù���ӳ��,���Ŀ���ֵ:
  *     @arg GPIO_Remap_SPI1
  *     @arg GPIO_Remap_I2C1
  *     @arg GPIO_Remap_USART1
  *     @arg GPIO_Remap_USART2
  *     @arg GPIO_PartialRemap_USART3
  *     @arg GPIO_FullRemap_USART3
  *     @arg GPIO_PartialRemap_TIM1
  *     @arg GPIO_FullRemap_TIM1
  *     @arg GPIO_PartialRemap1_TIM2
  *     @arg GPIO_PartialRemap2_TIM2
  *     @arg GPIO_FullRemap_TIM2
  *     @arg GPIO_PartialRemap_TIM3
  *     @arg GPIO_FullRemap_TIM3
  *     @arg GPIO_Remap_TIM4
  *     @arg GPIO_Remap1_CAN1
  *     @arg GPIO_Remap2_CAN1
  *     @arg GPIO_Remap_PD01
  *     @arg GPIO_Remap_TIM5CH4_LSI
  *     @arg GPIO_Remap_ADC1_ETRGINJ
  *     @arg GPIO_Remap_ADC1_ETRGREG
  *     @arg GPIO_Remap_ADC2_ETRGINJ
  *     @arg GPIO_Remap_ADC2_ETRGREG
  *     @arg GPIO_Remap_ETH
  *     @arg GPIO_Remap_CAN2  
  *     @arg GPIO_Remap_SWJ_NoJTRST
  *     @arg GPIO_Remap_SWJ_JTAGDisable
  *     @arg GPIO_Remap_SWJ_Disable
  *     @arg GPIO_Remap_SPI3
  *     @arg GPIO_Remap_TIM2ITR1_PTP_SOF
  *     @arg GPIO_Remap_PTP_PPS  
  * @ע��  ��� GPIO_Remap_TIM2ITR1_PTP_SOF ������  TIM2 ITR1 ���ӵ���̫�� PTP ���
  *        ����λ��ʱ�� TIM2 ITR1 ���ӵ� USB OTG SOF ���.       
  * @����  NewState: �˿�������ӳ�����״̬.
  *                  �ò�����Ϊ��ENABLE��DISABLE.
  * @����  û��
  */
void GPIO_PinRemapConfig(uint32_t GPIO_Remap, FunctionalState NewState)
{
  uint32_t tmp = 0x00, tmp1 = 0x00, tmpreg = 0x00, tmpmask = 0x00;

  /* ������ */
  assert_param(IS_GPIO_REMAP(GPIO_Remap));
  assert_param(IS_FUNCTIONAL_STATE(NewState));  
  
  tmpreg = AFIO->MAPR;

  tmpmask = (GPIO_Remap & DBGAFR_POSITION_MASK) >> 0x10;
  tmp = GPIO_Remap & LSB_MASK;

  if ((GPIO_Remap & (DBGAFR_LOCATION_MASK | DBGAFR_NUMBITS_MASK)) == (DBGAFR_LOCATION_MASK | DBGAFR_NUMBITS_MASK))
  {
    tmpreg &= DBGAFR_SWJCFG_MASK;
    AFIO->MAPR &= DBGAFR_SWJCFG_MASK;
  }
  else if ((GPIO_Remap & DBGAFR_NUMBITS_MASK) == DBGAFR_NUMBITS_MASK)
  {
    tmp1 = ((uint32_t)0x03) << tmpmask;
    tmpreg &= ~tmp1;
    tmpreg |= ~DBGAFR_SWJCFG_MASK;
  }
  else
  {
    tmpreg &= ~(tmp << ((GPIO_Remap >> 0x15)*0x10));
    tmpreg |= ~DBGAFR_SWJCFG_MASK;
  }

  if (NewState != DISABLE)
  {
    tmpreg |= (tmp << ((GPIO_Remap >> 0x15)*0x10));
  }

  AFIO->MAPR = tmpreg;
}

/**
  * @����  ѡ��GPIO������Ϊ EXTI ��.
  * @����  GPIO_PortSource: ѡ��Ҫ���� EXTI �ߵ�Դ�� GPIO ��.
  *                         GPIOx:x��ΪA��G��ѡ���ض���GPIO�豸.
  *        GPIO_PinSource:  ָ����Ҫ���õ�EXTI��.
  *                         �������������GPIO_PinSourcex,����xΪ0��15.
  * @����  û��
  */
void GPIO_EXTILineConfig(uint8_t GPIO_PortSource, uint8_t GPIO_PinSource)
{
  uint32_t tmp = 0x00;
  /* ������ */
  assert_param(IS_GPIO_EXTI_PORT_SOURCE(GPIO_PortSource));
  assert_param(IS_GPIO_PIN_SOURCE(GPIO_PinSource));
  
  tmp = ((uint32_t)0x0F) << (0x04 * (GPIO_PinSource & (uint8_t)0x03));
  AFIO->EXTICR[GPIO_PinSource >> 0x02] &= ~tmp;
  AFIO->EXTICR[GPIO_PinSource >> 0x02] |= (((uint32_t)GPIO_PortSource) << (0x04 * (GPIO_PinSource & (uint8_t)0x03)));
}

/**
  * @����  ָ����̫���ӿ�.
  * @ע��  �˹��ܽ�������STM32����������.  
  * @����  GPIO_ETH_MediaInterface: ָ���ӿ�ģʽ.
  *                                 �����������ѡ��:
  *                                 GPIO_ETH_MediaInterface_MII: MII mode    ->�����ӿ�
  *                                 GPIO_ETH_MediaInterface_RMII: RMII mode  ->��������ӿ�  
  * @����  û��
  */
void GPIO_ETH_MediaInterfaceConfig(uint32_t GPIO_ETH_MediaInterface) 
{ 
  assert_param(IS_GPIO_ETH_MEDIA_INTERFACE(GPIO_ETH_MediaInterface)); 

  /* ���� MII_RMII ѡ��λ */ 
  *(__IO uint32_t *) MAPR_MII_RMII_SEL_BB = GPIO_ETH_MediaInterface; 
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
