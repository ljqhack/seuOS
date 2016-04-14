/**
  ******************************************************************************
  * @�ļ�    stm32f10x_usart.c
  * @����    MCD Ӧ�ó�����
  * @�汾    V3.1.0
  * @����    06/19/2009
  * @����    ����ļ� �ṩ���� USART �̼�����.
  * @����    ANSON/sweet1985 Email:airanson110@126.com
  * @����    14/07/2009
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
#include "stm32f10x_usart.h"
#include "stm32f10x_rcc.h"

/* �������� --------------------------------------------------------------*/
/* ���ö��� --------------------------------------------------------------*/

#define CR1_UE_Set                ((uint16_t)0x2000)  /*!< USART Enable Mask */
#define CR1_UE_Reset              ((uint16_t)0xDFFF)  /*!< USART Disable Mask */

#define CR1_WAKE_Mask             ((uint16_t)0xF7FF)  /*!< USART WakeUp Method Mask */

#define CR1_RWU_Set               ((uint16_t)0x0002)  /*!< USART mute mode Enable Mask */
#define CR1_RWU_Reset             ((uint16_t)0xFFFD)  /*!< USART mute mode Enable Mask */
#define CR1_SBK_Set               ((uint16_t)0x0001)  /*!< USART Break Character send Mask */
#define CR1_CLEAR_Mask            ((uint16_t)0xE9F3)  /*!< USART CR1 Mask */
#define CR2_Address_Mask          ((uint16_t)0xFFF0)  /*!< USART address Mask */

#define CR2_LINEN_Set              ((uint16_t)0x4000)  /*!< USART LIN Enable Mask */
#define CR2_LINEN_Reset            ((uint16_t)0xBFFF)  /*!< USART LIN Disable Mask */

#define CR2_LBDL_Mask             ((uint16_t)0xFFDF)  /*!< USART LIN Break detection Mask */
#define CR2_STOP_CLEAR_Mask       ((uint16_t)0xCFFF)  /*!< USART CR2 STOP Bits Mask */
#define CR2_CLOCK_CLEAR_Mask      ((uint16_t)0xF0FF)  /*!< USART CR2 Clock Mask */

#define CR3_SCEN_Set              ((uint16_t)0x0020)  /*!< USART SC Enable Mask */
#define CR3_SCEN_Reset            ((uint16_t)0xFFDF)  /*!< USART SC Disable Mask */

#define CR3_NACK_Set              ((uint16_t)0x0010)  /*!< USART SC NACK Enable Mask */
#define CR3_NACK_Reset            ((uint16_t)0xFFEF)  /*!< USART SC NACK Disable Mask */

#define CR3_HDSEL_Set             ((uint16_t)0x0008)  /*!< USART Half-Duplex Enable Mask */
#define CR3_HDSEL_Reset           ((uint16_t)0xFFF7)  /*!< USART Half-Duplex Disable Mask */

#define CR3_IRLP_Mask             ((uint16_t)0xFFFB)  /*!< USART IrDA LowPower mode Mask */
#define CR3_CLEAR_Mask            ((uint16_t)0xFCFF)  /*!< USART CR3 Mask */

#define CR3_IREN_Set              ((uint16_t)0x0002)  /*!< USART IrDA Enable Mask */
#define CR3_IREN_Reset            ((uint16_t)0xFFFD)  /*!< USART IrDA Disable Mask */
#define GTPR_LSB_Mask             ((uint16_t)0x00FF)  /*!< Guard Time Register LSB Mask */
#define GTPR_MSB_Mask             ((uint16_t)0xFF00)  /*!< Guard Time Register MSB Mask */
#define IT_Mask                   ((uint16_t)0x001F)  /*!< USART Interrupt Mask */

/* ���ú� -------------------------------------------------------------*/
/* ���ñ��� -----------------------------------------------------------*/
/* ���ú���ԭ�� -------------------------------------------------------*/
/* ���ú��� -----------------------------------------------------------*/

/**
  * @����  ������ USARTx �Ĵ�������Ϊȱʡֵ.
  * @����  USARTx: ѡ�� USART ��Χ�豸. 
  *                �����������ȡ����ֵ֮һ: USART1, USART2, USART3, UART4 �� UART5.
  * @����  û��
  */
void USART_DeInit(USART_TypeDef* USARTx)
{
  /* ������ */
  assert_param(IS_USART_ALL_PERIPH(USARTx));

  if (USARTx == USART1)
  {
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_USART1, DISABLE);
  }
  else if (USARTx == USART2)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2, DISABLE);
  }
  else if (USARTx == USART3)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART3, DISABLE);
  }    
  else if (USARTx == UART4)
  {
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART4, ENABLE);
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART4, DISABLE);
  }    
  else
  {
    if (USARTx == UART5)
    { 
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART5, ENABLE);
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_UART5, DISABLE);
    }
  }
}

