/**
  ******************************************************************************
  * @�ļ�    stm32f10x_spi.c
  * @����    MCD Application Team
  * @�汾    V3.1.0
  * @����    06/19/2009
  * @����    ����ļ� �ṩ���� SPI �̼�����.
  * @����    ANSON/sweet1985 Email:airanson110@126.com
  * @����    25/07/2009
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
#include "stm32f10x_spi.h"
#include "stm32f10x_rcc.h"

/* �������� --------------------------------------------------------------*/
/* ���ö��� --------------------------------------------------------------*/

/* SPI SPE mask */
#define CR1_SPE_Set          ((uint16_t)0x0040)
#define CR1_SPE_Reset        ((uint16_t)0xFFBF)

/* I2S I2SE mask */
#define I2SCFGR_I2SE_Set     ((uint16_t)0x0400)
#define I2SCFGR_I2SE_Reset   ((uint16_t)0xFBFF)

/* SPI CRCNext mask */
#define CR1_CRCNext_Set      ((uint16_t)0x1000)

/* SPI CRCEN mask */
#define CR1_CRCEN_Set        ((uint16_t)0x2000)
#define CR1_CRCEN_Reset      ((uint16_t)0xDFFF)

/* SPI SSOE mask */
#define CR2_SSOE_Set         ((uint16_t)0x0004)
#define CR2_SSOE_Reset       ((uint16_t)0xFFFB)

/* SPI registers Masks */
#define CR1_CLEAR_Mask       ((uint16_t)0x3040)
#define I2SCFGR_CLEAR_Mask   ((uint16_t)0xF040)

/* SPI or I2S mode selection masks */
#define SPI_Mode_Select      ((uint16_t)0xF7FF)
#define I2S_Mode_Select      ((uint16_t)0x0800) 

/* I2S clock source selection masks */
#define I2S2_CLOCK_SRC       ((u32)(0x00020000))
#define I2S3_CLOCK_SRC       ((u32)(0x00040000))
#define I2S_MUL_MASK         ((u32)(0x0000F000))
#define I2S_DIV_MASK         ((u32)(0x000000F0))

/* ���ú� -------------------------------------------------------------*/
/* ���ñ��� -----------------------------------------------------------*/
/* ���ú���ԭ�� -------------------------------------------------------*/
/* ���ú��� -----------------------------------------------------------*/

/**
  * @����  ������ SPIx �Ĵ�������Ϊȱʡֵ (Ҳ���� I2Ss).
  * @����  SPIx: xΪ1��2��3����ѡ�� SPI ����.
  * @����  û��
  */
void SPI_I2S_DeInit(SPI_TypeDef* SPIx)
{
  /* ������ */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));

  if (SPIx == SPI1)
  {
    /* ʹ�� SPI1 ��λ״̬ */
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, ENABLE);
    /* �ͷ� SPI1 �Ӹ�λ״̬ */
    RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, DISABLE);
  }
  else if (SPIx == SPI2)
  {
    /* ʹ�� SPI2 ��λ״̬ */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, ENABLE);
    /* �ͷ� SPI2 �Ӹ�λ״̬ */
    RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI2, DISABLE);
  }
  else
  {
    if (SPIx == SPI3)
    {
      /* ʹ�� SPI3 ��λ״̬ */
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, ENABLE);
      /* �ͷ� SPI3 �Ӹ�λ״̬ */
      RCC_APB1PeriphResetCmd(RCC_APB1Periph_SPI3, DISABLE);
    }
  }
}

/**
  * @����  ���� SPI_InitStruct ��ָ���Ĳ�����ʼ������ SPIx �Ĵ���.
  * @����  SPIx: x Ϊ1��2��3����ѡ�� SPI ����.
  * @����  SPI_InitStruct: ָ��һ�������ض� SPI ����������Ϣ�� SPI_InitTypeDef �ṹ���ָ��.
  * @����  û��
  */
void SPI_Init(SPI_TypeDef* SPIx, SPI_InitTypeDef* SPI_InitStruct)
{
  uint16_t tmpreg = 0;
  
  /* ������ */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));   
  
  /* ��� SPI �Ĳ��� */
  assert_param(IS_SPI_DIRECTION_MODE(SPI_InitStruct->SPI_Direction));
  assert_param(IS_SPI_MODE(SPI_InitStruct->SPI_Mode));
  assert_param(IS_SPI_DATASIZE(SPI_InitStruct->SPI_DataSize));
  assert_param(IS_SPI_CPOL(SPI_InitStruct->SPI_CPOL));
  assert_param(IS_SPI_CPHA(SPI_InitStruct->SPI_CPHA));
  assert_param(IS_SPI_NSS(SPI_InitStruct->SPI_NSS));
  assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_InitStruct->SPI_BaudRatePrescaler));
  assert_param(IS_SPI_FIRST_BIT(SPI_InitStruct->SPI_FirstBit));
  assert_param(IS_SPI_CRC_POLYNOMIAL(SPI_InitStruct->SPI_CRCPolynomial));

