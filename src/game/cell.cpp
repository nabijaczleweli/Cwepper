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
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNE
// FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
// COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
// IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
// CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


#include "cell.hpp"


using namespace sf;
using namespace std;


static Color half_cyan([&]() {
	Color temp(Color::Cyan);
	temp.a = 128;
	return temp;
}());


void cell::draw(RenderTarget & target, RenderStates states) const {
	const Vector2f pos(indices.x * size.x, indices.y * size.y);

	if(size.x != 0 && size.y != 0) {
		if(mine_inside) {
			CircleShape circle(1);
			circle.setPosition(pos);
			circle.setFillColor(half_cyan);
			circle.setScale(size / 2.f);
			target.draw(circle, states);
		}

		if(uncovered) {
			if(mines_around) {

			} else {
				RectangleShape rect(size);
				rect.setPosition(pos);
				rect.setFillColor(Color::Red);
				target.draw(rect, states);
			}
		}
	}
}

cell::cell() : mines_around(-1), mine_inside(false), uncovered(false) {}
cell::cell(const Vector2u & theindices, const Vector2f & thesize, const function<bool()> & gen) : mines_around(-1), indices(theindices), size(thesize),
                                                                                                  mine_inside(gen()), uncovered(false) {}

void cell::click(const function<cell &(int, int)> & getter) {
	uncovered = true;

	if(mines_around < 0) {
		mines_around = -mine_inside;  // Loops count everything (including self), so remove our mines

		for(int x = indices.x - 1; x <= static_cast<int>(indices.x) + 1; ++x)
			for(int y = indices.y - 1; y <= static_cast<int>(indices.y) + 1; ++y)
				try {
					mines_around += getter(x, y).mine_inside;
				} catch(const out_of_range & err) {}
	}
}
