#ifndef _LIB_CRYPTO_H_
#define _LIB_CRYPTO_H_

#include "libutil.h"
#include "liblist.h"

#define ARRAY_SIZE 1024

int encryptMsg(entryInfo_t *inInfo);
int decryptMsgWithKeysFile(entryInfo_t *inInfo);
int decryptMsgWithCipherBook(entryInfo_t *inInfo);

#endif