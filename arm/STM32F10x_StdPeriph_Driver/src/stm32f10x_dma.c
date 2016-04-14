/**
  ******************************************************************************
  * @�ļ�    stm32f10x_dma.c
  * @����    MCD Ӧ�ó�����
  * @�汾    V3.1.0
  * @����    06/19/2009
  * @����    ����ļ� �ṩ���� DMA �̼�����.
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
#include "stm32f10x_dma.h"
#include "stm32f10x_rcc.h"

/* �������� --------------------------------------------------------------*/
/* ���ö��� --------------------------------------------------------------*/

/* DMA ENABLE mask */
#define CCR_ENABLE_Set          ((uint32_t)0x00000001)
#define CCR_ENABLE_Reset        ((uint32_t)0xFFFFFFFE)

/* DMA1 Channelx interrupt pending bit masks */
#define DMA1_Channel1_IT_Mask    ((uint32_t)0x0000000F)
#define DMA1_Channel2_IT_Mask    ((uint32_t)0x000000F0)
#define DMA1_Channel3_IT_Mask    ((uint32_t)0x00000F00)
#define DMA1_Channel4_IT_Mask    ((uint32_t)0x0000F000)
#define DMA1_Channel5_IT_Mask    ((uint32_t)0x000F0000)
#define DMA1_Channel6_IT_Mask    ((uint32_t)0x00F00000)
#define DMA1_Channel7_IT_Mask    ((uint32_t)0x0F000000)

/* DMA2 Channelx interrupt pending bit masks */
#define DMA2_Channel1_IT_Mask    ((uint32_t)0x0000000F)
#define DMA2_Channel2_IT_Mask    ((uint32_t)0x000000F0)
#define DMA2_Channel3_IT_Mask    ((uint32_t)0x00000F00)
#define DMA2_Channel4_IT_Mask    ((uint32_t)0x0000F000)
#define DMA2_Channel5_IT_Mask    ((uint32_t)0x000F0000)

/* DMA2 FLAG mask */
#define FLAG_Mask                ((uint32_t)0x10000000)

/* DMA registers Masks */
#define CCR_CLEAR_Mask           ((uint32_t)0xFFFF800F)

/* ���ú� -------------------------------------------------------------*/
/* ���ñ��� -----------------------------------------------------------*/
/* ���ú���ԭ�� -------------------------------------------------------*/
/* ���ú��� -----------------------------------------------------------*/

/**
  * @����  �� DM A��ͨ��x �Ĵ�������Ϊȱʡֵ.
  * @����  DMAy_Channelx: y ������1 ����2��DMA1��x������1 ��7 �� DMA2 ��x ������1 ��5 ��ѡ���ͨ��.
  * @����  û��
  */
void DMA_DeInit(DMA_Channel_TypeDef* DMAy_Channelx)
{
  /* ������ */
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
  /* ʧ��ѡ���� DMA y ͨ�� x */
  DMAy_Channelx->CCR &= CCR_ENABLE_Reset;
  /* ��λ DMA y ͨ�� x ���ƼĴ��� */
  DMAy_Channelx->CCR  = 0;
  
  /* ��λ DMA y ͨ�� x ʣ���ֽڼĴ��� */
  DMAy_Channelx->CNDTR = 0;
  
  /* ��λ DMA y ͨ��x ��Χ�豸��ַ�Ĵ��� */
  DMAy_Channelx->CPAR  = 0;
  
  /* ��λ DMA y ͨ�� x �ڴ��ַ�Ĵ��� */
  DMAy_Channelx->CMAR = 0;
  
  if (DMAy_Channelx == DMA1_Channel1)
  {
    /* ��λ DMA1 ͨ��1 �жϹ���λ */
    DMA1->IFCR |= DMA1_Channel1_IT_Mask;
  }
  else if (DMAy_Channelx == DMA1_Channel2)
  {
    /* ��λ DMA1 ͨ��2 �жϹ���λ */
    DMA1->IFCR |= DMA1_Channel2_IT_Mask;
  }
  else if (DMAy_Channelx == DMA1_Channel3)
  {
    /* ��λ DMA1 ͨ��3 �жϹ���λ */
    DMA1->IFCR |= DMA1_Channel3_IT_Mask;
  }
  else if (DMAy_Channelx == DMA1_Channel4)
  {
    /* ��λ DMA1 ͨ��4 �жϹ���λ */
    DMA1->IFCR |= DMA1_Channel4_IT_Mask;
  }
  else if (DMAy_Channelx == DMA1_Channel5)
  {
    /* ��λ DMA1 ͨ��5 �жϹ���λ */
    DMA1->IFCR |= DMA1_Channel5_IT_Mask;
  }
  else if (DMAy_Channelx == DMA1_Channel6)
  {
    /* ��λ DMA1 ͨ��6 �жϹ���λ */
    DMA1->IFCR |= DMA1_Channel6_IT_Mask;
  }
  else if (DMAy_Channelx == DMA1_Channel7)
  {
    /* ��λ DMA1 ͨ��7 �жϹ���λ */
    DMA1->IFCR |= DMA1_Channel7_IT_Mask;
  }
  else if (DMAy_Channelx == DMA2_Channel1)
  {
    /* ��λ DMA2 ͨ��1 �жϹ���λ */
    DMA2->IFCR |= DMA2_Channel1_IT_Mask;
  }
  else if (DMAy_Channelx == DMA2_Channel2)
  {
    /* ��λ DMA2 ͨ��2 �жϹ���λ */
    DMA2->IFCR |= DMA2_Channel2_IT_Mask;
  }
  else if (DMAy_Channelx == DMA2_Channel3)
  {
    /* ��λ DMA2 ͨ��3 �жϹ���λ */
    DMA2->IFCR |= DMA2_Channel3_IT_Mask;
  }
  else if (DMAy_Channelx == DMA2_Channel4)
  {
    /* ��λ DMA2 ͨ��4 �жϹ���λ */
    DMA2->IFCR |= DMA2_Channel4_IT_Mask;
  }
  else
  { 
    if (DMAy_Channelx == DMA2_Channel5)
    {
      /* ��λ DMA2 ͨ��5 �жϹ���λ */
      DMA2->IFCR |= DMA2_Channel5_IT_Mask;
    }
  }
}