/**
  * @����  ���� USART_InitStruct ��ָ���Ĳ�����ʼ������ USARTx �Ĵ���.
  * @����  USARTx: ѡ�� USART ��Χ�豸. 
  *                �����������ȡ����ֵ֮һ: USART1, USART2, USART3, UART4 �� UART5.
  * @����  USART_InitStruct: ָ��ṹ USART_InitTypeDef ��ָ�룬���������� USART ��������Ϣ.
  * @����  û��
  */
void USART_Init(USART_TypeDef* USARTx, USART_InitTypeDef* USART_InitStruct)
{
  uint32_t tmpreg = 0x00, apbclock = 0x00;
  uint32_t integerdivider = 0x00;
  uint32_t fractionaldivider = 0x00;
  uint32_t usartxbase = 0;
  RCC_ClocksTypeDef RCC_ClocksStatus;
  /* ������ */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_BAUDRATE(USART_InitStruct->USART_BaudRate));  
  assert_param(IS_USART_WORD_LENGTH(USART_InitStruct->USART_WordLength));
  assert_param(IS_USART_STOPBITS(USART_InitStruct->USART_StopBits));
  assert_param(IS_USART_PARITY(USART_InitStruct->USART_Parity));
  assert_param(IS_USART_MODE(USART_InitStruct->USART_Mode));
  assert_param(IS_USART_HARDWARE_FLOW_CONTROL(USART_InitStruct->USART_HardwareFlowControl));
  /* The hardware flow control is available only for USART1, USART2 and USART3 */
  if (USART_InitStruct->USART_HardwareFlowControl != USART_HardwareFlowControl_None)
  {
    assert_param(IS_USART_123_PERIPH(USARTx));
  }

  usartxbase = (uint32_t)USARTx;

/*---------------------------- USART CR2 Configuration -----------------------*/
  tmpreg = USARTx->CR2;
  /* ��� STOP[13:12] λ */
  tmpreg &= CR2_STOP_CLEAR_Mask;
  /* ���� USART ����λ, ʱ��, ʱ�Ӽ���, ʱ����λ �� ĩλ ------------*/
  /* ���� STOP[13:12] λ������ USART_StopBits ��ֵ */
  tmpreg |= (uint32_t)USART_InitStruct->USART_StopBits;
  
  /* д USART CR2 */
  USARTx->CR2 = (uint16_t)tmpreg;

/*---------------------------- USART CR1 Configuration -----------------------*/
  tmpreg = USARTx->CR1;
  /* ��� M, PCE, PS, TE �� RE λ */
  tmpreg &= CR1_CLEAR_Mask;
  /* ���� USART �ֳ�, ��ż �� ģʽ ----------------------- */
  /* ���� M   λ������ USART_WordLength ��ֵ */
  /* ���� PCE �� PS λ������ USART_Parity ��ֵ */
  /* ���� TE  �� RE λ������ USART_Mode ��ֵ */
  tmpreg |= (uint32_t)USART_InitStruct->USART_WordLength | USART_InitStruct->USART_Parity |
            USART_InitStruct->USART_Mode;
  /* д USART CR1 */
  USARTx->CR1 = (uint16_t)tmpreg;

/*---------------------------- USART CR3 Configuration -----------------------*/  
  tmpreg = USARTx->CR3;
  /* ��� CTSE �� RTSE λ */
  tmpreg &= CR3_CLEAR_Mask;
  /* ���� USART HFC -------------------------------------------------*/
  /* ���� CTSE �� RTSE λ������ USART_HardwareFlowControl ��ֵ */
  tmpreg |= USART_InitStruct->USART_HardwareFlowControl;
  /* д USART CR3 */
  USARTx->CR3 = (uint16_t)tmpreg;

/*---------------------------- USART BRR Configuration -----------------------*/
  /* ���� USART ������ -------------------------------------------*/
  RCC_GetClocksFreq(&RCC_ClocksStatus);
  if (usartxbase == USART1_BASE)
  {
    apbclock = RCC_ClocksStatus.PCLK2_Frequency;
  }
  else
  {
    apbclock = RCC_ClocksStatus.PCLK1_Frequency;
  }
  /* ȷ���������� */
  integerdivider = ((0x19 * apbclock) / (0x04 * (USART_InitStruct->USART_BaudRate)));
  tmpreg = (integerdivider / 0x64) << 0x04;
  /* ȷ���������� */
  fractionaldivider = integerdivider - (0x64 * (tmpreg >> 0x04));
  tmpreg |= ((((fractionaldivider * 0x10) + 0x32) / 0x64)) & ((uint8_t)0x0F);
  /* д USART BRR */
  USARTx->BRR = (uint16_t)tmpreg;
}

