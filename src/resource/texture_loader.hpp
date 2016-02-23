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


#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <memory>
#include <string>


class image_loader;


class texture_loader {
private:
	class image_caching_texture : public virtual sf::Texture {
	private:
		texture_loader & loader;

	public:
		explicit image_caching_texture(texture_loader & ldr);
		image_caching_texture(const image_caching_texture &) = default;
		image_caching_texture(image_caching_texture &&) = default;

		virtual bool loadFromFile(const std::string & filename, const sf::IntRect & area = sf::IntRect()) /*override*/;
	};


	std::unordered_map<std::string, std::unique_ptr<image_caching_texture>> cache;
	image_loader & image_cache;

public:
	explicit texture_loader(image_loader & ldr);
	texture_loader(const texture_loader &) = default;
	texture_loader(texture_loader &&) = default;

	sf::Texture & operator[](const std::string & filename);
};


