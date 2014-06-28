#include "main.h"
#include "math.h"
int main(void){
	SystemInit();
  	UB_STemWIN_init();
  	MainTask();
	/* Program never will reach bellow this point until emWin terminates! */
 	while(1){
 	}
}
