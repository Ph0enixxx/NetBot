#ifndef __SEURAT_SYSTEMINFO_H__
#define __SEURAT_SYSTEMINFO_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <stdio.h> //sprintf
#include "Command.h"

//************************************
// Method:    GetSystemInfo
// Access:    public 
// Returns:   BOOL
// Parameter: SysInfo& info
//************************************
BOOL GetSystemInfo(SysInfo& info);

#endif //__SEURAT_SYSTEMINFO_H__