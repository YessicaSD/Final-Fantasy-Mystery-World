#ifndef __ENTITY_H__
#define __ENTITY_H__

#include "p2Point.h"
#include <string>
#include "p2Animation.h"
#include "PugiXml\src\pugixml.hpp"
#include "j1PerfTimer.h"


enum class EntityType;
struct SDL_Texture;

enum class State {
	IDLE, WALKING
};

struct TileSetEntity {

	SDL_Rect GetTileRect(int id) const;

	std::string name;
	uint tilewidth = 0;
	uint tileheight = 0;
	uint spacing = 0;
	uint margin = 0;
	uint tilecount = 0;
	uint columns = 0;
	std::string imagePath;
	SDL_Texture* texture = nullptr;
	uint width = 0;
	uint height = 0;
};

struct EntityAnim {
	uint id = 0;
	uint num_frames = 0;
	SDL_Rect* frames = nullptr;
	State animType;

	uint FrameCount(pugi::xml_node&);
};

struct EntityInfo {
	TileSetEntity tileset;
	EntityAnim* animations = nullptr;
	uint num_animations = 0;
};

class Entity
{
public:

	Entity();
	virtual ~Entity();

	bool LoadEntityData(const char*);
	//virtual void LoadProperties(pugi::xml_node&);
	virtual void IdAnimToEnum();
	virtual void PushBack() {};

	virtual bool PreUpdate() { return true; };
	virtual bool Update(float dt) { return true; };
	virtual bool PostUpdate() { return true; };
	virtual bool CleanUp() { return true; };
	virtual void Draw(SDL_Texture* tex, float dt);

	virtual bool Load(pugi::xml_node&) { return true; };
	virtual bool Save(pugi::xml_node&) const { return true; };

	void LoadXML(std::string name_xml_file);
	Animation LoadPushbacks(pugi::xml_node&, std::string NameAnim) const;

public:

	EntityType type;
	EntityInfo data;

	iPoint position;

	Animation* current_animation = nullptr;

	pugi::xml_document	config_file;
	pugi::xml_document	entity_file;
	pugi::xml_node		config;
	pugi::xml_node		node;


	bool has_turn;

};

#endif
