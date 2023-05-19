#ifndef _OPERATIONKEY_H
#define _OPERATIONKEY_H
#include "SYglobal.h"

#define DEBUG_UART1					1
#if DEBUG_UART1
#define PRINTF_UART1(format,...)	printf(format "\r\n",##__VA_ARGS__)
#else
#define PRINTF_UART1(format,...)
#endif


/**************************2--SM4(ECB)*****************************/
UINT8 SM4_Get_M(void);
UINT8 SM4_Get_Q(void);
UINT8 SM4_Verify_P(void);

/**************************3--AES(128bits,ECB)*****************************/
UINT8 AES_Get_M(void);
UINT8 AES_Get_Q(void);
UINT8 AES_Verify_P(void);

/**************************4--DES(16Bytes,ECB)*****************************/
UINT8 DES_Get_M(void);
UINT8 DES_Get_Q(void);
UINT8 DES_Verify_P(void);

/**************************20--RSA(1024bits)*****************************/
UINT8 RSA_Get_M(void);
UINT8 RSA_Get_Q(void);
UINT8 RSA_Verify_P(void);

/**************************21--ECC(256bits)*****************************/
UINT8 ECC_Get_M(void);
UINT8 ECC_Get_Q(void);
UINT8 ECC_Verify_P(void);


#endif




