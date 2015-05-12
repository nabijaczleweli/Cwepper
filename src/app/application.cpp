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
#include <chrono>


using namespace sf;
using namespace std;
using namespace std::chrono;
using namespace std::chrono_literals;
using namespace cpponfig;


application::application() : force_redraw(true) {}

application::~application() {}

int application::run() {
	window.create(VideoMode::getDesktopMode(), app_name, Style::Fullscreen);

	const Image & icon = main_texture_loader(textures_root + "/gui/general/window_main.png");
	window.setIcon(icon.getSize().x, icon.getSize().x, icon.getPixelsPtr());

	return loop();
}

int application::loop() {
	thread([&]() {
		while(window.isOpen()) {
			const bool wasforced = force_redraw;
			const auto start = high_resolution_clock::now();

			// Handle it somehow? Draw big text on screen? Turn into future and read every event poll?
			if(const int i = draw())
				throw i;

			const auto end = high_resolution_clock::now();
			const auto stepped_sleep_duration = chrono::milliseconds(duration_cast<chrono::milliseconds>(1s - (end - start)).count() / 10);

			for(unsigned int i = 0; !force_redraw && i < 10; ++i)
				this_thread::sleep_for(stepped_sleep_duration);
			if(wasforced)
				force_redraw = false;
		}

		return 0;
	}).detach();

	window.setActive(false);
	while(window.isOpen()) {
		Event event;
		while(window.waitEvent(event))
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
				case Event::GainedFocus :
				case Event::LostFocus :
					schedule_redraw();
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

void application::schedule_redraw() {
	force_redraw = true;
}

void application::config(configuration &) {}
