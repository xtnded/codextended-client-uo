#include "stdafx.h"

void Q_strcat(char *dest, int size, const char *src);
void Q_strncpyz(char *dest, const char *src, int destsize);

#include <ShlObj.h>
#include <Shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

#pragma comment(lib, "Comdlg32.lib")
#include "Commdlg.h"

int codversion = COD_UNKNOWN;

std::string GetLastErrorAsString()
{
	//Get the error message, if any.
	DWORD errorMessageID = ::GetLastError();
	if (errorMessageID == 0)
		return "";

	LPSTR messageBuffer = NULL;
	size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);

	std::string message(messageBuffer, size);

	//Free the buffer.
	LocalFree(messageBuffer);

	return message;
}

int Sys_GetModulePathInfo(HMODULE module, char **path, char **filename, char **extension) {
	int sep = '/';
	static char szFileName[MAX_PATH + 1];
	if (path)
		*path = NULL;
	if (filename)
		*filename = NULL;
	if (extension)
		*extension = NULL;

	GetModuleFileNameA(module, szFileName, MAX_PATH);

	char *fn = strrchr(szFileName, sep);
	if (fn == nullptr) {
		sep = '\\';
		fn = strrchr(szFileName, sep);
	}
	if (fn != NULL) {
		*fn++ = 0;

		char *ext = strrchr(fn, '.');

		if (ext != NULL) {
			if (fn != ext) {
				if (extension)
					*ext++ = 0;
				if (path)
					*path = szFileName;
				if (filename)
					*filename = fn;
				if (extension)
					*extension = ext;
			}
		}
	}
	return sep;
}

#include <Wincrypt.h>
#include <sstream>

std::string GetHashText(const void* data, const size_t data_size, HashType hashType)
{
	HCRYPTPROV hProv = NULL;

	if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_AES, CRYPT_VERIFYCONTEXT)) {
		return "";
	}

	BOOL hash_ok = FALSE;
	HCRYPTPROV hHash = NULL;
	switch (hashType) {
	case HashSha1: hash_ok = CryptCreateHash(hProv, CALG_SHA1, 0, 0, &hHash); break;
	case HashMd5: hash_ok = CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash); break;
	case HashSha256: hash_ok = CryptCreateHash(hProv, CALG_SHA_256, 0, 0, &hHash); break;
	}

	if (!hash_ok) {
		CryptReleaseContext(hProv, 0);
		return "";
	}

	if (!CryptHashData(hHash, static_cast<const BYTE*>(data), data_size, 0)) {
		CryptDestroyHash(hHash);
		CryptReleaseContext(hProv, 0);
		return "";
	}

	DWORD cbHashSize = 0, dwCount = sizeof(DWORD);
	if (!CryptGetHashParam(hHash, HP_HASHSIZE, (BYTE*)&cbHashSize, &dwCount, 0)) {
		CryptDestroyHash(hHash);
		CryptReleaseContext(hProv, 0);
		return "";
	}

	std::vector<BYTE> buffer(cbHashSize);
	if (!CryptGetHashParam(hHash, HP_HASHVAL, reinterpret_cast<BYTE*>(&buffer[0]), &cbHashSize, 0)) {
		CryptDestroyHash(hHash);
		CryptReleaseContext(hProv, 0);
		return "";
	}

	std::ostringstream oss;

	for (std::vector<BYTE>::const_iterator iter = buffer.begin(); iter != buffer.end(); ++iter) {
		oss.fill('0');
		oss.width(2);
		oss << std::hex << static_cast<const int>(*iter);
	}

	CryptDestroyHash(hHash);
	CryptReleaseContext(hProv, 0);
	return oss.str();
}