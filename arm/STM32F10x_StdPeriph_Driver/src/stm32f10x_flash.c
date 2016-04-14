/**
  ******************************************************************************
  * @�ļ�    stm32f10x_flash.c
  * @����    MCD Ӧ�ó�����
  * @�汾    V3.1.0
  * @����    06/19/2009
  * @����    ����ļ� �ṩ���� FLASH �̼�����.
  * @����    ANSON/sweet1985 Email:airanson110@126.com
  * @����    23/07/2009
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
#include "stm32f10x_flash.h"

/* �������� --------------------------------------------------------------*/
/* ���ö��� --------------------------------------------------------------*/ 

/* Flash Access Control Register bits */
#define ACR_LATENCY_Mask         ((uint32_t)0x00000038)
#define ACR_HLFCYA_Mask          ((uint32_t)0xFFFFFFF7)
#define ACR_PRFTBE_Mask          ((uint32_t)0xFFFFFFEF)

/* Flash Access Control Register bits */
#define ACR_PRFTBS_Mask          ((uint32_t)0x00000020) 

/* Flash Control Register bits */
#define CR_PG_Set                ((uint32_t)0x00000001)
#define CR_PG_Reset              ((uint32_t)0x00001FFE) 
#define CR_PER_Set               ((uint32_t)0x00000002)
#define CR_PER_Reset             ((uint32_t)0x00001FFD)
#define CR_MER_Set               ((uint32_t)0x00000004)
#define CR_MER_Reset             ((uint32_t)0x00001FFB)
#define CR_OPTPG_Set             ((uint32_t)0x00000010)
#define CR_OPTPG_Reset           ((uint32_t)0x00001FEF)
#define CR_OPTER_Set             ((uint32_t)0x00000020)
#define CR_OPTER_Reset           ((uint32_t)0x00001FDF)
#define CR_STRT_Set              ((uint32_t)0x00000040)
#define CR_LOCK_Set              ((uint32_t)0x00000080)

/* FLASH Mask */
#define RDPRT_Mask               ((uint32_t)0x00000002)
#define WRP0_Mask                ((uint32_t)0x000000FF)
#define WRP1_Mask                ((uint32_t)0x0000FF00)
#define WRP2_Mask                ((uint32_t)0x00FF0000)
#define WRP3_Mask                ((uint32_t)0xFF000000)

/* FLASH Keys */
#define RDP_Key                  ((uint16_t)0x00A5)
#define FLASH_KEY1               ((uint32_t)0x45670123)
#define FLASH_KEY2               ((uint32_t)0xCDEF89AB)

/* Delay definition */   
#define EraseTimeout             ((uint32_t)0x00000FFF)
#define ProgramTimeout           ((uint32_t)0x0000000F)

/* ���ú� -------------------------------------------------------------*/
/* ���ñ��� -----------------------------------------------------------*/
/* ���ú���ԭ�� -------------------------------------------------------*/
/* ���ú��� -----------------------------------------------------------*/
static void delay(void);
/**
  * @����  ���ô�����ʱֵ.
  * @����  FLASH_Latency: ָ���� FLASH ��ʱֵ.
  *          ������������������ֵ֮һ:
  *          FLASH_Latency_0: FLASH 0����ʱ����
  *          FLASH_Latency_1: FLASH 1����ʱ����
  *          FLASH_Latency_2: FLASH 2����ʱ����
  * @����  û��
  */
void FLASH_SetLatency(uint32_t FLASH_Latency)
{
  uint32_t tmpreg = 0;
  
  /* ������ */
  assert_param(IS_FLASH_LATENCY(FLASH_Latency));
  
  /* �� ACR �Ĵ��� */
  tmpreg = FLASH->ACR;  
  
  /* ������ʱֵ */
  tmpreg &= ACR_LATENCY_Mask;
  tmpreg |= FLASH_Latency;
  
  /* д ACR �Ĵ��� */
  FLASH->ACR = tmpreg;
}

