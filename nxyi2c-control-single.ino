/**
 * @file nxyi2c_host_template.ino
 * @author Takeshi Mori
 * @brief A sample of each tube given a different lighting pattern
 * 
 * @copyright Copyright (c) 2021 Takeshi Mori All Rights reserved.
 * 
 */
#include <Wire.h>
#include "from_nxyi2c_default.h"

// configuration
#define I2CADDR (20)  // 20 = default
#define PATTERN_DURATION (800)
const unsigned short TUBES_COUNT = 5;

// globals
unsigned short g_displayPattern;
unsigned short g_displayNumber;
NXYI2C_CMD g_cmd;

void sendCommand(const int dest_addr, const NXYI2C_CMD cmd)
{
  Wire.beginTransmission(dest_addr);
  Wire.write((unsigned char *)(&cmd), sizeof(cmd));
  Wire.endTransmission();
}

void setup()
{
  Wire.begin();
  g_displayPattern = nxyi2c_pattern_show;
  g_displayNumber = 0;
}

void loop()
{
  // put your main code here, to run repeatedly:

  // set pattern
  g_cmd.command = nxyi2c_set_pattern;
  g_cmd.value = g_displayPattern;
  sendCommand(I2CADDR, g_cmd);

  // set duration
  g_cmd.command = nxyi2c_set_duration;
  g_cmd.value = PATTERN_DURATION;
  sendCommand(I2CADDR, g_cmd);

  // set number
  g_cmd.command = nxyi2c_set_number;
  g_cmd.value = PATTERN_DURATION;
  sendCommand(I2CADDR, g_cmd);

  // set display number
  g_cmd.command = nxyi2c_set_number;
  g_cmd.value = g_displayNumber;
  sendCommand(I2CADDR, g_cmd);

  // set dot (if the tube having dots)
  g_cmd.command = nxyi2c_set_dot;
  g_cmd.value = g_displayNumber % 2;  // odd => nxyi2c_dots_right = 1 / even => nxyi2c_dots_left = 0
  sendCommand(I2CADDR, g_cmd);

  // start pattern
  g_cmd.command = nxyi2c_start;
  sendCommand(I2CADDR, g_cmd);

  delay(1000);

  g_displayNumber++;
  if (g_displayNumber > 9) {
    g_displayNumber = 0;  // start from 0
    g_displayPattern++;   // change pattern;
    if (g_displayPattern > nxyi2c_pattern_into_cloud) { // nxyi2c_pattern_into_cloud = 7
      g_displayPattern = nxyi2c_pattern_show; // nxyi2c_pattern_show = 1 (don't use off)
    }
  }
}
