// Support for Linux will be added in the future
#ifdef ARDUINO

#include "account/accountManager.h"
#include "random/random.h"
#include "storage/storage.h"

using namespace iotex;
using namespace std;

static const auto& logModule = logModuleNamesLookupTable[LogModules::ACCOUNT];

namespace
{
    /**
     * @brief Checks if the private key is a valid private key or empty storage.
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

AccountManager& iotex::accountManager = AccountManager::getInstance();

AccountManager::AccountManager(uint8_t maxAccounts) :
maxAccounts(maxAccounts)
{
    storage.Initialize(maxAccounts * IOTEX_PRIVATE_KEY_SIZE);
    availableIds.reserve(maxAccounts);
    for(uint8_t i=0; i<maxAccounts; i++)
    {
        availableIds.push_back(i);
    }

    // Load all existent private keys from storage.
    for(int i=0; i<maxAccounts; i++)
    {
        uint8_t privateKey[IOTEX_PRIVATE_KEY_SIZE];
        uint32_t eepromAddress = i*IOTEX_PRIVATE_KEY_SIZE;
        storage.readPrivateKey(eepromAddress, privateKey);
        if (isPrivateKeyValid(privateKey))
        {
            Account account(privateKey);
            accounts.insert(std::pair<AccountId, Account>(availableIds.back(), account));
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
    storage.savePrivateKey(eepromAddress, privateKey);
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
    uint8_t privateKeyAll1[IOTEX_PRIVATE_KEY_SIZE];
    memset(privateKeyAll1, 0xFF, IOTEX_PRIVATE_KEY_SIZE);
    storage.savePrivateKey(id*IOTEX_PRIVATE_KEY_SIZE, privateKeyAll1);
}

#endif