#include "account/accountManager.h"
#include "random/random.h"
#include "storage/storage.h"

using namespace iotex;
using namespace std;

static const auto& logModule = logModuleNamesLookupTable[LogModules::ACCOUNT];

uint8_t AccountManager::maxAccounts = 10;
uint8_t AccountManager::passwordHash[IOTEX_HASH_SIZE] = {0};
#ifndef ARDUINO
std::string AccountManager::path;
#endif

namespace
{
    /**
     * @brief Checks whether the private key stored is a valid private key or empty storage.
     * If the NVM is empty or uninitialized, the value is either 0xFF or 0x00.
     */ 
    bool isPrivateKeyValid(uint8_t pK[IOTEX_PRIVATE_KEY_SIZE])
    {
        uint8_t privateKeyAll0[IOTEX_PRIVATE_KEY_SIZE];
        uint8_t privateKeyAll1[IOTEX_PRIVATE_KEY_SIZE];
        memset(privateKeyAll0, 0x00, IOTEX_PRIVATE_KEY_SIZE);
        memset(privateKeyAll1, 0xFF, IOTEX_PRIVATE_KEY_SIZE);

        if
        (
            !memcmp(pK, privateKeyAll0, IOTEX_PRIVATE_KEY_SIZE) ||
            !memcmp(pK, privateKeyAll1, IOTEX_PRIVATE_KEY_SIZE)
        )
        {
            return false;
        }
        else return true;
    }
}

void AccountManager::SetPassword(std::string password)
{
    // Store the password hash for encryption and decryption
    signer.getHash((const uint8_t*)password.c_str(), password.length(), AccountManager::passwordHash);
}

AccountManager::AccountManager()
{
    storage.Initialize(maxAccounts * IOTEX_PRIVATE_KEY_SIZE);
    for(uint8_t i=0; i<maxAccounts; i++)
    {
        availableIds.insert(i);
    }

    LoadAccountsFromNvm();
}

void AccountManager::LoadAccountsFromNvm()
{
    // Load all existent private keys from storage.
    for(int i=0; i<maxAccounts; i++)
    {
        uint8_t privateKeyEncrypted[IOTEX_PRIVATE_KEY_SIZE];
        #ifdef ARDUINO
        uint32_t eepromAddress = i*IOTEX_PRIVATE_KEY_SIZE;
        storage.readPrivateKey(eepromAddress, privateKeyEncrypted);
        #else
        string accountFile = path + "/" + std::to_string(i);
        storage.readPrivateKey(accountFile.c_str(), privateKeyEncrypted);
        #endif
        if (isPrivateKeyValid(privateKeyEncrypted))
        {
            uint8_t privateKey[IOTEX_PRIVATE_KEY_SIZE];

            // Decrypt the private key
            decrypt(privateKeyEncrypted, privateKey);

            Account account(privateKey);
            accounts.insert(std::pair<AccountId, Account>(i, account));
            // Mark the id as non valid now that is used
            availableIds.erase(i);
        }
    }
}

AccountId AccountManager::CreateNewAccount()
{
    if (accounts.size() == maxAccounts)
    {
        return -1;
    }

    uint8_t privateKey[IOTEX_PRIVATE_KEY_SIZE];
    randomGenerator.fillRandom(privateKey, sizeof(privateKey));
    return CreateAccountFromPrivateKey(privateKey);
}

#ifdef ESP32
#include "mbedtls/aes.h"
int AccountManager::encrypt(const uint8_t* in, uint8_t* out)
{
    // ESP32 esp-idf library already uses mbedtls AES128, we cannot override it easily so we just use that
    mbedtls_aes_context aes_ctx;
    mbedtls_aes_init( &aes_ctx );
    // Passing 128 bits as key size even though the key size is 256. We simply use the first 128b.
    mbedtls_aes_setkey_enc( &aes_ctx, (const unsigned char*) passwordHash, 128);
    mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_ENCRYPT, (const unsigned char*)in, out);
    mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_ENCRYPT, (const unsigned char*)in+16, out+16);
    mbedtls_aes_free( &aes_ctx );
    return 0;
}

