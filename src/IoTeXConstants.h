#ifndef IOTEX_CONSTANTS_H
#define IOTEX_CONSTANTS_H

/**
 * @brief Size of the hash in bytes
 *
 */
#define IOTEX_HASH_SIZE 32

/**
 * @brief Size of a private key in bytes
 *
 */
#define IOTEX_PRIVATE_KEY_SIZE 32

/**
 * @brief Size of a public key in bytes
 *
 */
#define IOTEX_PUBLIC_KEY_SIZE 65

/**
 * @brief Size of a signature in bytes
 *
 */
#define IOTEX_SIGNATURE_SIZE 65

#define IOTEX_ADDRESS_STRLEN 41

/**
 * @brief Size of a null terminated string containing a IoTeX address. Including "io" prefix
 *
 */
#define IOTEX_ADDRESS_C_STRING_SIZE (IOTEX_ADDRESS_STRLEN + 1)

#define IOTEX_MAX_BALANCE_STRLEN 30
/**
 * @brief Maximum size of a string containing a balance in RAU
 *
 */
#define IOTEX_MAX_BALANCE_C_STRING_SIZE (IOTEX_MAX_BALANCE_STRLEN + 1)

#define ETH_ADDRESS_SIZE 20
#define ETH_PREFIX_SIZE 2
#define ETH_ADDRESS_STRLEN (ETH_ADDRESS_SIZE * 2 + ETH_PREFIX_SIZE)

/**
 * @brief Size of a null terminated string containing a Etehreum address. Including "0x" prefix
 *
 */
#define ETH_ADDRESS_C_STRING_SIZE (ETH_ADDRESS_STRLEN + 1)
#define ETH_ADDRESS_C_STRING_SIZE_NON_PREFIXED (ETH_ADDRESS_C_STRING_SIZE - ETH_PREFIX_SIZE)

#define IOTEX_HASH_STRLEN (IOTEX_HASH_SIZE * 2)

/**
 * @brief Size of a null terminated string containing a hash
 *
 */
#define IOTEX_HASH_C_STRING_SIZE (IOTEX_HASH_STRLEN + 1)

#define IOTEX_PUBLIC_KEY_STRLEN (IOTEX_PUBLIC_KEY_SIZE * 2)

/**
 * @brief Size of a null terminated string containing a public key
 *
 */
#define IOTEX_PUBLIC_KEY_C_STRING_SIZE (IOTEX_PUBLIC_KEY_STRLEN + 1)

#define IOTEX_PRIVATE_KEY_STRLEN (IOTEX_PRIVATE_KEY_SIZE * 2)

/**
 * @brief Size of a null terminated string containing a private key
 *
 */
#define IOTEX_PRIVATE_KEY_C_STRING_SIZE (IOTEX_PRIVATE_KEY_STRLEN + 1)

#define IOTEX_SIGNATURE_STRLEN (IOTEX_SIGNATURE_SIZE * 2)

/**
 * @brief Size of a null terminated string containing a signature
 *
 */
#define IOTEX_SIGNATURE_C_STRING_SIZE (IOTEX_SIGNATURE_STRLEN + 1)

/**
 * @brief Maximum size of a string containing a timestamp
 *
 */
#define IOTEX_TIMESTAMP_STRLEN 21 // TODO check if this is correct or it can be bigger

#endif
