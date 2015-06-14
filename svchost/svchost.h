#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")

#include "../Seu_lib/FileOperator.h"
#include "../Seu_lib/SystemInfo.h"
#include "../Seu_lib/Process.h"
#include "../Seu_lib/DosShell.h"
#include "../Seu_lib/Command.h"

#include "../Seu_lib/XScreenXor.h"
#include "../Seu_lib/VideoCap.h"

#include "../Seu_lib/Functions.h"
#include "../Seu_lib/zconf.h"
#include "../Seu_lib/zlib.h"
#pragma comment(lib,"../Seu_lib/zlib.lib")	//图象无损数据压缩使用zlib库函数

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

DWORD _stdcall ConnectThread(LPVOID lParam);
DWORD _stdcall FileManageThread(LPVOID lParam);
DWORD _stdcall ScreenThread(LPVOID lParam);
DWORD _stdcall ProcessThread(LPVOID lParam);
DWORD _stdcall ShellThread(LPVOID lParam);
DWORD _stdcall VideoThread(LPVOID lParam);
DWORD _stdcall FileDownThread(LPVOID lParam);
DWORD _stdcall FileUpThread(LPVOID lParam);

