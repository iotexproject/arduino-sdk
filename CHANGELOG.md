# Changelog

All notable changes to this project will be documented in this file. See [standard-version](https://github.com/conventional-changelog/standard-version) for commit guidelines.

### [0.2.6](https://github.com/iotexproject/arduino-sdk/compare/v0.2.5...v0.2.6) (2022-07-30)


### Bug Fixes

* replace hardcoded serial print with debug macro ([bcc77c6](https://github.com/iotexproject/arduino-sdk/commit/bcc77c6da70152577ef38f0b8351459c8c1bd8f9))

### [0.2.5](https://github.com/iotexproject/arduino-sdk/compare/v0.2.4...v0.2.5) (2022-05-30)

### [0.2.4](https://github.com/iotexproject/arduino-sdk/compare/v0.1.6...v0.2.4) (2022-05-30)


### Features

* add abi decode namespaces to arduino header ([88d592d](https://github.com/iotexproject/arduino-sdk/commit/88d592d514c14007248ddf6bf573728988d11faf))
* add address abi decoding ([738dfae](https://github.com/iotexproject/arduino-sdk/commit/738dfaedf50f3a81fd727cc1b8014e1a8fcdf9f0))
* add array abi decoding ([2b043f3](https://github.com/iotexproject/arduino-sdk/commit/2b043f3eb862e6917ec49003b405daed86f32d3a))
* add bignumber abi decoding ([59d22f1](https://github.com/iotexproject/arduino-sdk/commit/59d22f1dac3eb7686975ad82c07cb0e5ad09d3a2))
* add bool abi decoding ([20f1a58](https://github.com/iotexproject/arduino-sdk/commit/20f1a58fa1275f4104d53e9059086603fccaf30a))
* add bytes abi decoding ([5890dc0](https://github.com/iotexproject/arduino-sdk/commit/5890dc055bb2b69d6b24e065b7d0b9104ffd307c))
* add decode address tests ([a4495a9](https://github.com/iotexproject/arduino-sdk/commit/a4495a92dce1cb10cb0cbf8e3dd3e2973bbd94de))
* add integer abi decoding ([6bc3d58](https://github.com/iotexproject/arduino-sdk/commit/6bc3d5829f0d07235318b20e64f553c61b91892b))
* add readContract api endpoint ([4c26429](https://github.com/iotexproject/arduino-sdk/commit/4c26429f1265b6c6e4301c843435e8faef1be443))
* add string abi decoding ([4a571b0](https://github.com/iotexproject/arduino-sdk/commit/4a571b0333c7eddccea7ede2e7ab08c20a03a982))
* add tests for readContract ([323a361](https://github.com/iotexproject/arduino-sdk/commit/323a361f2ae45065d34407230f4dc10b17f05155))
* add uint256_t library ([1aa1af8](https://github.com/iotexproject/arduino-sdk/commit/1aa1af8271de94ba1374c9da329276601aea35c8))
* address class ([a1fd14a](https://github.com/iotexproject/arduino-sdk/commit/a1fd14ad7717866e66833ba36db4c8184aeef1ba))
* overload equality operator in bignum ([a51f358](https://github.com/iotexproject/arduino-sdk/commit/a51f358edc9006374ae3360afb1d4fb0609e9ac8))


### Bug Fixes

* address construction ([4bb6534](https://github.com/iotexproject/arduino-sdk/commit/4bb653446f90a3015a7b38cdb885cf589b1def8d))
* array initialization ([81587c7](https://github.com/iotexproject/arduino-sdk/commit/81587c7f7069ee622671ea9f9d35f9d9cea51092))
* bignum ([0854a4c](https://github.com/iotexproject/arduino-sdk/commit/0854a4c9f163526a6540c2471a8364e9f05ef7de))
* change the tests to use the test address and fake server ([0af0be6](https://github.com/iotexproject/arduino-sdk/commit/0af0be6b03fcf749a84f6fcaa67126e0f362d0c4))
* ensure api messages are initialized ([a67ff71](https://github.com/iotexproject/arduino-sdk/commit/a67ff71032eb12d0e7c60da39b8ea5ed67624f55))
* fake server grpc-status header error ([84c9ffe](https://github.com/iotexproject/arduino-sdk/commit/84c9ffea5cc0338d337c4d7cd81f2f47fd9f8a36))
* hex2str return error if invalid chars ([04911ad](https://github.com/iotexproject/arduino-sdk/commit/04911ad78d50aba1bec7b79a602caae7b4fe9ea0))
* mark input parameters as const ([629b539](https://github.com/iotexproject/arduino-sdk/commit/629b53993224ad0ad6536d4605db7620d0e90eec))
* remove external lib exceptions from Arduino builds ([ac96b99](https://github.com/iotexproject/arduino-sdk/commit/ac96b9946ef1722dfa006a3d03ca5a5c180995e9))
* tests ([1af7b05](https://github.com/iotexproject/arduino-sdk/commit/1af7b05df76e155d59cb09e2c008352eacdb3155))

### [0.2.3](https://github.com/iotexproject/arduino-sdk/compare/v0.1.6...v0.2.3) (2022-04-20)


### Features

* add abi decode namespaces to arduino header ([88d592d](https://github.com/iotexproject/arduino-sdk/commit/88d592d514c14007248ddf6bf573728988d11faf))
* add address abi decoding ([738dfae](https://github.com/iotexproject/arduino-sdk/commit/738dfaedf50f3a81fd727cc1b8014e1a8fcdf9f0))
* add array abi decoding ([2b043f3](https://github.com/iotexproject/arduino-sdk/commit/2b043f3eb862e6917ec49003b405daed86f32d3a))
* add bignumber abi decoding ([59d22f1](https://github.com/iotexproject/arduino-sdk/commit/59d22f1dac3eb7686975ad82c07cb0e5ad09d3a2))
* add bool abi decoding ([20f1a58](https://github.com/iotexproject/arduino-sdk/commit/20f1a58fa1275f4104d53e9059086603fccaf30a))
* add bytes abi decoding ([5890dc0](https://github.com/iotexproject/arduino-sdk/commit/5890dc055bb2b69d6b24e065b7d0b9104ffd307c))
* add decode address tests ([a4495a9](https://github.com/iotexproject/arduino-sdk/commit/a4495a92dce1cb10cb0cbf8e3dd3e2973bbd94de))
* add integer abi decoding ([6bc3d58](https://github.com/iotexproject/arduino-sdk/commit/6bc3d5829f0d07235318b20e64f553c61b91892b))
* add readContract api endpoint ([4c26429](https://github.com/iotexproject/arduino-sdk/commit/4c26429f1265b6c6e4301c843435e8faef1be443))
* add string abi decoding ([4a571b0](https://github.com/iotexproject/arduino-sdk/commit/4a571b0333c7eddccea7ede2e7ab08c20a03a982))
* add tests for readContract ([323a361](https://github.com/iotexproject/arduino-sdk/commit/323a361f2ae45065d34407230f4dc10b17f05155))
* add uint256_t library ([1aa1af8](https://github.com/iotexproject/arduino-sdk/commit/1aa1af8271de94ba1374c9da329276601aea35c8))
* address class ([a1fd14a](https://github.com/iotexproject/arduino-sdk/commit/a1fd14ad7717866e66833ba36db4c8184aeef1ba))
* overload equality operator in bignum ([a51f358](https://github.com/iotexproject/arduino-sdk/commit/a51f358edc9006374ae3360afb1d4fb0609e9ac8))


### Bug Fixes

* address construction ([43714c6](https://github.com/iotexproject/arduino-sdk/commit/43714c69763792f4fc476294e25ba90969f46094))
* array initialization ([81587c7](https://github.com/iotexproject/arduino-sdk/commit/81587c7f7069ee622671ea9f9d35f9d9cea51092))
* bignum ([0854a4c](https://github.com/iotexproject/arduino-sdk/commit/0854a4c9f163526a6540c2471a8364e9f05ef7de))
* change the tests to use the test address and fake server ([0af0be6](https://github.com/iotexproject/arduino-sdk/commit/0af0be6b03fcf749a84f6fcaa67126e0f362d0c4))
* ensure api messages are initialized ([a67ff71](https://github.com/iotexproject/arduino-sdk/commit/a67ff71032eb12d0e7c60da39b8ea5ed67624f55))
* fake server grpc-status header error ([84c9ffe](https://github.com/iotexproject/arduino-sdk/commit/84c9ffea5cc0338d337c4d7cd81f2f47fd9f8a36))
* hex2str return error if invalid chars ([04911ad](https://github.com/iotexproject/arduino-sdk/commit/04911ad78d50aba1bec7b79a602caae7b4fe9ea0))
* mark input parameters as const ([629b539](https://github.com/iotexproject/arduino-sdk/commit/629b53993224ad0ad6536d4605db7620d0e90eec))
* tests ([1af7b05](https://github.com/iotexproject/arduino-sdk/commit/1af7b05df76e155d59cb09e2c008352eacdb3155))

### [0.2.2](https://github.com/iotexproject/arduino-sdk/compare/v0.1.6...v0.2.2) (2022-04-17)


### Features

* add abi decode namespaces to arduino header ([abc3779](https://github.com/iotexproject/arduino-sdk/commit/abc3779004ca95f1c85ab68a81a8d291e85f2ffe))
* add address abi decoding ([dcd2339](https://github.com/iotexproject/arduino-sdk/commit/dcd2339d3877d4cbc2bf6141f038066986bdb0e5))
* add array abi decoding ([f46c9af](https://github.com/iotexproject/arduino-sdk/commit/f46c9af71bb2a05f1cb0a648ed3c8f576c3ad847))
* add bignumber abi decoding ([b90156e](https://github.com/iotexproject/arduino-sdk/commit/b90156e14c0ab09d83c68360764e402db09241f2))
* add bool abi decoding ([853484c](https://github.com/iotexproject/arduino-sdk/commit/853484c59c89a98703238be798d85a6b89a63425))
* add bytes abi decoding ([10b61f5](https://github.com/iotexproject/arduino-sdk/commit/10b61f59b37253c7d7d5881c82267301a30c1554))
* add decode address tests ([812ba9f](https://github.com/iotexproject/arduino-sdk/commit/812ba9f9cff7a32920ea3fce9cf79dbb122024a3))
* add integer abi decoding ([5f0b3bb](https://github.com/iotexproject/arduino-sdk/commit/5f0b3bba9b75b83cda58b1ba2fbd5af3698677c8))
* add readContract api endpoint ([93b38bd](https://github.com/iotexproject/arduino-sdk/commit/93b38bd5bf005cfc75045785063a71516a4a7b92))
* add string abi decoding ([ee0d445](https://github.com/iotexproject/arduino-sdk/commit/ee0d4455f45b238481661c595d7d07cd1a7cb8a0))
* add tests for readContract ([f8cbce4](https://github.com/iotexproject/arduino-sdk/commit/f8cbce4decac059b6e68cbe7acab3ccb19bd3896))
* add uint256_t library ([345d572](https://github.com/iotexproject/arduino-sdk/commit/345d5727b2706c60b155f27b2bca8aa1f92b8c89))
* address class ([714a008](https://github.com/iotexproject/arduino-sdk/commit/714a0086a46ce3a3086fb71ce6ad01d442ecb1a1))
* overload equality operator in bignum ([ed394d8](https://github.com/iotexproject/arduino-sdk/commit/ed394d8a8b3ea868269f7902f44c7412fea9f3c4))


### Bug Fixes

* array initialization ([df8af01](https://github.com/iotexproject/arduino-sdk/commit/df8af01aab0424ab5b6691decd7c47a34c95c109))
* bignum ([69fba61](https://github.com/iotexproject/arduino-sdk/commit/69fba61cb187ddd8646ea7a38b53f37dd6b39e64))
* change the tests to use the test address and fake server ([0a5059a](https://github.com/iotexproject/arduino-sdk/commit/0a5059a18ee2cccbd4e8ee197345fdee092bc75b))
* ensure api messages are initialized ([769e169](https://github.com/iotexproject/arduino-sdk/commit/769e1696caca588f024a0d61aa5a8d2e91e12578))
* fake server grpc-status header error ([97c1e93](https://github.com/iotexproject/arduino-sdk/commit/97c1e936d52f348849ac93f1ba67fd7c063f8024))
* hex2str return error if invalid chars ([6d083e3](https://github.com/iotexproject/arduino-sdk/commit/6d083e371cb06e5efd19e70912d2a42fda2988fc))
* mark input parameters as const ([aef9c9f](https://github.com/iotexproject/arduino-sdk/commit/aef9c9f276ba63a3a5eee0a3f2909fb6b833dee8))
* tests ([22006b4](https://github.com/iotexproject/arduino-sdk/commit/22006b45b00a40b56ea31a699114410d0628cd30))

### [0.2.1](https://github.com/iotexproject/arduino-sdk/compare/v0.1.6...v0.2.1) (2022-04-17)


### Features

* add abi decode namespaces to arduino header ([abc3779](https://github.com/iotexproject/arduino-sdk/commit/abc3779004ca95f1c85ab68a81a8d291e85f2ffe))
* add address abi decoding ([dcd2339](https://github.com/iotexproject/arduino-sdk/commit/dcd2339d3877d4cbc2bf6141f038066986bdb0e5))
* add array abi decoding ([f46c9af](https://github.com/iotexproject/arduino-sdk/commit/f46c9af71bb2a05f1cb0a648ed3c8f576c3ad847))
* add bignumber abi decoding ([b90156e](https://github.com/iotexproject/arduino-sdk/commit/b90156e14c0ab09d83c68360764e402db09241f2))
* add bool abi decoding ([853484c](https://github.com/iotexproject/arduino-sdk/commit/853484c59c89a98703238be798d85a6b89a63425))
* add bytes abi decoding ([10b61f5](https://github.com/iotexproject/arduino-sdk/commit/10b61f59b37253c7d7d5881c82267301a30c1554))
* add decode address tests ([812ba9f](https://github.com/iotexproject/arduino-sdk/commit/812ba9f9cff7a32920ea3fce9cf79dbb122024a3))
* add integer abi decoding ([5f0b3bb](https://github.com/iotexproject/arduino-sdk/commit/5f0b3bba9b75b83cda58b1ba2fbd5af3698677c8))
* add readContract api endpoint ([93b38bd](https://github.com/iotexproject/arduino-sdk/commit/93b38bd5bf005cfc75045785063a71516a4a7b92))
* add string abi decoding ([ee0d445](https://github.com/iotexproject/arduino-sdk/commit/ee0d4455f45b238481661c595d7d07cd1a7cb8a0))
* add tests for readContract ([f8cbce4](https://github.com/iotexproject/arduino-sdk/commit/f8cbce4decac059b6e68cbe7acab3ccb19bd3896))
* add uint256_t library ([345d572](https://github.com/iotexproject/arduino-sdk/commit/345d5727b2706c60b155f27b2bca8aa1f92b8c89))
* address class ([714a008](https://github.com/iotexproject/arduino-sdk/commit/714a0086a46ce3a3086fb71ce6ad01d442ecb1a1))
* overload equality operator in bignum ([ed394d8](https://github.com/iotexproject/arduino-sdk/commit/ed394d8a8b3ea868269f7902f44c7412fea9f3c4))


### Bug Fixes

* array initialization ([df8af01](https://github.com/iotexproject/arduino-sdk/commit/df8af01aab0424ab5b6691decd7c47a34c95c109))
* bignum ([69fba61](https://github.com/iotexproject/arduino-sdk/commit/69fba61cb187ddd8646ea7a38b53f37dd6b39e64))
* change the tests to use the test address and fake server ([0a5059a](https://github.com/iotexproject/arduino-sdk/commit/0a5059a18ee2cccbd4e8ee197345fdee092bc75b))
* ensure api messages are initialized ([769e169](https://github.com/iotexproject/arduino-sdk/commit/769e1696caca588f024a0d61aa5a8d2e91e12578))
* fake server grpc-status header error ([97c1e93](https://github.com/iotexproject/arduino-sdk/commit/97c1e936d52f348849ac93f1ba67fd7c063f8024))
* hex2str return error if invalid chars ([6d083e3](https://github.com/iotexproject/arduino-sdk/commit/6d083e371cb06e5efd19e70912d2a42fda2988fc))
* mark input parameters as const ([aef9c9f](https://github.com/iotexproject/arduino-sdk/commit/aef9c9f276ba63a3a5eee0a3f2909fb6b833dee8))
* tests ([22006b4](https://github.com/iotexproject/arduino-sdk/commit/22006b45b00a40b56ea31a699114410d0628cd30))
