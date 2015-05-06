// The MIT License (MIT)

// Copyright (c) 2014 nabijaczleweli

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

#include "application.hpp"
#include "../reference/container.hpp"
#include "../util/broken_gcc.hpp"
#include "../util/configurable.hpp"
#include "../util/file.hpp"
#include <algorithm>
#include <cstdlib>
#include <SFML/System.hpp>


using namespace sf;
using namespace std;
using namespace cpponfig;


application::application() {}

application::~application() {}

int application::run() {
	window.create(VideoMode::getDesktopMode(), app_name, Style::Fullscreen);

	// Window icon's pixels aren't needed after loading them.
	Image * icon = new Image;
	if(icon->loadFromFile(textures_root + "/gui/general/window_main.png"))
		window.setIcon(icon->getSize().x, icon->getSize().x, icon->getPixelsPtr());
	delete icon; icon = nullptr;

	return loop();
}

int application::loop() {
	while(window.isOpen()) {
		if(const int i = draw())
			return i;

		Event event;
		while(window.pollEvent(event))
			switch(event.type) {
				case Event::KeyPressed :
					if(event.key.code != Keyboard::Escape)
						break;
					// Fallthrough (if Escape presed)
				case Event::Closed :
					window.close();
					break;
				case Event::MouseButtonPressed :
					window.requestFocus();
					break;
				case Event::Count :
					throw Event::Count;
				default:
					break;
			}
	}
	return 0;
}

int application::draw() {
	window.clear(Color::Black);

	// Draw stuff here!

	window.display();
	return 0;
}

void application::config(configuration &) {}