/**
  * @����  �� USART_InitStruct �е�ÿһ��������ȱʡֵ����.
  * @����  USART_InitStruct: ָ��ṹ USART_InitTypeDef ��ָ�룬����ʼ��.
  * @����  û��
  */
void USART_StructInit(USART_InitTypeDef* USART_InitStruct)
{
  /* USART_InitStruct �ṹ���ȱʡֵ */
  USART_InitStruct->USART_BaudRate = 9600;
  USART_InitStruct->USART_WordLength = USART_WordLength_8b;
  USART_InitStruct->USART_StopBits = USART_StopBits_1;
  USART_InitStruct->USART_Parity = USART_Parity_No ;
  USART_InitStruct->USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
  USART_InitStruct->USART_HardwareFlowControl = USART_HardwareFlowControl_None;  
}

/**
  * @����  ��ʼ�� USARTx ��Χʱ�ӣ����� USART_ClockInitStruct �ڵĲ���.
  * @����  USARTx: ����x����ȡ 1, 2, 3 ��ѡ�� USART ��Χ�豸.
  * @����  USART_ClockInitStruct: ָ�� USART_ClockInitTypeDef ��ָ�룬���������� USART ��������Ϣ.  
  * @ע��  ������ܿ�ģʽ����Ӧ���� UART4 �� UART5.
  * @����  û��
  */
void USART_ClockInit(USART_TypeDef* USARTx, USART_ClockInitTypeDef* USART_ClockInitStruct)
{
  uint32_t tmpreg = 0x00;
  /* ������ */
  assert_param(IS_USART_123_PERIPH(USARTx));
  assert_param(IS_USART_CLOCK(USART_ClockInitStruct->USART_Clock));
  assert_param(IS_USART_CPOL(USART_ClockInitStruct->USART_CPOL));
  assert_param(IS_USART_CPHA(USART_ClockInitStruct->USART_CPHA));
  assert_param(IS_USART_LASTBIT(USART_ClockInitStruct->USART_LastBit));
  
/*---------------------------- USART CR2 Configuration -----------------------*/
  tmpreg = USARTx->CR2;
  /* ��� CLKEN, CPOL, CPHA �� LBCL λ */
  tmpreg &= CR2_CLOCK_CLEAR_Mask;
  /* ���� USART Clock, CPOL, CPHA and LastBit ------------*/
  /* ���� CLKEN λ������ USART_Clock ��ֵ */
  /* ���� CPOL  λ������ USART_CPOL ��ֵ */
  /* ���� CPHA  λ������ USART_CPHA ��ֵ */
  /* ���� LBCL  λ������ USART_LastBit ��ֵ */
  tmpreg |= (uint32_t)USART_ClockInitStruct->USART_Clock | USART_ClockInitStruct->USART_CPOL | 
                 USART_ClockInitStruct->USART_CPHA | USART_ClockInitStruct->USART_LastBit;
  /* д USART CR2 */
  USARTx->CR2 = (uint16_t)tmpreg;
}

/**
  * @����  ��Ĭ��ֵ���ÿһ�� USART_ClockInitStruct �ĳ�Ա.
  * @����  USART_ClockInitStruct: ָ�� USART_ClockInitTypeDef �ṹ��ָ�룬���ڳ�ʼ��.
  * @����  û��
  */
void USART_ClockStructInit(USART_ClockInitTypeDef* USART_ClockInitStruct)
{
  /* USART_ClockInitStruct �ṹ���ȱʡֵ */
  USART_ClockInitStruct->USART_Clock = USART_Clock_Disable;
  USART_ClockInitStruct->USART_CPOL = USART_CPOL_Low;
  USART_ClockInitStruct->USART_CPHA = USART_CPHA_1Edge;
  USART_ClockInitStruct->USART_LastBit = USART_LastBit_Disable;
}

/**
  * @����  ʹ�ܻ���ʧ�� USART ����.
  * @����  USARTx: ѡ�� USART ���� UART ����Χ�豸. 
  *                ������������������ֵ֮һ : USART1, USART2, USART3, UART4 �� UART5.
  * @����  NewState: USARTx ��Χ�豸����״̬.
  *                  �������������: ENABLE or DISABLE.
  * @����  û��
  */
void USART_Cmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* Enable the selected USART by setting the UE bit in the CR1 register */
    USARTx->CR1 |= CR1_UE_Set;
  }
  else
  {
    /* Disable the selected USART by clearing the UE bit in the CR1 register */
    USARTx->CR1 &= CR1_UE_Reset;
  }
}

