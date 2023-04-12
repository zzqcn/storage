// app2.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <Windows.h>
#include <stdio.h>
#include <tchar.h>

#include "dlib.h"

typedef void (*DLibFunc)(int);

static void ShowErrorAndExit(void) {
  LPTSTR lpMsgBuf;
  DWORD err = GetLastError();

  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                    FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
  _tprintf(TEXT("Error: %s\n"), lpMsgBuf);
  LocalFree(lpMsgBuf);
  exit(EXIT_FAILURE);
}

int main() {
  HMODULE hDll;
  DLibFunc func;

  hDll = LoadLibrary(TEXT("dlib.dll"));
  if (NULL == hDll)
    ShowErrorAndExit();

  func = (DLibFunc)GetProcAddress(hDll, "dlib_func");
  if (NULL == func)
    ShowErrorAndExit();

  func(123);

  FreeLibrary(hDll);

  return 0;
}
