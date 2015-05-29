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
#ifndef LOCALIZER_HPP
#define LOCALIZER_HPP


#include "../util/strings.hpp"
#include <iosfwd>
#include <string>
#include <unordered_map>


class localizer {
	public:
		using string_t = std::wstring;
		using stream_t = std::wistream;

	private:
		std::unordered_map<string_t, string_t> language;

		void init(stream_t & from);

	public:
		static std::string          default_locale;
		static std::string          file_extension;
		static string_t::value_type assignment_character;
		static string_t::value_type comment_character;


		localizer();
		/** Used to disambugate the opening and non-opening constructors */
		explicit localizer(nullptr_t);
		explicit localizer(stream_t & from);
		explicit localizer(const std::string & locale);
		localizer(localizer && loc);
		localizer(const localizer & loc);
		/** Equivalent to `localizer(loc0).merge(loc1)` */
		explicit localizer(const localizer & loc0, const localizer & loc1);

		/** Adds keys from `loc` for which there are no elements in `this` */
		localizer & merge(const localizer & loc);
		localizer & open(const std::string & locale = default_locale);

		bool empty() const;

		template<class StringT>
		inline bool can_translate_key(const StringT & key) const {
			return can_translate_key(to_wstring<string_t>(key));
		}

		bool can_translate_key(const string_t & key) const;

		template<class StringT>
		inline const string_t & translate_key(const StringT & key) const {
			return translate_key(to_wstring<string_t>(key));
		}

		const string_t & translate_key(const string_t & key) const;
};


#endif  // LOCALIZER_HPP