/**
  * @����  ʹ�ܻ���ʧ��ָ���� USART �ж�.
  * @����  USARTx: ѡ�� USART ���� UART ����Χ�豸. 
  *                ������������������ֵ֮һ : USART1, USART2, USART3, UART4 or UART5.
  * @����  USART_IT: ָ��Ҫʹ�ܻ�ʧ�ܵ� USART �ж�Դ.
  *          ������������������ֵ֮һ :
  *          USART_IT_CTS:  CTS �ı��ж� (����Ӧ���� UART4 �� UART5)
  *          USART_IT_LBD:  LIN �������ж�
  *          USART_IT_TXE:  �������ݼĴ������ж�
  *          USART_IT_TC:   ��������ж�
  *          USART_IT_RXNE: �������ݼĴ�����Ϊ���ж�
  *          USART_IT_IDLE: ���������ж�
  *          USART_IT_PE:   ��ż�����ж�
  *          USART_IT_ERR:  �����ж�(�����, ��������, �������)
  * @����  NewState: ָ���� USARTx �жϵ���״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void USART_ITConfig(USART_TypeDef* USARTx, uint16_t USART_IT, FunctionalState NewState)
{
  uint32_t usartreg = 0x00, itpos = 0x00, itmask = 0x00;
  uint32_t usartxbase = 0x00;
  /* ������ */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_CONFIG_IT(USART_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  /* CTS �жϲ���Ӧ���� UART4 �� UART5 */
  if (USART_IT == USART_IT_CTS)
  {
    assert_param(IS_USART_123_PERIPH(USARTx));
  }   
  
  usartxbase = (uint32_t)USARTx;

  /* �õ� USART �Ĵ�����ַ */
  usartreg = (((uint8_t)USART_IT) >> 0x05);

  /* �õ��ж�λ�� */
  itpos = USART_IT & IT_Mask;
  itmask = (((uint32_t)0x01) << itpos);
    
  if (usartreg == 0x01) /* The IT is in CR1 register */
  {
    usartxbase += 0x0C;
  }
  else if (usartreg == 0x02) /* The IT is in CR2 register */
  {
    usartxbase += 0x10;
  }
  else /* The IT is in CR3 register */
  {
    usartxbase += 0x14; 
  }
  if (NewState != DISABLE)
  {
    *(__IO uint32_t*)usartxbase  |= itmask;
  }
  else
  {
    *(__IO uint32_t*)usartxbase &= ~itmask;
  }
}

/**
  * @����  ʹ�ܻ���ʧ��ָ�� USART �� DMA ����.
  * @����  USARTx: ѡ�� USART ���� UART ����Χ�豸. 
  *                ������������������ֵ֮һ : USART1, USART2, USART3 or UART4.  
  * @����  USART_DMAReq:ָ�� DMA ����.
  *          ���������������������ֵ�����:
  *          USART_DMAReq_Tx: USART DMA ��������
  *          USART_DMAReq_Rx: USART DMA ��������
  * @����  NewState: DMA ����Դ����״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @ע��  DMA ģʽ����Ӧ���� UART5.  
  * @����  û��
  */
void USART_DMACmd(USART_TypeDef* USARTx, uint16_t USART_DMAReq, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_USART_1234_PERIPH(USARTx));
  assert_param(IS_USART_DMAREQ(USART_DMAReq));  
  assert_param(IS_FUNCTIONAL_STATE(NewState)); 
  if (NewState != DISABLE)
  {
    /* ͨ������USART CR3 �Ĵ�����DMAT��DMATλ���,��ʹ��ѡ��DMA�������� */
    USARTx->CR3 |= USART_DMAReq;
  }
  else
  {
    /* ͨ������USART CR3 �Ĵ�����DMAT��DMATλ���,��ʧ��ѡ��DMA�������� */
    USARTx->CR3 &= (uint16_t)~USART_DMAReq;
  }
}

/**
  * @����  ���� USART �ڵ�ĵ�ַ.
  * @����  USARTx: ѡ�� USART ���� UART ����Χ�豸. 
  *                ������������������ֵ֮һ : USART1, USART2, USART3, UART4 or UART5.
  * @����  USART_Address: ���� USART �ڵ�ĵ�ַ.
  * @����  û��
  */ 
void USART_SetAddress(USART_TypeDef* USARTx, uint8_t USART_Address)
{
  /* ������ */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_ADDRESS(USART_Address)); 
    
  /* ��� USART ��ַ */
  USARTx->CR2 &= CR2_Address_Mask;
  /* ���� USART �ڵ�ĵ�ַ */
  USARTx->CR2 |= USART_Address;
}

