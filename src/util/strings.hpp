// The MIT License (MIT)

// Copyright (c) 2015 nabijaczleweli

// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


#pragma once
#ifndef STRINGS_HPP
#define STRINGS_HPP


#include <functional>
#include <algorithm>
#include <sstream>
#include <cctype>
#include <locale>


namespace {
	const constexpr static auto whitespace_selector = [&](auto c) {
		return !std::isspace(c);
	};
}

// Stolen from http://stackoverflow.com/a/217605/2851815
template<class CharT, class Traits, class Alloc>
static inline std::basic_string<CharT, Traits, Alloc> & ltrim(std::basic_string<CharT, Traits, Alloc> & s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), whitespace_selector));
	return s;
}

// Stolen from http://stackoverflow.com/a/217605/2851815
template<class CharT, class Traits, class Alloc>
static inline std::basic_string<CharT, Traits, Alloc> & rtrim(std::basic_string<CharT, Traits, Alloc> & s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), whitespace_selector).base(), s.end());
	return s;
}

// Stolen from http://stackoverflow.com/a/217605/2851815
template<class CharT, class Traits, class Alloc>
static inline std::basic_string<CharT, Traits, Alloc> & trim(std::basic_string<CharT, Traits, Alloc> & s) {
	return ltrim(rtrim(s));
}

template<class CharT, class Traits, class Alloc>
static inline std::basic_string<CharT, Traits, Alloc> & ltrim(std::basic_string<CharT, Traits, Alloc> && s) {
	return ltrim(s);
}

template<class CharT, class Traits, class Alloc>
static inline std::basic_string<CharT, Traits, Alloc> & rtrim(std::basic_string<CharT, Traits, Alloc> && s) {
	return rtrim(s);
}

template<class CharT, class Traits, class Alloc>
static inline std::basic_string<CharT, Traits, Alloc> & trim(std::basic_string<CharT, Traits, Alloc> && s) {
	return trim(s);
}


template<class StringT, class T>
inline constexpr StringT to_wstring(const T & from) {
	using stream_t = std::basic_ostringstream<StringT::value_type, StringT::traits_type, StringT::allocator_type>;
	return static_cast<stream_t &>(stream_t() << from).str();
}


#endif  // STRINGS_HPP