/**
  * @����  ʹ�ܻ���ʧ�� FLASH �����ڷ���.
  * @����  FLASH_HalfCycleAccess: ָ���� FLASH �����ڷ���ģʽ.
  *          ������������������ֵ֮һ:
  *          FLASH_HalfCycleAccess_Enable:  �����ڷ���ʹ��
  *          FLASH_HalfCycleAccess_Disable: �����ڷ���ʧ��
  * @����  û��
  */
void FLASH_HalfCycleAccessCmd(uint32_t FLASH_HalfCycleAccess)
{
  /* ������ */
  assert_param(IS_FLASH_HALFCYCLEACCESS_STATE(FLASH_HalfCycleAccess));
  
  /* ʹ�ܻ���ʧ�ܰ����ڷ��� */
  FLASH->ACR &= ACR_HLFCYA_Mask;
  FLASH->ACR |= FLASH_HalfCycleAccess;
}

/**
  * @����  ʹ�ܻ���ʧ��Ԥȡ����.
  * @����  FLASH_PrefetchBuffer: ָ����Ԥȡ����״̬.
  *          ������������������ֵ֮һ:
  *          FLASH_PrefetchBuffer_Enable:  ʹ��Ԥȡָ����
  *          FLASH_PrefetchBuffer_Disable: ʧ��Ԥȡָ����
  * @����  û��
  */
void FLASH_PrefetchBufferCmd(uint32_t FLASH_PrefetchBuffer)
{
  /* ������ */
  assert_param(IS_FLASH_PREFETCHBUFFER_STATE(FLASH_PrefetchBuffer));
  
  /* ʹ�ܻ���ʧ��Ԥȡ���� */
  FLASH->ACR &= ACR_PRFTBE_Mask;
  FLASH->ACR |= FLASH_PrefetchBuffer;
}

/**
  * @����  ���� FLASH ��д����������.
  * @����  û��
  * @����  û��
  */
void FLASH_Unlock(void)
{
  /* ��Ȩ FPEC ��ȡ */
  FLASH->KEYR = FLASH_KEY1;
  FLASH->KEYR = FLASH_KEY2;
}

/**
  * @����  ���� FLASH ��д����������.
  * @����  û��
  * @����  û��
  */
void FLASH_Lock(void)
{
  /* ��������λ������ FPEC �� FCR */
  FLASH->CR |= CR_LOCK_Set;
}

/**
  * @����  ����һ�� FLASH ҳ��.
  * @����  Page_Address: ��Ҫ������ҳ.
  * @����  FLASH ״̬: ����ֵ������: FLASH_BUSY, FLASH_ERROR_PG,
  *                    FLASH_ERROR_WRP, FLASH_COMPLETE �� FLASH_TIMEOUT.
  */
FLASH_Status FLASH_ErasePage(uint32_t Page_Address)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* ������ */
  assert_param(IS_FLASH_ADDRESS(Page_Address));
  /* �ȴ����һ��������� */
  status = FLASH_WaitForLastOperation(EraseTimeout);
  
  if(status == FLASH_COMPLETE)
  { 
    /* ���ǰһ�������Ѿ���� */
    FLASH->CR|= CR_PER_Set;
    FLASH->AR = Page_Address; 
    FLASH->CR|= CR_STRT_Set;
    
    /* �ȴ����һ��������� */
    status = FLASH_WaitForLastOperation(EraseTimeout);
    if(status != FLASH_TIMEOUT)
    {
      /* �������������ɣ�ʧ�� PER λ */
      FLASH->CR &= CR_PER_Reset;
    }
  }
  /* ���ز���״̬ */
  return status;
}

/**
  * @����  ����ȫ�� FLASH ҳ��.
  * @����  û��
  * @����  FLASH ״̬: ����ֵ������: FLASH_ERROR_PG, FLASH_ERROR_WRP, 
  *                    FLASH_COMPLETE �� FLASH_TIMEOUT.
  */