/*---------------------------- SPIx CR1 ���� ------------------------*/
  /* �õ� SPIx CR1 ��ֵ */
  tmpreg = SPIx->CR1;
  /* ��� BIDIMode, BIDIOE, RxONLY, SSM, SSI, LSBFirst, BR, MSTR, CPOL �� CPHA λ */
  tmpreg &= CR1_CLEAR_Mask;
  /* ���� SPIx: direction(����), NSS management(NSS����), first transmitted bit(��һ������λ), BaudRate prescaler(������Ԥ��Ƶ)
     master/salve mode(��/��ģʽ), CPOL(ʱ�Ӽ���) �� CPHA(ʱ����λ) */
  /* ���� BIDImode, BIDIOE �� RxONLY λ������ SPI_Direction ��ֵ */
  /* ���� SSM, SSI and MSTR λ������ SPI_Mode �� SPI_NSS ��ֵ */
  /* ���� LSBFirst λ������ SPI_FirstBit ��ֵ */
  /* ���� BR λ������ SPI_BaudRatePrescaler ��ֵ */
  /* ���� CPOL λ������ SPI_CPOL ��ֵ */
  /* ���� CPHA λ������ SPI_CPHA ��ֵ */
  tmpreg |= (uint16_t)((uint32_t)SPI_InitStruct->SPI_Direction | SPI_InitStruct->SPI_Mode |
                  SPI_InitStruct->SPI_DataSize | SPI_InitStruct->SPI_CPOL |  
                  SPI_InitStruct->SPI_CPHA | SPI_InitStruct->SPI_NSS |  
                  SPI_InitStruct->SPI_BaudRatePrescaler | SPI_InitStruct->SPI_FirstBit);
  /* д SPIx CR1 */
  SPIx->CR1 = tmpreg;
  
  /* ���� SPI ģʽ (��λ I2SMOD λ���� I2SCFGR �Ĵ���) */
  SPIx->I2SCFGR &= SPI_Mode_Select;		

/*---------------------------- SPIx CRCPOLY ���� --------------------*/
  /* д SPIx CRCPOLY */
  SPIx->CRCPR = SPI_InitStruct->SPI_CRCPolynomial;
}

/**
  * @����  ���� SPI_InitStruct �е��ض�������ʼ�� SPIx ����.
  * @����  SPIx: xΪ1��2��3����ѡ��SPI���� (���� I2S ģʽ).
  * @����  I2S_InitStruct: ָ��һ����ģʽ I2S �����ض� SPI ����������Ϣ�� SPI_InitTypeDef �ṹ���ָ��.
  * @ע��
  *  The function calculates the optimal prescaler needed to obtain the most 
  *  accurate audio frequency (depending on the I2S clock source, the PLL values 
  *  and the product configuration). But in case the prescaler value is greater 
  *  than 511, the default value (0x02) will be configured instead.  *    
  * @����  û��
  */
