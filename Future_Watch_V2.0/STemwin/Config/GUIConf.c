#include "GUI.h"

#define GUI_NUMBYTES  (80*1024) 
#define GUI_BLOCKSIZE 0x80  		//ÿ���ڴ�Ĵ�С

void GUI_X_Config(void) {
  //
  // 32 bit aligned memory area
  //
  static U32 aMemory[GUI_NUMBYTES / 4];
  //
  // Assign memory to emWin
  //
  GUI_ALLOC_AssignMemory(aMemory, GUI_NUMBYTES);
  GUI_ALLOC_SetAvBlockSize(GUI_BLOCKSIZE); //���ô洢���ƽ���ߴ�,����Խ��,���õĴ洢������Խ��
  GUI_SetDefaultFont(GUI_FONT_6X8);
}
