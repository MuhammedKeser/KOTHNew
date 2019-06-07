#pragma once

struct Vector
{
	LONG x;
	LONG y;
	Vector(long x, long y)
	{
		this->x = x;
		this->y = y;
	};
	Vector()
	{
		this->x = 0;
		this->y = 0;
		
	};
};

class Camera
{
//Members
protected:
	Vector m_position;
//Constructors
public:
	Camera(LONG x, LONG y) { m_position.x = x; m_position.y = y;};
	Camera() {};
//GETTERS + SETTERS
public:
	void SetPosition(POINT newPosition) { m_position.x = newPosition.x; m_position.y = newPosition.y; };
	void SetPosition(LONG x, LONG y) { m_position.x = x; m_position.y = y; };
	Vector GetPosition() { return m_position; };
};

