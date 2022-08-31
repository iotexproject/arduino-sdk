#include "account/accountManager.h"
#include "random/random.h"
#include "storage/storage.h"
#include "extern/crypto/aes/aesopt.h"

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
    availableIds.reserve(maxAccounts);
    for(uint8_t i=0; i<maxAccounts; i++)
    {
        availableIds.push_back(i);
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
            AESdecrypt decrypter;
            decrypter.key256(passwordHash);
            decrypter.decrypt(privateKeyEncrypted, privateKey);
            decrypter.decrypt(privateKeyEncrypted+16, privateKey+16);

            Account account(privateKey);
            accounts.insert(std::pair<AccountId, Account>(availableIds.back(), account));

            // Mark the id as non valid now that is used
            availableIds.pop_back();
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

AccountId AccountManager::CreateAccountFromPrivateKey(const uint8_t privateKey[IOTEX_PRIVATE_KEY_SIZE])
{
    if (accounts.size() == maxAccounts)
    {
        return -1;
    }

    Account account(privateKey);
    uint32_t id = availableIds.back();
    uint32_t eepromAddress = id*IOTEX_PRIVATE_KEY_SIZE;
    availableIds.pop_back();

    // Encrypt the private key
    int ret = aes_init();
    uint8_t encryptedPk[32] = {0};

    AESencrypt encrypter;
    encrypter.key256(passwordHash);

    // AES block size is 16 bytes. Each call to encrypt only encrypts 1 block, so we call it twice
    encrypter.encrypt(privateKey, encryptedPk);   // First block
    encrypter.encrypt(privateKey+AES_BLOCK_SIZE, encryptedPk+AES_BLOCK_SIZE); // Second block
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

const Account& AccountManager::GetAccount(AccountId id)
{
    return accounts.at(id);
}

void AccountManager::DeleteAccount(AccountId id)
{
    if (accounts.find(id) == accounts.end())
    {
        return;
    }
    accounts.erase(id);
    availableIds.push_back(id);
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
