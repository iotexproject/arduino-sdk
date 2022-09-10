#ifndef IOTEX_ACCOUNT_MANAGER_H
#define IOTEX_ACCOUNT_MANAGER_H

// Support for Linux will be added in the future
#include "account/account.h"
#include <map>
#include <string>
#include <set>

#ifndef TESTABLE
#ifdef UNIT_TEST_BUILD
#define TESTABLE public
#else
#define TESTABLE protected
#endif
#endif

#define IOTEX_ACCOUNT_MANAGER_MAX_ACCOUNTS 10
namespace iotex
{
using AccountId = int32_t;

/**
 * @brief A singleton that abstracts account creation and private key storage.
 */
class AccountManager
{
  public:
	/**
	 * @brief Sets the password for private key encryption. The same password will be used for all
	 * accounts.
	 */
	static void SetPassword(std::string password);

#ifndef ARDUINO
	/**
	 * @brief Sets the path where the private keys are stored. Ignored for Arduino as it uses
	 * EEPROM.
	 */
	static void SetPath(std::string path)
	{
		AccountManager::path = path;
	}
#endif

	/**
	 * @brief Sets the max number of accounts that can be created.
	 */
	static void SetMaxAccounts(uint8_t max)
	{
		AccountManager::maxAccounts = max;
	}

	/**
	 * @brief Get the Instance object.
	 *
	 * @param path The directory where the encrypted private keys should be stored
	 * 		The path parameter only needs passed on the first call and is only used in non Arduino
	 * builds
	 * @return AccountManager&
	 */
	static AccountManager& getInstance()
	{
		static AccountManager instance;
		return instance;
	}

	/**
	 *  @brief Creates an account from a random private key.  Also stores it in NVM.
	 */
	AccountId CreateNewAccount();

	/**
	 *  @brief Creates an account from a supplied private key. Also stores it in NVM.
	 *
	 */
	AccountId CreateAccountFromPrivateKey(const uint8_t privateKey[IOTEX_PRIVATE_KEY_SIZE]);

	/**
	 *  @brief Gets the account object
	 */
	const Account* GetAccount(AccountId id);

	/**
	 *  @brief Deletes the account. Also erases it from NVM.
	 */
	void DeleteAccount(AccountId id);

	/**
	 *  @brief Deletes all the accounts. Also erases them from NVM.
	 */
	void DeleteAllAccounts();

	/**
	 * @brief Gets all the accounts.
	 */
	std::vector<AccountId> GetAllAccountIds();

	TESTABLE : void LoadAccountsFromNvm();

  private:
	AccountManager();
	std::set<AccountId> availableIds;
	std::map<AccountId, Account> accounts;
	static uint8_t maxAccounts;
	static uint8_t passwordHash[IOTEX_HASH_SIZE];
#ifndef ARDUINO
	static std::string path;
#endif
	int encrypt(const uint8_t* in, uint8_t* out);
	int decrypt(const uint8_t* in, uint8_t* out);

  public:
	AccountManager(AccountManager const&) = delete;
	void operator=(AccountManager const&) = delete;
};
} // namespace iotex
#endif
