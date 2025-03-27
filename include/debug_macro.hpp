/*******************************************************************************

                                debug_macro(...) macro

License (MIT):

  Copyright (c) 2019 David Peter <mail@david-peter.de>

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to
  deal in the Software without restriction, including without limitation the
  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
  sell copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

*******************************************************************************/

/* -------------------------------------------------------------------------- */
/*                usage: debug_macro((expr1), (expr2), (expr2))               */
/* -------------------------------------------------------------------------- */

#ifndef _DEBUG_MACRO_HXX
#define _DEBUG_MACRO_HXX

/* -------------------------------------------------------------------------- */
/*                          standard library headers                          */
/* -------------------------------------------------------------------------- */

#include <format>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <queue>
#include <ranges>
#include <set>
#include <stack>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <variant>

/* -------------------------------------------------------------------------- */
/*                           Colorful output support                          */
/* -------------------------------------------------------------------------- */

#if defined(_WIN32) || defined(_WIN64)
# include <windows.h>
#elif defined(__unix__) || defined(__APPLE__) || defined(__MACH__)
# include <unistd.h>
#endif

/* -------------------------------------------------------------------------- */
/*                         Auxiliary macro definition                         */
/* -------------------------------------------------------------------------- */

#define DEBUG_EXPAND(...) \
  __VA_ARGS__ \
  /**/

#define DEBUG_STRINGIFY(...)     #__VA_ARGS__

#define DEBUG_APPLY(_MACRO, ...) _MACRO(__VA_OPT__(__VA_ARGS__))

#define DEBUG_TO_STRING_ARRAY(...) \
  std::to_array( \
    {DEBUG_FOR_EACH(DEBUG_STRINGIFY, __VA_ARGS__) __VA_OPT__(, ) "_"})

/* -------------------------------------------------------------------------- */
/*                                  DEBUG_FOR_EACH */
/* -------------------------------------------------------------------------- */

#define DEBUG_EXPAND_R(...) \
  DEBUG_EXPAND_R1( \
    DEBUG_EXPAND_R1(DEBUG_EXPAND_R1(DEBUG_EXPAND_R1(__VA_ARGS__)))) \
  /**/
#define DEBUG_EXPAND_R1(...) \
  DEBUG_EXPAND_R2( \
    DEBUG_EXPAND_R2(DEBUG_EXPAND_R2(DEBUG_EXPAND_R2(__VA_ARGS__)))) \
  /**/
#define DEBUG_EXPAND_R2(...) \
  DEBUG_EXPAND_R3( \
    DEBUG_EXPAND_R3(DEBUG_EXPAND_R3(DEBUG_EXPAND_R3(__VA_ARGS__)))) \
  /**/
#define DEBUG_EXPAND_R3(...) \
  DEBUG_EXPAND(DEBUG_EXPAND(DEBUG_EXPAND(DEBUG_EXPAND(__VA_ARGS__)))) \
  /**/
#define DEBUG_PARENS ()
#define DEBUG_FOR_EACH(_MACRO, ...) \
  __VA_OPT__(DEBUG_EXPAND_R(DEBUG_FOR_EACH_HELPER(_MACRO, __VA_ARGS__))) \
  /**/
#define DEBUG_FOR_EACH_HELPER(_MACRO, _A1, ...) \
  _MACRO(_A1) \
  __VA_OPT__(, ) \
  __VA_OPT__(DEBUG_FOR_EACH_AGAIN DEBUG_PARENS(_MACRO, __VA_ARGS__)) /**/
#define DEBUG_FOR_EACH_AGAIN() DEBUG_FOR_EACH_HELPER

/* -------------------------------------------------------------------------- */
#define debug_macro(...) \
  debug_macro::__output(__FILE__, \
                        __LINE__, \
                        __FUNCTION__, \
                        DEBUG_TO_STRING_ARRAY(__VA_ARGS__) __VA_OPT__(, ) \
                          __VA_ARGS__)
/* -------------------------------------------------------------------------- */

