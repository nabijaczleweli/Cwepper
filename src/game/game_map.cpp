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


#include "game_map.hpp"
#include "../reference/container.hpp"
#include "cppformat/format.h"
#include <stdexcept>
#include <iostream>
#include <string>


using namespace sf;
using namespace std;

using fmt::format;


static cell placeholder_cell;


void game_map::draw(RenderTarget & target, RenderStates states) const {
	target.clear(Color::Transparent);

	for(int y = 0; y < map.rows(); ++y)
		for(int x = 0; x < map.cols(); ++x)
			target.draw(map(y, x), states);
}

game_map::game_map(unsigned int width, unsigned int height, const Vector2u & destsize) : map(height, width) {
	cell_size.x = min(destsize.x / map.cols(), destsize.y / map.rows());
	cell_size.y = cell_size.x;

	bernoulli_distribution dist(app_configuration.mine_distribution);

	for(int y = 0; y < map.rows(); ++y)
		for(int x = 0; x < map.cols(); ++x)
			map(y, x) = cell(Vector2u(x, y), cell_size, [&]() { return dist(random_engine); });
}

void game_map::click(int x, int y) {
	if(cell_size.x == 0 || cell_size.y == 0)
		return;

	x = floor(x / cell_size.x);
	y = floor(y / cell_size.y);

	if(y < map.rows() && x < map.cols())
		map(y, x).click([&](int x, int y) -> const cell & {
			if(x < 0 || y < 0 || x >= map.cols() || y >= map.rows())
				throw out_of_range(format("Indices not in map! ({0} < 0 || {1} < 0 || {0} >= {2} || {1} >= {3})", x, y, map.cols(), map.rows()));
			return map(y, x);
		});
}
