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
#include "../reference/container.hpp"
#include "cppformat/format.h"
#include <vector>
#include <map>


using namespace sf;
using namespace std;

using fmt::format;


template <class T1, class T2>
inline static auto operator*(const Vector2<T1> & lhs, const Vector2<T2> & rhs) -> Vector2<decltype(lhs.x * rhs.x)> {
	return {lhs.x * rhs.x, lhs.y * rhs.y};
}


static map<int, vector<Vector2f>> points({{1, {{.5, .5}}},
                                          {2, {{.3, .3}, {.7, .7}}},
                                          {3, {{.3, .3}, {.5, .5}, {.7, .7}}},
                                          {4, {{.3, .3}, {.7, .3}, {.7, .7}, {.3, .7}}},
                                          {5, {{.3, .3}, {.7, .3}, {.5, .5}, {.7, .7}, {.3, .7}}},
                                          {6, {{.3, .3}, {.7, .3}, {.7, .5}, {.7, .7}, {.3, .7}, {.3, .5}}},
                                          {7, {{.3, .3}, {.7, .3}, {.7, .5}, {.5, .5}, {.7, .7}, {.3, .7}, {.3, .5}}},
                                          {8, {{.3, .3}, {.5, .3}, {.7, .3}, {.7, .5}, {.7, .7}, {.3, .7}, {.3, .5}, {.5, .7}}}});


void cell::draw(RenderTarget & target, RenderStates states) const {
	const Vector2f pos = indices * size;

	if(size.x != 0 && size.y != 0) {
		RectangleShape shape(size);
		shape.setPosition(pos);
		shape.setOutlineThickness(-1);
		shape.setOutlineColor(Color(0x25, 0x25, 0x25));
		shape.setFillColor(Color::Transparent);
		target.draw(shape, states);

		if(uncovered) {
			Text mines(format("{}{}", mines_around, static_cast<int>(mine_inside)), font_7segment, size.y);
			mines.setPosition(pos);
			target.draw(mines, states);
		}
	}
}

cell::cell() : mines_around(-1), mine_inside(false), uncovered(false) {}
cell::cell(const Vector2u & theindices, const Vector2f & thesize, const function<bool()> & gen)
      : mines_around(-1), indices(theindices), size(thesize), mine_inside(gen()), uncovered(false) {}

void cell::click(const function<const cell &(int, int)> & getter) {
	uncovered = true;

	if(mines_around < 0) {
		mines_around = -mine_inside;  // Loops count everything (including self), so remove our mines

		for(int x = indices.x - 1; x <= floor(indices.x + 1); ++x)
			for(int y = indices.y - 1; y <= floor(indices.y + 1); ++y)
				try {
					mines_around += getter(x, y).mine_inside;
				} catch(const out_of_range &) {
				}
	}
}