/**
  * @����  ѡ�� USART �Ļ��ѷ�ʽ.
  * @����  USARTx: ѡ�� USART ���� UART ����Χ�豸. 
  *                ������������������ֵ֮һ : USART1, USART2, USART3, UART4 or UART5.
  * @����  USART_WakeUp: ָ�� USART �Ļ��ѷ�ʽ.
  *          ������������������ֵ֮һ :
  *          USART_WakeUp_IdleLine:    �������߻���
  *          USART_WakeUp_AddressMark: ��ַ��ǻ���
  * @����  û��
  */
void USART_WakeUpConfig(USART_TypeDef* USARTx, uint16_t USART_WakeUp)
{
  /* ������ */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_WAKEUP(USART_WakeUp));
  
  USARTx->CR1 &= CR1_WAKE_Mask;
  USARTx->CR1 |= USART_WakeUp;
}

/**
  * @����  ��� USART �Ƿ��ھ�Ĭģʽ.
  * @����  USARTx: ѡ�� USART ���� UART ����Χ�豸. 
  *                ������������������ֵ֮һ : USART1, USART2, USART3, UART4 or UART5.
  * @����  NewState: USART ��Ĭģʽ����״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void USART_ReceiverWakeUpCmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_FUNCTIONAL_STATE(NewState)); 
  
  if (NewState != DISABLE)
  {
    /* ͨ����λCR1�Ĵ�����RWUλʹ��USART����ģʽ */
    USARTx->CR1 |= CR1_RWU_Set;
  }
  else
  {
    /* ͨ������CR1�Ĵ�����RWUλʧ��USART����ģʽ */
    USARTx->CR1 &= CR1_RWU_Reset;
  }
}

/**
  * @����  ���� USART LIN �жϼ�ⳤ��.
  * @����  USARTx: ѡ�� USART ���� UART ����Χ�豸. 
  *                ������������������ֵ֮һ : USART1, USART2, USART3, UART4 �� UART5.
  * @����  USART_LINBreakDetectLength: LIN �жϼ�ⳤ��.
  *          ������������������ֵ֮һ :
  *          USART_LINBreakDetectLength_10b: 10λ��ϼ��
  *          USART_LINBreakDetectLength_11b: 11λ��ϼ��
  * @����  û��
  */
void USART_LINBreakDetectLengthConfig(USART_TypeDef* USARTx, uint16_t USART_LINBreakDetectLength)
{
  /* ������ */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_LIN_BREAK_DETECT_LENGTH(USART_LINBreakDetectLength));
  
  USARTx->CR2 &= CR2_LBDL_Mask;
  USARTx->CR2 |= USART_LINBreakDetectLength;  
}

/**
  * @����  ʹ�ܻ���ʧ�� USARTx �� LIN ģʽ.
  * @����  USARTx: ѡ�� USART ���� UART ����Χ�豸. 
  *                ������������������ֵ֮һ : USART1, USART2, USART3, UART4 or UART5.
  * @����  NewState: USART LIN ģʽ����״̬.
  *                  �������������: ENABLE or DISABLE.
  * @����  û��
  */
void USART_LINCmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* ͨ����λCR2�Ĵ�����LINENλʹ��LINģʽ */
    USARTx->CR2 |= CR2_LINEN_Set;
  }
  else
  {
    /* ͨ������CR2�Ĵ�����LINENλʧ��LINģʽ */
    USARTx->CR2 &= CR2_LINEN_Reset;
  }
}

/**
  * @����  ͨ������ USARTx ���͵�������.
  * @����  USARTx: ѡ�� USART ���� UART ����Χ�豸.
  *                ������������������ֵ֮һ : USART1, USART2, USART3, UART4 �� UART5.
  * @����  Data: Ҫ���͵�����.
  * @����  û��
  */
void USART_SendData(USART_TypeDef* USARTx, uint16_t Data)
{
  /* ������ */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_DATA(Data)); 
    
  /* �������� */
  USARTx->DR = (Data & (uint16_t)0x01FF);
}

/**
  * @����  ���� USARTx ������յ�������.
  * @����  USARTx: ѡ�� USART ���� UART ����Χ�豸.
  *                ������������������ֵ֮һ : USART1, USART2, USART3, UART4 �� UART5.
  * @����  ��������.
  */
uint16_t USART_ReceiveData(USART_TypeDef* USARTx)
{
  /* ������ */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  
  /* �������� */
  return (uint16_t)(USARTx->DR & (uint16_t)0x01FF);
}

/**
  * @����  �����ж���.
  * @����  USARTx: ѡ�� USART ���� UART ����Χ�豸. 
  *                ������������������ֵ֮һ : USART1, USART2, USART3, UART4 �� UART5.
  * @����  û��
  */
void USART_SendBreak(USART_TypeDef* USARTx)
{
  /* ������ */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  
  /* �����ж��� */
  USARTx->CR1 |= CR1_SBK_Set;
}

