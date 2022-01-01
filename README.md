# Arduino Due Static RAM Checker #

Visual Studio Code project.

This program is used to check static ram by writing and then reading from it, with minor changes that can be used to read flash memory, EPROM, or EEPROM.

The test is run three times looping writing and reading back verifying if data is the same. The test will fail if even one cell fails. After the test is completed serial will print if the test pass or fail. 
Progress and test results are monitored through native Arduino Due USB. 

