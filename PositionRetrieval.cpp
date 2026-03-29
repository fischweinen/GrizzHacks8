#include "fwwasm.h"
#include <stdint.h>

volatile uint8_t exitApp = 0;

// Simple accelerometer data processor (just X, Y, Z)
void processAccelData(uint8_t *event_data)
{
  int16_t iX, iY, iZ;

  // Convert 2 bytes each to int16_t
  iX = static_cast<int16_t>(event_data[0] | (event_data[1] << 8));
  iY = static_cast<int16_t>(event_data[2] | (event_data[3] << 8));
  iZ = static_cast<int16_t>(event_data[4] | (event_data[5] << 8));

  // Print accelerometer data
  printInt("Got accel: X:%d\n", printOutColor::printColorBlue, printOutDataType::printInt16, iX);
  printInt("Got accel: Y:%d\n", printOutColor::printColorBlue, printOutDataType::printInt16, iY);
  printInt("Got accel: Z:%d\n", printOutColor::printColorBlue, printOutDataType::printInt16, iZ);
}

// Main loop: handles events and calls processor
void loop()
{
  uint8_t event_data[FW_GET_EVENT_DATA_MAX] = {0};
  int last_event = 0;

  // Check if there is a new event
  if (hasEvent())
  {
    last_event = getEventData(event_data);
  }

  // Process accelerometer event
  if (last_event == FWGUI_EVENT_GUI_SENSOR_DATA)
  {
    processAccelData(event_data);
  }

  // Exit condition: red button pressed
  if (last_event == FWGUI_EVENT_RED_BUTTON) {
    printInt("Exit...\n", printOutColor::printColorBlack, printOutDataType::printUInt32, 0);
    exitApp = 1;
  }
}

int main()
{
  // Configure sensor
  setSensorSettings(1, 0, 1000, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0);
  printInt("\nStarting main loop...\n", printOutColor::printColorBlack, printOutDataType::printUInt32, 0);

  // Run loop until exit button is pressed
  while (!exitApp)
  {
    loop();
    waitms(100); // Give time for events to be processed
  }

  printInt("Application exited.\n", printOutColor::printColorBlack, printOutDataType::printUInt32, 0);
  return 0;
}