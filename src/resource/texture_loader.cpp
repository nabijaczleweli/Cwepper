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


#include "texture_loader.hpp"


using namespace sf;
using namespace std;


void texture_loader::load(const string & filename) {
	auto itr = cache.find(filename);
	if(itr == cache.end()) {
		caching_image img(cache);
		img.loadFromFile(filename);
	}
}

Texture & texture_loader::operator[](const string & filename) {
	load(filename);
	return cache.find(filename)->second.first;
}

Image & texture_loader::operator()(const string & filename) {
	load(filename);
	return cache.find(filename)->second.second;
}


texture_loader::caching_image::caching_image(cache_t & txts) : Image(), textures(txts) {}

bool texture_loader::caching_image::loadFromFile(const string & filename) {
	const bool result = Image::loadFromFile(filename);

	Texture txt;
	txt.loadFromImage(*this);
	textures.insert({filename, make_pair(txt, *this)});

	return result;
}
