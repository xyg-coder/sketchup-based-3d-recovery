#include "stdafx.h"
#include "charaTrans.h"
//#include <string.h>


bool MByteToWChar(const char* lpcszStr, wchar_t* lpwszStr, unsigned int dwSize)
{
	// Get the required size of the buffer that receives the Unicode 
	// string. 
	unsigned int dwMinSize;
	dwMinSize = MultiByteToWideChar(CP_ACP,0,lpcszStr, -1, NULL, 0);

	if(dwSize < dwMinSize)
	{
		return false;
	}


	// Convert headers from ASCII to Unicode.
	MultiByteToWideChar (CP_ACP, 0, lpcszStr, -1, lpwszStr, dwMinSize);  

	return true;
}

bool WCharToMByte(const wchar_t* lpcwszStr, char* lpszStr, unsigned int dwSize)
{
	unsigned int dwMinSize;
	dwMinSize = WideCharToMultiByte(CP_OEMCP,NULL,lpcwszStr,-1,NULL,0,NULL,false);
	if(dwSize < dwMinSize)
	{
		return false;
	}
	WideCharToMultiByte(CP_OEMCP,NULL,lpcwszStr,-1,lpszStr,dwSize,NULL,false);
	return true;
}


/*static string UnicodeToAnsi(const wstring& str)  
{  
int iTextLen = WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, NULL, 0, NULL, NULL);  

if (iTextLen <= 0)  
{  
return "";  
}  

char* pElementText = new char[iTextLen + 1];  
memset(pElementText, 0, sizeof(char)*(iTextLen + 1));  

::WideCharToMultiByte(CP_ACP, 0, str.c_str(), -1, pElementText, iTextLen, NULL, NULL);  

std::string strText = pElementText;  

delete[] pElementText;  
pElementText = NULL;  

return strText;  
} */ 