void I2S_Init(SPI_TypeDef* SPIx, I2S_InitTypeDef* I2S_InitStruct)
{
  uint16_t tmpreg = 0, i2sdiv = 2, i2sodd = 0, packetlength = 1;
  uint32_t tmp = 0;
  RCC_ClocksTypeDef RCC_Clocks;
  uint32_t sourceclock = 0;
  
  /* ��� I2S ���� */
  assert_param(IS_SPI_23_PERIPH(SPIx));
  assert_param(IS_I2S_MODE(I2S_InitStruct->I2S_Mode));
  assert_param(IS_I2S_STANDARD(I2S_InitStruct->I2S_Standard));
  assert_param(IS_I2S_DATA_FORMAT(I2S_InitStruct->I2S_DataFormat));
  assert_param(IS_I2S_MCLK_OUTPUT(I2S_InitStruct->I2S_MCLKOutput));
  assert_param(IS_I2S_AUDIO_FREQ(I2S_InitStruct->I2S_AudioFreq));
  assert_param(IS_I2S_CPOL(I2S_InitStruct->I2S_CPOL));  

/*----------------------- SPIx I2SCFGR & I2SPR ���� -----------------*/
  /* ��� I2SMOD, I2SE, I2SCFG, PCMSYNC, I2SSTD, CKPOL, DATLEN �� CHLEN λ */
  SPIx->I2SCFGR &= I2SCFGR_CLEAR_Mask; 
  SPIx->I2SPR = 0x0002;
  
  /* �õ� I2SCFGR �Ĵ�����ֵ */
  tmpreg = SPIx->I2SCFGR;
  
  /* ���Ĭ��ֵ�Ѿ�д�ˣ����³�ʼ�� i2sdiv �� i2sodd */
  if(I2S_InitStruct->I2S_AudioFreq == I2S_AudioFreq_Default)
  {
    i2sodd = (uint16_t)0;
    i2sdiv = (uint16_t)2;   
  }
  /* �������ĵ���Ƶ����Ĭ��ֵ������Ԥ��Ƶ */
  else
  {
    /* �������(����Ԥ��Ƶ��) */
    if(I2S_InitStruct->I2S_DataFormat == I2S_DataFormat_16b)
    {
      /* ���ĳ���16λ */
      packetlength = 1;
    }
    else
    {
      /* ���ĳ���32λ */
      packetlength = 2;
    }

    /* ȡ�� I2S ϵͳʱ��Ƶ�� */
    if(((uint32_t)SPIx) == SPI2_BASE)
    {
      /* I2S2 */
      tmp = I2S2_CLOCK_SRC;
    }
    else 
    {
      /* I2S3 */      
      tmp = I2S3_CLOCK_SRC;
    }

    /* ��� I2S ʱ������Դ: ������ͨ�����豸�� PLL3 VCO ʱ�� */
#ifdef STM32F10X_CL
    if((RCC->CFGR2 & tmp) != 0)
    {
      /* �õ� RCC PLL3 ����ֵ������λ */
      tmp = (uint32_t)((RCC->CFGR2 & I2S_MUL_MASK) >> 12);

      /* �õ� PLL3 ��������ֵ */      
      if((tmp > 5) && (tmp < 15))
      {
        /* ����ֵ�� 8 �� 14 ֮�� (��ֹ 15 ) */
        tmp += 2;
      }
      else
      {
        if (tmp == 15)
        {
          /* �������� 20 */
          tmp = 20;
        }
      }      
      /* �õ� PREDIV2 ��ֵ */
      sourceclock = (uint32_t)(((RCC->CFGR2 & I2S_DIV_MASK) >> 4) + 1);
      
      /* ����ʱ��Ƶ�ʣ����� PLL3 �� PREDIV2 ��ֵ */
      sourceclock = (uint32_t) ((HSE_Value / sourceclock) * tmp * 2); 
    }
    else
    {
      /* I2S ʱ����ϵͳʱ��: �õ�ϵͳʱ��Ƶ�� */
      RCC_GetClocksFreq(&RCC_Clocks);      
      
      /* �õ���Դʱ�ӵ�ֵ: ����ϵͳʱ�ӵ�ֵ */
      sourceclock = RCC_Clocks.SYSCLK_Frequency;
    }        
#else /* STM32F10X_HD */
    /* I2S ʱ��Դ��ϵͳʱ��: �õ�ϵͳʱ��Ƶ�� */
    RCC_GetClocksFreq(&RCC_Clocks);      
      
    /* �õ���Դʱ�ӵ�ֵ: ����ϵͳʱ�ӵ�ֵ */
    sourceclock = RCC_Clocks.SYSCLK_Frequency;    
#endif /* STM32F10X_CL */    

    /* �ø���ָ�����MCLK���״̬������ʵ�ָ��� */
    if(I2S_InitStruct->I2S_MCLKOutput == I2S_MCLKOutput_Enable)
    {
      /* MCLK���ʹ�� */
      tmp = (uint16_t)(((((sourceclock / 256) * 10) / I2S_InitStruct->I2S_AudioFreq)) + 5);
    }
    else
    {
      /* MCLK���ʧ�� */
      tmp = (uint16_t)(((((sourceclock / (32 * packetlength)) *10 ) / I2S_InitStruct->I2S_AudioFreq)) + 5);
    }
    
    /* ɾ������ָ�� */
    tmp = tmp / 10;  
      
    /* ����������ż */
    i2sodd = (uint16_t)(tmp & (u16)0x0001);
   
    /* ����i2sdivԤ��Ƶ�� */
    i2sdiv = (uint16_t)((tmp - i2sodd) / 2);
   
    /* ȡ��(SPI_I2SPR[8])�Ĵ����ɵı�־λ */
    i2sodd = (uint16_t) (i2sodd << 8);
  }
  
  /* ���Գ�����1����0 ����� 0xFF */
  if ((i2sdiv < 2) || (i2sdiv > 0xFF))
  {
    /* ����ȱʡֵ */
    i2sdiv = 2;
    i2sodd = 0;
  }

  /* д SPIx I2SPR �Ĵ����ļ���ֵ */
  SPIx->I2SPR = (uint16_t)(i2sdiv | (uint16_t)(i2sodd | (uint16_t)I2S_InitStruct->I2S_MCLKOutput));  
 
  /* �� SPI_InitStruct ��ֵ������ I2S  */
  tmpreg |= (uint16_t)(I2S_Mode_Select | (uint16_t)(I2S_InitStruct->I2S_Mode | \
                  (uint16_t)(I2S_InitStruct->I2S_Standard | (uint16_t)(I2S_InitStruct->I2S_DataFormat | \
                  (uint16_t)I2S_InitStruct->I2S_CPOL))));
 
  /* д SPIx I2SCFGR */  
  SPIx->I2SCFGR = tmpreg;   
}