/**
  * @����  ����ָ���� USART ����ʱ��.
  * @����  USARTx: x����Ϊ1,2��3��ѡ��USART����Χ�豸.
  * @����  USART_GuardTime: ָ���ı���ʱ��.
  * @ע��  �������ʱ��λ����Ӧ���� UART4 �� UART5.   
  * @����  û��
  */
void USART_SetGuardTime(USART_TypeDef* USARTx, uint8_t USART_GuardTime)
{    
  /* ������ */
  assert_param(IS_USART_123_PERIPH(USARTx));
  
  /* ��� USART ����ʱ�� */
  USARTx->GTPR &= GTPR_LSB_Mask;
  /* ���� USART ����ʱ�� */
  USARTx->GTPR |= (uint16_t)((uint16_t)USART_GuardTime << 0x08);
}

/**
  * @����  ���� USART ʱ��Ԥ��Ƶ.
  * @����  USARTx: ѡ�� USART ���� UART ����Χ�豸. 
  *                ������������������ֵ֮һ : USART1, USART2, USART3, UART4 or UART5.
  * @����  USART_Prescaler: ָ��Ԥ��Ƶ��.  
  * @ע��  ������������� UART4 �� UART5 �� IrDA ģʽ.
  * @����  û��
  */
void USART_SetPrescaler(USART_TypeDef* USARTx, uint8_t USART_Prescaler)
{ 
  /* ������ */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  
  /* ��� USART Ԥ��Ƶ�� */
  USARTx->GTPR &= GTPR_MSB_Mask;
  /* ���� USART Ԥ��Ƶ�� */
  USARTx->GTPR |= USART_Prescaler;
}

/**
  * @����  ʹ�ܻ���ʧ��ָ�� USART �����ܿ�ģʽ.
  * @����  USARTx: x����Ϊ1,2��3��ѡ��USART����Χ�豸.
  * @����  NewState: USART ���ܿ�ģʽ����״̬.
  *                  �������������: ENABLE �� DISABLE.     
  * @ע��  ������ܿ�ģʽ����Ӧ���� UART4 �� UART5 . 
  * @����  û��
  */
void USART_SmartCardCmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_USART_123_PERIPH(USARTx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* ͨ����λCR3�Ĵ�����SCENλʹ��SCģʽ */
    USARTx->CR3 |= CR3_SCEN_Set;
  }
  else
  {
    /* ͨ������CR3�Ĵ�����SCENλʧ��SCģʽ */
    USARTx->CR3 &= CR3_SCEN_Reset;
  }
}

/**
  * @����  ʹ�ܻ���ʧ�� NACK ����.
  * @����  USARTx: x����Ϊ1,2��3��ѡ��USART����Χ�豸. 
  * @����  NewState: NACK �������״̬.
  *                  �������������: ENABLE �� DISABLE.  
  * @ע��  ������ܿ�ģʽ����Ӧ���� UART4 �� UART5 .
  * @����  û��
  */
void USART_SmartCardNACKCmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_USART_123_PERIPH(USARTx));  
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* ͨ����λCR3�Ĵ�����NACKλʹ��NACK���� */
    USARTx->CR3 |= CR3_NACK_Set;
  }
  else
  {
    /* ͨ������CR3�Ĵ�����NACKλʧ��NACK���� */
    USARTx->CR3 &= CR3_NACK_Reset;
  }
}

/**
  * @����  ʹ�ܻ���ʧ�� USART ��˫��ģʽ.
  * @����  USARTx: ѡ�� USART ���� UART ����Χ�豸. 
  *                ������������������ֵ֮һ : USART1, USART2, USART3, UART4 or UART5.
  * @����  NewState: USART ��˫��ģʽ����״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void USART_HalfDuplexCmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  
  if (NewState != DISABLE)
  {
    /* ͨ����λCR3�Ĵ�����HDSELλʹ��Half-Duplexģʽ */
    USARTx->CR3 |= CR3_HDSEL_Set;
  }
  else
  {
    /* ͨ������CR3�Ĵ�����HDSELλʧ��Half-Duplexģʽ */
    USARTx->CR3 &= CR3_HDSEL_Reset;
  }
}

/**
  * @����  ���� USART IrDA ģʽ.
  * @����  USARTx: ѡ�� USART ���� UART ����Χ�豸. 
  *                ������������������ֵ֮һ : USART1, USART2, USART3, UART4 �� UART5.
  * @����  USART_IrDAMode: ָ�� IrDA ģʽ.
  *                        ������������������ֵ֮һ :
  *                        USART_IrDAMode_LowPower: �͹���ģʽ
  *                        USART_IrDAMode_Normal  : ����ģʽ
  * @����  û��
  */
