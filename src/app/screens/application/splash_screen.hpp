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


#include "../screen.hpp"
#include <functional>
#include <SFML/Graphics.hpp>


/**
 * Design philosophy: the launched timing thread is guaranteed to have life >= of that of splash_screen,
 * which means, that the `ended` ref, used to communicate with that thread, needs to be taken from therein,
 * rather than passing the ref to splash_screen's `ended`, which'll be destroyed.
 *
 * So, rough draft:
 *   thread spawned              -> give it ref to `ended`, make it point to thread's local -- now guaranteed to be valid;
 *   interrupted by key or mouse -> tell it to thread, will simply not force new screen after sleeping for long enough;
 *   not interrupted             -> thread enforces new screen.
 */
class splash_screen : public screen {
private:
	sf::Sprite background;
	sf::Text text;
	std::reference_wrapper<volatile bool> ended;

	void end();

public:
	virtual void setup() override;
	virtual int draw() override;
	virtual int handle_event(const sf::Event & event) override;

	splash_screen(application & theapp);
	virtual ~splash_screen();
};
