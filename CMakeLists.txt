cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project(iotex-client)

set(CMAKE_CXX_STANDARD 11)

add_library(iotex-client STATIC
    # Extern
        # JSON
        src/extern/cjson/cJSON.c
        # Base64
        src/extern/base64/base64.c
        # NanoPb
        src/extern/nanopb/pb_common.c
        src/extern/nanopb/pb_encode.c
        src/extern/nanopb/pb_decode.c
        # Crypto
        # src/extern/crypto/secp256k1.c
        src/extern/crypto/address.c     
        src/extern/crypto/base58.c
        src/extern/crypto/bignum.c
        src/extern/crypto/bip32.c
        src/extern/crypto/blake256.c
        src/extern/crypto/blake2b.c
        src/extern/crypto/curves.c
        src/extern/crypto/ecdsa.c
        src/extern/crypto/groestl.c
        src/extern/crypto/hasher.c    
        src/extern/crypto/hmac.c    
        src/extern/crypto/hmac_drbg.c    
        src/extern/crypto/memzero.c    
        src/extern/crypto/nist256p1.c    
        src/extern/crypto/rand.c    
        src/extern/crypto/rfc6979.c    
        src/extern/crypto/ripemd160.c    
        src/extern/crypto/secp256k1.c
        src/extern/crypto/sha2.c    
        src/extern/crypto/sha3.c    
        src/extern/crypto/script.c    
        src/extern/crypto/aes/aes_modes.c
        src/extern/crypto/aes/aescrypt.c
        src/extern/crypto/aes/aeskey.c
        src/extern/crypto/aes/aestab.c
        src/extern/crypto/ed25519-donna/ed25519.c
        # CppLogger
        src/extern/cpplogger/cpplogger.cpp
        # Bignum
        src/extern/uint256_t/uint256_t.cpp
        src/extern/uint256_t/uint128_t/uint128_t.cpp
   
    # Protobuf
        src/protobuf_files/c_files/action.pb.c

    # Iotex-arduino
        src/http/os/http.cpp        
        src/host/host.cpp        
        src/api/rpc.cpp
        src/api/wallet/wallets.cpp
        src/protobuf/pb_api.cpp
        src/signer/signer.cpp
        src/encoder/encoder.cpp
        src/encoder/bech32.cpp
        src/random/os/random.cpp
        src/random/platformRand32.cpp
        src/storage/os/storage.cpp
        src/account/account.cpp
        src/abi/abi.cpp
        src/abi/abiDecode.cpp
        src/helpers/json_helper.cpp
        src/helpers/client_helper.cpp
        src/bignum/bignum.cpp
        src/contract/contract.cpp
        src/contract/xrc20Contract.cpp
)

target_include_directories(iotex-client 
    # Iotex-arduino
    PUBLIC src
    
    # Others
    PRIVATE src/encoder         # For util subfolder
    PRIVATE src/protobuf_files/c_files
    PRIVATE src/extern/crypto        # Needed for sources inside crypto
    PRIVATE src/extern/nanopb
    PUBLIC src/extern/uint256_t/uint128_t/ # Needed for sources inside uint256_t
    PUBLIC src/extern
)

target_compile_definitions(iotex-client
    PUBLIC OS
)

target_link_libraries(iotex-client
    PRIVATE curl
)

# ------------------------------------------------------------------------------
# Conditiotionally build unit tests
#
# Disabled by default.
#
# Use `cmake -DUNIT_TEST=ON ..` to enable unit test build
# ------------------------------------------------------------------------------

option(UNIT_TEST "Unit tests disabled by default" ON)

if(UNIT_TEST)
    add_subdirectory(tests)
endif()

SET(LOG_LEVEL "NONE" CACHE STRING "The level for debug logs. Set to none by default")
target_compile_definitions(iotex-client
    PUBLIC IOTEX_LOG_LEVEL=IOTEX_LOG_LEVEL_${LOG_LEVEL}
)