FLASH_Status FLASH_EraseAllPages(void)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* �ȴ����һ��������� */
  status = FLASH_WaitForLastOperation(EraseTimeout);
  
  if(status == FLASH_COMPLETE)
  {
    /* ���ǰһ�������Ѿ���ɣ��������е�ҳ */
     FLASH->CR |= CR_MER_Set;
     FLASH->CR |= CR_STRT_Set;
    
    /* �ȴ����һ��������� */
    status = FLASH_WaitForLastOperation(EraseTimeout);
    if(status != FLASH_TIMEOUT)
    {
      /* �������������ɣ�ʧ�� MER λ */
      FLASH->CR &= CR_MER_Reset;
    }
  }	   
  /* ���ز���״̬ */
  return status;
}

/**
  * @����  ���� FLASH ѡ���ֽ�.
  * @����  û��
  * @����  FLASH Status: ����ֵ������: FLASH_ERROR_PG, FLASH_ERROR_WRP, 
  *                      FLASH_COMPLETE �� FLASH_TIMEOUT.
  */
FLASH_Status FLASH_EraseOptionBytes(void)
{
  FLASH_Status status = FLASH_COMPLETE;
  
  /* �ȴ����һ��������� */
  status = FLASH_WaitForLastOperation(EraseTimeout);
  if(status == FLASH_COMPLETE)
  {
    /* ��ȨС��Ϣ���� */
    FLASH->OPTKEYR = FLASH_KEY1;
    FLASH->OPTKEYR = FLASH_KEY2;
    
    /* ���ǰһ��������ɣ�����ѡ���ֽ� */
    FLASH->CR |= CR_OPTER_Set;
    FLASH->CR |= CR_STRT_Set;
    /* �ȴ����һ��������� */
    status = FLASH_WaitForLastOperation(EraseTimeout);
    
    if(status == FLASH_COMPLETE)
    {
      /* �������������ɣ�ʧ�� OPTER λ */
      FLASH->CR &= CR_OPTER_Reset;
       
      /* ʹ���ֽڱ�̲���ѡ�� */
      FLASH->CR |= CR_OPTPG_Set;
      /* ʹ�ܶ������� */
      OB->RDP= RDP_Key; 
      /* �ȴ����һ��������� */
      status = FLASH_WaitForLastOperation(ProgramTimeout);
 
      if(status != FLASH_TIMEOUT)
      {
        /* �����̲�����ɣ�ʧ�� OPTPG λ */
        FLASH->CR &= CR_OPTPG_Reset;
      }
    }
    else
    {
      if (status != FLASH_TIMEOUT)
      {
        /* ʧ�� OPTPG λ */
        FLASH->CR &= CR_OPTPG_Reset;
      }
    }  
  }
  /* ���ز���״̬ */
  return status;
}

/**
  * @����  ��ָ����ַ��дһ����.
  * @����  Address: ��Ҫ��̵ĵ�ַ.
  * @����  Data: ָ������̵�����.
  * @����  FLASH Status: ����ֵ������: FLASH_ERROR_PG, FLASH_ERROR_WRP, 
  *                      FLASH_COMPLETE �� FLASH_TIMEOUT. 
  */