/**
  * @����  ���� DMA_InitStruct ��ָ���Ĳ�����ʼ�� DMA ��ͨ��x �Ĵ���.
  * @����  DMAy_Channelx: y ������1 ����2 ��DMA1 ��x ������ 1�� 7 ��DMA2 ��x ������1 ��5 ��ѡ���ͨ��.
  * @����  DMA_InitStruct: ָ�� DMA_InitTypeDef �Ľṹ�壬�������ض� DMA ͨ����������Ϣ.
  * @����  û��
  */
void DMA_Init(DMA_Channel_TypeDef* DMAy_Channelx, DMA_InitTypeDef* DMA_InitStruct)
{
  uint32_t tmpreg = 0;

  /* ������ */
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
  assert_param(IS_DMA_DIR(DMA_InitStruct->DMA_DIR));
  assert_param(IS_DMA_BUFFER_SIZE(DMA_InitStruct->DMA_BufferSize));
  assert_param(IS_DMA_PERIPHERAL_INC_STATE(DMA_InitStruct->DMA_PeripheralInc));
  assert_param(IS_DMA_MEMORY_INC_STATE(DMA_InitStruct->DMA_MemoryInc));   
  assert_param(IS_DMA_PERIPHERAL_DATA_SIZE(DMA_InitStruct->DMA_PeripheralDataSize));
  assert_param(IS_DMA_MEMORY_DATA_SIZE(DMA_InitStruct->DMA_MemoryDataSize));
  assert_param(IS_DMA_MODE(DMA_InitStruct->DMA_Mode));
  assert_param(IS_DMA_PRIORITY(DMA_InitStruct->DMA_Priority));
  assert_param(IS_DMA_M2M_STATE(DMA_InitStruct->DMA_M2M));

/*--------------------------- DMAy Channelx CCR Configuration -----------------*/
  /* Get the DMAy_Channelx CCR value */
  tmpreg = DMAy_Channelx->CCR;
  /* Clear MEM2MEM, PL, MSIZE, PSIZE, MINC, PINC, CIRC and DIR bits */
  tmpreg &= CCR_CLEAR_Mask;
  /* Configure DMAy Channelx: data transfer, data size, priority level and mode */
  /* Set DIR bit according to DMA_DIR value */
  /* Set CIRC bit according to DMA_Mode value */
  /* Set PINC bit according to DMA_PeripheralInc value */
  /* Set MINC bit according to DMA_MemoryInc value */
  /* Set PSIZE bits according to DMA_PeripheralDataSize value */
  /* Set MSIZE bits according to DMA_MemoryDataSize value */
  /* Set PL bits according to DMA_Priority value */
  /* Set the MEM2MEM bit according to DMA_M2M value */
  tmpreg |= DMA_InitStruct->DMA_DIR | DMA_InitStruct->DMA_Mode |
            DMA_InitStruct->DMA_PeripheralInc | DMA_InitStruct->DMA_MemoryInc |
            DMA_InitStruct->DMA_PeripheralDataSize | DMA_InitStruct->DMA_MemoryDataSize |
            DMA_InitStruct->DMA_Priority | DMA_InitStruct->DMA_M2M;

  /* Write to DMAy Channelx CCR */
  DMAy_Channelx->CCR = tmpreg;

/*--------------------------- DMAy Channelx CNDTR Configuration ---------------*/
  /* Write to DMAy Channelx CNDTR */
  DMAy_Channelx->CNDTR = DMA_InitStruct->DMA_BufferSize;

/*--------------------------- DMAy Channelx CPAR Configuration ----------------*/
  /* Write to DMAy Channelx CPAR */
  DMAy_Channelx->CPAR = DMA_InitStruct->DMA_PeripheralBaseAddr;

/*--------------------------- DMAy Channelx CMAR Configuration ----------------*/
  /* Write to DMAy Channelx CMAR */
  DMAy_Channelx->CMAR = DMA_InitStruct->DMA_MemoryBaseAddr;
}

