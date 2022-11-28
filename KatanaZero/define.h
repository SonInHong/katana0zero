#pragma once

struct WindowData
{
	HWND hwnd;
	HDC hdc;
	int height;
	int width;

	void clear()
	{
		hwnd = 0;
		hdc = 0;
		height = 0;
		width = 0;
	}

};

struct doublepoint
{
	double x;
	double y;

	doublepoint operator + (doublepoint _other)
	{
		return doublepoint{ x + _other.x, y + _other.y };
	}

	doublepoint operator - (doublepoint _other)
	{
		return doublepoint{ x - _other.x, y - _other.y };
	}

	doublepoint operator * (double a)
	{
		return doublepoint{ x * a,y * a };
	}

	doublepoint operator / (double a)
	{
		return doublepoint{ x / a,y / a };
	}

	double Norm() { return sqrt(pow(x, 2) + pow(y, 2)); }

	double Angle() { return atan2(y, x) * 180 / M_PI; }
	
};

enum class Object_State
{
	ON_FLOOR,
	JUMP,
	FALL,
	RUN,
	END,
};

enum class GROUP_TYPE
{
	DEFAULT,
	BACKGROUND,
	MAP,
	FLOOR,
	TILE,
	MONSTER,
	PLAYER,
	PLAYER_PROJECTILE,

	UI = 30,
	CURSOR = 31,
	END = 32,
};

enum class SCENE_TYPE
{
	TOOL,
	START,
	STAGE_01,
	STAGE_02,
	END,

};

enum class COMPONENT_TYPE
{
	DEFAULT,
	ANIMATOR,
	RIGIDBODY,
	COLLIDER,

	END,
};

enum class EVENT_TYPE
{
	CREATEOBJECT,
	DESTROYOBJECT,
	CHANGESCENE,
	END,
};

enum class ANIMATOR_TYPE
{
	MAIN,
	EFFECT,
	END,
};

enum class Main_Order
{
	RoamAround,
	GetToRoamingPoint,
	PlayerDetected,
	GetHurt,
	End,
};

enum class Move_Order
{
	Start,
	Stay,
	MoveRight,
	MoveLeft,
	ClimbDownRight,
	ClimbDownLeft,
	MoveToTopFloorRight,
	MoveToTopFloorLeft,

	CannotDetectPlayer,

	End,

};

enum class Action_Order
{
	IdleLeft,
	IdleRight,
	TurnLeft,
	TurnRight,
	AttackLeft,
	AttackRight,
	WalkRight,
	WalkLeft,
	RunRight,
	RunLeft,
	HurtGround,
	HurtFly,
	End,
};

union CollisionID
{
	struct
	{
		UINT LEFT;
		UINT RIGHT;
	} INPUT;

	ULONGLONG ID;
};

#define ColliderRender 1
#define Right 1
#define Left -1