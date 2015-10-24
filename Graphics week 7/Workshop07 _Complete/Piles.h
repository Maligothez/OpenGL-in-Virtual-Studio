#pragma once

#ifndef PILES
#define PILES

#include "Geometry.h"
#include "Entity.h"
#include "Card.h"
#include <vector>

class Pile: public Entity
{

	public:



	private:
		const float getLocationX();
	    const float getLocationZ();
		float height;
		float width;
		int power;
		int toughness;
		

}

class Graveyard: public Pile

{
	private:
		int graveCount;




}

class Deck: public Pile


{
	private:
		int deckCount;
		vector<Card> Cards;


}


class BattleSpot public Pile


{

	private:
		bool isFull = false;
		bool isAttacking = false;


}



void makeDeck
{




}
#endif

