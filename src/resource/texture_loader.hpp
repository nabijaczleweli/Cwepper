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


#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <iterator>
#include <string>


class texture_loader {
	private:
		class caching_image : public sf::Image {
			public:
				/** `typedef`ed here, since we can't use incomplete type (forward declaration) above */
				typedef std::unordered_map<std::string, std::pair<sf::Texture, caching_image>> cache_t;

			private:
				cache_t & textures;

			public:
				explicit caching_image(cache_t & txts);
				caching_image(const caching_image &) = default;
				caching_image(caching_image &&) = default;

				bool loadFromFile(const std::string & filename) /*override*/;
		};

		typedef caching_image::cache_t cache_t;

		cache_t cache;

		void load(const std::string & filename);

	public:
		sf::Texture & operator[](const std::string & filename);
		sf::Image & operator()(const std::string & filename);
};
