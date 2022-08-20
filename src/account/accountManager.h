#ifndef IOTEX_ACCOUNT_MANAGER_H
#define IOTEX_ACCOUNT_MANAGER_H

// Support for Linux will be added in the future
#ifdef ARDUINO

#include "account/account.h"
#include <map>

#define IOTEX_ACCOUNT_MANAGER_MAX_ACCOUNTS 10

namespace iotex
{
using AccountId = int32_t;

/**
 * @brief A singleton that abstracts account creation and private key storage
 */
class AccountManager
{
  public:
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
	const Account& GetAccount(AccountId id);

	/**
	 *  @brief Deletes the account. Also erases it from NVM.
	 */
	void DeleteAccount(AccountId id);

  private:
	AccountManager(uint8_t maxAccounts = IOTEX_ACCOUNT_MANAGER_MAX_ACCOUNTS);
	const uint8_t maxAccounts;
	std::vector<AccountId> availableIds;
	std::map<AccountId, Account> accounts;

  public:
	AccountManager(AccountManager const&) = delete;
	void operator=(AccountManager const&) = delete;
};

extern AccountManager& accountManager;
} // namespace iotex
#endif

#endif