#ifdef DLIB_EXPORTS
#define DLIB_API __declspec(dllexport)
#else
#define DLIB_API __declspec(dllimport)
#endif

DLIB_API void dlib_func(int n);
