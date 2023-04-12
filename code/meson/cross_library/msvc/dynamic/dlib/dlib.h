#pragma once

#ifdef DLIB_EXPORTS
#define DLIB_API extern "C" __declspec(dllexport)
#else
#define DLIB_API extern "C" __declspec(dllimport)
#endif

DLIB_API void dlib_func(int n);
