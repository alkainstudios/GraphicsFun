#include "Sprite.h"
#include "Vertex.h"

#include <cstddef>

Sprite::Sprite()
{
	_vboID = 0;
}

Sprite::~Sprite()
{
	if (_vboID != 0) {
		glDeleteBuffers(1, &_vboID);
	}
}

void Sprite::initSprite(float x, float y, float width, float height) {
	// Set Private Variables
	_x = x;
	_y = y;
	_width = width;
	_height = height;

	if (_vboID == 0) {
		glGenBuffers(1, &_vboID);
	}

	Vertex vertextData[6];

#pragma region Vert_Sets
	// First Triangle
	vertextData[0].setPosition(x + width, y + height);
	vertextData[1].setPosition(x, y + height);
	vertextData[2].setPosition(x, y);

	// Second Triangle
	vertextData[3].setPosition(x, y);
	vertextData[4].setPosition(x + width, y);
	vertextData[5].setPosition(x + width, y + height);

#pragma endregion

#pragma region UV_Sets
	// First Triangle
	vertextData[0].setUV(1.0f, 1.0f);
	vertextData[1].setUV(0.0f, 1.0f);
	vertextData[2].setUV(0.0f, 0.0f);

	// Second Triangle
	vertextData[3].setUV(0.0f, 0.0f);
	vertextData[4].setUV(1.0f, 0.0f);
	vertextData[5].setUV(1.0f, 1.0f);

#pragma endregion
	
#pragma region Colors
	for (int i = 0; i < 6; i++)
	{
		vertextData[1].setColor(200, 0, 200, 255);
	}
	
	vertextData[1].setColor(0, 0, 150, 255);

	vertextData[4].setColor(0, 150, 0, 255);
	

#pragma endregion


	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertextData), vertextData, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Sprite::drawSprite() {
	//
	glBindBuffer(GL_ARRAY_BUFFER, _vboID);
	
	//
	glEnableVertexAttribArray(0);

	// This is the position attribute pointer
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));

	// This is the color attribute pointer
	glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	// This is the color attribute pointer
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

	//
	glDrawArrays(GL_TRIANGLES, 0, 6);

	//
	glDisableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}