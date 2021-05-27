#ifndef __LIBFOO_H__
#define __LIBFOO_H__

typedef int (*pref_set_func)(const char* pref, int val);
typedef int (*pref_get_func)(const char* pref, int* val);

struct foo
{
    pref_set_func set;
    pref_get_func get;
};

#define FOO_REGISTER_NAME "foo_register"

#endif
