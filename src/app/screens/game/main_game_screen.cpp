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


#include "main_game_screen.hpp"
#include "../../../reference/container.hpp"
#include "../../../resource/localizer.hpp"
#include "../../application.hpp"


using namespace std;
using namespace sf;


void main_game_screen::setup() {
	screen::setup();
}

int main_game_screen::draw() {
	if(!map_sprite.getTexture()) {
		map_texture.create(window.getSize().x * .6f, window.getSize().y * 99.f / 100.f);
		map_sprite.setPosition(static_cast<Vector2f>(window.getSize()) / 200.f);
		map_sprite.setTexture(map_texture.getTexture());
	}

	map_texture.draw(map);
	map_texture.display();
	window.draw(map_sprite);
	return 0;
}

int main_game_screen::handle_event(const Event & event) {
	if(int i = screen::handle_event(event))
		return i;

	if(event.type == Event::MouseButtonPressed) {
		const auto interpolated = Vector2f(event.mouseButton.x, event.mouseButton.y) - map_sprite.getPosition();
		if(map_sprite.getGlobalBounds().contains(interpolated)) {
			map.click(interpolated.x, interpolated.y);

			app.schedule_redraw();
		}
	}

	return 0;
}

main_game_screen::main_game_screen(application & theapp) : screen(theapp), map(100, 100, window.getSize()) {}
