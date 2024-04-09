**Home garage - Home automation system for Arduino microcontrollers**

System is using PCF8575 16pins expanders with INT pin connected. The pcfs are placed between microcontroller and buttons/relays. This configuration allows for use up to 64 input/output devices per one interrupt line. With code modification up to 128. If the microcontroller supports two interrupt line then it is possible to use up to 128 input/outputs devices.



features:
- autodetection of the pressed button's address. It eases the process of configuration buttons.
- autodetection of addresses of pcf expanders connected to i2c bus.
- manual switching on/off specified relays in scanner mode.
- debug output on/off

