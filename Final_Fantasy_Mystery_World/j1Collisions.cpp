#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collisions.h"

#include "Brofiler/Brofiler.h"

j1Collision::j1Collision()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
		colliders[i] = nullptr;

	matrix[COLLIDER_PLAYER][COLLIDER_WALL] = true;
	matrix[COLLIDER_WALL][COLLIDER_PLAYER] = true;

	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_WALL][COLLIDER_WALL] = false;
}

// Destructor
j1Collision::~j1Collision()
{}

bool j1Collision::PreUpdate()
{
	BROFILER_CATEGORY("Collisions: PreUpdate", Profiler::Color::Magenta);
	// Remove all colliders scheduled for deletion
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	// Test all collisions
	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		// skip empty colliders
		if (colliders[i] == nullptr)
			continue;

		c1 = colliders[i];

		// avoid checking collisions already checked
		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			// skip empty colliders
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			switch (c1->type) {
			case COLLIDER_PLAYER: {
				if (c1->CheckCollision(c2->line) == true) {
					if (matrix[c1->type][c2->type] && c1->callback)
						c1->callback->OnCollision(c1, c2);

					if (matrix[c2->type][c1->type] && c2->callback)
						c2->callback->OnCollision(c2, c1);
				}
			} break;
			default: {
				if (c1->CheckCollision(c2->rect) == true) {
					if (matrix[c1->type][c2->type] && c1->callback)
						c1->callback->OnCollision(c1, c2);

					if (matrix[c2->type][c1->type] && c2->callback)
						c2->callback->OnCollision(c2, c1);
				}
			} break;
			}
		}
	}

	return true;
}

// Called before render is available
bool j1Collision::Update(float dt)
{
	BROFILER_CATEGORY("Collisions: Update", Profiler::Color::Magenta);
	if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN)
		debug = !debug;
	DebugDraw();


	return true;
}

void j1Collision::DebugDraw()
{

	if (debug == false)
		return;

	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case COLLIDER_NONE: // white
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_PLAYER: // white
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 255, alpha);
			break;
		case COLLIDER_WALL: // white
			App->render->DrawLine(colliders[i]->line.x1, colliders[i]->line.y1, colliders[i]->line.x2, colliders[i]->line.y2, 0, 0, 255, alpha);
			break;
		}

	}

}



// Called before quitting
bool j1Collision::CleanUp()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

Collider* j1Collision::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}

	return ret;
}

Collider * j1Collision::AddCollider(iLine line, COLLIDER_TYPE type, j1Module * callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(line, type, callback);
			break;
		}
	}

	return ret;
}



// -----------------------------------------------------

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	bool ret;

	if (!HasIntersection(r.x, r.y, r.x + r.w, r.y, line.x1, line.y1, line.x2, line.y2) && !HasIntersection(r.x, r.y, r.x, r.y + r.h, line.x1, line.y1, line.x2, line.y2)
		&& !HasIntersection(r.x + r.w, r.y, r.x + r.w, r.y + r.h, line.x1, line.y1, line.x2, line.y2) && !HasIntersection(r.x, r.y + r.h, r.x + r.w, r.y + r.h, line.x1, line.y1, line.x2, line.y2)) {
		ret = false;
	}
	else {
		ret = true;
	}

	return ret;
}

bool Collider::CheckCollision(const iLine & l) const
{
	bool ret;

	if (!HasIntersection(rect.x, rect.y, rect.x + rect.w, rect.y, l.x1, l.y1, l.x2, l.y2) && !HasIntersection(rect.x, rect.y, rect.x, rect.y + rect.h, l.x1, l.y1, l.x2, l.y2)
		&& !HasIntersection(rect.x + rect.w, rect.y, rect.x + rect.w, rect.y + rect.h, l.x1, l.y1, l.x2, l.y2) && !HasIntersection(rect.x, rect.y + rect.h, rect.x + rect.w, rect.y + rect.h, l.x1, l.y1, l.x2, l.y2)) {
		ret = false;
	}
	else {
		ret = true;
	}

	return ret;
}



bool Collider::HasIntersection(const int &x1_1, const int &y1_1, const int &x2_1, const int &y2_1, const int &x1_2, const int& y1_2, const int &x2_2, const int &y2_2) const
{
	float ax = x2_1 - x1_1;    
	float ay = y2_1 - y1_1;  

	float bx = x1_2 - x2_2; 
	float by = y1_2 - y2_2;    

	float dx = x1_2 - x1_1;   
	float dy = y2_2 - y2_1;

	float det = ax * by - ay * bx;

	if (det == 0) {
		return false;
	}

	float r = (dx * by - dy * bx) / det;
	float s = (ax * dy - ay * dx) / det;

	return !(r < 0 || r > 1 || s < 0 || s > 1);
}
