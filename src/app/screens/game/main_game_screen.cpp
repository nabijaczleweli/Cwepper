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
#include <array>
#include <iostream>


using namespace std;
using namespace sf;


struct generator {
	unsigned int idx;
	unsigned int radius;
	float step;

	generator(unsigned int rds, float stp) : idx(0), radius(rds), step(stp) {}

	Vertex operator()() {
		const float i = idx * step;
		const Vertex temp({radius + radius * cos(i), radius + radius * sin(i)});
		++idx;
		return temp;
	}
};


void main_game_screen::config(cpponfig::configuration &) {}

void main_game_screen::setup() {
	screen::setup();
}

int main_game_screen::draw() {
	Sprite circle(points.getTexture());
	circle.setPosition(pos);
	window.draw(circle);
	return 0;
}

int main_game_screen::handle_event(const Event & event) {
	if(int i = screen::handle_event(event))
		return i;

	if(event.type == Event::MouseMoved) {
		pos = Vector2f(event.mouseMove.x, event.mouseMove.y);
		app.schedule_redraw();
	} else if(event.type == Event::MouseButtonPressed) {
		const unsigned int radius = sqrt(event.mouseButton.x * event.mouseButton.x + event.mouseButton.y * event.mouseButton.y);
		const unsigned int diameter = 2 * radius;
		points.create(diameter ? diameter : 1, diameter ? diameter : 1);
		points.setSmooth(true);
		points.clear(Color::Transparent);
		array<Vertex, 62832> vertices;  // Magic number here = (tau / .0001) rounded up
		generate(vertices.begin(), vertices.end(), generator(radius, .0001));
		points.draw(vertices.data(), vertices.size(), PrimitiveType::LinesStrip);
		points.display();
	}

	return 0;
}

main_game_screen::main_game_screen(application & theapp) : screen(theapp), configurable() {}
main_game_screen::main_game_screen(const main_game_screen & other) : screen(other), configurable(other) {}
main_game_screen::main_game_screen(main_game_screen && other) : screen(move(other)), configurable(move(other)) {}

main_game_screen::~main_game_screen() {}
