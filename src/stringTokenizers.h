/* 
 * File:   stringTokenizers.h
 * Author: massimo
 *
 * Created on November 21, 2017, 5:57 PM
 */

#ifndef STRINGTOKENIZERS_H
#define STRINGTOKENIZERS_H

#include <string>
#include <regex>
////////////////////////////////////////////////////////////////////////////////
namespace utilities
{
// String tokenizer class
class stringTokenizer final
{
 private:
  std::string delim_ {" \f\n\r\t\v\""};
  const std::string str_;
  mutable std::string sRest_Of_;
  mutable long count_;
  mutable std::string::size_type begin_;
  mutable std::string::size_type end_;

 public:
  // we don't want these objects allocated on the heap
  void* operator new(std::size_t) = delete;
  void* operator new[](std::size_t) = delete;
  stringTokenizer() = delete;
  stringTokenizer(const stringTokenizer& rhs) = delete;
  stringTokenizer& operator=(const stringTokenizer& rhs) = delete;
  stringTokenizer(stringTokenizer&& rhs) = delete;
  stringTokenizer& operator=(stringTokenizer&& rhs) = delete;

  explicit
  stringTokenizer(const std::string& s, const std::string& delim) noexcept;

  ~stringTokenizer() = default;

  size_t countTokens() const noexcept;

  constexpr
  bool
  hasMoreTokens() const noexcept
  {
    return (begin_ != end_);
  }

  stringTokenizer&
  nextToken(std::string& s) noexcept;

  std::string srest_of() const noexcept(false);
};  // class stringTokenizer

// Example
//void stringTokenizerExample ( )
//{
//  using container = std::vector<std::string>;
//  std::string s {" razzle      dazzle          giddyup"};
//  std::string token {};
//  container tokens {};
//  utilities::stringTokenizer st(s, " ");
//
//  std::cout << "there are "
//            << st.countTokens( )
//            << " tokens"
//            << std::endl;
//
//  while ( st.hasMoreTokens( ) )
//  {
//    st.nextToken(token);
//    tokens.push_back(token);
//    std::cout << "token = '"
//              << token
//              << "'\n";
//  }
//}  // stringTokenizerExample
////////////////////////////////////////////////////////////////////////////////
template <class Container>
void
strTokenize(const std::string& str,
            Container& container,
            const std::string& delims = " ") noexcept
{
  if ( str.empty() )
  {
    return;
  }
  if ( delims.empty() )
  {
    container.push_back(str);
    return;
  }
  std::size_t current;
  std::size_t previous = 0;
  current = str.find_first_of(delims);
  while ( std::string::npos != current )
  {
    container.push_back(str.substr(previous, current - previous));
    previous = current + 1;
    current = str.find_first_of(delims, previous);
  }
  container.push_back(str.substr(previous, current - previous));
}

template <class Container>
Container
strTokenize(const std::string& str,
            const std::string& delims = " ") noexcept
{
  Container container {};
  
  if ( str.empty() )
  {
    return container;
  }
  if ( delims.empty() )
  {
    container.push_back(str);
    return container;
  }
  std::size_t current;
  std::size_t previous = 0;
  current = str.find_first_of(delims);
  while ( std::string::npos != current )
  {
    container.push_back(str.substr(previous, current - previous));
    previous = current + 1;
    current = str.find_first_of(delims, previous);
  }
  container.push_back(str.substr(previous, current - previous));
  return container;
}

template <class Container>
Container
strRegexTokenize(const std::string& str,
                 const std::string& regExStr = R"(\s*[,;]\s*)") noexcept
{
  Container container {};
  
  if ( str.empty() )
  {
    return container;
  }
  std::regex regEx {regExStr};
  const std::sregex_token_iterator end;
  for (std::sregex_token_iterator iter {str.cbegin(), str.cend(), regEx, -1};
       iter != end;
       ++iter)
  {
    container.push_back(*iter);
  }
  return container;
}
}  // namespace utilities
#endif /* STRINGTOKENIZERS_H */
