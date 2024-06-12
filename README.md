dsPIC33CK AK test codes
Author Zell, last edit 12.July.2024
PWM test， UART，Mathbench tests and so on...
need to add following line to the debug part:
    printf("code compiled: %s %s\n",  __DATE__, __TIME__);
