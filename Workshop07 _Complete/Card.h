#pragma once

#ifndef CARD
#define CARD

#include "Geometry.h"
#include "Entity.h"

class Card: public Entity
{

	public:



	private:
		const float getLocationX();
	    const float getLocationZ();
		float height;
		float width;
		float power;
		float toughness;
		

}


#endif