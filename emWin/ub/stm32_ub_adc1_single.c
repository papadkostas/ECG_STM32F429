//--------------------------------------------------------------
// File     : stm32_ub_adc1_single.c
// Datum    : 30.10.2013
// Version  : 1.0
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F429
// IDE      : CooCox CoIDE 1.7.4
// GCC      : 4.7 2012q4 
// Module   : GPIO,ADC
// Funktion : AD-Wandler (ADC1 im Single-Conversion-Mode)
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_adc1_single.h"


//--------------------------------------------------------------
// Definition der benutzten ADC Pins
// Reihenfolge wie bei ADC1s_NAME_t
//--------------------------------------------------------------
ADC1s_t ADC1s[] = {
  //NAME  ,PORT , PIN      , CLOCK              , Kanal        , Mittelwerte
  {ADC_PA5,GPIOA,GPIO_Pin_5,RCC_AHB1Periph_GPIOA,ADC_Channel_5 ,MW_32},   // ADC an PA5 = ADC12_IN5
  {ADC_PC3,GPIOC,GPIO_Pin_3,RCC_AHB1Periph_GPIOC,ADC_Channel_13,MW_64},   // ADC an PC3 = ADC123_IN13
};



//--------------------------------------------------------------
// interne Funktionen
//--------------------------------------------------------------
void P_ADC1s_InitIO(void);
void P_ADC1s_InitADC(void);


//--------------------------------------------------------------
// init vom ADC1 im Single-Conversion-Mode
//--------------------------------------------------------------
void UB_ADC1_SINGLE_Init(void)
{
  P_ADC1s_InitIO();
  P_ADC1s_InitADC();
}

//--------------------------------------------------------------
// starten einer Messung
// und auslesen des Messwertes
//--------------------------------------------------------------
uint16_t UB_ADC1_SINGLE_Read(ADC1s_NAME_t adc_name)
{
  uint16_t messwert=0;

  // Messkanal einrichten
  ADC_RegularChannelConfig(ADC1,ADC1s[adc_name].ADC_CH,1,ADC_SampleTime_3Cycles);
  // Messung starten
  ADC_SoftwareStartConv(ADC1);
  // warte bis Messung fertig ist
  while(ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC)==RESET);
  // Messwert auslesen
  messwert=ADC_GetConversionValue(ADC1);

  return(messwert);
}

//--------------------------------------------------------------
// starten von n-Messungen
// und auslesen vom Mittelwert
//--------------------------------------------------------------
uint16_t UB_ADC1_SINGLE_Read_MW(ADC1s_NAME_t adc_name)
{
  uint32_t mittelwert=0;
  uint16_t messwert,n;
  uint16_t anz_mw=1,anz_bit=0;

  if(ADC1s[adc_name].ADC_MW==MW_NONE) {
    anz_mw=1;anz_bit=0;
  }
  else if(ADC1s[adc_name].ADC_MW==MW_2) {
    anz_mw=2;anz_bit=1;
  }
  else if(ADC1s[adc_name].ADC_MW==MW_4) {
    anz_mw=4;anz_bit=2;
  }
  else if(ADC1s[adc_name].ADC_MW==MW_8) {
    anz_mw=8;anz_bit=3;
  }
  else if(ADC1s[adc_name].ADC_MW==MW_16) {
    anz_mw=16;anz_bit=4;
  }
  else if(ADC1s[adc_name].ADC_MW==MW_32) {
    anz_mw=32;anz_bit=5;
  }
  else if(ADC1s[adc_name].ADC_MW==MW_64) {
    anz_mw=64;anz_bit=6;
  }
  else if(ADC1s[adc_name].ADC_MW==MW_128) {
    anz_mw=128;anz_bit=7;
  }

  for(n=0;n<anz_mw;n++) {
    messwert=UB_ADC1_SINGLE_Read(adc_name);
    mittelwert+=messwert;
  }

  messwert=(mittelwert >> anz_bit);

  return(messwert);
}


//--------------------------------------------------------------
// interne Funktion
// Init aller IO-Pins
//--------------------------------------------------------------
void P_ADC1s_InitIO(void) {
  GPIO_InitTypeDef      GPIO_InitStructure;
  ADC1s_NAME_t adc_name;

  for(adc_name=0;adc_name<ADC1s_ANZ;adc_name++) {
    // Clock Enable
    RCC_AHB1PeriphClockCmd(ADC1s[adc_name].ADC_CLK, ENABLE);

    // Config des Pins als Analog-Eingang
    GPIO_InitStructure.GPIO_Pin = ADC1s[adc_name].ADC_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
    GPIO_Init(ADC1s[adc_name].ADC_PORT, &GPIO_InitStructure);
  }
}

//--------------------------------------------------------------
// interne Funktion
// Init von ADC Nr.1
//--------------------------------------------------------------
void P_ADC1s_InitADC(void)
{
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  ADC_InitTypeDef       ADC_InitStructure;

  // Clock Enable
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

  // ADC-Config
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;
  ADC_CommonInitStructure.ADC_Prescaler = ADC1s_VORTEILER;
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
  ADC_CommonInit(&ADC_CommonInitStructure);

  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
  ADC_InitStructure.ADC_NbrOfConversion = 1;
  ADC_Init(ADC1, &ADC_InitStructure);

  // ADC-Enable
  ADC_Cmd(ADC1, ENABLE);
}