FLASH_Status FLASH_ProgramWord(uint32_t Address, uint32_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  __IO uint32_t tmp = 0;

  /* ������ */
  assert_param(IS_FLASH_ADDRESS(Address));
  /* �ȴ����һ��������� */
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {
    /* ���ǰһ��������ɣ�����µĵ�һ������ */
    FLASH->CR |= CR_PG_Set;
  
    *(__IO uint16_t*)Address = (uint16_t)Data;
    /* �ȴ����һ��������� */
    status = FLASH_WaitForLastOperation(ProgramTimeout);
 
    if(status == FLASH_COMPLETE)
    {
      /* ���ǰһ��������ɣ�����µĵڶ������� */
      tmp = Address + 2;

      *(__IO uint16_t*) tmp = Data >> 16;
    
      /* �ȴ����һ��������� */
      status = FLASH_WaitForLastOperation(ProgramTimeout);
        
      if(status != FLASH_TIMEOUT)
      {
        /* ʧ�� PG λ */
        FLASH->CR &= CR_PG_Reset;
      }
    }
    else
    {
      if (status != FLASH_TIMEOUT)
      {
        /* ʧ�� PG λ */
        FLASH->CR &= CR_PG_Reset;
      }
     }
  }
  /* ���ر��״̬ */
  return status;
}

/**
  * @����  ���ض���ַ���һ������.
  * @����  Address: ����̵ĵ�ַ.
  * @����  Data: ָ������̵İ�������.
  * @����  FLASH Status: ����ֵ������: FLASH_ERROR_PG, FLASH_ERROR_WRP, 
  *                      FLASH_COMPLETE �� FLASH_TIMEOUT. 
  */
FLASH_Status FLASH_ProgramHalfWord(uint32_t Address, uint16_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* ������ */
  assert_param(IS_FLASH_ADDRESS(Address));
  /* �ȴ����һ��������� */
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {
    /* ���ǰһ��������ɣ�����µ����� */
    FLASH->CR |= CR_PG_Set;
  
    *(__IO uint16_t*)Address = Data;
    /* �ȴ����һ��������� */
    status = FLASH_WaitForLastOperation(ProgramTimeout);
    if(status != FLASH_TIMEOUT)
    {
      /* �����̲�����ɣ�ʧ�� PG λ */
      FLASH->CR &= CR_PG_Reset;
    }
  } 
  /* ���ر��״̬ */
  return status;
}

/**
  * @����  ��ָ����ѡ���ֽ����ݵ�ַ�ϱ�дһ������.
  * @����  Address: ����̵ĵ�ַ.
  *                 ������������� 0x1FFFF804 �� 0x1FFFF806. 
  * @����  Data: ����д������.
  * @����  FLASH Status: ����ֵ������: FLASH_ERROR_PG, FLASH_ERROR_WRP, 
  *                      FLASH_COMPLETE �� FLASH_TIMEOUT. 
  */
FLASH_Status FLASH_ProgramOptionByteData(uint32_t Address, uint8_t Data)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* ������ */
  assert_param(IS_OB_DATA_ADDRESS(Address));
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  if(status == FLASH_COMPLETE)
  {
    /* ��ȨС��Ϣ���� */
    FLASH->OPTKEYR = FLASH_KEY1;
    FLASH->OPTKEYR = FLASH_KEY2;
    /* ʹ���ֽڱ�̲���ѡ�� */
    FLASH->CR |= CR_OPTPG_Set; 
    *(__IO uint16_t*)Address = Data;
    
    /* �ȴ����һ��������� */
    status = FLASH_WaitForLastOperation(ProgramTimeout);
    if(status != FLASH_TIMEOUT)
    {
      /* �����̲�����ɣ�ʧ�� OPTPG λ */
      FLASH->CR &= CR_OPTPG_Reset;
    }
  }    
  /* ����ѡ���ֽ����ݵı��״̬ */
  return status;
}

/**
  * @����  ��������ҳ��д����
  * @����  FLASH_Pages: ��д����ҳ��ĵ�ַ.
  *                 �������������:
  *                 STM32_���ܶȴ�����: value between FLASH_WRProt_Pages0to3 �� FLASH_WRProt_Pages28to31  
  *                 STM32_���ܶȴ�����: value between FLASH_WRProt_Pages0to3 �� FLASH_WRProt_Pages124to127
  *                 STM32_���ܶȴ�����: value between FLASH_WRProt_Pages0to1 �� FLASH_WRProt_Pages60to61 �� FLASH_WRProt_Pages62to255
  *                 STM32_��ͨ�Դ�����: value between FLASH_WRProt_Pages0to1 �� FLASH_WRProt_Pages60to61 �� FLASH_WRProt_Pages62to127    
  *                 FLASH_WRProt_AllPages
  * @����  FLASH Status: ����ֵ������: FLASH_ERROR_PG, FLASH_ERROR_WRP, 
  *                      FLASH_COMPLETE �� FLASH_TIMEOUT.
  */
