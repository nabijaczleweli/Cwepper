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


#include "localizer.hpp"
#include "../reference/container.hpp"
#include "../util/file.hpp"
#include <SFML/System.hpp>
#include <fstream>
#include <algorithm>


using namespace std;
using namespace sf;

using string_t = localizer::string_t;
using stream_t = localizer::stream_t;


vector<string> available_languages(const string & root) {
	auto files(list_files(root));

	files.erase(remove_if(files.begin(), files.end(),
	                      [&](const auto & name) {
		                      static const auto extension_index = localizer::default_locale.size() + 1;
		                      static const auto name_length     = localizer::default_locale.size() + 1 + localizer::file_extension.size();

		                      return name.size() != name_length || name.find(localizer::file_extension) != extension_index;
		                    }),
	            files.end());

	transform(files.begin(), files.end(), files.begin(), [&](const auto & name) {
		static const auto whole_extension_index = localizer::default_locale.size();

		return name.substr(0, whole_extension_index);
	});

	return files;
}


string localizer::default_locale                     = "en_US";
string localizer::file_extension                     = "lang";
string_t::value_type localizer::assignment_character = L'=';
string_t::value_type localizer::comment_character    = L'#';


localizer::localizer() : localizer(default_locale) {}

localizer::localizer(nothrow_t) {}

localizer::localizer(stream_t & in) {
	init(in);
}

localizer::localizer(const string & locale) {
	basic_ifstream<stream_t::char_type, stream_t::traits_type> in(localization_root + '/' + locale + '.' + file_extension);
	if(in.is_open())
		init(in);
}

localizer::localizer(const localizer & loc0, const localizer & loc1) : localizer(loc0) {
	merge(loc1);
}

/** Adds keys from `loc` for which there are no elements in `this` */
localizer & localizer::merge(const localizer & loc) {
	language.insert(loc.language.begin(), loc.language.cend());
	return *this;
}

localizer & localizer::open(const string & loc) {
	return merge(localizer(loc));
}

bool localizer::operator==(const localizer & loc) {
	return language == loc.language;
}

bool localizer::operator!=(const localizer & loc) {
	return language != loc.language;
}

void localizer::init(stream_t & in) {
	for(string line; getline(in, line);) {
		if(!line.size())
			continue;

		ltrim(line);
		if(!line.size() || line[0] == comment_character)
			continue;

		const auto idx = line.find('=');
		if(idx == string_t::npos)
			continue;

		string_t localized;
		Uint32 local_char;
		for(auto itr = line.begin(); itr != line.end();) {
			itr = Utf8::decode(itr, line.end(), local_char);
			localized += local_char;
		}

		language.emplace(rtrim(localized.substr(0, idx)), trim(localized.substr(idx + 1)));
	}
}

bool localizer::empty() const {
	return language.empty();
}

bool localizer::can_translate(const string_t & key) const {
	return !language.empty() && language.find(key) != language.end();
}

const string_t & localizer::translate(const string_t & key) const {
	const auto & itr = language.find(key);
	if(itr == language.end())
		return key;
	else
		return itr->second;
}
