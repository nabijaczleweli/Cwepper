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


void cell::draw(RenderTarget & target, RenderStates states) const {
	const Vector2f pos(indices.x * size.x, indices.y * size.y);

	if(size.x != 0 && size.y != 0) {
		if(mine_inside) {
			VertexArray var(PrimitiveType::LinesStrip, 4);
			var[0].position = {pos.x + size.x / 2, pos.y};
			var[1].position = {pos.x + size.x, pos.y + size.y};
			var[2].position = {pos.x, pos.y + size.y};
			var[3] = var[0];
			target.draw(var, states);
		}
		if(uncovered) {
			VertexArray var(PrimitiveType::LinesStrip, 4);
			var[0].position = {pos.x + size.x / 2, pos.y + size.y};
			var[1].position = {pos.x + size.x, pos.y};
			var[2].position = {pos.x, pos.y};
			var[3] = var[0];
			target.draw(var, states);
		}
	}
}

cell::cell() : mine_inside(true), uncovered(false) {}
cell::cell(const sf::Vector2u & theindices, const sf::Vector2f & thesize) : indices(theindices), size(thesize), mine_inside(true), uncovered(false) {}