/**
  * @brief  Fills each DMA_InitStruct member with its default value.
  * @param  DMA_InitStruct : pointer to a DMA_InitTypeDef structure which will
  *   be initialized.
  * @retval None
  */
void DMA_StructInit(DMA_InitTypeDef* DMA_InitStruct)
{
/*-------------- Reset DMA init structure parameters values ------------------*/
  /* ��ʼ�� DMA_PeripheralBaseAddr ��Ա */
  DMA_InitStruct->DMA_PeripheralBaseAddr = 0;
  /* ��ʼ�� DMA_MemoryBaseAddr ��Ա */
  DMA_InitStruct->DMA_MemoryBaseAddr = 0;
  /* ��ʼ�� DMA_DIR ��Ա */
  DMA_InitStruct->DMA_DIR = DMA_DIR_PeripheralSRC;
  /* ��ʼ�� DMA_BufferSize ��Ա */
  DMA_InitStruct->DMA_BufferSize = 0;
  /* ��ʼ�� DMA_PeripheralInc ��Ա */
  DMA_InitStruct->DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  /* ��ʼ�� DMA_MemoryInc ��Ա */
  DMA_InitStruct->DMA_MemoryInc = DMA_MemoryInc_Disable;
  /* ��ʼ�� DMA_PeripheralDataSize ��Ա */
  DMA_InitStruct->DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  /* ��ʼ�� DMA_MemoryDataSize ��Ա */
  DMA_InitStruct->DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  /* ��ʼ�� DMA_Mode ��Ա */
  DMA_InitStruct->DMA_Mode = DMA_Mode_Normal;
  /* ��ʼ�� DMA_Priority ��Ա */
  DMA_InitStruct->DMA_Priority = DMA_Priority_Low;
  /* ��ʼ�� DMA_M2M ��Ա */
  DMA_InitStruct->DMA_M2M = DMA_M2M_Disable;
}

/**
  * @����  ʹ�ܻ�ʧ��ָ���� DMA y ͨ�� x.
  * @����  DMAy_Channelx: y ������1 ����2 ��DMA1 ��x ������1 ��7 ��DMA2 ��x ������1 ��5 ��ѡ���ͨ��.
  * @����  NewState: DMA y ͨ��x ����״̬. 
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void DMA_Cmd(DMA_Channel_TypeDef* DMAy_Channelx, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
  assert_param(IS_FUNCTIONAL_STATE(NewState));

  if (NewState != DISABLE)
  {
    /* ʹ��ָ���� DMA y ͨ�� x */
    DMAy_Channelx->CCR |= CCR_ENABLE_Set;
  }
  else
  {
    /* ʧ��ָ���� DMA y ͨ�� x */
    DMAy_Channelx->CCR &= CCR_ENABLE_Reset;
  }
}

/**
  * @����  ʹ�ܻ�ʧ��ָ���� DMA y ͨ�� x �ж�.
  * @����  DMAy_Channelx: y ������1 ����2 ��DMA1 ��x ������1 ��7 ��DMA2 ��x ������1 ��5 ��ѡ���ͨ��.
  * @����  DMA_IT: ָ�� DMA ���ж�. 
  *            �����������������ֵ���������:
  *            DMA_IT_TC:  ��������ж�����
  *            DMA_IT_HT:  ��������ж�����
  *            DMA_IT_TE:  ��������ж�����
  * @����  NewState: ָ���� DMA �жϵ���״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @����  û��
  */
