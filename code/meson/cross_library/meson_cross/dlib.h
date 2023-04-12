#if _MSC_VER
#  ifdef DLIB_EXPORTS
#    define DLIB_API __declspec(dllexport)
#  else
#    define DLIB_API __declspec(dllimport)
#  endif
#else
#  define DLIB_API __attribute__((visibility("default")))
#endif

DLIB_API void dlib_func(int n);