/**
  * @����  ʹ��ȱʡֵ��� SPI_InitStruct ÿһ����Ա.
  * @����  SPI_InitStruct : ָ��һ��������ʼ���� SPI_InitTypeDef �ṹ��ָ��.
  * @����  û��
  */
void SPI_StructInit(SPI_InitTypeDef* SPI_InitStruct)
{
/*--------------- ���� SPI I��ʼ���ṹ����ֵ -----------------*/
  /* ��ʼ�� SPI_Direction ��Ա */
  SPI_InitStruct->SPI_Direction = SPI_Direction_2Lines_FullDuplex;
  /* ��ʼ�� SPI_Mode ��Ա */
  SPI_InitStruct->SPI_Mode = SPI_Mode_Slave;
  /* ��ʼ�� SPI_DataSize ��Ա */
  SPI_InitStruct->SPI_DataSize = SPI_DataSize_8b;
  /* ��ʼ�� SPI_CPOL ��Ա */
  SPI_InitStruct->SPI_CPOL = SPI_CPOL_Low;
  /* ��ʼ�� SPI_CPHA ��Ա */
  SPI_InitStruct->SPI_CPHA = SPI_CPHA_1Edge;
  /* ��ʼ�� SPI_NSS ��Ա */
  SPI_InitStruct->SPI_NSS = SPI_NSS_Hard;
  /* ��ʼ�� SPI_BaudRatePrescaler ��Ա */
  SPI_InitStruct->SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
  /* ��ʼ�� SPI_FirstBit ��Ա */
  SPI_InitStruct->SPI_FirstBit = SPI_FirstBit_MSB;
  /* ��ʼ�� SPI_CRCPolynomial ��Ա */
  SPI_InitStruct->SPI_CRCPolynomial = 7;
}

/**
  * @����  ��Ĭ��ֵ���ÿ�� I2S_InitStruct �ṹ��Ա.
  * @����  I2S_InitStruct : ָ��Ҫ����ʼ����I2S_InitTypeDef �ṹָ��.
  * @����  û��
  */
void I2S_StructInit(I2S_InitTypeDef* I2S_InitStruct)
{
/*--------------- ���� I2S ��ʼ���ṹ����ֵ -----------------*/
  /* ��ʼ�� I2S_Mode ��Ա */
  I2S_InitStruct->I2S_Mode = I2S_Mode_SlaveTx;
  
  /* ��ʼ�� I2S_Standard ��Ա */
  I2S_InitStruct->I2S_Standard = I2S_Standard_Phillips;
  
  /* ��ʼ�� I2S_DataFormat ��Ա */
  I2S_InitStruct->I2S_DataFormat = I2S_DataFormat_16b;
  
  /* ��ʼ�� I2S_MCLKOutput ��Ա */
  I2S_InitStruct->I2S_MCLKOutput = I2S_MCLKOutput_Disable;
  
  /* ��ʼ�� I2S_AudioFreq ��Ա */
  I2S_InitStruct->I2S_AudioFreq = I2S_AudioFreq_Default;
  
  /* ��ʼ�� I2S_CPOL ��Ա */
  I2S_InitStruct->I2S_CPOL = I2S_CPOL_Low;
}

/**
  * @����  ʹ�ܻ���ʧ�� SPI ����.
  * @����  SPIx: x Ϊ1��2��3����ѡ�� SPI ����.
  * @����  NewState: SPIx ��Χ�豸����״̬. 
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void SPI_Cmd(SPI_TypeDef* SPIx, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* ʹ�� SPI ���� */
    SPIx->CR1 |= CR1_SPE_Set;
  }
  else
  {
    /* ʧ�� SPI ���� */
    SPIx->CR1 &= CR1_SPE_Reset;
  }
}

