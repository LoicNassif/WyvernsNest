#include "attack.hpp"

#include "unit.hpp"
#include "../combat.hpp"

Attack::Attack(std::string name,
	Unit * source,
	AttackType type,
	AttackRange range,
	AttackEffect * effect,
	AttackAoE aoe,
	bool affect_self)
	:
	name(name),
	source(source),
	type(type),
	range(range),
	effect(effect),
	aoe(aoe),
	affect_self(affect_self),
	validSprite("res/assets/valid.png"),
	targetValidSprite("res/assets/valid_circle.png"),
	targetInvalidSprite("res/assets/invalid_circle.png")
{
	
}

void Attack::attack(ScreenCoord pos, Combat& combat) {
	switch (type) {
	case AttackType::SELF: {
		if (affect_self) {
			effect->attack(source->position, combat);
		}
		attackAoE(source->position, combat);
	} break;
	case AttackType::MELEE: {
		// TODO: Make sure the attack is valid before running it
		effect->attack(pos, combat);
		attackAoE(pos, combat);
	} break;
	case AttackType::RANGED: {
		// TODO: Make sure the attack is valid before running it
		effect->attack(pos, combat);
		attackAoE(pos, combat);
	} break;
	}
	// TODO: figure out how to apply the attack effect to the surrounding aoe
}

// Display the valid attack tiles on the grid
void Attack::renderValidGrid() {
	switch (type) {
	case AttackType::SELF: {
		// TODO: Somehow display the valid sprite
	} break;
	case AttackType::MELEE: {
		validSprite.setPos((source->position.x() - 1) * tile_width, source->position.y() * tile_height);
		validSprite.render();
		validSprite.setPos(source->position.x() * tile_width, (source->position.y() - 1) * tile_height);
		validSprite.render();
		validSprite.setPos((source->position.x() + 1) * tile_width, source->position.y() * tile_height);
		validSprite.render();
		validSprite.setPos(source->position.x() * tile_width, (source->position.y() + 1) * tile_height);
		validSprite.render();
	} break;
	case AttackType::RANGED: {
		for (int y = -range; y < range + 1 ; y++) {
			for (int x = -range; x < range + 1 ; x++) {
				if (!(y == x && y == 0)) {
					if (abs(x) + abs(y) <= range) {
						validSprite.setPos((source->position.x() + x) * tile_width, (source->position.y() + y) * tile_height);
						validSprite.render();
					}
				}
			}
		}
	} break;
	}

	int mouseX; int mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	int x = static_cast<int>(floor(mouseX / tile_width));
	int y = static_cast<int>(floor(mouseY / tile_height));

	if (isValid(ScreenCoord(x, y))) {
		targetValidSprite.setPos(x * tile_width, y * tile_height);
		targetValidSprite.render();
	}
	else {
		targetInvalidSprite.setPos(x * tile_width, y * tile_height);
		targetInvalidSprite.render();
	}
	// TODO: get the mouse position somehow to render whether the target position is valid
	/*
	if (isValid(pos)) {
		//TODO values here are temporary
		targetValidprite.setPos(pos.x() * 213, pos.y() * 180);
		targetValidprite.setSize(213, 180);
		targetValidprite.render();
	}
	else {
		//TODO values here are temporary
		targetInvalidSprite.setPos(pos.x() * 213, pos.y() * 180);
		targetInvalidSprite.setSize(213, 180);
		targetInvalidSprite.render();
	}
	*/
}

bool Attack::isValid(ScreenCoord pos) {
	switch (type) {
	case AttackType::SELF: {
		return pos == source->position;
	} break;
	case AttackType::MELEE: {
		int x_diff = std::abs(pos.x() - source->position.x());
		int y_diff = std::abs(pos.y() - source->position.y());
		if (x_diff + y_diff == 1) return true;
		return false;
		/*
		if ((pos.x() == source->position.x() + 1 || pos.x() == source->position.x() - 1) && pos.y() == source->position.y()) {
			return true;
		}
		else if ((pos.y() == source->position.y() + 1 || pos.y() == source->position.y() - 1) && pos.x() == source->position.x()) {
			return true;
		}
		*/
	} break;
	case AttackType::RANGED: {
		int x_diff = std::abs(pos.x() - source->position.x());
		int y_diff = std::abs(pos.y() - source->position.y());
		if (x_diff + y_diff <= range) return true;
		return false;
	} break;
	}
	return false;
}

void Attack::setTileSize(int width, int height) {
	tile_width = width;
	tile_height = height;
	validSprite.setSize(tile_width, tile_height);
	targetValidSprite.setSize(tile_width, tile_height);
	targetInvalidSprite.setSize(tile_width, tile_height);
}
	
void Attack::attackAoE(ScreenCoord pos, Combat & combat) {
	for (int i = -aoe; i <= aoe; ++i) {
		int height = aoe - std::abs(i);
		for (int j = -height; j <= height; ++j) {
			// Apply the attack effect on grid cell [i, j]
			if (!(Vec2<int>(i, j) == Vec2<int>(0, 0))) {
				effect->attack(source->position + Vec2<int>(i, j), combat);
			}
		}
	}
}

void DamageEffect::attack(ScreenCoord pos, Combat & combat) {
	Unit * unit = combat.getUnitAt(pos);
	if (unit) {
		unit->takeDamage(damage);
		// Do something if the unit dies
	}
}