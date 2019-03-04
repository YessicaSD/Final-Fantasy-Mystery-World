#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1PathFinding.h"


j1PathFinding::j1PathFinding() : j1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH),width(0), height(0)
{
	name.assign("pathfinding");
}

// Destructor
j1PathFinding::~j1PathFinding()
{
	RELEASE_ARRAY(map);
}

// Called before quitting
bool j1PathFinding::CleanUp()
{
	LOG("Freeing pathfinding library");

	last_path.clear();
	RELEASE_ARRAY(map);
	return true;
}

// Sets up the walkability map
void j1PathFinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	memcpy(map, data, width*height);
}

// Utility: return true if pos is inside the map boundaries
bool j1PathFinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width &&
			pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true is the tile is walkable
bool j1PathFinding::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}

// Utility: return the walkability value of a tile
uchar j1PathFinding::GetTileAt(const iPoint& pos) const
{
	if(CheckBoundaries(pos))
		return map[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}

// To request all tiles involved in the last generated path
const std::list<iPoint>* j1PathFinding::GetLastPath() const
{
	return &last_path;
}

// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
std::list<PathNode>* PathList::Find(const iPoint& point) const
{
	std::list<PathNode>::const_iterator item = list.begin();
	while(item != list.end())
	{
		if ((*item).pos == point) {
			std::list<PathNode> item2;
			item2.push_back(*item);
			
			return &item2;
		}
		
		++item;
	}
	return NULL;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
std::list<PathNode>* PathList::GetNodeLowestScore() const
{
	std::list<PathNode>* ret = NULL;
	int min = 65535;

	std::list<PathNode>::const_reverse_iterator item = list.crbegin();
	while(item != list.crend())
	{
		if((*item).Score() < min)
		{
			min = (*item).Score();
			std::list<PathNode> item2;
			item2.push_back(*item);
			ret = &item2;
		}
		++item;
	}
	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

// PathNode -------------------------------------------------------------------------
// Fills a list (PathList) of all valid adjacent pathnodes
// ----------------------------------------------------------------------------------
uint PathNode::FindWalkableAdjacents(PathList& list_to_fill) const
{
	iPoint cell;
	uint before = list_to_fill.list.size();

	cell.create(pos.x, pos.y + 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	// south
	cell.create(pos.x, pos.y - 1);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	// east
	cell.create(pos.x + 1, pos.y);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	// west
	cell.create(pos.x - 1, pos.y);
	if (App->pathfinding->IsWalkable(cell))
		list_to_fill.list.push_back(PathNode(-1, -1, cell, this));

	return list_to_fill.list.size();
}

// PathNode -------------------------------------------------------------------------
// Calculates this tile score
// ----------------------------------------------------------------------------------
int PathNode::Score() const
{
	return g + h;
}

// PathNode -------------------------------------------------------------------------
// Calculate the F for a specific destination tile
// ----------------------------------------------------------------------------------
int PathNode::CalculateF(const iPoint& destination)
{
	g = parent->g + 1;
	h = pos.DistanceTo(destination);

	return g + h;
}

// ----------------------------------------------------------------------------------
// Actual A* algorithm: return number of steps in the creation of the path or -1 ----
// ----------------------------------------------------------------------------------
int j1PathFinding::CreatePath(const iPoint& origin, const iPoint& destination)
{
	last_path.clear();
	// TODO 1: if origin or destination are not walkable, return -1

	if (!IsWalkable(origin) || !IsWalkable(destination))
		return -1;

	// TODO 2: Create two lists: open, close
	// Add the origin tile to open
	// Iterate while we have tile in the open list

	PathList open, close;
	PathNode Origin(0, origin.DistanceNoSqrt(destination), origin, NULL);
	open.list.push_back(Origin);

	while (open.list.size() > 0) {
		// TODO 3: Move the lowest score cell from open list to the closed list
		PathNode* current = (PathNode*)open.GetNodeLowestScore();
		close.list.push_back(*current);
		std::list<PathNode>::iterator item = open.list.begin();
		while (item != open.list.end()) {

			if (&(*item) == &(*current))
				break;
			++item;
		}
		open.list.erase(item);
		if (close.list.back().pos == destination) {
			// TODO 4: If we just added the destination, we are done!
			// Backtrack to create the final path
			// Use the Pathnode::parent and Flip() the path when you are finish
			std::list<PathNode>::reverse_iterator item = close.list.rbegin();
			for (; item != close.list.rend(); ++item) {
				last_path.push_back((*item).pos);
				if ((*item).parent == NULL)
					last_path.push_back((*item).pos);
			}
			last_path.push_back((*item).pos);
			last_path.reverse(); //SHA DE MIRAR AIXO!!!!!!!!!!!!!!!!!!!!!!!
			break;
		}
		else if (close.list.back().pos != destination) {
			// TODO 5: Fill a list of all adjancent nodes
			PathList AdjacentsNodes;
			close.list.back().FindWalkableAdjacents(AdjacentsNodes);

			// TODO 6: Iterate adjancent nodes:
			// ignore nodes in the closed list
			// If it is NOT found, calculate its F and add it to the open list
			// If it is already in the open list, check if it is a better path (compare G)
			// If it is a better path, Update the parent
			std::list<PathNode>::iterator item = AdjacentsNodes.list.begin();
			while (item != AdjacentsNodes.list.end()) {
				bool InCloseList = false;
				if (close.Find((*item).pos)) {
					InCloseList = true;
				}
				if (!InCloseList) {
					if (open.Find((*item).pos))
					{
						std::list<PathNode> node = *open.Find((*item).pos);
						if ((*item).g < node.back().g)
							node.back().parent = (*item).parent;
					}
					else {
						(*item).CalculateF(destination);
						open.list.push_back((*item));
					}
				}
				++item;
			}
		}
	}
	return 1;
}
