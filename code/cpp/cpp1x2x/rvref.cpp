#include "common.h"

namespace cpp1x2x {

class unit_rvref : public code_unit {
public:
  unit_rvref() : name_("RVRef") {}
  const std::string &name(void) const { return name_; }
  int init(void);
  void fini(void);
  int run(void);

private:
  std::string name_;
};

int unit_rvref::init() {
  return 0;
}

void unit_rvref::fini() {
}

class lvref {
public:
  lvref() {}
  lvref(const lvref &) {}
  lvref &operator=(const lvref &) { return *this; }
};

lvref make_lvref() {
  return lvref();
}

class rvref {
public:
  rvref() { std::cout << "rvref ctor" << std::endl; }
  rvref(const rvref &) { std::cout << "rvref copy ctor" << std::endl; }
  ~rvref() { std::cout << "rvref dtor" << std::endl; }
  void show() { std::cout << "show rvref" << std::endl; }
};

rvref make_rvref() {
  rvref v;
  return v;
}

class mempool {
public:
  static const int POOL_SIZE = 4096;
  mempool() : pool(new char[POOL_SIZE]){};
  ~mempool() {
    if (pool != nullptr)
      delete[] pool;
  }
  mempool(const mempool &other) : pool(new char[POOL_SIZE]) {
    std::cout << "copy big mempool" << std::endl;
    memcpy(pool, other.pool, POOL_SIZE);
  }

private:
  char *pool;
};

mempool get_pool(const mempool &pool) {
  return pool;
}

mempool make_pool() {
  mempool pool;
  return get_pool(pool);
}

int unit_rvref::run() {
  // 常量左值引用
  lvref a;
  lvref b(a);
  // 如果把lvref的拷贝构造函数和赋值运算符函数参数的const删除, 以下两行将编译失败,
  // 因为make_lvref()产生的是右值, 它无法绑定上述常量左值引用
  lvref c(make_lvref());
  c = make_lvref();
  std::cout << "c: " << type_name<decltype(c)>() << std::endl;

  std::cout << "normal: " << std::endl;
  rvref d = make_rvref();
  d.show();
  std::cout << "rv ref: " << std::endl;
  rvref &&e = make_rvref();
  e.show();

  mempool pool = make_pool();

  return 0;
}

code_unit_register rvref_reg(new unit_rvref);

} // namespace cpp1x2x
