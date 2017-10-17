/* 
 * File:   is_string.h
 * Author: massimo
 *
 * Created on October 17, 2017, 1:54 PM
 */

#ifndef IS_STRING_H
#define IS_STRING_H

#include <string>

#ifndef NO_DEMANGLE
#include <cxxabi.h>
////////////////////////////////////////////////////////////////////////////////
namespace utilities
{
template <typename T>
std::string
type (void)
{
  std::string result;
  int         status;
  char*       demangled = abi::__cxa_demangle(typeid(T).name(),
                                              nullptr, nullptr, &status);

  result = demangled;
  free(demangled);

  return result;
}
#else
// no demangling
template <typename T>
std::string
type (void)
{
  return typeid(T).name();
}
#endif

template <typename T>
struct is_string
{
  static inline const bool value = std::is_same<T, std::string>::value ||
                                   std::is_same<T, std::wstring>::value ||
                                   std::is_same<T, std::u16string>::value ||
                                   std::is_same<T, std::u32string>::value;
};

}  // namespace utilities
#endif /* IS_STRING_H */