/**
  * @����  ʹ�ܻ��ָֹ���� I2S ���� (�� I2S ģʽ).
  * @����  SPIx: xΪ1��2��3����ѡ�� SPI ����.
  * @����  NewState: SPIx ��Χ�豸����״̬. 
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void I2S_Cmd(SPI_TypeDef* SPIx, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_SPI_23_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* ʹ��ѡ���� SPI ���� (�� I2S ģʽ) */
    SPIx->I2SCFGR |= I2SCFGR_I2SE_Set;
  }
  else
  {
    /* ʧ��ѡ���� SPI ���� (�� I2S ģʽ) */
    SPIx->I2SCFGR &= I2SCFGR_I2SE_Reset;
  }
}

/**
  * @����  ʹ�ܻ���ʧ��ָ���� SPI/I2S �ж�.
  * @����  SPIx: x ������
  *            - 1, 2 �� 3 �� SPI ģʽ 
  *            - 2 �� 3 �� I2S ģʽ
  * @����  SPI_I2S_IT: ָ�����ж�Դ. 
  *          ������������������ֵ֮һ:
  *          SPI_I2S_IT_TXE:  ���ͻ�����ж�
  *          SPI_I2S_IT_RXNE: ���ջ���ǿ��ж�
  *          SPI_I2S_IT_ERR:  �����ж�
  * @����  NewState: ָ���� SPI/I2S �жϵ���״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @retval None
  */
void SPI_I2S_ITConfig(SPI_TypeDef* SPIx, uint8_t SPI_I2S_IT, FunctionalState NewState)
{
  uint16_t itpos = 0, itmask = 0 ;
  /* ������ */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_SPI_I2S_CONFIG_IT(SPI_I2S_IT));

  /* �õ� SPI/I2S �ж����� */
  itpos = SPI_I2S_IT >> 4;

  /* ���� IT */
  itmask = (uint16_t)1 << (uint16_t)itpos;

  if (NewState != DISABLE)
  {
    /* ʹ�� SPI/I2S �ж� */
    SPIx->CR2 |= itmask;
  }
  else
  {
    /* ʧ�� SPI/I2S �ж� */
    SPIx->CR2 &= (uint16_t)~itmask;
  }
}

/**
  * @����  ʹ�ܻ���ʧ��ָ�� SPIx/I2Sx ��DMA����.
  * @����  SPIx: x ������
  *             - 1, 2 �� 3 �� SPI ģʽ 
  *             - 2 �� 3 �� I2S ģʽ
  * @����  SPI_I2S_DMAReq: ָ���� SPI/I2S DMA ����. 
  *          ���������������������ֵ�����:
  *          SPI_I2S_DMAReq_Tx: ���ͻ��� DMA ��������
  *          SPI_I2S_DMAReq_Rx: ���ջ��� DMA ��������
  * @����  NewState: ѡ���� SPI/I2S DMA �������״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @retval None
  */
void SPI_I2S_DMACmd(SPI_TypeDef* SPIx, uint16_t SPI_I2S_DMAReq, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  assert_param(IS_SPI_I2S_DMAREQ(SPI_I2S_DMAReq));
  if (NewState != DISABLE)
  {
    /* ʹ��ѡ���� SPI/I2S DMA ���� */
    SPIx->CR2 |= SPI_I2S_DMAReq;
  }
  else
  {
    /* ʧ��ѡ���� SPI/I2S DMA ���� */
    SPIx->CR2 &= (uint16_t)~SPI_I2S_DMAReq;
  }
}

/**
  * @����  ͨ�� SPIx/I2Sx ���跢������.
  * @����  SPIx: x ������
  *            - 1, 2 �� 3 �� SPI ģʽ 
  *            - 2 or 3 �� I2S ģʽ
  * @����  Data : �����͵�����.
  * @����  û��
  */
void SPI_I2S_SendData(SPI_TypeDef* SPIx, uint16_t Data)
{
  /* ������ */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  
  /* ������д�� DR �Ĵ������� */
  SPIx->DR = Data;
}

/**
  * @����  ��������� SPIx/I2Sx �ⲿ�豸���յ�����. 
  * @����  SPIx: x ������
  *            - 1, 2 �� 3 �� SPI ģʽ 
  *            - 2 �� 3 �� I2S ģʽ
  * @����  ���յ�����.
  */
