#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <fstream>

#include "constants.h"
#include "account/accountManager.h"
#include "extern/crypto/aes/aesopt.h"
#include "signer/signer.h"

using namespace std;
using namespace testing;
using namespace iotex;

std::string accountPath = "/tmp/accounts/";
std::string password = "Password";

class AccountManagerTests : public Test
{
	public:
		void SetUp()
		{
			AccountManager::SetPassword(password);
			AccountManager::SetPath(accountPath);
			AccountManager::SetMaxAccounts(1);
			AccountManager& uut = AccountManager::getInstance();
			uut.DeleteAllAccounts();
		}

		void WriteAccountToFile(int id, uint8_t pk[IOTEX_PRIVATE_KEY_SIZE])
		{
			FILE* pFile = fopen((accountPath + std::to_string(id)).c_str(), "wb");
			uint8_t encryptedPk[IOTEX_PRIVATE_KEY_SIZE];
			encrypt(pk, encryptedPk);
			int ret = fwrite(encryptedPk, 1, IOTEX_PRIVATE_KEY_SIZE, pFile);
			fclose(pFile);
		}

		void ReadPkFromFile(int id, uint8_t pk[IOTEX_PRIVATE_KEY_SIZE])
		{
			FILE* pFile = fopen((accountPath + std::to_string(id)).c_str(), "rb");
			uint8_t encryptedPk[IOTEX_PRIVATE_KEY_SIZE];
			fread(encryptedPk, 1, IOTEX_PRIVATE_KEY_SIZE, pFile);
			decrypt(encryptedPk, pk);
			fclose(pFile);
		}

		void encrypt(uint8_t privateKey[IOTEX_PRIVATE_KEY_SIZE], uint8_t encryptedPk[IOTEX_PRIVATE_KEY_SIZE])
		{
			uint8_t passwordHash[IOTEX_HASH_SIZE];
			signer.getHash((const uint8_t*)password.c_str(), password.length(), passwordHash);
			int ret = aes_init();
			AESencrypt encrypter;
			encrypter.key256(passwordHash);
			encrypter.encrypt(privateKey, encryptedPk);   // First block
			encrypter.encrypt(privateKey+AES_BLOCK_SIZE, encryptedPk+AES_BLOCK_SIZE); // Second block
		}

		void decrypt(uint8_t encryptedPk[IOTEX_PRIVATE_KEY_SIZE], uint8_t privateKey[IOTEX_PRIVATE_KEY_SIZE])
		{
			uint8_t passwordHash[IOTEX_HASH_SIZE];
			signer.getHash((const uint8_t*)password.c_str(), password.length(), passwordHash);
            AESdecrypt decrypter;
            decrypter.key256(passwordHash);
            decrypter.decrypt(encryptedPk, privateKey);
            decrypter.decrypt(encryptedPk+16, privateKey+16);
		}

		void printPrivateKey(uint8_t* pk)
		{
			for (int i=0; i<IOTEX_PRIVATE_KEY_SIZE; i++)
			{
				printf("%02X", pk[i]);
			}
			printf("\n");
		}
};

TEST_F(AccountManagerTests, loadAccountFromFile)
{               
	// Create the account file
	AccountManager& uut = AccountManager::getInstance();
    char pK[] = "3333333333333333333333333333333333333333333333333333333333333333";
	uint8_t pk[IOTEX_PRIVATE_KEY_SIZE] = {0};
	signer.str2hex(pK, pk, IOTEX_PRIVATE_KEY_SIZE);
	WriteAccountToFile(0, pk);

    uut.LoadAccountsFromNvm();
	uint8_t retrievedPk[IOTEX_PRIVATE_KEY_SIZE] = {0};
	uut.GetAccount(0).getPrivateKey(retrievedPk);

	// printPrivateKey(retrievedPk);
	ASSERT_EQ(0, memcmp(pk, retrievedPk, IOTEX_PRIVATE_KEY_SIZE));
}

TEST_F(AccountManagerTests, saveAccountToFile)
{
	AccountManager& uut = AccountManager::getInstance();
	uut.DeleteAccount(0);
	char pK[] = "3333333333333333333333333333333333333333333333333333333333333333";
	uint8_t pk[IOTEX_PRIVATE_KEY_SIZE] = {0};
	signer.str2hex(pK, pk, IOTEX_PRIVATE_KEY_SIZE);
	uut.CreateAccountFromPrivateKey(pk);

	uint8_t retrievedPk[IOTEX_PRIVATE_KEY_SIZE] = {0};
	ReadPkFromFile(0, retrievedPk);
	ASSERT_EQ(0, memcmp(pk, retrievedPk, IOTEX_PRIVATE_KEY_SIZE));
}

TEST_F(AccountManagerTests, deleteAccount)
{
	AccountManager& uut = AccountManager::getInstance();
    char pK[] = "3333333333333333333333333333333333333333333333333333333333333333";
	uint8_t pk[IOTEX_PRIVATE_KEY_SIZE] = {0};
	signer.str2hex(pK, pk, IOTEX_PRIVATE_KEY_SIZE);
	uut.CreateAccountFromPrivateKey(pk);
	// Check the file is created
	ifstream f((accountPath + std::to_string(0)).c_str());
    ASSERT_FALSE(f.fail());
	f.close();

	// Delete the account and check it deletes the file
	uut.DeleteAccount(0);
	ifstream f2((accountPath + std::to_string(0)).c_str());
	ASSERT_TRUE(f2.fail());
}