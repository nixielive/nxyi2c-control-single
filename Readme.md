# **NXYI2C Single tube control sample**

This sketch displays some patterns with `I2C nixie tube control module (= NXYI2C)`.

For more information about NXYI2C, please visit [here](https://www.nixielive.com).

This sketch requires a NXYI2C with I2C addresses 20 (default address).
See the following for how to write the I2C addresses to NXYI2C (if you need).

* [I2C nixietube control module (NXYI2C)](https://www.nixielive.com/docs/nxyi2c_module)
* [How to upload your sketch to NXYI2C](https://www.nixielive.com/docs/nxyi2c_upload)
* [Default firmware sketch of NXYI2C](https://github.com/nixielive/nxyi2c-default-firmware)

# **Sketch Description**
Display the numbers from 0 to 9 on a Nixie tube.
After the number 9 is displayed, the pattern will change.
If the nixie tubes having dots, the right dot will be displayed when the odd number is displayed, and the left dot will be displayed if it is the even number displayed.

## **How to send command to a NXYI2C**

NXYI2C is controlled by data with the following structure

```
typedef struct _nxyi2c_cmd
{
    unsigned char command;
    signed short value;
} NXYI2C_CMD;
```

Refer to [Default firmware sketch of NXYI2C](https://github.com/nixielive/nxyi2c-default-firmware) for the types of commands and the parameters of the commands.

In this sketch, the flow required to light up the numbers is simply described in loop().

```
// globals
unsigned short g_displayPattern;
unsigned short g_displayNumber;
NXYI2C_CMD g_cmd;

  :

void loop()
{
    :
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
    :
}

```

In this sketch, the sendCommand function is used to send commands.

```
void sendCommand(const int dest_addr, const NXYI2C_CMD cmd)
{
  Wire.beginTransmission(dest_addr);
  Wire.write((unsigned char *)(&cmd), sizeof(cmd));
  Wire.endTransmission();
}
```