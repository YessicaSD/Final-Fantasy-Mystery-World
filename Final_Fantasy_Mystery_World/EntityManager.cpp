#include "j1App.h"
#include "j1Render.h"
#include "EntityManager.h"
#include "j1Textures.h"
#include "Entity.h"
#include "j1Audio.h"
#include "j1Window.h"
#include "p2Log.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "Player.h"
#include "Enemy.h"



EntityManager::EntityManager() : j1Module()
{
	name.assign("entity_manager");
	

}

// Destructor
EntityManager::~EntityManager()
{

}

bool EntityManager::Awake(pugi::xml_node& config)
{
	bool ret = true;


	return ret;
}

bool EntityManager::Start()
{
	bool ret = true;


	texture = App->tex->Load("textures/Animist.png");

	return ret;
}

bool EntityManager::PreUpdate()
{
	
	std::list<Entity*>::iterator item = entities.begin();
	for (; item != entities.end(); ++item) {
		if ((*item) != nullptr) {
			if ((*item)->has_turn) {
				(*item)->PreUpdate();
			}
		}
	}
	
	return true;
}

// Called before render is available
bool EntityManager::Update(float dt)
{
	std::list<Entity*>::iterator item = entities.begin();
	for (; item != entities.end(); ++item) {
		if ((*item) != nullptr)
				(*item)->Update(dt);

	}
	item = entities.begin();
	for (; item != entities.end(); ++item) {
		if ((*item) != nullptr)
			(*item)->Draw(texture, dt);
	}
	
	return true;
}

bool EntityManager::PostUpdate()
{
	std::list<Entity*>::iterator item = entities.begin();
	for (; item != entities.end(); ++item) {
		if ((*item) != nullptr)
				(*item)->PostUpdate();

	}
	return true;
}


// Called before quitting
bool EntityManager::CleanUp()
{
	std::list<Entity*>::iterator item = entities.begin();
	for (; item != entities.end(); ++item) {
		if ((*item) != nullptr) {
			(*item)->CleanUp();
			delete(*item);
			(*item) = nullptr;
			//entities.erase(item);
		}
	}
	entities.clear();
	App->tex->UnLoad(texture);
	return true;
}




Player * EntityManager::CreatePlayer()
{
	Player* ret = nullptr;

	ret = new Player();
	ret->type = EntityType::PLAYER;
	ret->has_turn = true;
	if (ret != nullptr)
		entities.push_back(ret);

	return ret;
}

Enemy * EntityManager::CreateEnemy()
{
	Enemy* ret = nullptr;

	ret = new Enemy();
	ret->type = EntityType::ENEMY;
	ret->has_turn = false;
	if (ret != nullptr)
		entities.push_back(ret);

	return ret;
}

void EntityManager::DeleteEntities()
{

	std::list<Entity*>::iterator item = entities.begin();
	for (; item != entities.end(); ++item) {
		if ((*item) != nullptr) {
			(*item)->CleanUp();
			delete(*item);
			(*item) = nullptr;
			//entities.erase(item);
		}
	}
	entities.clear();


}

Player* EntityManager::GetPlayerData() const {

	std::list<Entity*>::const_iterator item = entities.begin();
	for (; item != entities.end(); ++item) {
		if ((*item) != nullptr)
		{
			if ((*item)->type == EntityType::PLAYER)
				return (Player*)(*item);
		}
	}

	return nullptr;
}

const std::list<Entity*> EntityManager::GetEntities()
{
	return entities;
}

bool EntityManager::Load(pugi::xml_node& load)
{
	bool ret = true;

	
	return ret;
}

bool EntityManager::Save(pugi::xml_node& save) const
{
	bool ret = true;
	
	return ret;
}





