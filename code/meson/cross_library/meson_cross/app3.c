#ifdef _MSC_VER
#  include <Windows.h>
#  include <tchar.h>
#else
#  include <dlfcn.h>
#  include <stdlib.h>
#endif

#include <stdio.h>

#include "dlib.h"

typedef void (*DLibFunc)(int);

static void ShowErrorAndExit(void) {
#ifdef _MSC_VER
  LPTSTR lpMsgBuf;
  DWORD err = GetLastError();

  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |
                    FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);
  _tprintf(TEXT("Error: %s\n"), lpMsgBuf);
  LocalFree(lpMsgBuf);
#else
  printf("Error: %s\n", dlerror());
#endif

  exit(EXIT_FAILURE);
}

int main() {
#if _MSC_VER
  HMODULE hDll;
  DLibFunc func;

  hDll = LoadLibrary(TEXT("dlib.dll"));
  if (NULL == hDll)
    ShowErrorAndExit();

  func = (DLibFunc)GetProcAddress(hDll, "dlib_func");
  if (NULL == func)
    ShowErrorAndExit();
#else
  void *hDll;
  DLibFunc func;

  hDll = dlopen("libdlib.so", RTLD_LAZY);
  if (NULL == hDll)
    ShowErrorAndExit();

  func = (DLibFunc)dlsym(hDll, "dlib_func");
  if (NULL == func)
    ShowErrorAndExit();
#endif

  func(123);

#if _MSC_VER
  FreeLibrary(hDll);
#else
  dlclose(hDll);
#endif

  return 0;
}