int AccountManager::decrypt(const uint8_t* in, uint8_t* out)
{
    // ESP32 esp-idf library already uses mbedtls AES128, we cannot override it easily so we just use that
    mbedtls_aes_context aes_ctx;
    mbedtls_aes_init( &aes_ctx );
    // Passing 128 bits as key size even though the key size is 256. We simply use the first 128b.
    mbedtls_aes_setkey_dec( &aes_ctx, (const unsigned char*) passwordHash, 128 );
    mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_DECRYPT, (const unsigned char*)in, out);
    mbedtls_aes_crypt_ecb(&aes_ctx, MBEDTLS_AES_DECRYPT, (const unsigned char*)in+16, out+16);
    mbedtls_aes_free( &aes_ctx );
    return 0;
}
#endif // #ifdef ESP32

#ifndef ESP32
#include "extern/crypto/aes/aesopt.h"
int AccountManager::encrypt(const uint8_t* in, uint8_t* out)
{    
    AESencrypt encrypter;
    int ret = encrypter.key256(passwordHash);
    if(ret != 0) return ret;
    // AES block size is 16 bytes. Each call to encrypt only encrypts 1 block, so we call it twice
    ret = encrypter.encrypt(in, out);   // First block
    if(ret != 0) return ret;
    ret = encrypter.encrypt(in+AES_BLOCK_SIZE, out+AES_BLOCK_SIZE); // Second block
    return ret;
}

int AccountManager::decrypt(const uint8_t* in, uint8_t* out)
{
    AESdecrypt decrypter;
    int ret = decrypter.key256(passwordHash);
    if(ret != 0) return ret;
    ret = decrypter.decrypt(in, out);
    if(ret != 0) return ret;
    ret = decrypter.decrypt(in+16, out+16);
    return ret;
}
#endif // #ifdef OS

AccountId AccountManager::CreateAccountFromPrivateKey(const uint8_t privateKey[IOTEX_PRIVATE_KEY_SIZE])
{
    if (accounts.size() == maxAccounts)
    {
        return -1;
    }

    Account account(privateKey);
    AccountId id = *(availableIds.begin());
    availableIds.erase(id);
    uint32_t eepromAddress = id*IOTEX_PRIVATE_KEY_SIZE;

    // Encrypt the private key
    uint8_t encryptedPk[32] = {0};
    encrypt(privateKey, encryptedPk);

    #ifndef ARDUINO
    string accountFile = path + "/" + std::to_string(id);
    storage.savePrivateKey(accountFile.c_str(), encryptedPk);
    #else
    storage.savePrivateKey(eepromAddress, encryptedPk);
    #endif

    // Finally store the account in the container
    accounts.insert(std::pair<AccountId, Account>(id, account));

    return id;
}

const Account* AccountManager::GetAccount(AccountId id)
{
    if (accounts.find(id) == accounts.end())
    {
        return nullptr;
    }
    return &(accounts.at(id));
}

void AccountManager::DeleteAccount(AccountId id)
{
    if (accounts.find(id) == accounts.end())
    {
        return;
    }
    accounts.erase(id);
    availableIds.erase(id);
    #ifdef ARDUINO
    storage.deletePrivateKey(id*IOTEX_PRIVATE_KEY_SIZE);
    #else
    string accountFile = path + "/" + std::to_string(id);
    storage.deletePrivateKey(accountFile.c_str());
    #endif
}

void AccountManager::DeleteAllAccounts()
{
    for(int i=0; i<maxAccounts; i++)
    {
        DeleteAccount(i);
    }
}

std::vector<AccountId> AccountManager::GetAllAccountIds()
{
    std::vector<AccountId> accountIds;
    for (auto& account : accounts)
    {
        accountIds.push_back(account.first);
    }
    return accountIds;
}