uint16_t SPI_I2S_ReceiveData(SPI_TypeDef* SPIx)
{
  /* ������ */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  
  /* ���� DR �Ĵ��������� */
  return SPIx->DR;
}

/**
  * @����  Ϊѡ���� SPI ��������ڲ� NSS �ܽ�.
  * @����  SPIx: x Ϊ1��2��3����ѡ��SPI����.
  * @����  SPI_NSSInternalSoft: ָ���� SPI NSS �ڲ�״̬.
  *          ������������������ֵ֮һ:
  *          SPI_NSSInternalSoft_Set:   ���� NSS �ڲ�����
  *          SPI_NSSInternalSoft_Reset: ��λ NSS �ڲ�����
  * @����  û��
  */
void SPI_NSSInternalSoftwareConfig(SPI_TypeDef* SPIx, uint16_t SPI_NSSInternalSoft)
{
  /* ������ */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_NSS_INTERNAL(SPI_NSSInternalSoft));
  if (SPI_NSSInternalSoft != SPI_NSSInternalSoft_Reset)
  {
    /* ������� NSS �ڲ����� */
    SPIx->CR1 |= SPI_NSSInternalSoft_Set;
  }
  else
  {
    /* �����λ NSS �ڲ����� */
    SPIx->CR1 &= SPI_NSSInternalSoft_Reset;
  }
}

/**
  * @����  ʹ�ܻ���ʧ��ָ���� SPI SS ���.
  * @����  SPIx: x Ϊ1��2��3����ѡ�� SPI ����.
  * @����  NewState: ѡ���� SPI SS �������״̬. 
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void SPI_SSOutputCmd(SPI_TypeDef* SPIx, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* ʹ��ָ���� SPI SS ��� */
    SPIx->CR2 |= CR2_SSOE_Set;
  }
  else
  {
    /* ʧ��ָ���� SPI SS ��� */
    SPIx->CR2 &= CR2_SSOE_Reset;
  }
}

/**
  * @����  ����ѡ���� SPI ���ݴ�С.
  * @����  SPIx: x Ϊ1��2��3����ѡ�� SPI ����.
  * @����  SPI_DataSize: ָ���� SPI ���ݴ�С.
  *          ������������������ֵ֮һ:
  *          SPI_DataSize_16b: �������ݴ�СΪ16λ
  *          SPI_DataSize_8b:  �������ݴ�СΪ 8λ
  * @����  û��
  */
void SPI_DataSizeConfig(SPI_TypeDef* SPIx, uint16_t SPI_DataSize)
{
  /* ������ */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_DATASIZE(SPI_DataSize));
  /* ��� DFF λ */
  SPIx->CR1 &= (uint16_t)~SPI_DataSize_16b;
  /* �����µ� DFF λ��ֵ */
  SPIx->CR1 |= SPI_DataSize;
}

/**
  * @����  ����ĳ�� SPI �� CRC У��ֵ.
  * @����  SPIx: x Ϊ1��2��3����ѡ��SPI����.
  * @����  û��
  */
void SPI_TransmitCRC(SPI_TypeDef* SPIx)
{
  /* ������ */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  
  /* ʹ��ѡ�е� SPI CRC ���� */
  SPIx->CR1 |= CR1_CRCNext_Set;
}

/**
  * @����  ʹ�ܻ���ʧ��ָ�� SPI �Ĵ����� CRC ֵ����.
  * @����  SPIx: x Ϊ1��2��3����ѡ��SPI����.
  * @����  NewState: ѡ�� SPI �ӿڵ� CRC �������״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void SPI_CalculateCRC(SPI_TypeDef* SPIx, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* ʹ��ѡ���� SPI CRC ���� */
    SPIx->CR1 |= CR1_CRCEN_Set;
  }
  else
  {
    /* ʧ��ѡ���� SPI CRC ���� */
    SPIx->CR1 &= CR1_CRCEN_Reset;
  }
}

/**
  * @����  ����ָ�� SPI ���ͻ���յ� CRC ֵ.
  * @����  SPIx: x Ϊ1��2��3����ѡ�� SPI ����.
  * @����  SPI_CRC: ������ȡ�� CRC �Ĵ���.
  *          ������������������ֵ֮һ:
  *          SPI_CRC_Tx: ѡ�� Tx CRC �Ĵ���
  *          SPI_CRC_Rx: ѡ�� Rx CRC �Ĵ���
  * @����  ѡ���� CRC �Ĵ�����ֵ.
  */
