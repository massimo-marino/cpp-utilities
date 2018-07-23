/* 
 * File:   stringTokenizers.cpp
 * Author: massimo
 *
 * Created on November 21, 2017, 5:57 PM
 */
#include "stringTokenizers.h"
////////////////////////////////////////////////////////////////////////////////
namespace utilities
{
stringTokenizer::stringTokenizer(const std::string& s, const std::string& delim) noexcept
:
str_(s),
count_(ULONG_MAX),
begin_(0),
end_(0)
{
  if ( "" != delim )
  {
    delim_ = delim;
  }

  // Point to the first token
  begin_ = str_.find_first_not_of(delim_);
  end_   = str_.find_first_of(delim_, begin_);
}

size_t
stringTokenizer::countTokens() const noexcept
{
  if ( count_ != ULONG_MAX )
  {
    // return if we've already counted
    return (static_cast<size_t>(count_));
  }

  std::string::size_type n {0};
  std::string::size_type i {0};

  for (;;)
  {
    // advance to the first token
    if ( std::string::npos == (i = str_.find_first_not_of(delim_, i)) )
    {
      break;
    }
    // advance to the next delimiter
    i = str_.find_first_of(delim_, (i + 1));
    ++n;
    if ( std::string::npos == i )
    {
      break;
    }
  }
  count_ = n;
  return static_cast<size_t>(count_);
}

stringTokenizer&
stringTokenizer::nextToken(std::string& s) noexcept
{
  if ( (std::string::npos != begin_) &&
       (std::string::npos != end_) )
  {
    s      = str_.substr(begin_, end_ - begin_);
    begin_ = str_.find_first_not_of(delim_, end_);
    end_   = str_.find_first_of(delim_, begin_);
  }
  else if ( (std::string::npos != begin_) &&
            (std::string::npos == end_) )
  {
    s      = str_.substr(begin_, str_.length( ) - begin_);
    begin_ = str_.find_first_not_of(delim_, end_);
  }
  return *this;
}

std::string
stringTokenizer::srest_of() const noexcept(false)
{
  std::string::size_type iTemp {};

  try
  {
    if ( (std::string::npos != begin_) &&
         (std::string::npos != end_) )
    {
      iTemp = str_.find_first_not_of(delim_, end_);
      sRest_Of_ = str_.substr(iTemp,
                              (str_.find_first_of(delim_, begin_) - iTemp));
      return sRest_Of_;
    }
    else if ( (std::string::npos != begin_) &&
              (std::string::npos == end_) )
    {
      iTemp = str_.find_first_not_of(delim_, end_);
      sRest_Of_ = str_.substr(iTemp,
                              (str_.length( ) - iTemp));
      return sRest_Of_;
    }
  }
  catch (...)
  {
    sRest_Of_= "";
    return sRest_Of_;
  }
  return sRest_Of_;
}
}  // namespace utilities 
