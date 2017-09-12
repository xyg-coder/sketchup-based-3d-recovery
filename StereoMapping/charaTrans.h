#include "stdafx.h"

//#include <windows.h>

//-------------------------------------------------------------------------------------
//Description:
// This function maps a character string to a wide-character (Unicode) string
//
//Parameters:
// lpcszStr: [in] Pointer to the character string to be converted 
// lpwszStr: [out] Pointer to a buffer that receives the translated string. 
// dwSize: [in] Size of the buffer
//
//Return Values:
// TRUE: Succeed
// FALSE: Failed
// 
//Example:
// MByteToWChar(szA,szW,sizeof(szW)/sizeof(szW[0]));
//---------------------------------------------------------------------------------------
bool MByteToWChar(const char* lpcszStr, wchar_t* lpwszStr, unsigned int dwSize);


//-------------------------------------------------------------------------------------
//Description:
// This function maps a wide-character string to a new character string
//
//Parameters:
// lpcwszStr: [in] Pointer to the character string to be converted 
// lpszStr: [out] Pointer to a buffer that receives the translated string. 
// dwSize: [in] Size of the buffer
//
//Return Values:
// TRUE: Succeed
// FALSE: Failed
// 
//Example:
// MByteToWChar(szW,szA,sizeof(szA)/sizeof(szA[0]));
//---------------------------------------------------------------------------------------
bool WCharToMByte(const wchar_t* lpcwszStr, char* lpszStr, unsigned int dwSize);