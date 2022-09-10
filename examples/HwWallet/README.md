# Hardware wallet example

This example demonstrates how to use the IoTeX blockchain client to create your own hardware wallet.  

## How it works

When the device boots, it will ask for a password over the serial port. This password is used to encrypt/decrypt the private keys when stored in NVM.  
Once you input the password it will display a menu on the serial port with several options to choose from.  
These include create/remove accounts, getting account balances and metadata, import/export accounts, transfer IOTX/XRC20 tokens, etc.  
See `initMenu()` for the implememnted options.  

### Security of accounts stored in NVM

Then the encrypted pk is stored stored in NVM. This will be EEPROM if using Arduino, or a file if using Linux.  
Therefore, the private key is protected agains flash read atacks. Even if someone gains access to the device, they won't be able to read the private key without the password.  

### Runtime security

Note that once the device is booted and the user introduces the correct password, both the password and the private will be stored in RAM. Any attacker with JTAG access could read them if JTAG is enabled in the system, specially you compiled your sketch in debug mode.
A better approach would be to make use of a secure element like the ATECC608A present in Arduino Nano 33 IoT. But, the moment, this is not supported by the library.  