void DMA_ITConfig(DMA_Channel_TypeDef* DMAy_Channelx, uint32_t DMA_IT, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
  assert_param(IS_DMA_CONFIG_IT(DMA_IT));
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if (NewState != DISABLE)
  {
    /* ʹ��ָ���� DMA �ж� */
    DMAy_Channelx->CCR |= DMA_IT;
  }
  else
  {
    /* ʧ��ָ���� DMA �ж� */
    DMAy_Channelx->CCR &= ~DMA_IT;
  }
}

/**
  * @����  ���ص�ǰ DMA y ͨ��x ʣ��Ĵ�����������Ŀ.
  * @����  DMAy_Channelx: y ������1 ����2 ��DMA1 ��x ������1 ��7 ��DMA2 ��x ������1 ��5 ��ѡ���ͨ��.
  * @����  ��ǰ DMA y ͨ��x ������ʣ������ݵ�Ԫ������.
  */
uint16_t DMA_GetCurrDataCounter(DMA_Channel_TypeDef* DMAy_Channelx)
{
  /* ������ */
  assert_param(IS_DMA_ALL_PERIPH(DMAy_Channelx));
  /* ���� DMA y ͨ��x ������ʣ������ݵ�Ԫ������ */
  return ((uint16_t)(DMAy_Channelx->CNDTR));
}

/**
  * @����  ���ָ���� DMA y ͨ�� x ��־λ�������.
  * @����  DMA_FLAG: ָ���ı�־.
  *          ������������������ֵ֮һ:
*            DMA1_FLAG_GL1: DMA1ͨ��1ȫ�ֱ�־
*            DMA1_FLAG_TC1: DMA1ͨ��1���������־
*            DMA1_FLAG_HT1: DMA1ͨ��1�봫���־
*            DMA1_FLAG_TE1: DMA1ͨ��1��������־
*            DMA1_FLAG_GL2: DMA1 ͨ��2 ȫ�ֱ�־.
*            DMA1_FLAG_TC2: DMA1 ͨ��2 ���������־.
*            DMA1_FLAG_HT2: DMA1 ͨ��2 �봫���־.
*            DMA1_FLAG_TE2: DMA1 ͨ��2 ��������־.
*            DMA1_FLAG_GL3: DMA1 ͨ��3 ȫ�ֱ�־.
*            DMA1_FLAG_TC3: DMA1 ͨ��3 ���������־.
*            DMA1_FLAG_HT3: DMA1 ͨ��3 �봫���־.
*            DMA1_FLAG_TE3: DMA1 ͨ��3 ��������־.
*            DMA1_FLAG_GL4: DMA1 ͨ��4 ȫ�ֱ�־.
*            DMA1_FLAG_TC4: DMA1 ͨ��4 ���������־.
*            DMA1_FLAG_HT4: DMA1 ͨ��4 �봫���־.
*            DMA1_FLAG_TE4: DMA1 ͨ��4 ��������־.
*            DMA1_FLAG_GL5: DMA1 ͨ��5 ȫ�ֱ�־.
*            DMA1_FLAG_TC5: DMA1 ͨ��5 ���������־.
*            DMA1_FLAG_HT5: DMA1 ͨ��5 �봫���־.
*            DMA1_FLAG_TE5: DMA1 ͨ��5 ��������־.
*            DMA1_FLAG_GL6: DMA1 ͨ��6 ȫ�ֱ�־.
*            DMA1_FLAG_TC6: DMA1 ͨ��6 ���������־.
*            DMA1_FLAG_HT6: DMA1 ͨ��6 �봫���־.
*            DMA1_FLAG_TE6: DMA1 ͨ��6 ��������־.
*            DMA1_FLAG_GL7: DMA1 ͨ��7 ȫ�ֱ�־.
*            DMA1_FLAG_TC7: DMA1 ͨ��7 ���������־.
*            DMA1_FLAG_HT7: DMA1 ͨ��7 �봫���־.
*            DMA1_FLAG_TE7: DMA1 ͨ��7 ��������־.
*            DMA2_FLAG_GL1: DMA2 ͨ��1 ȫ�ֱ�־.
*            DMA2_FLAG_TC1: DMA2 ͨ��1 ���������־.
*            DMA2_FLAG_HT1: DMA2 ͨ��1 �봫���־.
*            DMA2_FLAG_TE1: DMA2 ͨ��1 ��������־.
*            DMA2_FLAG_GL2: DMA2 ͨ��2 ȫ�ֱ�־.
*            DMA2_FLAG_TC2: DMA2 ͨ��2 ���������־.
*            DMA2_FLAG_HT2: DMA2 ͨ��2 �봫���־.
*            DMA2_FLAG_TE2: DMA2 ͨ��2 ��������־.
*            DMA2_FLAG_GL3: DMA2 ͨ��3 ȫ�ֱ�־.
*            DMA2_FLAG_TC3: DMA2 ͨ��3 ���������־.
*            DMA2_FLAG_HT3: DMA2 ͨ��3 �봫���־.
*            DMA2_FLAG_TE3: DMA2 ͨ��3 ��������־.
*            DMA2_FLAG_GL4: DMA2 ͨ��4 ȫ�ֱ�־.
*            DMA2_FLAG_TC4: DMA2 ͨ��4 ���������־.
*            DMA2_FLAG_HT4: DMA2 ͨ��4 �봫���־.
*            DMA2_FLAG_TE4: DMA2 ͨ��4 ��������־.
*            DMA2_FLAG_GL5: DMA2 ͨ��5 ȫ�ֱ�־.
*            DMA2_FLAG_TC5: DMA2 ͨ��5 ���������־.
*            DMA2_FLAG_HT5: DMA2 ͨ��5 �봫���־.
*            DMA2_FLAG_TE5: DMA2 ͨ��5 ��������־.
  * @����  DMA_FLAG ����״̬ (SET �� RESET).
  */
