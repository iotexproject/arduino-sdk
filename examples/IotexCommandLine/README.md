# Iotex-arduino PlatformIO example

## Summary

Minimal example of how using the library to create a new account and query info from the blockchain

## Usage

1. Create a file named `secrets.h` inside the `src` directory
2. Add the required definitions to it. Eg:

```
#ifndef SECRETS_H
#define SECRETS_H

#define WIFI_SSID   "WifiSsid"
#define WIFI_PASS   "WifiPassword"

#endif
```
3. Build and upload using PlatformIO VSCode extension, or Platformio cli
4. Monitor the serial port. Eg using `platformio monitor`
5. The following menu is presented in the serial port once connected to Wifi:
```
Select command
1 - GetBalance
2 - GetAccountMeta
3 - GetTransactionByHash
4 - CreateAccount
5 - ReadPrivateKeyFromEeprom
6 - SignMessage
7 - SendTransfer
```
5. To select a command, type the number into the serial port. Do not input any other characters
6. Type any data required by the selected command after it is requested

**NOTES:** 
- Wait for a couple of seconds after typing text to the serial port
- Depending on the system the input will be picked up and displayed in the serial port prefixed by `Input: (length)`
- If after a couple seconds the device doesn't pick up the data from the serial port, press the `Enter` key or send a `/r` character
- Different OS might use `\n` as a line terminator. If this is the case, then replace  
  `String ret = Serial.readStringUntil('\r');` for `String ret = Serial.readStringUntil('\n');` in `commands.h`
