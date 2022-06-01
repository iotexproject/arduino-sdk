# IoTeX Blockchain Client for Embedded systems
A simple IoTeX Blockchain client implementation in C++ for embedded systems. Supporting Arduino IDE and PlatformIO.   
Compatible with some popular Arduino boards and Raspberry Pi/Unix environments.  

## Supported boards
The SDK has been successfully tested on ESP32 and SAMD21 -based boards. ESP8266 will also work well for anything exchept smart contract calls: in this case we have experienced random crashes.

## Documentation / Usage

You can find the installation instructions in the [wiki](https://github.com/iotexproject/arduino-sdk/wiki/Home).  
For more detailed documentation refer to the [official documentation](https://docs.iotex.io/verifiable-data/arduino-and-more) or the [API reference](https://iotexproject.github.io/arduino-sdk).  

## Examples

The `examples` directory contains examples that demonstrate most of the library functions.
They can be used with both Arduino IDE and PlatformIO.  

## Development environment

### Docker

The Dockerfile provided sets up a Ubuntu based dev container with all required dependencies to build and test the library.  

#### Using the dev container with VSCode

Follow the below steps in order to use VSCode to develop and contribute to the library:  

1. Install the *Remote - Containers* VSCode extension.

2. Open the directory containing this repository in VSCode.

3. Press `F1` and enter `Remote-Containers: Rebuild and reopen in container`.  

4. Wait until the container is started. It should take a couple of minutes.  

#### Building using the dev container

Once the container is started and open in VSCode you should be able to run the provided tasks in `tasks.json`.  
You can get a list of tasks and run them by pressing`F1` and entering `Run task`. The main tasks are described below:  

##### Build tasks  

The following tasks can be used to build the tests. The build output will be placed in the `build` directory. The tests executable will be located at `build/tests/iotex_unit_tests`.  

- **IoTeX SDK: Build unit tests (debug):** builds the unit tests in debug mode
- **IoTeX SDK: Build unit tests (release):** builds the unit tests in release mode  

##### Test/tooling services tasks  

- **HTTP-JSONRPC server: start (mainnet)**: starts the envoy HTTP-JSONRPC proxy pointing it to mainnet. The configuration file is located in `tools/server/envoy-conf-testnet.yaml`.  
- **HTTP-JSONRPC server: start (testnet):** starts the envoy HTTP-JSONRPC proxy pointing it to testnet. The configuration file is located in `tools/server/envoy-conf-mainnet.yaml`.  
- **HTTP-JSONRPC server: stop:** starts the envoy HTTP-JSONRPC proxy.  
- **HTTP-JSONRPC server: restart (mainnet):** restarts the envoy HTTP-JSONRPC proxy pointing it to mainnet.  
- **HTTP-JSONRPC server: restart (testnet):** restarts the envoy HTTP-JSONRPC proxy pointing it to testnet.  
- **Fake HTTP-JSONRPC server: launch:** starts the fake server. This is a fake python HTTP-JSONRPC server that provides canned responses preconfigured in `tools/server-fake/config.json`. The fake server is used by the tests that.  

##### Launch configurations  

VSCode launch configurations are also set up for launching the unit tests in debug mode.  
Use the VSCode Run and Debug panel to launch them.  
Edit the `--gtest_filter` argument in `launch.json` if you want to pass a filter to the launch command. Eg: `"--gtest_filter=\"ContractTests.*\""` will only run tests from the `ContracTests` test suite.  

## Tools

The `tools` directory contains a set of tools that can be used to help with the development of the library. Some of them are described below:  

### HTTP-JSONRPC server

Located in `tools/server`.  
A HTTP-JSONRPC server using Envoy proxy. Two configurations files are provided to connect to either mainnet or testnet. It implements the following endpoints:  

- **/GetAccount**
- **/GetActions**
- **/SendAction**
- **/ReadContract**

### Fake server

Located in `tools/server-fake`.  
A fake HTTP-JSONRPC server that provides canned responses. The responses are set up in `tools/server-fake/config.json`.  

### Clang-format

The clang-format configuration file is located in `tools/.clang-format`.  
Run `./tools/clang_format.sh` from the root directory to apply clang-format.  

### Abi decoder/encoder

A JS helper script that can be used used to test/verify abi encoding. It uses the `abi-decoder` JS library.  

### Build scripts

#### build_native.sh

Builds the library and unit tests for the native (Linux) environemnt.  

#### build_arduino-cli.sh

Builds all the examples for each of the supported boards using `arduino-cli`.  

#### build_platformio_.sh

Builds all the examples for each of the supported boards using `platformio`.  

## Generating doxygen documentation  

Run `doxygen Doxyfile` to regenerate the doxygen documentation. The output will be placed in the `docs` directory.

## Generating and publishing a new release  

This library uses *Conventional Commits* and *Conventional Changelog* to automete the release and changelog generation.  
In order to generate a new release:

1. Update the version in `library.json` for PlatformIO `library.properties` for Arduino.
2. Commit both files
3. Run `npm run release`.  
4. Push to Github including tags `git push --follow-tags`.  
5. Create a tag matching the new release version in Github. The Arduino library manager should pick up the new release in about an hour or less.  
6. For PlatformIO run `pio package publish`. Credentials are needed that belong to the author of the library.  

### Troubleshooting release errors

The Arduino library manager checks the repository for new updates periodically. The logs can bee seen at <https://downloads.arduino.cc/libraries/logs/github.com/iotexproject/arduino-sdk/>.

## TODO

- [ ] Set up CI/CD (build and test in Linux, build using arduino-cli and platformio).   
- [ ] Automate release generation/publishing in CI.  
- [ ] Automate doxygen generatin in CI.  
- [ ] Support more ABI types.  
- [ ] Improve JSON parsing. Implement a genereric JSON parsing scheme that makes it easier to add new endpoints.  