FlagStatus DMA_GetFlagStatus(uint32_t DMA_FLAG)
{
  FlagStatus bitstatus = RESET;
  uint32_t tmpreg = 0;
  /* ������ */
  assert_param(IS_DMA_GET_FLAG(DMA_FLAG));

  /* ����ʹ�õ� DMA */
  if ((DMA_FLAG & FLAG_Mask) != (uint32_t)RESET)
  {
    /* �õ� DMA2 ISR �Ĵ�����ֵ */
    tmpreg = DMA2->ISR ;
  }
  else
  {
    /* �õ� DMA1 ISR �Ĵ�����ֵ */
    tmpreg = DMA1->ISR ;
  }

  /* ���ָ���� DMA ��־����״̬ */
  if ((tmpreg & DMA_FLAG) != (uint32_t)RESET)
  {
    /* ���� DMA_FLAG */
    bitstatus = SET;
  }
  else
  {
    /* ��λ DMA_FLAG */
    bitstatus = RESET;
  }
  
  /* ���� DMA_FLAG ״̬ */
  return  bitstatus;
}

/**
  * @����  ������� DMA y ͨ�� x �������־λ.
  * @����  DMA_FLAG: ָ���Ĵ������־λ.
  *          �����������������ֵ��������� (����ͬһ�� DMA) :
*            DMA1_FLAG_GL1: DMA1ͨ��1ȫ�ֱ�־
*            DMA1_FLAG_TC1: DMA1ͨ��1���������־
*            DMA1_FLAG_HT1: DMA1ͨ��1�봫���־
*            DMA1_FLAG_TE1: DMA1ͨ��1��������־
*            DMA1_FLAG_GL2: DMA1 ͨ��2 ȫ�ֱ�־.
*            DMA1_FLAG_TC2: DMA1 ͨ��2 ���������־.
*            DMA1_FLAG_HT2: DMA1 ͨ��2 �봫���־.
*            DMA1_FLAG_TE2: DMA1 ͨ��2 ��������־.
*            DMA1_FLAG_GL3: DMA1 ͨ��3 ȫ�ֱ�־.
*            DMA1_FLAG_TC3: DMA1 ͨ��3 ���������־.
*            DMA1_FLAG_HT3: DMA1 ͨ��3 �봫���־.
*            DMA1_FLAG_TE3: DMA1 ͨ��3 ��������־.
*            DMA1_FLAG_GL4: DMA1 ͨ��4 ȫ�ֱ�־.
*            DMA1_FLAG_TC4: DMA1 ͨ��4 ���������־.
*            DMA1_FLAG_HT4: DMA1 ͨ��4 �봫���־.
*            DMA1_FLAG_TE4: DMA1 ͨ��4 ��������־.
*            DMA1_FLAG_GL5: DMA1 ͨ��5 ȫ�ֱ�־.
*            DMA1_FLAG_TC5: DMA1 ͨ��5 ���������־.
*            DMA1_FLAG_HT5: DMA1 ͨ��5 �봫���־.
*            DMA1_FLAG_TE5: DMA1 ͨ��5 ��������־.
*            DMA1_FLAG_GL6: DMA1 ͨ��6 ȫ�ֱ�־.
*            DMA1_FLAG_TC6: DMA1 ͨ��6 ���������־.
*            DMA1_FLAG_HT6: DMA1 ͨ��6 �봫���־.
*            DMA1_FLAG_TE6: DMA1 ͨ��6 ��������־.
*            DMA1_FLAG_GL7: DMA1 ͨ��7 ȫ�ֱ�־.
*            DMA1_FLAG_TC7: DMA1 ͨ��7 ���������־.
*            DMA1_FLAG_HT7: DMA1 ͨ��7 �봫���־.
*            DMA1_FLAG_TE7: DMA1 ͨ��7 ��������־.
*            DMA2_FLAG_GL1: DMA2 ͨ��1 ȫ�ֱ�־.
*            DMA2_FLAG_TC1: DMA2 ͨ��1 ���������־.
*            DMA2_FLAG_HT1: DMA2 ͨ��1 �봫���־.
*            DMA2_FLAG_TE1: DMA2 ͨ��1 ��������־.
*            DMA2_FLAG_GL2: DMA2 ͨ��2 ȫ�ֱ�־.
*            DMA2_FLAG_TC2: DMA2 ͨ��2 ���������־.
*            DMA2_FLAG_HT2: DMA2 ͨ��2 �봫���־.
*            DMA2_FLAG_TE2: DMA2 ͨ��2 ��������־.
*            DMA2_FLAG_GL3: DMA2 ͨ��3 ȫ�ֱ�־.
*            DMA2_FLAG_TC3: DMA2 ͨ��3 ���������־.
*            DMA2_FLAG_HT3: DMA2 ͨ��3 �봫���־.
*            DMA2_FLAG_TE3: DMA2 ͨ��3 ��������־.
*            DMA2_FLAG_GL4: DMA2 ͨ��4 ȫ�ֱ�־.
*            DMA2_FLAG_TC4: DMA2 ͨ��4 ���������־.
*            DMA2_FLAG_HT4: DMA2 ͨ��4 �봫���־.
*            DMA2_FLAG_TE4: DMA2 ͨ��4 ��������־.
*            DMA2_FLAG_GL5: DMA2 ͨ��5 ȫ�ֱ�־.
*            DMA2_FLAG_TC5: DMA2 ͨ��5 ���������־.
*            DMA2_FLAG_HT5: DMA2 ͨ��5 �봫���־.
*            DMA2_FLAG_TE5: DMA2 ͨ��5 ��������־.
  * @����  û��
  */