uint16_t SPI_GetCRC(SPI_TypeDef* SPIx, uint8_t SPI_CRC)
{
  uint16_t crcreg = 0;
  /* ������ */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_CRC(SPI_CRC));
  if (SPI_CRC != SPI_CRC_Rx)
  {
    /* �õ� Tx CRC �Ĵ�����ֵ */
    crcreg = SPIx->TXCRCR;
  }
  else
  {
    /* �õ� Rx CRC �Ĵ�����ֵ */
    crcreg = SPIx->RXCRCR;
  }
  /* ����ָ���� CRC �Ĵ�����ֵ */
  return crcreg;
}

/**
  * @����  ����ָ�� SPI �� CRC ����ʽ�Ĵ���ֵ.
  * @����  SPIx: x Ϊ1��2��3����ѡ��SPI����.
  * @����  ָ���� CRC ����ʽ�Ĵ���ֵ.
  */
uint16_t SPI_GetCRCPolynomial(SPI_TypeDef* SPIx)
{
  /* ������ */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  
  /* ����ָ�� CRC ����ʽ�Ĵ���ֵ */
  return SPIx->CRCPR;
}

/**
  * @����  ѡ��ָ�� SPI ��˫��ģʽ�µ����ݴ��䷽��.
  * @����  SPIx: x Ϊ1��2��3����ѡ��SPI����.
  * @����  SPI_Direction: ��˫��ģʽ�е����ݴ��䷽��. 
  *          ������������������ֵ֮һ:
  *          SPI_Direction_Tx: ѡ�� Tx ���ͷ���
  *          SPI_Direction_Rx: ѡ�� Rx ���շ���
  * @����  û��
  */
void SPI_BiDirectionalLineConfig(SPI_TypeDef* SPIx, uint16_t SPI_Direction)
{
  /* ������ */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_DIRECTION(SPI_Direction));
  if (SPI_Direction == SPI_Direction_Tx)
  {
    /* ����ֻ����ģʽ */
    SPIx->CR1 |= SPI_Direction_Tx;
  }
  else
  {
    /* ����ֻ����ģʽ */
    SPIx->CR1 &= SPI_Direction_Rx;
  }
}

/**
  * @����  ���ָ���� SPI/I2S ����Ƿ���λ.
  * @����  SPIx: x ������
  *            - 1, 2 �� 3 �� SPI ģʽ 
  *            - 2 �� 3 �� I2S ģʽ
  * @����  SPI_I2S_FLAG: ָ���� SPI/I2S ��־. 
  *          ������������������ֵ֮һ:
  *          SPI_I2S_FLAG_TXE:  ���仺��Ϊ�ձ�־.
  *          SPI_I2S_FLAG_RXNE: ���ջ��岻�ձ�־.
  *          SPI_I2S_FLAG_BSY:  æ��־.
  *          SPI_I2S_FLAG_OVR:  �����־.
  *          SPI_FLAG_MODF:     ģʽ�����־.
  *          SPI_FLAG_CRCERR:   CRC У������־.
  *          I2S_FLAG_UDR:      ��ջ�������־.
  *          I2S_FLAG_CHSIDE:   ͨ���߽��־.
  * @����  SPI_I2S_FLAG ����״̬ (SET �� RESET).
  */
FlagStatus SPI_I2S_GetFlagStatus(SPI_TypeDef* SPIx, uint16_t SPI_I2S_FLAG)
{
  FlagStatus bitstatus = RESET;
  /* ������ */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_I2S_GET_FLAG(SPI_I2S_FLAG));
  /* ���ָ���� SPI/I2S ��־״̬ */
  if ((SPIx->SR & SPI_I2S_FLAG) != (uint16_t)RESET)
  {
    /* ���� SPI_I2S_FLAG */
    bitstatus = SET;
  }
  else
  {
    /* ��λ SPI_I2S_FLAG */
    bitstatus = RESET;
  }
  /* ���� SPI_I2S_FLAG ״̬ */
  return  bitstatus;
}

/**
  * @����  ��� SPIx CRC ���� (CRCERR) ��־.
  * @����  SPIx: x ������
  *            - 1, 2 �� 3 �� SPI ģʽ 
  * @����  SPI_I2S_FLAG: ָ��Ҫ����� SPI ��־. 
  *           ������������������ CRCERR ��־.
  * @ע��
*        - OVR (���ٴ���)�жϹ���λ���Ա������˳�����: һ����  
*          SPI_DR�Ĵ������� (SPI_I2S_ReceiveData()) ����һ����  
*          SPI_SR�Ĵ������� (SPI_I2S_GetITStatus()).
*        - UDR (��ջ������) �жϹ���λ���Ա�һ����SPI_SR�Ĵ�����
*          �����(SPI_I2S_GetITStatus())��
*        - MODF (ģʽ����) �жϹ���λ�������˳�����:һ����/д 
*          SPI_SR�Ĵ�������(SPI_I2S_GetITStatus())����һ��дSPI_CR1 
*          �Ĵ������� (SPI_Cmd() ʹ�� SPI).  
  * @����  û��
  */