FLASH_Status FLASH_EnableWriteProtection(uint32_t FLASH_Pages)
{
  uint16_t WRP0_Data = 0xFFFF, WRP1_Data = 0xFFFF, WRP2_Data = 0xFFFF, WRP3_Data = 0xFFFF;
  
  FLASH_Status status = FLASH_COMPLETE;
  
  /* ������ */
  assert_param(IS_FLASH_WRPROT_PAGE(FLASH_Pages));
  
  FLASH_Pages = (uint32_t)(~FLASH_Pages);
  WRP0_Data = (uint16_t)(FLASH_Pages & WRP0_Mask);
  WRP1_Data = (uint16_t)((FLASH_Pages & WRP1_Mask) >> 8);
  WRP2_Data = (uint16_t)((FLASH_Pages & WRP2_Mask) >> 16);
  WRP3_Data = (uint16_t)((FLASH_Pages & WRP3_Mask) >> 24);
  
  /* �ȴ����һ��������� */
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {
    /* ��ȨС��Ϣ���� */
    FLASH->OPTKEYR = FLASH_KEY1;
    FLASH->OPTKEYR = FLASH_KEY2;
    FLASH->CR |= CR_OPTPG_Set;
    if(WRP0_Data != 0xFF)
    {
      OB->WRP0 = WRP0_Data;
      
      /* �ȴ����һ��������� */
      status = FLASH_WaitForLastOperation(ProgramTimeout);
    }
    if((status == FLASH_COMPLETE) && (WRP1_Data != 0xFF))
    {
      OB->WRP1 = WRP1_Data;
      
      /* �ȴ����һ��������� */
      status = FLASH_WaitForLastOperation(ProgramTimeout);
    }
    if((status == FLASH_COMPLETE) && (WRP2_Data != 0xFF))
    {
      OB->WRP2 = WRP2_Data;
      
      /* �ȴ����һ��������� */
      status = FLASH_WaitForLastOperation(ProgramTimeout);
    }
    
    if((status == FLASH_COMPLETE)&& (WRP3_Data != 0xFF))
    {
      OB->WRP3 = WRP3_Data;
     
      /* �ȴ����һ��������� */
      status = FLASH_WaitForLastOperation(ProgramTimeout);
    }
          
    if(status != FLASH_TIMEOUT)
    {
      /* �����̲�����ɣ�ʧ�� OPTPG λ */
      FLASH->CR &= CR_OPTPG_Reset;
    }
  } 
  /* ����д��������״̬ */
  return status;       
}