void DMA_ClearFlag(uint32_t DMA_FLAG)
{
  /* ������ */
  assert_param(IS_DMA_CLEAR_FLAG(DMA_FLAG));
  /* ����ʹ�õ� DMA */

  if ((DMA_FLAG & FLAG_Mask) != (uint32_t)RESET)
  {
    /* ���ѡ���� DMA ��־ */
    DMA2->IFCR = DMA_FLAG;
  }
  else
  {
    /* ���ѡ���� DMA ��־ */
    DMA1->IFCR = DMA_FLAG;
  }
}

/**
  * @����  ���ָ���� DMA y ͨ�� x �жϷ������.
  * @����  DMA_IT: ָ���� DMA �ж�Դ. 
*             ������������������ֵ֮һ:
*             DMA1_IT_GL1: DMA1 ͨ��1 ȫ���ж�.
*             DMA1_IT_TC1: DMA1 ͨ��1 ��������ж�.
*             DMA1_IT_HT1: DMA1 ͨ��1 �봫���ж�.
*             DMA1_IT_TE1: DMA1 ͨ��1 ��������ж�.
*             DMA1_IT_GL2: DMA1 ͨ��2 ȫ���ж�.
*             DMA1_IT_TC2: DMA1 ͨ��2 ��������ж�.
*             DMA1_IT_HT2: DMA1 ͨ��2 �봫���ж�.
*             DMA1_IT_TE2: DMA1 ͨ��2 ��������ж�.
*             DMA1_IT_GL3: DMA1 ͨ��3 ȫ���ж�.
*             DMA1_IT_TC3: DMA1 ͨ��3 ��������ж�.
*             DMA1_IT_HT3: DMA1 ͨ��3 �봫���ж�.
*             DMA1_IT_TE3: DMA1 ͨ��3 ��������ж�.
*             DMA1_IT_GL4: DMA1 ͨ��4 ȫ���ж�.
*             DMA1_IT_TC4: DMA1 ͨ��4 ��������ж�.
*             DMA1_IT_HT4: DMA1 ͨ��4 �봫���ж�.
*             DMA1_IT_TE4: DMA1 ͨ��4 ��������ж�.
*             DMA1_IT_GL5: DMA1 ͨ��5 ȫ���ж�.
*             DMA1_IT_TC5: DMA1 ͨ��5 ��������ж�.
*             DMA1_IT_HT5: DMA1 ͨ��5 �봫���ж�.
*             DMA1_IT_TE5: DMA1 ͨ��5 ��������ж�.
*             DMA1_IT_GL6: DMA1 ͨ��6 ȫ���ж�.
*             DMA1_IT_TC6: DMA1 ͨ��6 ��������ж�.
*             DMA1_IT_HT6: DMA1 ͨ��6 �봫���ж�.
*             DMA1_IT_TE6: DMA1 ͨ��6 ��������ж�.
*             DMA1_IT_GL7: DMA1 ͨ��7 ȫ���ж�.
*             DMA1_IT_TC7: DMA1 ͨ��7 ��������ж�.
*             DMA1_IT_HT7: DMA1 ͨ��7 �봫���ж�.
*             DMA1_IT_TE7: DMA1 ͨ��7 ��������ж�.
*             DMA2_IT_GL1: DMA2 ͨ��1 ȫ���ж�.
*             DMA2_IT_TC1: DMA2 ͨ��1 ��������ж�.
*             DMA2_IT_HT1: DMA2 ͨ��1 �봫���ж�.
*             DMA2_IT_TE1: DMA2 ͨ��1 ��������ж�.
*             DMA2_IT_GL2: DMA2 ͨ��2 ȫ���ж�.
*             DMA2_IT_TC2: DMA2 ͨ��2 ��������ж�.
*             DMA2_IT_HT2: DMA2 ͨ��2 �봫���ж�.
*             DMA2_IT_TE2: DMA2 ͨ��2 ��������ж�.
*             DMA2_IT_GL3: DMA2 ͨ��3 ȫ���ж�.
*             DMA2_IT_TC3: DMA2 ͨ��3 ��������ж�.
*             DMA2_IT_HT3: DMA2 ͨ��3 �봫���ж�.
*             DMA2_IT_TE3: DMA2 ͨ��3 ��������ж�.
*             DMA2_IT_GL4: DMA2 ͨ��4 ȫ���ж�.
*             DMA2_IT_TC4: DMA2 ͨ��4 ��������ж�.
*             DMA2_IT_HT4: DMA2 ͨ��4 �봫���ж�.
*             DMA2_IT_TE4: DMA2 ͨ��4 ��������ж�.
*             DMA2_IT_GL5: DMA2 ͨ��5 ȫ���ж�.
*             DMA2_IT_TC5: DMA2 ͨ��5 ��������ж�.
*             DMA2_IT_HT5: DMA2 ͨ��5 �봫���ж�.
*             DMA2_IT_TE5: DMA2 ͨ��5 ��������ж�.
  * @����  DMA_IT ����״̬ (SET �� RESET).
  */