void SPI_I2S_ClearFlag(SPI_TypeDef* SPIx, uint16_t SPI_I2S_FLAG)
{
  /* ������ */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_I2S_CLEAR_FLAG(SPI_I2S_FLAG));
    
    /* ���ѡ��� SPI CRC ���� (CRCERR) ��־ */
    SPIx->SR = (uint16_t)~SPI_I2S_FLAG;
}

/**
  * @����  ���ָ���ĵ� SPI/I2S �ж��Ƿ�����.
  * @����  SPIx: x ������
  *            - 1, 2 �� 3 �� SPI ģʽ 
  *            - 2 �� 3 �� I2S ģʽ
  * @����  SPI_I2S_IT: ������ SPI/I2S �ж�Դ. 
  *          ������������������ֵ֮һ:
  *          SPI_I2S_IT_TXE:  ���仺��Ϊ���ж�.
  *          SPI_I2S_IT_RXNE: ���ջ��岻���ж�.
  *          SPI_I2S_IT_OVR:  ����ж�.
  *          SPI_IT_MODF:     ģʽ�����ж�.
  *          SPI_IT_CRCERR:   CRC У������ж�.
  *          I2S_IT_UDR:      ��ջ�������ж�.
  * @����  SPI_I2S_IT ����״̬ (SET �� RESET).
  */
ITStatus SPI_I2S_GetITStatus(SPI_TypeDef* SPIx, uint8_t SPI_I2S_IT)
{
  ITStatus bitstatus = RESET;
  uint16_t itpos = 0, itmask = 0, enablestatus = 0;

  /* ������ */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_I2S_GET_IT(SPI_I2S_IT));

  /* �õ� SPI/I2S �ж����� */
  itpos = 0x01 << (SPI_I2S_IT & 0x0F);

  /* �õ� SPI/I2S �жϺ� */
  itmask = SPI_I2S_IT >> 4;

  /* �����жϺ� */
  itmask = 0x01 << itmask;

  /* ȡ�� SPI_I2S_IT ʹ��״̬λ */
  enablestatus = (SPIx->CR2 & itmask) ;

  /* ���ָ���� SPI/I2S �ж�״̬ */
  if (((SPIx->SR & itpos) != (uint16_t)RESET) && enablestatus)
  {
    /* ���� SPI_I2S_IT */
    bitstatus = SET;
  }
  else
  {
    /* ��λ SPI_I2S_IT */
    bitstatus = RESET;
  }
  /* ���� SPI_I2S_IT ״̬ */
  return bitstatus;
}

/**
  * @����  ���ָ���� SPI CRC ���� (CRCERR) �жϹ���λ.
  * @����  SPIx: x ������
  *            - 1, 2 �� 3 �� SPI ģʽ 
  * @����  SPI_I2S_IT: ���ָ�����жϹ���λ.
  *           �������ֻ��� CRCERR �жϹ���λ.   
  * @note
*          - OVR (���ٴ���)�жϹ���λ���Ա������˳�����: һ����  
*            SPI_DR�Ĵ������� (SPI_I2S_ReceiveData()) ����һ����  
*            SPI_SR�Ĵ������� (SPI_I2S_GetITStatus()).
*          - UDR (��ջ������) �жϹ���λ���Ա�һ����SPI_SR�Ĵ�����
*            �����(SPI_I2S_GetITStatus())��
*          - MODF (ģʽ����) �жϹ���λ�������˳�����:һ����/д 
*            SPI_SR�Ĵ�������(SPI_I2S_GetITStatus())����һ��дSPI_CR1 
*            �Ĵ������� (SPI_Cmd() ʹ�� SPI).  
  * @����  û��
  */
void SPI_I2S_ClearITPendingBit(SPI_TypeDef* SPIx, uint8_t SPI_I2S_IT)
{
  uint16_t itpos = 0;
  /* ������ */
  assert_param(IS_SPI_ALL_PERIPH(SPIx));
  assert_param(IS_SPI_I2S_CLEAR_IT(SPI_I2S_IT));

  /* �õ� SPI �ж����� */
  itpos = 0x01 << (SPI_I2S_IT & 0x0F);

  /* ���ָ���� SPI CRC ���� (CRCERR) �жϹ���λ */
  SPIx->SR = (uint16_t)~itpos;
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