/**
  * @����  ʹ�ܻ���ʧ�ܶ�������.
  * @ע��   ����ڵ����������֮ǰʹ�����Ѿ������������ѡ���ֽڣ������
  *         �����������е�ѡ���ֽ��Ժ����������±����Щѡ���ֽ�.
  * @����  Newstate: ������������״̬.
  *                  �������������: ENABLE �� DISABLE.
  * @����  FLASH Status: ����ֵ������: FLASH_ERROR_PG, FLASH_ERROR_WRP, 
  *                      FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_ReadOutProtection(FunctionalState NewState)
{
  FLASH_Status status = FLASH_COMPLETE;
  /* ������ */
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  status = FLASH_WaitForLastOperation(EraseTimeout);
  if(status == FLASH_COMPLETE)
  {
    /* ��ȨС��Ϣ���� */
    FLASH->OPTKEYR = FLASH_KEY1;
    FLASH->OPTKEYR = FLASH_KEY2;
    FLASH->CR |= CR_OPTER_Set;
    FLASH->CR |= CR_STRT_Set;
    /* �ȴ����һ��������� */
    status = FLASH_WaitForLastOperation(EraseTimeout);
    if(status == FLASH_COMPLETE)
    {
      /* �������������ɣ�ʧ�� OPTER λ */
      FLASH->CR &= CR_OPTER_Reset;
      /* ʹ���ֽڱ�̲���ѡ�� */
      FLASH->CR |= CR_OPTPG_Set; 
      if(NewState != DISABLE)
      {
        OB->RDP = 0x00;
      }
      else
      {
        OB->RDP = RDP_Key;  
      }
      /* �ȴ����һ��������� */
      status = FLASH_WaitForLastOperation(EraseTimeout); 
    
      if(status != FLASH_TIMEOUT)
      {
        /* �����̲�����ɣ�ʧ�� OPTPG λ */
        FLASH->CR &= CR_OPTPG_Reset;
      }
    }
    else 
    {
      if(status != FLASH_TIMEOUT)
      {
        /* ʧ�� OPTER λ */
        FLASH->CR &= CR_OPTER_Reset;
      }
    }
  }
  /* ����д��������״̬ */
  return status;      
}

/**
  * @����  ��дFLASH�û�ѡ���ֽ�: IWDG_SW / RST_STOP / RST_STDBY.
  * @����  OB_IWDG: ѡ�� IWDG ģʽ
  *          ������������������ֵ֮һ:
  *          OB_IWDG_SW: ѡ������������Ź�
  *          OB_IWDG_HW: ѡ��Ӳ���������Ź�
  * @����  OB_STOP: ������ STOP ģʽ������λ�¼�.
  *          ������������������ֵ֮һ:
  *          OB_STOP_NoRST: ���� STOP ģʽ��������λ
  *          OB_STOP_RST:   ���� STOP ģʽ������λ
  * @����  OB_STDBY: ������ Standby ģʽ������λ�¼�.
  *          ������������������ֵ֮һ:
  *          OB_STDBY_NoRST: ���� Standby ģʽ��������λ
  *          OB_STDBY_RST:   ���� Standby ģʽ��������λ
  * @����  FLASH Status: ����ֵ������: FLASH_ERROR_PG, FLASH_ERROR_WRP, 
  *                      FLASH_COMPLETE or FLASH_TIMEOUT.
  */
FLASH_Status FLASH_UserOptionByteConfig(uint16_t OB_IWDG, uint16_t OB_STOP, uint16_t OB_STDBY)
{
  FLASH_Status status = FLASH_COMPLETE; 

  /* ������ */
  assert_param(IS_OB_IWDG_SOURCE(OB_IWDG));
  assert_param(IS_OB_STOP_SOURCE(OB_STOP));
  assert_param(IS_OB_STDBY_SOURCE(OB_STDBY));

  /* ��ȨС��Ϣ���� */
  FLASH->OPTKEYR = FLASH_KEY1;
  FLASH->OPTKEYR = FLASH_KEY2;
  
  /* �ȴ����һ��������� */
  status = FLASH_WaitForLastOperation(ProgramTimeout);
  
  if(status == FLASH_COMPLETE)
  {  
    /* ʹ���ֽڱ�̲���ѡ�� */
    FLASH->CR |= CR_OPTPG_Set; 
           
    OB->USER = OB_IWDG | (uint16_t)(OB_STOP | (uint16_t)(OB_STDBY | ((uint16_t)0xF8))); 
  
    /* �ȴ����һ��������� */
    status = FLASH_WaitForLastOperation(ProgramTimeout);
    if(status != FLASH_TIMEOUT)
    {
      /* �����̲�����ɣ�ʧ�� OPTPG λ */
      FLASH->CR &= CR_OPTPG_Reset;
    }
  }    
  /* ����ѡ���ֽڱ��״̬ */
  return status;
}

