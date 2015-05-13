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
#include "image_loader.hpp"


using namespace sf;
using namespace std;


texture_loader::texture_loader(image_loader & ldr) : image_cache(ldr) {}

Texture & texture_loader::operator[](const string & filename) {
	auto itr = cache.find(filename);
	if(itr == cache.end()) {
		auto txt = make_unique<image_caching_texture>(*this);
		txt->loadFromFile(filename);
		itr = cache.emplace(filename, move(txt)).first;
	}
	return *itr->second;
}


texture_loader::image_caching_texture::image_caching_texture(texture_loader & ldr) : Texture(), loader(ldr) {}

bool texture_loader::image_caching_texture::loadFromFile(const string & filename, const IntRect & area) {
	return loadFromImage(loader.image_cache[filename], area);
}
