//--------------------------------------------------------------
// File     : stm32_ub_stemwin.h
//--------------------------------------------------------------

//--------------------------------------------------------------
#ifndef __STM32F4_UB_STEMWIN_H
#define __STM32F4_UB_STEMWIN_H


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32f4xx.h"
#include "stm32f4xx_rcc.h"
#include "GUI.h"                      // die GUI.h wird IMMER benötigt
#include "stm32_ub_touch_stmpe811.h"  // für Touch-Funktionen
#include "stm32_ub_lcd_ili9341.h"     // für Display-Funktionen


//--------------------------------------------------------------
// sonstige includes von emWIN-Funktionen (je nach Bedarf)
//--------------------------------------------------------------
#include "BUTTON.h"


#define  UB_STEMWIN_TOUCH_DELAY   30   // 30ms


//--------------------------------------------------------------
typedef enum {
  ROTATE_0 = 0, // Portrait
  ROTATE_90,    // Landscape
  ROTATE_180,   // Portrait
  ROTATE_270    // Landscape
}STEMWIN_Rotate_t;


//--------------------------------------------------------------
// Globale Funktionen
//--------------------------------------------------------------
ErrorStatus UB_STemWIN_init(void);
ErrorStatus UB_STemWIN_GUIRotate(STEMWIN_Rotate_t mode);
//--------------------------------------------------------------
void UB_STemWIN_updateTouch(void);


//--------------------------------------------------------------
#endif // __STM32F4_UB_STEMWIN_H