/**
  * @����  ���� FLASH �û�ѡ���ֽڵ�ֵ.
  * @����  û��
  * @����  FLASH �û�ѡ���ֽڵ�ֵ:IWDG_SW(Bit0), RST_STOP(Bit1) �� RST_STDBY(Bit2).
  */
uint32_t FLASH_GetUserOptionByte(void)
{
  /* �����û�ѡ���ֽ�ֵ */
  return (uint32_t)(FLASH->OBR >> 2);
}

/**
  * @����  ���� FLASH д����ѡ���ֽڼĴ�����ֵ.
  * @����  û��
  * @����  FLASH д����ѡ���ֽڼĴ���ֵ
  */
uint32_t FLASH_GetWriteProtectionOptionByte(void)
{
  /* ���� FLASH д�����Ĵ���ֵ */
  return (uint32_t)(FLASH->WRPR);
}

/**
  * @����  ��� FLASH ���������������.
  * @����  û��
  * @����  FLASH ����������״̬ (SET �� RESET)
  */
FlagStatus FLASH_GetReadOutProtectionStatus(void)
{
  FlagStatus readoutstatus = RESET;
  if ((FLASH->OBR & RDPRT_Mask) != (uint32_t)RESET)
  {
    readoutstatus = SET;
  }
  else
  {
    readoutstatus = RESET;
  }
  return readoutstatus;
}

/**
  * @����  ��� FLASH Ԥȡָ�����������.
  * @����  û��
  * @����  FLASH Ԥȡ��������״̬ (SET �� RESET).
  */
FlagStatus FLASH_GetPrefetchBufferStatus(void)
{
  FlagStatus bitstatus = RESET;
  
  if ((FLASH->ACR & ACR_PRFTBS_Mask) != (uint32_t)RESET)
  {
    bitstatus = SET;
  }
  else
  {
    bitstatus = RESET;
  }
  /* ��� FLASH Ԥȡָ����������� (SET �� RESET) */
  return bitstatus; 
}

/**
  * @����  ʹ�ܻ���ʧ��ָ�� FLASH �ж�.
  * @����  FLASH_IT: ��ʹ�ܻ���ʧ�ܵ�ָ�� FLASH �ж�Դ.
  *          �����������������ֵ���������:
  *          FLASH_IT_ERROR: FLASH �����ж�Դ
  *          FLASH_IT_EOP:   FLASH ���������ж�Դ
  * @����  NewState: ָ���� FLASH �жϵ���״̬.
  *                  �������������: ENABLE �� DISABLE.      
  * @����  û�� 
  */
void FLASH_ITConfig(uint16_t FLASH_IT, FunctionalState NewState)
{
  /* ������ */
  assert_param(IS_FLASH_IT(FLASH_IT)); 
  assert_param(IS_FUNCTIONAL_STATE(NewState));
  if(NewState != DISABLE)
  {
    /* ʹ���ж�Դ */
    FLASH->CR |= FLASH_IT;
  }
  else
  {
    /* ʧ���ж�Դ */
    FLASH->CR &= ~(uint32_t)FLASH_IT;
  }
}

/**
  * @����  ���ָ���� FLASH ��־λ�������.
  * @����  FLASH_FLAG: ��Ҫ���ı�־.
  *          ������������������ֵ֮һ:
  *          FLASH_FLAG_BSY:      FLASH æ��־           
  *          FLASH_FLAG_PGERR:    FLASH ��������־       
  *          FLASH_FLAG_WRPRTERR: FLASH ҳ��д���������־    
  *          FLASH_FLAG_EOP:      FLASH ����������־          
  *          FLASH_FLAG_OPTERR:   FLASH ѡ���ֽڴ����־   
  * @����  FLASH_FLAG ����״̬ (SET �� RESET).
  */
