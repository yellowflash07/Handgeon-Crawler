#pragma once

class iCharacter
{
public:
	iCharacter() {};
	virtual ~iCharacter() {};

	virtual void Update(float deltaTime) = 0;
};