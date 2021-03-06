#ifndef __DYNAMICENTITY_H__
#define __DYNAMICENTITY_H__

#include "p2Point.h"
#include <string>
#include "p2Animation.h"
#include "PugiXml\src\pugixml.hpp"
#include "p2PerfTimer.h"
#include "e1Entity.h"

enum class Attacks {
	BASIC, NONE
};

enum class Direction {
	UP, DOWN, RIGHT, LEFT, UP_LEFT, DOWN_LEFT, UP_RIGHT, DOWN_RIGHT, NONE
};

class e1DynamicEntity  : public e1Entity
{
public:

	e1DynamicEntity(const int &x, const int &y);
	virtual ~e1DynamicEntity();
	
	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool PostUpdate() { return true; };
	virtual bool CleanUp() { return true; };
	
	virtual bool Load(pugi::xml_node&) { return true; };
	virtual bool Save(pugi::xml_node&) const { return true; };

	void ChangeTurn(EntityType type);

	void PushBack();

	bool NextTileFree(const Direction & dir) const;

	void RestTimeAfterAttack(float time_finish);

	void CheckBasicAttackEfects(const e1Entity::EntityType &type, const Direction & direction, const int &attack_damage);

	void ChangeAnimation(Direction &dir, State &states, Attacks attacks = Attacks::NONE);

	void ResetAnims();

public:

	Direction direction = Direction::NONE;
	State state = State::NONE;

	Animation GoDownLeft;
	Animation IdleDownLeft;
	Animation BasicAttackDownLeft;

	Animation GoDownRight;
	Animation IdleDownRight;
	Animation BasicAttackDownRight;

	Animation GoUpRight;
	Animation IdleUpRight;
	Animation BasicAttackUpRight;

	Animation GoUpLeft;
	Animation IdleUpLeft;
	Animation BasicAttackUpLeft;

	Animation GoLeft;
	Animation IdleLeft;
	Animation BasicAttackLeft;

	Animation GoRight;
	Animation IdleRight;
	Animation BasicAttackRight;

	Animation GoUp;
	Animation IdleUp;
	Animation BasicAttackUp;

	Animation GoDown;
	Animation IdleDown;
	Animation BasicAttackDown;

	iPoint velocity;

	iPoint target_position;
	iPoint initial_position; //IMPORTANT: SEMPRE QUE ES CARREGUI UN NOU MAPA AQUESTA VARIABLE SHA DIGUALAR A LA POSICIO INICIAL!!
	iPoint movement_count; //IMPORTANT: SEMPRE QUE ES CARREGUI UN NOU MAPA AQUESTA VARIABLE SHA DE POSAR A 0!! 


	SDL_Texture * ground = nullptr;

	Attacks type_attack = Attacks::NONE;

	float time_attack;
	float time_after_attack = 500;

};

#endif