namespace debug_macro::inline __detail {
/* -------------------------------------------------------------------------- */
/*                           colorful console ouput                           */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/*                                color options                               */
/* -------------------------------------------------------------------------- */

enum __console_color : unsigned char {
  _S_black,
  _S_red,
  _S_green,
  _S_yellow,
  _S_blue,
  _S_magenta,
  _S_cyan,
  _S_white,
  _S_bright_black,
  _S_bright_red,
  _S_bright_green,
  _S_bright_yellow,
  _S_bright_blue,
  _S_bright_magenta,
  _S_bright_cyan,
  _S_bright_white
};
__attribute__((__always_inline__, __artificial__)) inline void
__set_console_color(__console_color __color,
                    std::FILE      *__stream = stdout) noexcept {
#if defined(_WIN32) || defined(_WIN64)
  static ::HANDLE __console = ::GetStdHandle(STD_OUTPUT_HANDLE);
  static ::CONSOLE_SCREEN_BUFFER_INFO __original;
  static bool                         __uninitialized = true;

  if (__uninitialized) [[unlikely]] {
    ::GetConsoleScreenBufferInfo(__console, &__original);
    __initialized = false;
  }

  ::WORD __attrs = __original.wAttributes;
  switch (__color) {
  case __console_color::_S_black:
    ::SetConsoleTextAttribute(__console, 0);
    break;
  case __console_color::_S_red:
    ::SetConsoleTextAttribute(__console, FOREGROUND_RED);
    break;
  case __console_color::_S_green:
    ::SetConsoleTextAttribute(__console, FOREGROUND_GREEN);
    break;
  case __console_color::_S_yellow:
    ::SetConsoleTextAttribute(__console, FOREGROUND_RED | FOREGROUND_GREEN);
    break;
  case __console_color::_S_blue:
    ::SetConsoleTextAttribute(__console, FOREGROUND_BLUE);
    break;
  case __console_color::_S_magenta:
    ::SetConsoleTextAttribute(__console, FOREGROUND_RED | FOREGROUND_BLUE);
    break;
  case __console_color::_S_cyan:
    ::SetConsoleTextAttribute(__console, FOREGROUND_GREEN | FOREGROUND_BLUE);
    break;
  case __console_color::_S_white:
    ::SetConsoleTextAttribute(
      __console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
    break;
  case __console_color::_S_bright_red:
    ::SetConsoleTextAttribute(__console, FOREGROUND_RED | FOREGROUND_INTENSITY);
    break;
  case __console_color::_S_bright_green:
    ::SetConsoleTextAttribute(__console,
                              FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    break;
  case __console_color::_S_bright_yellow:
    ::SetConsoleTextAttribute(
      __console, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
    break;
  case __console_color::_S_bright_blue:
    ::SetConsoleTextAttribute(__console,
                              FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    break;
  case __console_color::_S_bright_magenta:
    ::SetConsoleTextAttribute(
      __console, FOREGROUND_RED | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    break;
  case __console_color::_S_bright_cyan:
    ::SetConsoleTextAttribute(
      __console, FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    break;
  case __console_color::_S_bright_white:
    ::SetConsoleTextAttribute(__console,
                              FOREGROUND_RED | FOREGROUND_GREEN |
                                FOREGROUND_BLUE | FOREGROUND_INTENSITY);
    break;
  default: ::SetConsoleTextAttribute(__console, __attrs); break;
  }
#elif defined(__unix__) || defined(__APPLE__) || defined(__MACH__)
  static constexpr char const *__color_codes[] = {"\033[30m",
                                                  "\033[31m",
                                                  "\033[32m",
                                                  "\033[33m",
                                                  "\033[34m",
                                                  "\033[35m",
                                                  "\033[36m",
                                                  "\033[37m",
                                                  "\033[90m",
                                                  "\033[91m",
                                                  "\033[92m",
                                                  "\033[93m",
                                                  "\033[94m",
                                                  "\033[95m",
                                                  "\033[96m",
                                                  "\033[97m"};

  (void)std::fprintf(
    __stream,
    "%s",
    __color_codes[static_cast<std::size_t>(__color)]); // NOLINT
#endif
}
__attribute__((__always_inline__, __artificial__)) inline void
__reset_console_color(std::FILE *__stream = stdout) noexcept {
#if defined(_WIN32) || defined(_WIN64)
  static ::HANDLE __console = GetStdHandle(STD_OUTPUT_HANDLE);
  static ::CONSOLE_SCREEN_BUFFER_INFO __original;
  static bool                         __uninitialized = true;

  if (__uninitialized) [[unlikely]] {
    ::GetConsoleScreenBufferInfo(__console, &__original);
    initialized = false;
  }
  ::SetConsoleTextAttribute(__console, __original.wAttributes);
#elif defined(__unix__) || defined(__APPLE__) || defined(__MACH__)
  (void)std::fprintf(__stream, "\033[0m");
#endif
}
/* -------------------------------------------------------------------------- */
/*                    colorfully formattable wrapper                          */
/* -------------------------------------------------------------------------- */

template <typename _Ty> struct __colorfully {
  __console_color _M_color;
  _Ty             _M_ref;
};
template <typename _Ty>
__colorfully(__console_color, _Ty &&) -> __colorfully<_Ty &&>;
template <typename _CharT, typename _Ty>
__attribute__((__always_inline__, __artificial__)) inline auto
operator<<(std::basic_ostream<_CharT> &__ostream,
           __colorfully<_Ty> __fmt) noexcept -> std::basic_ostream<_CharT> & {
  __set_console_color(__fmt._M_color);
  __ostream << __fmt._M_ref;
  __reset_console_color();
  return __ostream;
}
/* --------------------------------------------------------------------------
 */
/*                                  type name */
/* --------------------------------------------------------------------------
 */

template <typename _Ty>
static constexpr std::string_view
__nameof() noexcept {
  return []<typename _Uy>() noexcept {
    std::string_view __s{__PRETTY_FUNCTION__};
    __s.remove_prefix((std::min)(__s.find_last_of('=') + 2, __s.length()));
    __s.remove_suffix((std::min)(1ZU, __s.length()));
    return __s;
  }.template operator()<_Ty>();
};
/* -------------------------------------------------------------------------- */
/*                               enumerator name                              */
/* -------------------------------------------------------------------------- */

template <auto _Val>
static constexpr std::string_view __unscoped_enum_name = []<auto _> noexcept {
  std::string_view __s{__PRETTY_FUNCTION__};
  __s.remove_suffix((std::min)(1ZU, __s.length()));
  if (auto __d = __s.back(); __d >= '0' && __d <= '9') [[unlikely]] {
    return std::string_view{};
  }
  __s.remove_prefix((std::min)(__s.find_last_of("= ") + 1, __s.length()));
  return __s;
}.template operator()<_Val>();

template <auto _Val>
static constexpr std::string_view __scoped_enum_name = []<auto _> noexcept {
  std::string_view __s{__PRETTY_FUNCTION__};
  __s.remove_suffix((std::min)(1ZU, __s.length()));
  if (auto __d = __s.back(); __d >= '0' && __d <= '9') [[unlikely]] {
    return std::string_view{};
  }
  __s.remove_prefix((std::min)(__s.find_last_of("::") + 1, __s.length()));
  return __s;
}.template operator()<_Val>();
template <auto _Val>
requires std::is_enum_v<decltype(_Val)> static constexpr std::string_view
__nameof() noexcept {
  return std::is_scoped_enum_v<decltype(_Val)> ? __scoped_enum_name<_Val>
                                               : __unscoped_enum_name<_Val>;
}
/* -------------------------------------------------------------------------- */
/*                                 type traits                                */
/* -------------------------------------------------------------------------- */

template <typename _Container>
struct __container_adactor_ref : public _Container {
  using _Container::c;
};
template <typename _Ty, template <typename...> typename _Primary>
struct __is_specialization_of : std::false_type {};
template <template <class...> typename _Primary, typename... _Args>
struct __is_specialization_of<_Primary<_Args...>, _Primary> : std::true_type {};
template <typename _Ty, template <typename...> typename _Primary>
static constexpr bool __is_specialization_of_v =
  __is_specialization_of<_Ty, _Primary>::value;

template <typename _Ty, template <typename...> typename... _Primarys>
static constexpr bool __is_any_specialization_of_v =
  bool((__is_specialization_of_v<_Ty, _Primarys> || ...));
namespace __switch {
template <bool _Val, std::size_t _Np> struct __case : std::bool_constant<_Val> {
  using type = std::integral_constant<std::size_t, _Np>;
};
template <std::size_t _Np> struct __default : std::true_type {
  using type = std::integral_constant<std::size_t, _Np>;
};
} // namespace __switch

template <typename _Ty> struct __member_object_traits;
template <typename _Ty, typename _Uy>
struct __member_object_traits<_Ty _Uy::*> {
  using __memobj_ = _Ty;
  using __class_  = _Uy;
};
template <typename _Ty> concept __non_linear_container =
  __is_any_specialization_of_v<_Ty,
                               std::list,
                               std::map,
                               std::multimap,
                               std::unordered_map,
                               std::unordered_multimap,
                               std::unordered_set,
                               std::unordered_multiset,
                               std::set,
                               std::multiset>;
/* --------------------------------------------------------------------------
 */
/*                       get enumerator name at runtime */
/* --------------------------------------------------------------------------
 */

template <typename _Ty,
          auto __value_,
          auto __max_,
          auto __enumerator_ = _Ty(__value_)>
static consteval auto
__detect_enumerators() noexcept {
  if constexpr (__value_ == __max_) {
    return std::tuple<std::pair<_Ty, std::string_view>>{};
  } else if constexpr (__nameof<__enumerator_>().empty()) {
    return __detect_enumerators<_Ty, __value_ + 1, __max_>();
  } else {
    return std::tuple_cat(
      std::make_tuple(std::make_pair(__enumerator_, __nameof<__enumerator_>())),
      __detect_enumerators<_Ty, __value_ + 1, __max_>());
  }
}
template <typename _Ty, typename _Uy = std::underlying_type_t<_Ty>>
requires std::is_enum_v<_Ty> static constexpr auto
__nameof(_Ty _Val) noexcept {
  static constexpr auto __min_ =
    (std::max)(std::make_signed_t<_Uy>(-28),
               std::make_signed_t<_Uy>(std::numeric_limits<_Uy>::min()));
  static constexpr auto __max_ =
    (std::max)(std::make_signed_t<_Uy>(28),
               std::make_signed_t<_Uy>(std::numeric_limits<_Uy>::max()));
  static constexpr auto __collection_ =
    __detect_enumerators<_Ty, __min_, __max_>();
  return std::apply(
    [_Val](auto const &...__enumerators) noexcept {
      std::string_view __result;
      ((__enumerators.first == _Val && (__result = __enumerators.second, 0)),
       ...);
      return __result;
    },
    __collection_);
}
/* -------------------------------------------------------------------------- */
/*                    count data member of aggregate struct                   */
/* -------------------------------------------------------------------------- */

struct __universal_type {
  consteval __universal_type(std::size_t) {}
  template <typename _Ty>
  requires(std::is_copy_constructible_v<_Ty>) operator _Ty &() const noexcept;

  template <typename _Ty>
  requires(std::is_move_constructible_v<_Ty> &&
           !std::is_copy_constructible_v<_Ty>) operator _Ty &&() const noexcept;
  struct __empty {};
  template <typename _Ty>
  requires(
    !std::is_copy_constructible_v<_Ty> && !std::is_move_constructible_v<_Ty> &&
    !std::is_constructible_v<_Ty, __empty>) operator _Ty() const noexcept;
};
template <typename _Ty, std::size_t _Np>
consteval auto
__test_direct_initializable() {
  return []<std::size_t... _Is>(std::index_sequence<_Is...>) {
    return requires { _Ty{__universal_type(_Is)...}; };
  }(std::make_index_sequence<_Np>{});
}
template <typename _Ty, std::size_t _Np = 0>
consteval std::size_t
__imprecise_struct_size() {
  if constexpr (__test_direct_initializable<_Ty, _Np>() &&
                !__test_direct_initializable<_Ty, _Np + 1>()) {
    return _Np;
  } else {
    return __imprecise_struct_size<_Ty, _Np + 1>();
  }
}
/* -------------------------------------------------------------------------- */
/*                       fine-tune the number of members                      */
/* -------------------------------------------------------------------------- */

template <typename _Ty, std::size_t _Np1, std::size_t _Np2, std::size_t _Np3>
consteval bool
__test_three_parts_initializable() {
  return []<std::size_t... _Is1, std::size_t... _Is2, std::size_t... _Is3>(
           std::index_sequence<_Is1...>,
           std::index_sequence<_Is2...>,
           std::index_sequence<_Is3...>) {
    return requires {
      _Ty{__universal_type(_Is1)...,
          {__universal_type(_Is2)...},
          __universal_type(_Is3)...};
    };
  }(std::make_index_sequence<_Np1>{},
         std::make_index_sequence<_Np2>{},
         std::make_index_sequence<_Np3>{});
}
template <typename _Ty, std::size_t __position, std::size_t _Np>
consteval bool
__try_place_n_in_pos() {
  constexpr auto __total = __imprecise_struct_size<_Ty>();
  if constexpr (_Np == 0) {
    return true;
  } else if constexpr (__position + _Np <= __total) {
    return __test_three_parts_initializable<_Ty,
                                            __position,
                                            _Np,
                                            __total - __position - _Np>();
  } else {
    return false;
  }
}
template <typename _Ty,
          std::size_t __position,
          std::size_t _Np  = 0,
          std::size_t _Max = 10>
consteval bool
__has_extra_elements() {
  constexpr auto __total = __imprecise_struct_size<_Ty>();
  if constexpr (__test_three_parts_initializable<_Ty,
                                                 __position,
                                                 _Np,
                                                 __total - __position - 1>()) {
    return false;
  } else if constexpr (_Np + 1 <= _Max) {
    return __has_extra_elements<_Ty, __position, _Np + 1>();
  } else {
    return true;
  }
}
template <typename _Ty, std::size_t __position, std::size_t _Np = 0>
consteval std::size_t
__search_max_in_pos() {
  constexpr auto __total = __imprecise_struct_size<_Ty>();
  if constexpr (!__has_extra_elements<_Ty, __position>()) {
    return 1UZ;
  } else {
    std::size_t __result{};
    [&]<std::size_t... _Is>(std::index_sequence<_Is...>) {
      ((__try_place_n_in_pos<_Ty, __position, _Is>() ? __result = _Is : 0),
       ...);
    }(std::make_index_sequence<__total + 1>());
    return __result;
  }
}
template <typename _Ty, std::size_t _Np = 0>
consteval void
__search_all_extra_index(auto &__array) {
  constexpr auto __total = __imprecise_struct_size<_Ty>();
  constexpr auto __num   = __search_max_in_pos<_Ty, _Np>();
  constexpr auto __value = __num > 1 ? __num : 1;
  __array[_Np]           = __value;
  if constexpr (_Np + __value < __total) {
    __search_all_extra_index<_Ty, _Np + __value>(__array);
  }
}
template <typename _Ty>
consteval std::size_t
__exact_struct_size() {
  constexpr auto __total = __imprecise_struct_size<_Ty>();
  if constexpr (__total == 0) {
    return 0;
  } else {
    std::array<std::size_t, __total> __indices = {1UZ};
    __search_all_extra_index<_Ty>(__indices);
    std::size_t __result = __total;
    std::size_t __index{};
    while (__index < __total) {
      auto __n  = __indices[__index]; // NOLINT
      __result -= __n - 1;
      __index  += __n;
    }
    return __result;
  }
}
/* -------------------------------------------------------------------------- */
/*                      convert aggregate struct to tuple                     */
/* -------------------------------------------------------------------------- */

template <typename _Ty>
constexpr auto
__converts_from_aggregate(_Ty &&__val) noexcept {
  constexpr std::size_t __size =
    __exact_struct_size<std::remove_cvref_t<_Ty>>();

  if constexpr (__size == 0) {
    return std::tuple<>();
  } else if constexpr (__size == 1) {
    auto &&[a] = std::forward<_Ty>(__val);
    return std::forward_as_tuple(a);
  } else if constexpr (__size == 2) {
    auto &&[a, b] = std::forward<_Ty>(__val);
    return std::forward_as_tuple(a, b);
  } else if constexpr (__size == 3) {
    auto &&[a, b, c] = std::forward<_Ty>(__val);
    return std::forward_as_tuple(a, b, c);
  } else if constexpr (__size == 4) {
    auto &&[a, b, c, d] = std::forward<_Ty>(__val);
    return std::forward_as_tuple(a, b, c, d);
  } else if constexpr (__size == 5) {
    auto &&[a, b, c, d, e] = std::forward<_Ty>(__val);
    return std::forward_as_tuple(a, b, c, d, e);
  } else if constexpr (__size == 6) {
    auto &&[a, b, c, d, e, f] = std::forward<_Ty>(__val);
    return std::forward_as_tuple(a, b, c, d, e, f);
  } else {
    static_assert(false, "aggregate struct has too many members.");
  }
}
/* -------------------------------------------------------------------------- */
/*                            select format pattern                           */
/* -------------------------------------------------------------------------- */

template <typename _Ty, typename _XTy = std::remove_cv_t<_Ty>>
using __overload_selector = typename std::disjunction<
  //                            string-like
  __switch::__case<__is_specialization_of_v<_XTy, std::basic_string>, 1UZ>,
  __switch::__case<__is_specialization_of_v<_XTy, std::basic_string_view>, 1UZ>,
  __switch::__case<std::is_same_v<_XTy, char>, 1UZ>,
  __switch::__case<std::is_array_v<_XTy> &&
                     std::is_same_v<char, std::remove_extent_t<_XTy>>,
                   1UZ>,
  __switch::__case<std::is_same_v<_XTy, char const *>, 1UZ>,
  //                            range
  __switch::__case<std::ranges::range<_XTy>, 2UZ>,
  //                            tuple-like
  __switch::__case<__is_specialization_of_v<_XTy, std::tuple>, 3UZ>,
  __switch::__case<__is_specialization_of_v<_XTy, std::pair>, 3UZ>,
  __switch::__case<std::is_aggregate_v<_Ty>, 3UZ>,
  //                            special container
  __switch::__case<__is_specialization_of_v<_XTy, std::variant>, 4UZ>,
  __switch::__case<__is_specialization_of_v<_XTy, std::optional>, 5UZ>,
  //                            container adaptor
  __switch::__case<__is_specialization_of_v<_XTy, std::queue>, 6UZ>,
  __switch::__case<__is_specialization_of_v<_XTy, std::priority_queue>, 6UZ>,
  __switch::__case<__is_specialization_of_v<_XTy, std::stack>, 6UZ>,
  //                            pointer
  __switch::__case<std::is_pointer_v<_Ty>, 7UZ>,
  __switch::__case<__is_specialization_of_v<_XTy, std::unique_ptr>, 7UZ>,
  __switch::__case<__is_specialization_of_v<_XTy, std::shared_ptr>, 7UZ>,
  __switch::__case<__is_specialization_of_v<_XTy, std::weak_ptr>, 7UZ>,
  //                            enumerator
  __switch::__case<std::is_enum_v<_XTy>, 8UZ>,
  //                            member object pointer
  __switch::__case<std::is_member_object_pointer_v<_XTy>, 9UZ>,
  //                            basic type
  __switch::__default<0>>::type;
/* -------------------------------------------------------------------------- */
/*                          __format_impl despatcher                          */
/* -------------------------------------------------------------------------- */

struct __formatter {
  template <typename _Ty, typename __selector_ = __overload_selector<_Ty>>
  static std::string __format(_Ty const &__val) noexcept {
    return __format_impl(__val, __selector_{});
  }
  /* --------------------------------------------------------------------------
   */
  /*                           output straightforward */
  /* --------------------------------------------------------------------------
   */

  static std::string __format_impl(
    auto const &__val, std::integral_constant<std::size_t, 0UZ>) noexcept {
    return std::format("{}", __val);
  }
  /* --------------------------------------------------------------------------
   */
  /*                           string-like | character */
  /* --------------------------------------------------------------------------
   */

  template <typename _Ty>
  static std::string __format_impl(
    _Ty const &__val, std::integral_constant<std::size_t, 1UZ>) noexcept {
    if constexpr (std::is_same_v<_Ty, char>) {
      return std::format(R"('{}')", __val);
    } else {
      return std::format(R"("{}")", __val);
    }
  }
  /* --------------------------------------------------------------------------
   */
  /*        requires(_Tp& __t) { ranges::begin(__t); ranges::end(__t); } */
  /* --------------------------------------------------------------------------
   */

  template <typename _Ty>
  static std::string __format_impl(
    _Ty const &__range, std::integral_constant<std::size_t, 2UZ>) noexcept {
    std::string __result;
    if constexpr (__non_linear_container<_Ty>) {
      __result.append("{");
    } else {
      __result.append("[");
    }
    for (auto const &__val: __range) {
      std::format_to(std::back_inserter(__result), "{}, ", __format(__val));
    }
    if (!std::ranges::empty(__range)) [[likely]] {
      __result.pop_back();
      __result.pop_back();
    }
    if constexpr (__non_linear_container<_Ty>) {
      __result.append("}");
    } else {
      __result.append("]");
    }
    return __result;
  }
  /* --------------------------------------------------------------------------
   */
  /*                        tuple-like | aggregate struct */
  /* --------------------------------------------------------------------------
   */

  template <typename _Ty>
  static std::string __format_impl(
    _Ty const &__val, std::integral_constant<std::size_t, 3UZ>) noexcept {
    if constexpr (__is_specialization_of_v<_Ty, std::pair>) {
      return std::format(
        "{} : {}", __format(__val.first), __format(__val.second));
    } else {
      static constexpr auto __handler =
        []<typename... _Args>(_Args const &...__xs) noexcept {
          std::string __result;
          if constexpr (std::is_aggregate_v<_Ty>) {
            __result.append("{");
          } else {
            __result.append("(");
          }
          (std::format_to(std::back_inserter(__result), "{}, ", __format(__xs)),
           ...);

          if constexpr (sizeof...(__xs) > 0) {
            __result.pop_back();
            __result.pop_back();
          }
          if constexpr (std::is_aggregate_v<_Ty>) {
            __result.append("}");
          } else {
            __result.append(")");
          }
          return __result;
        };
      if constexpr (std::is_aggregate_v<_Ty>) {
        return std::apply(__handler, __converts_from_aggregate(__val));
      } else {
        return std::apply(__handler, __val);
      }
    }
  }
  /* --------------------------------------------------------------------------
   */
  /*                                   variant */
  /* --------------------------------------------------------------------------
   */

  template <typename _Ty>
  static std::string __format_impl(
    _Ty const &__val, std::integral_constant<std::size_t, 4UZ>) noexcept {
    return std::visit(
      []<typename _Uy>(_Uy const &__x) noexcept {
        return std::format("|{}|", __format(__x));
      },
      __val);
  }
  /* --------------------------------------------------------------------------
   */
  /*                                  optional */
  /* --------------------------------------------------------------------------
   */

  template <typename _Uy>
  static std::string __format_impl(
    std::optional<_Uy> const &__val,
    std::integral_constant<std::size_t, 5UZ>) noexcept {
    if (!__val.has_value()) { return "(std::nullopt)"; }
    return std::format("|{}|", __format(*__val));
  }
  /* --------------------------------------------------------------------------
   */
  /*                              container adaptor */
  /* --------------------------------------------------------------------------
   */

  template <typename _Ty>
  static std::string __format_impl(
    _Ty const &__val, std::integral_constant<std::size_t, 6UZ>) noexcept {
    return __format(static_cast<__container_adactor_ref<_Ty> const &>(__val).c);
  }
  /* --------------------------------------------------------------------------
   */
  /*                      pointer | unique_ptr | shared_ptr */
  /* --------------------------------------------------------------------------
   */

  template <typename _Ty>
  static std::string __format_impl(
    _Ty const &__val, std::integral_constant<std::size_t, 7UZ>) noexcept {
    if (!__val) [[unlikely]] { return "(nullptr)"; }
    if constexpr (std::is_pointer_v<_Ty>) {
      return std::format("{}", static_cast<void *>(__val));
    } else if constexpr (__is_specialization_of_v<_Ty, std::unique_ptr> ||
                         __is_specialization_of_v<_Ty, std::shared_ptr>) {
      return std::format("{}", static_cast<void *>(__val.get()));
    } else {
      std::format("{}", __val.lock().get());
    }
  }
  /* --------------------------------------------------------------------------
   */
  /*                                 enumerator */
  /* --------------------------------------------------------------------------
   */

  template <typename _Ty>
  static std::string __format_impl(
    _Ty const &__val, std::integral_constant<std::size_t, 8UZ>) noexcept {
    using _Uy = std::underlying_type_t<_Ty>;
    if (auto __name = __nameof(__val); __name.empty()) [[unlikely]] {
      return std::format("{}({})", __nameof<_Ty>(), _Uy(__val));
    } else {
      return std::format("{}::{}({})", __nameof<_Ty>(), __name, _Uy(__val));
    }
  }
  /* --------------------------------------------------------------------------
   */
  /*                         data member object pointer */
  /* --------------------------------------------------------------------------
   */

  template <typename _Ty>
  static std::string __format_impl(
    _Ty const &__val, std::integral_constant<std::size_t, 9UZ>) noexcept {
    using _Uy = typename __member_object_traits<_Ty>::__class_;
    static_assert(std::is_standard_layout_v<_Uy>);
    auto __result =
      reinterpret_cast<std::size_t>(&reinterpret_cast<char const volatile &>(
        static_cast<_Uy *>(nullptr)->*__val));
    return std::format("{}", __result);
  }
};
/* -------------------------------------------------------------------------- */
/*                             shortcut long name                             */
/* -------------------------------------------------------------------------- */

__attribute__((__always_inline__, __artificial__)) inline std::string_view
__strip_filepath(std::string_view __path) noexcept {
  std::size_t __last_slash = __path.find_last_of('/');
  if (__last_slash == std::string_view::npos) [[unlikely]] { return __path; }
  std::string_view __before_last       = __path.substr(0, __last_slash);
  std::size_t      __second_last_slash = __before_last.find_last_of('/');
  if (__second_last_slash == std::string_view::npos) {
    return __path.substr(__last_slash + 1);
  }
  return __path.substr(__second_last_slash + 1);
}
/* -------------------------------------------------------------------------- */
/*                        console output implementation                       */
/* -------------------------------------------------------------------------- */

template <std::size_t _Np, typename... _Args>
__attribute__((__always_inline__, __artificial__)) inline void
__output(char const                   *__path,
         int                           __line,
         char const                   *__funcsig,
         std::array<char const *, _Np> __names,
         _Args &&...__vals) {
  std::cout << "[" << __colorfully{_S_bright_black, __strip_filepath(__path)}
            << ":" << __colorfully{_S_bright_black, __line} << " ("
            << __colorfully{_S_cyan, __funcsig} << ")]\n";
  if constexpr (sizeof...(_Args) > 0) {
    std::puts("{");
    [&]<std::size_t... _Is>(std::index_sequence<_Is...>) noexcept {
      ((void)(std::cout << "\t" << __colorfully{_S_blue, __names[_Is]} << " = "
                        << __colorfully{_S_bright_red,
                                        __formatter::__format(__vals)}
                        << " ("
                        << __colorfully{_S_bright_yellow, __nameof<_Args>()}
                        << ")\n"),
       ...);
      std::puts("}");
    }(std::index_sequence_for<_Args...>{});
  }
}
} // namespace debug_macro::inline __detail
#endif
