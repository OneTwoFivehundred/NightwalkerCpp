#pragma once

class Enemy {
public:
	Enemy() {};
	virtual ~Enemy() {};

	virtual void Move() = 0;
	virtual void Attack() = 0;
	virtual void Die() = 0;
};