void USART_IrDAConfig(USART_TypeDef* USARTx, uint16_t USART_IrDAMode)
{
  /* ������ */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_IRDA_MODE(USART_IrDAMode));
    
  USARTx->CR3 &= CR3_IRLP_Mask;
  USARTx->CR3 |= USART_IrDAMode;
}

/**
  * @����  ʹ�ܻ���ʧ�� USART IrDA ģʽ.
  * @����  USARTx: ѡ�� USART ���� UART ����Χ�豸.
  *                ������������������ֵ֮һ : USART1, USART2, USART3, UART4 �� UART5.
  * @����  NewState: IrDA ģʽ����״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void USART_IrDACmd(USART_TypeDef* USARTx, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
    
  if (NewState != DISABLE)
  {
    /* ͨ����λCR3�Ĵ�����IRENλʹ��IrDAģʽ */
    USARTx->CR3 |= CR3_IREN_Set;
  }
  else
  {
    /* ͨ������CR3�Ĵ�����IRENλʧ��IrDAģʽ */
    USARTx->CR3 &= CR3_IREN_Reset;
  }
}

/**
  * @����  ���ָ���� USART ��־λ�������.
  * @����  USARTx: ѡ�� USART ���� UART ����Χ�豸. 
  *                ������������������ֵ֮һ : USART1, USART2, USART3, UART4 or UART5.
  * @����  USART_FLAG: ָ��Ҫ���ı��.
  *          ������������������ֵ֮һ :
  *          USART_FLAG_CTS:  CTS �ı��־ (����Ӧ���� UART4 �� UART5)
  *          USART_FLAG_LBD:  LIN �������־
  *          USART_FLAG_TXE:  �������ݼĴ���Ϊ�ձ�־
  *          USART_FLAG_TC:   ������ɱ�־
  *          USART_FLAG_RXNE: �������ݼĴ�����Ϊ�ձ�־
  *          USART_FLAG_IDLE: �������߱�־
  *          USART_FLAG_ORE:  ��������־
  *          USART_FLAG_NE:   ���������־
  *          USART_FLAG_FE:   ֡�����־
  *          USART_FLAG_PE:   ��ż�����־
  * @retval  USART_FLAG ����״̬(SET �� RESET).
  */
FlagStatus USART_GetFlagStatus(USART_TypeDef* USARTx, uint16_t USART_FLAG)
{
  FlagStatus bitstatus = RESET;
  /* ������ */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_FLAG(USART_FLAG));
  /* CTS ��־����Ӧ���� UART4 �� UART5 */
  if (USART_FLAG == USART_FLAG_CTS)
  {
    assert_param(IS_USART_123_PERIPH(USARTx));
  }  
  
  if ((USARTx->SR & USART_FLAG) != (uint16_t)RESET)
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
  * @����  ��� USARTx �Ĵ������־λ.
  * @����  USARTx: ѡ�� USART ���� UART ����Χ�豸.
  *                ������������������ֵ֮һ : USART1, USART2, USART3, UART4 or UART5.
  * @����  USART_FLAG: ָ��Ҫ������ı��.
  *          ������������������ֵ���������:
  *          USART_FLAG_CTS:  CTS �ı��־ (����Ӧ���� UART4 �� UART5).
  *          USART_FLAG_LBD:  LIN �������־.
  *          USART_FLAG_TC:   ������ɱ�־.
  *          USART_FLAG_RXNE: �������ݼĴ�����Ϊ�ձ�־.
  *   
  * @ע��
*          - PE (��żУ�����), FE (�����), NE (��������),
*            ORE (���ٴ���) �� IDLE (�ҵ�������) ��־��־�������˳�����:
*            һ����USART_SR�Ĵ���������USART_GetFlagStatus()��
*            ����һ����USART_DR�Ĵ�������(USART_ReceiveData()).
*          - RXNE��־Ҳ���Ա�һ����USART_DR�Ĵ����������(USART_ReceiveData()).
*          - TC��־��־�������˳�����: һ����USART_SR�Ĵ���������USART_GetFlagStatus()��
*            ����һ����USART_DR�Ĵ�������(USART_ReceiveData()).                                                      
*          - TXE��־ֻ�ܿ�дUSART_DR�Ĵ������(USART_SendData()).     
  * @����  û��
  */
void USART_ClearFlag(USART_TypeDef* USARTx, uint16_t USART_FLAG)
{
  /* ������ */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_CLEAR_FLAG(USART_FLAG));
  /* CTS ��־����Ӧ���� UART4 �� UART5 */
  if ((USART_FLAG & USART_FLAG_CTS) == USART_FLAG_CTS)
  {
    assert_param(IS_USART_123_PERIPH(USARTx));
  } 
   
  USARTx->SR = (uint16_t)~USART_FLAG;
}

