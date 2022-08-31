#ifdef OS

#include "storage/storage.h"
#include "helpers/client_helper.h"

#include <cstring>
#include <stdio.h>

using namespace iotex;

// Define global object
Storage storage;

void Storage::Initialize(uint32_t storageSize = 0)
{
}

ResultCode Storage::savePrivateKey(const char* path,
								   const uint8_t privateKey[IOTEX_PRIVATE_KEY_SIZE])
{
	FILE* pFile;
	pFile = fopen(path, "wb");
	if(pFile == NULL)
	{
		perror("Failed: ");
		return ResultCode::ERROR_STORAGE_OPEN;
	}
	int ret = fwrite(privateKey, 1, IOTEX_PRIVATE_KEY_SIZE, pFile);
	fclose(pFile);

	if (ret == IOTEX_PRIVATE_KEY_SIZE)
	{
		return ResultCode::SUCCESS;
	}
	return ResultCode::ERROR_STORAGE_OPEN;
}

ResultCode Storage::readPrivateKey(const char* path, uint8_t privateKey[IOTEX_PRIVATE_KEY_SIZE])
{
	memset(privateKey, 0, IOTEX_PRIVATE_KEY_SIZE);
	FILE* pFile;
	pFile = fopen(path, "rb");
	if(pFile == NULL)
	{
		return ResultCode::ERROR_STORAGE_OPEN;
	}

	size_t readSize = fread(privateKey, 1, IOTEX_PRIVATE_KEY_SIZE, pFile);
	fclose(pFile);

	return (readSize == IOTEX_PRIVATE_KEY_SIZE) ? ResultCode::SUCCESS :
													ResultCode::ERROR_STORAGE_BAD_READ;
}

ResultCode Storage::deletePrivateKey(const char* path)
{
	remove(path);
}

#endif