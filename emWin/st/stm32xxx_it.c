/**
  ******************************************************************************
  * @file    stm32xxx_it.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    20-September-2013
  * @brief   Exceptions Handlers
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global_includes.h"
#include "stm32_ub_stemwin.h"

//--------------------------------------------------------------
// Globale Variabeln
//--------------------------------------------------------------
extern volatile GUI_TIMER_TIME OS_TimeMS;
static volatile uint32_t touch_timer=0;


//--------------------------------------------------------------
// ISR-Funktionen
//--------------------------------------------------------------
extern void LTDC_ISR_Handler(void);
extern void DMA2D_ISR_Handler(void);

//--------------------------------------------------------------
void NMI_Handler(void)
{}

//--------------------------------------------------------------
void HardFault_Handler(void)
{
  while (1)
  {}
}

//--------------------------------------------------------------
void MemManage_Handler(void)
{
  while (1)
  {}
}

//--------------------------------------------------------------
void BusFault_Handler(void)
{
  while (1)
  {}
}

//--------------------------------------------------------------
void UsageFault_Handler(void)
{
  while (1)
  {}
}

//--------------------------------------------------------------
void DebugMon_Handler(void)
{}

//--------------------------------------------------------------
void SVC_Handler(void)
{}

//--------------------------------------------------------------
void PendSV_Handler(void)
{}

//--------------------------------------------------------------
void SysTick_Handler(void)
{

  //---------------------------
  // emWin Pausenzähler
  //---------------------------
  OS_TimeMS++;

  //---------------------------
  // Touch-Timer
  //---------------------------
  touch_timer++;
  if(touch_timer>UB_STEMWIN_TOUCH_DELAY) {
    touch_timer=0;
    UB_STemWIN_updateTouch();
  }

}

//--------------------------------------------------------------
void LTDC_IRQHandler(void)
{
  LTDC_ISR_Handler();
}

//--------------------------------------------------------------
void DMA2D_IRQHandler(void)
{
  DMA2D_ISR_Handler();
}


