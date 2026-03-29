#include "fwwasm.h"

#include <cstdint>
#include <string>
#include <cstdio>

#define BUT_NONE -1
#define RED_IDX 0
#define YEL_IDX 1
#define GRN_IDX 2
#define BLU_IDX 3
#define GRA_IDX 4

struct GameButton
{
    uint8_t index;
    uint8_t button_event;
};

GameButton game_buttons[4];

void flash_leds()
{
    for (auto led_index = 0; led_index < 7; ++led_index) {
      uint32_t r = wilirand();
      uint32_t g = wilirand();
      uint32_t b = wilirand();
      setBoardLED(led_index, r, g, b, 300, LEDManagerLEDMode::ledpulsefade);

      addControlText(1, (3 + led_index),120,(30 * led_index),1,1,255,255,255, "i hate my life");
      waitms(100);
    }
}

int getButtonPress()
{
    int retval = BUT_NONE;
    uint8_t event_data[FW_GET_EVENT_DATA_MAX] = { 0 };
    int last_event = getEventData(event_data);
    for (int i = 0; i < 4; i++)
    {
        if (last_event == game_buttons[i].button_event)
        {
            retval = game_buttons[i].index;
        }
    }
    if (last_event == FWGUI_EVENT_GRAY_BUTTON)
    {
        retval = GRA_IDX;
    }
    return retval;
}


int main()
{
  #define TITLE_INDEX 1
  #define SUBTITLE_INDEX 2
  addPanel(1,1,1,0,0,0,0,0,1);
  addControlText(1, TITLE_INDEX,30,0,1,1,255,255,255,"RevC");
  // addControlText(1, SUBTITLE_INDEX,30,30,1,1,255,255,255,"");

  setPanelMenuText(1,3,"flash");
  setPanelMenuText(1,4,"home");
  showPanel(1);

  game_buttons[0].button_event = FWGUI_EVENT_RED_BUTTON;
  game_buttons[0].index = RED_IDX;

  game_buttons[2].button_event = FWGUI_EVENT_GREEN_BUTTON;
  game_buttons[2].index = GRN_IDX;

  game_buttons[3].button_event = FWGUI_EVENT_BLUE_BUTTON;
  game_buttons[3].index = BLU_IDX;

  int button = BUT_NONE;

  waitms(1000);

  while (!RadioSetTx(1))
  {
    // Couldn't set Radio1 to transmit
    showDialogMsgBox("Radio Error...", 1, 0, 0, 0, 4);
    waitms(2000);
  }

  showDialogMsgBox("Radio Configured...", 1, 0, 0, 0, 4);
  waitms(2000);

  std::string radioSend = "Help";

  int16_t test = 12345;

  while (true)
  {
    if (hasEvent())
    {
      button = getButtonPress();

      if (button == BLU_IDX)
      {
        auto* bytes = reinterpret_cast<unsigned char*>(radioSend.data());
        int len = static_cast<int>(radioSend.size());

        while (!RadioWrite(1, bytes, len))
        {
            addControlText(1, SUBTITLE_INDEX,30,30,1,1,255,255,255,"Radio send failed...");
            waitms(500);
        }

        // printInt(const_cast<char*>(print.c_str()), printOutColor::printColorBlue, printOutDataType::printUInt16, test);
        // showDialogMsgBox("Radio", 1, 0, 0, 0, 10);
        addControlText(1, SUBTITLE_INDEX,30,30,1,1,255,255,255,"Sent Data...");
        flash_leds();
        addControlText(1, SUBTITLE_INDEX,30,30,1,1,255,255,255,"");
      }
    }

    waitms(10);
  }

  return 0;
}