FlagStatus FLASH_GetFlagStatus(uint16_t FLASH_FLAG)
{
  FlagStatus bitstatus = RESET;
  /* ������ */
  assert_param(IS_FLASH_GET_FLAG(FLASH_FLAG)) ;
  if(FLASH_FLAG == FLASH_FLAG_OPTERR) 
  {
    if((FLASH->OBR & FLASH_FLAG_OPTERR) != (uint32_t)RESET)
    {
      bitstatus = SET;
    }
    else
    {
      bitstatus = RESET;
    }
  }
  else
  {
   if((FLASH->SR & FLASH_FLAG) != (uint32_t)RESET)
    {
      bitstatus = SET;
    }
    else
    {
      bitstatus = RESET;
    }
  }
  /* ���� FLASH_FLAG ����״̬ (SET or RESET) */
  return bitstatus;
}

/**
  * @����  ��� FLASH �������־λ.
  * @����  FLASH_FLAG: ��Ҫ����ı�־.
  *          �����������������ֵ���������:        
  *          FLASH_FLAG_PGERR:    FLASH ��������־       
  *          FLASH_FLAG_WRPRTERR: FLASH Wҳ��д���������־      
  *          FLASH_FLAG_EOP:      FLASH ���������ı�־          
  * @����  û��
  */
void FLASH_ClearFlag(uint16_t FLASH_FLAG)
{
  /* ������ */
  assert_param(IS_FLASH_CLEAR_FLAG(FLASH_FLAG)) ;
  
  /* �����־ */
  FLASH->SR = FLASH_FLAG;
}

/**
  * @����  ���� FLASH ״̬.
  * @����  û��
  * @����  FLASH ״̬: ����ֵ������: FLASH_BUSY, FLASH_ERROR_PG,
  *                    FLASH_ERROR_WRP �� FLASH_COMPLETE
  */
FLASH_Status FLASH_GetStatus(void)
{
  FLASH_Status flashstatus = FLASH_COMPLETE;
  
  if((FLASH->SR & FLASH_FLAG_BSY) == FLASH_FLAG_BSY) 
  {
    flashstatus = FLASH_BUSY;
  }
  else 
  {  
    if((FLASH->SR & FLASH_FLAG_PGERR) != 0)
    { 
      flashstatus = FLASH_ERROR_PG;
    }
    else 
    {
      if((FLASH->SR & FLASH_FLAG_WRPRTERR) != 0 )
      {
        flashstatus = FLASH_ERROR_WRP;
      }
      else
      {
        flashstatus = FLASH_COMPLETE;
      }
    }
  }
  /* ���� FLASH ״̬ */
  return flashstatus;
}

/**
  * @����  �ȴ�һ�� Flash ������������һ�� TIMEOUT �¼�����.
  * @����  Timeout: FLASH ��̳�ʱʱ��
  * @����  FLASH Status: ����ֵ������: FLASH_ERROR_PG, FLASH_ERROR_WRP, 
  *                      FLASH_COMPLETE �� FLASH_TIMEOUT.
  */
FLASH_Status FLASH_WaitForLastOperation(uint32_t Timeout)
{ 
  FLASH_Status status = FLASH_COMPLETE;
   
  /* ��� FLASH ״̬ */
  status = FLASH_GetStatus();
  /* �ȴ� FLASH ������ɻ��߷�����ʱ */
  while((status == FLASH_BUSY) && (Timeout != 0x00))
  {
    delay();
    status = FLASH_GetStatus();
    Timeout--;
  }
  if(Timeout == 0x00 )
  {
    status = FLASH_TIMEOUT;
  }
  /* ���ز���״̬ */
  return status;
}

/**
  * @����  ����һ����ʱ.
  * @����  û��
  * @����  û��
  */
static void delay(void)
{
  __IO uint32_t i = 0;
  for(i = 0xFF; i != 0; i--)
  {
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