ITStatus DMA_GetITStatus(uint32_t DMA_IT)
{
  ITStatus bitstatus = RESET;
  uint32_t tmpreg = 0;
  /* ������ */
  assert_param(IS_DMA_GET_IT(DMA_IT));

  /* ����ʹ�õ� DMA */
  if ((DMA_IT & FLAG_Mask) != (uint32_t)RESET)
  {
    /* �õ� DMA2 ISR �Ĵ�����ֵ */
    tmpreg = DMA2->ISR ;
  }
  else
  {
    /* �õ� DMA1 ISR �Ĵ�����ֵ */
    tmpreg = DMA1->ISR ;
  }

  /* ���ָ���� DMA �ж� */
  if ((tmpreg & DMA_IT) != (uint32_t)RESET)
  {
    /* ���� DMA_IT */
    bitstatus = SET;
  }
  else
  {
    /* ��λ DMA_IT */
    bitstatus = RESET;
  }
  /* ���� DMA_IT ״̬ */
  return  bitstatus;
}

/**
  * @����  ��� DMA y ͨ�� x �жϴ������־λ.
  * @����  DMA_IT: ָ���� DMA �жϹ���λ.
*             ������������������ֵ֮һ:
*             DMA1_IT_GL1: DMA1 ͨ��1 ȫ���ж�.
*             DMA1_IT_TC1: DMA1 ͨ��1 ��������ж�.
*             DMA1_IT_HT1: DMA1 ͨ��1 �봫���ж�.
*             DMA1_IT_TE1: DMA1 ͨ��1 ��������ж�.
*             DMA1_IT_GL2: DMA1 ͨ��2 ȫ���ж�.
*             DMA1_IT_TC2: DMA1 ͨ��2 ��������ж�.
*             DMA1_IT_HT2: DMA1 ͨ��2 �봫���ж�.
*             DMA1_IT_TE2: DMA1 ͨ��2 ��������ж�.
*             DMA1_IT_GL3: DMA1 ͨ��3 ȫ���ж�.
*             DMA1_IT_TC3: DMA1 ͨ��3 ��������ж�.
*             DMA1_IT_HT3: DMA1 ͨ��3 �봫���ж�.
*             DMA1_IT_TE3: DMA1 ͨ��3 ��������ж�.
*             DMA1_IT_GL4: DMA1 ͨ��4 ȫ���ж�.
*             DMA1_IT_TC4: DMA1 ͨ��4 ��������ж�.
*             DMA1_IT_HT4: DMA1 ͨ��4 �봫���ж�.
*             DMA1_IT_TE4: DMA1 ͨ��4 ��������ж�.
*             DMA1_IT_GL5: DMA1 ͨ��5 ȫ���ж�.
*             DMA1_IT_TC5: DMA1 ͨ��5 ��������ж�.
*             DMA1_IT_HT5: DMA1 ͨ��5 �봫���ж�.
*             DMA1_IT_TE5: DMA1 ͨ��5 ��������ж�.
*             DMA1_IT_GL6: DMA1 ͨ��6 ȫ���ж�.
*             DMA1_IT_TC6: DMA1 ͨ��6 ��������ж�.
*             DMA1_IT_HT6: DMA1 ͨ��6 �봫���ж�.
*             DMA1_IT_TE6: DMA1 ͨ��6 ��������ж�.
*             DMA1_IT_GL7: DMA1 ͨ��7 ȫ���ж�.
*             DMA1_IT_TC7: DMA1 ͨ��7 ��������ж�.
*             DMA1_IT_HT7: DMA1 ͨ��7 �봫���ж�.
*             DMA1_IT_TE7: DMA1 ͨ��7 ��������ж�.
*             DMA2_IT_GL1: DMA2 ͨ��1 ȫ���ж�.
*             DMA2_IT_TC1: DMA2 ͨ��1 ��������ж�.
*             DMA2_IT_HT1: DMA2 ͨ��1 �봫���ж�.
*             DMA2_IT_TE1: DMA2 ͨ��1 ��������ж�.
*             DMA2_IT_GL2: DMA2 ͨ��2 ȫ���ж�.
*             DMA2_IT_TC2: DMA2 ͨ��2 ��������ж�.
*             DMA2_IT_HT2: DMA2 ͨ��2 �봫���ж�.
*             DMA2_IT_TE2: DMA2 ͨ��2 ��������ж�.
*             DMA2_IT_GL3: DMA2 ͨ��3 ȫ���ж�.
*             DMA2_IT_TC3: DMA2 ͨ��3 ��������ж�.
*             DMA2_IT_HT3: DMA2 ͨ��3 �봫���ж�.
*             DMA2_IT_TE3: DMA2 ͨ��3 ��������ж�.
*             DMA2_IT_GL4: DMA2 ͨ��4 ȫ���ж�.
*             DMA2_IT_TC4: DMA2 ͨ��4 ��������ж�.
*             DMA2_IT_HT4: DMA2 ͨ��4 �봫���ж�.
*             DMA2_IT_TE4: DMA2 ͨ��4 ��������ж�.
*             DMA2_IT_GL5: DMA2 ͨ��5 ȫ���ж�.
*             DMA2_IT_TC5: DMA2 ͨ��5 ��������ж�.
*             DMA2_IT_HT5: DMA2 ͨ��5 �봫���ж�.
*             DMA2_IT_TE5: DMA2 ͨ��5 ��������ж�.
  * @����  û��
  */
void DMA_ClearITPendingBit(uint32_t DMA_IT)
{
  /* ������ */
  assert_param(IS_DMA_CLEAR_IT(DMA_IT));

  /* ����ʹ�õ� DMA */
  if ((DMA_IT & FLAG_Mask) != (uint32_t)RESET)
  {
    /* ���ѡ���DMA�жϹ���λ */
    DMA2->IFCR = DMA_IT;
  }
  else
  {
    /* ���ѡ���DMA�жϹ���λ */
    DMA1->IFCR = DMA_IT;
  }
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