/**
  * @����  ���ָ���� USART �жϷ������.
  * @����  USARTx: ѡ�� USART ���� UART ����Χ�豸. 
  *                ������������������ֵ֮һ : USART1, USART2, USART3, UART4 or UART5.
  * @����  USART_IT: ָ��Ҫ����USART�ж�Դ.
  *          ������������������ֵ֮һ :
  *          USART_IT_CTS:  CTS �ı��ж� (����Ӧ���� UART4 �� UART5)
  *          USART_IT_LBD:  LIN �������ж�
  *          USART_IT_TXE:  �������ݼĴ���Ϊ���ж�
  *          USART_IT_TC:   ��������ж�
  *          USART_IT_RXNE: �������ݼĴ�����Ϊ���ж�
  *          USART_IT_IDLE: ���������ж�
  *          USART_IT_ORE:  ��������ж�
  *          USART_IT_NE:   ���������ж�
  *          USART_IT_FE:   ֡�����ж�
  *          USART_IT_PE:   ��ż�����ж�
  * @����  USART_IT ����״̬ (SET �� RESET).
  */
ITStatus USART_GetITStatus(USART_TypeDef* USARTx, uint16_t USART_IT)
{
  uint32_t bitpos = 0x00, itmask = 0x00, usartreg = 0x00;
  ITStatus bitstatus = RESET;
  /* ������ */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_GET_IT(USART_IT));
  /* CTS �жϲ���Ӧ���� UART4 �� UART5 */ 
  if (USART_IT == USART_IT_CTS)
  {
    assert_param(IS_USART_123_PERIPH(USARTx));
  }   
  
  /* �õ� USART �Ĵ������� */
  usartreg = (((uint8_t)USART_IT) >> 0x05);
  /*�õ��ж�λ�� */
  itmask = USART_IT & IT_Mask;
  itmask = (uint32_t)0x01 << itmask;
  
  if (usartreg == 0x01) /* The IT  is in CR1 register */
  {
    itmask &= USARTx->CR1;
  }
  else if (usartreg == 0x02) /* The IT  is in CR2 register */
  {
    itmask &= USARTx->CR2;
  }
  else /* The IT  is in CR3 register */
  {
    itmask &= USARTx->CR3;
  }
  
  bitpos = USART_IT >> 0x08;
  bitpos = (uint32_t)0x01 << bitpos;
  bitpos &= USARTx->SR;
  if ((itmask != (uint16_t)RESET)&&(bitpos != (uint16_t)RESET))
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
  * @����  ��� USARTx ���жϴ�����λ.
  * @����  USARTx: ѡ�� USART ���� UART ����Χ�豸. 
  *                ������������������ֵ֮һ : USART1, USART2, USART3, UART4 or UART5.
  * @����  USART_IT: ָ��Ҫ��������жϹ���λ.
  *          ������������������ֵ֮һ:
  *          USART_IT_CTS:  CTS �ı��ж� (����Ӧ���� UART4 �� UART5)
  *          USART_IT_LBD:  LIN �������ж�
  *          USART_IT_TC:   ��������ж�
  *          USART_IT_RXNE: �������ݼĴ�����Ϊ���ж�
  *   
  * @ע��
*          - PE (��żУ�����), FE (�����), NE (��������),
*            ORE (���ٴ���) �� IDLE (�ҵ�������)����λ 
*            �������˳�����: һ����USART_SR�Ĵ���������USART_GetFlagStatus()��
*            ����һ����USART_DR�Ĵ�������(USART_ReceiveData()).
*          - RXNE����λҲ���Ա�һ����USART_DR�Ĵ����������(USART_ReceiveData()).
*          - TC����λҲ���Ա������˳�����: һ����USART_SR�Ĵ���������USART_GetFlagStatus()��
*            ����һ����USART_DR�Ĵ�������(USART_ReceiveData()).                                                      
*          - TXE����λ�ÿ��Ա�һ��дUSART_DR�Ĵ������(USART_SendData()).  
  * @����  û��
  */
void USART_ClearITPendingBit(USART_TypeDef* USARTx, uint16_t USART_IT)
{
  uint16_t bitpos = 0x00, itmask = 0x00;
  /* ������ */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_CLEAR_IT(USART_IT));
  /* CTS �жϲ���Ӧ���� UART4 �� UART5 */
  if (USART_IT == USART_IT_CTS)
  {
    assert_param(IS_USART_123_PERIPH(USARTx));
  }   
  
  bitpos = USART_IT >> 0x08;
  itmask = ((uint16_t)0x01 << (uint16_t)bitpos);
  USARTx->SR = (uint16_t)~itmask;
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
