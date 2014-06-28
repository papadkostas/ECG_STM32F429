//--------------------------------------------------------------
// File     : stm32_ub_stemwin.c
// Datum    : 18.12.2013
// Version  : 1.1
// Autor    : UB
// EMail    : mc-4u(@)t-online.de
// Web      : www.mikrocontroller-4u.de
// CPU      : STM32F429
// IDE      : CooCox CoIDE 1.7.4
// GCC      : 4.7 2012q4
// Module   : emWIN, UB_LCD_ILI_9341, UB_TOUCH_STMPE811
// Funktion : STemWin-Funktionen (von SEGGER)
//            Version 5.22 (vom 04.07.2013)
//            Quelle : www.segger.com
//
// Doku und Beispiele : www.segger.com/emwin-samples.html
//
// LIB      : das File : "lib_STemWin522_CM4_GCC.a" muss bei CoIDE
//            unter "Configuration/Link" mit dem Button "Add"
//            bei "Linked Libraries" hinzugefügt werden
//
// FPU      : Hardware FPU muss in CoIDE aktiviert werden
//
// Landscape: falls das Display im Landscape-Mode betrieben werden soll,
//            muss das Linker-Script-File "ub_linkerscript_stm32f429i_disco.ld"
//            in CoIDE eingebunden werden und im File "GUIConf.c"
//            muss der define "USE_SDRAM_AS_GUIMEM" auf "1" gesetzt werden
//
// MATH     : fuer einige emWin-Beispiele ist die "MATH.h" notwendig
//            dazu in CoIDE unter "Link/MiscControl" "-lm;" hinzufügen
//--------------------------------------------------------------


//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "stm32_ub_stemwin.h"

//--------------------------------------------------------------
uint32_t TS_Orientation;

//--------------------------------------------------------------
// init vom Display/RAM/Touch und der emWin GUI
// Return_wert :
//  -> ERROR   , Fehler beim init
//  -> SUCCESS , alles OK
//--------------------------------------------------------------
ErrorStatus UB_STemWIN_init(void)
{
  RCC_ClocksTypeDef RCC_Clocks;

  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE); // Wichtig !!

  //-----------------------------
  // init vom Systick (1ms)
  //-----------------------------
  RCC_GetClocksFreq(&RCC_Clocks);
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);


  //-----------------------------
  // init vom Display und SDRAM
  //-----------------------------
  UB_LCD_Init();
  TS_Orientation=0;

  //-----------------------------
  // init vom Touch
  //-----------------------------
  if(UB_Touch_Init()!=SUCCESS) {
    return(ERROR);
  }

  //-----------------------------
  // init der emWIN GUI
  //-----------------------------
  if(GUI_Init()!=0) {
    return(ERROR);
  }

  return(SUCCESS);
}


//--------------------------------------------------------------
// zum drehen vom Screen und Touch
// (es muss genug RAM für die GUI frei sein !!)
// Return_wert :
//  -> ERROR   , Fehler beim drehen
//  -> SUCCESS , alles OK
//--------------------------------------------------------------
ErrorStatus UB_STemWIN_GUIRotate(STEMWIN_Rotate_t mode)
{
  ErrorStatus ret_wert=SUCCESS;
  int check;

  if(mode==ROTATE_0) {
    check=GUI_SetOrientation(0);
    TS_Orientation=0;
  }else if(mode==ROTATE_90) {
    check=GUI_SetOrientation(GUI_SWAP_XY | GUI_MIRROR_X);
    TS_Orientation=1;
  }else if(mode==ROTATE_180) {
    check=GUI_SetOrientation(GUI_MIRROR_X | GUI_MIRROR_Y);
    TS_Orientation=2;
  }else {
    check=GUI_SetOrientation(GUI_SWAP_XY | GUI_MIRROR_Y);
    TS_Orientation=3;
  }

  if(check!=0) {
    ret_wert=ERROR;
    TS_Orientation=0;
  }

  return(ret_wert);
}


//--------------------------------------------------------------
// Messung vom Touch
// (wird per Systick aufgerufen)
//--------------------------------------------------------------
void UB_STemWIN_updateTouch(void)
{
  GUI_PID_STATE TS_State;
  static uint8_t prev_state = 0;

  // Touch auslesen
  UB_Touch_Read();

  if(TS_Orientation == 0) {
    TS_State.x = Touch_Data.xp;
    TS_State.y = Touch_Data.yp;
  }else if(TS_Orientation == 1) {
    TS_State.y = LCD_MAXX-1 - Touch_Data.xp;
    TS_State.x = Touch_Data.yp;
  }else if(TS_Orientation == 2) {
    TS_State.x = LCD_MAXX-1 - Touch_Data.xp;
    TS_State.y = LCD_MAXY-1 - Touch_Data.yp;
  }else {
    TS_State.y = Touch_Data.xp;
    TS_State.x = LCD_MAXY-1 - Touch_Data.yp;
  }


  TS_State.Pressed = (Touch_Data.status == TOUCH_PRESSED);

  if(prev_state != TS_State.Pressed )
  {
    prev_state = TS_State.Pressed;
    TS_State.Layer = 0;
    GUI_TOUCH_StoreStateEx(&TS_State);
  }
}


