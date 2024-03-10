#pragma once
#include <map>
struct Input
{

    std::map<int, bool> keys;

    bool isKeyPressed(int key)
    {
        if (keys[key])
        {
			keys[key] = false;
			return true;
		}
		return false;
	}

    int keyHit;
    int scancode;
    int action;
    int mods;
};