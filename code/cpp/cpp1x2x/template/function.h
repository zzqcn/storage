#pragma once

#include "common.h"

#include <ranges>
#include <concepts>

// 经典C++函数模板
template <typename T> void print_container1(const T &items) {
  for (const auto &item : items) {
    std::cout << item << " ";
  }
};

// C++20缩写函数模板
void print_container2(const auto &items) {
  for (const auto &item : items) {
    std::cout << item << " ";
  }
}

// 使用概念约束的函数模板
template <typename T>
  requires std::integral<T> || std::floating_point<T>
T multiply(T a, T b) {
  return a * b;
}

// 自定义概念
template <typename T>
concept numeric = std::integral<T> || std::floating_point<T>;

// 在模块中使用自定义概念
template <typename T>
  requires numeric<T>
T multiply2(T a, T b) {
  return a * b;
}

// 如果只有一个用概念约束的类型参数, 建议用概念名代替typename
template <numeric T> T multiply3(T a, T b) {
  return a * b;
}

// 在缩写函数模板中使用概念
auto multiply4(numeric auto a, numeric auto b) {
  return a * b;
}

// requires表达式 ------------------------------------------
// 简单要求
template <typename T>
concept range = requires(T &t) {
  std::ranges::begin(t);
  std::ranges::end(t);
};

template <typename T>
concept arithmetic_type = requires(T a, T b) {
  a + b;
  a - b;
  a * b;
  a / b;
  a += b;
  a -= b;
  a *= b;
  a /= b;
};

// 类型要求
template <typename T>
concept has_value_type = requires { typename T::value_value; };

// 复合要求
template <typename T>
concept my_incrementable = requires(T i) {
  { i++ } -> std::same_as<T>;
  { ++i } -> std::same_as<T &>;
};

// 嵌套要求
template <typename T>
  requires requires(const T &t) {
    std::ranges::begin(t);
    std::ranges::end(t);
  }
void print_range(const T &range) {
  for (const auto &item : range) {
    std::cout << item << " ";
  }
}
// ------------------------------------------

void run_function(void);
