#pragma once

struct Vec2
{
	float x;
	float y;


	Vec2& operator = (POINT _pt)
	{
		x = (float)_pt.x;
		y = (float)_pt.y;
		return *this;
	}

	float Length()
	{
		return sqrt(x * x + y * y);
	}


	float Distance(Vec2 _vOther)
	{
		return sqrt(pow(_vOther.x - x, 2) + pow(_vOther.y - y, 2));
	}

	void Normalize()
	{
		float fLen = Length();
		x /= fLen;
		y /= fLen;
	}

	bool operator == (Vec2 _Other)
	{
		if (x == _Other.x && y == _Other.y)
			return true;
		return false;
	}

	bool operator != (Vec2 _Other)
	{
		if (x == _Other.x && y == _Other.y)
			return false;
		return true;
	}

	Vec2 operator -()
	{
		return Vec2(-x, -y);
	}


	Vec2 operator + (Vec2 _Other)
	{
		return Vec2(x + _Other.x, y + _Other.y);
	}

	void operator += (Vec2 _Other)
	{
		x += _Other.x;
		y += _Other.y;
	}

	Vec2 operator + (float _f)
	{
		return Vec2(x + _f, y + _f);
	}

	Vec2 operator - (Vec2 _Other)
	{
		return Vec2(x - _Other.x, y - _Other.y);
	}

	void operator -= (Vec2 _Other)
	{
		x -= _Other.x;
		y -= _Other.y;
	}

	Vec2 operator - (float _f)
	{
		return Vec2(x - _f, y - _f);
	}



	Vec2 operator * (Vec2 _Other)
	{
		return Vec2(x * _Other.x, y * _Other.y);
	}

	void operator *= (Vec2 _Other)
	{
		x *= _Other.x;
		y *= _Other.y;
	}

	Vec2 operator * (float _f)
	{
		return Vec2(x * _f, y * _f);
	}

	void operator *= (float _f)
	{
		x *= _f;
		y *= _f;
	}


	Vec2 operator / (Vec2 _Other)
	{
		return Vec2(x / _Other.x, y / _Other.y);
	}

	void operator /= (Vec2 _Other)
	{
		assert(_Other.x && _Other.y);

		x /= _Other.x;
		y /= _Other.y;
	}

	Vec2 operator / (float _f)
	{
		assert(_f);
		return Vec2(x / _f, y / _f);
	}



public:
	Vec2()
		: x(0.f)
		, y(0.f)
	{}

	Vec2(float _x, float _y)
		: x(_x)
		, y(_y)
	{}
};

struct tTask
{
	TASK_TYPE	TaskType;
	DWORD_PTR	Param_0;
	DWORD_PTR	Param_1;
};

struct tDebugShapeInfo
{
	DEBUG_SHAPE Shape;
	Vec2		Position;
	Vec2		Scale;
	PEN_TYPE	Pen;
	BRUSH_TYPE	BRUSH;
	float		Duration;	// DebugShape 유지 시간
	float		Time;		// 현재 진행 시간
};


struct tLog
{
	wstring		strLog; // 출력할 메세지	
	LOG_LEVEL	Level;	// 로그 수준
	float		Time;	// 현재 시간
};



class CTexture;
// 맵 오브젝트 정보
struct MapInfo
{
	CTexture*	MapTexture;	//맵 텍스쳐
	int			MapIndx;	// 사용 할 맵 번호
	UINT		PlatCol;	// 맵의 길이

	MapInfo()
		: MapTexture(nullptr)
		, MapIndx(0)
		, PlatCol(5)
	{}

};


// 유닛 스텟정보
struct Unit_Stat
{
	Vec2        Range;		//Range : 유닛의 사거리입니다.공격 범위 내에 가까운 유닛이나 기지를 포착하면 총을 발사합니다.
	UNIT_TYPE	Type;		//Type : 유닛의 타입입니다. 인간형, 차량형, 기지형으로 나뉩니다.
	int         FUND;       //FUND : 소환 시 필요한 유닛 코스트입니다.
	int         MaxHP;		//MaxHP : 최대 유닛의 체력입니다.
	int         HP;			//HP : 유닛의 현재 체력입니다.  MaxHP 이상이 될 수 없으며 0이하가 되면 사망합니다.
	int         HDemage;			//HDemage : 보병형 유닛에게 주는 피해입니다.
	int         MDemage;			//MDemage : 차량 유닛에게 주는 피해입니다.
	int         Armor;				//Armor : 피해가 들어오면 해당 수치만큼 차감되어 HP피해를 입습니다.
	float       AttackSpeed;		//AttackSpeed : 다음 발사 까지 걸리는 시간입니다.낮을 수록 공격 속도가 빠릅니다.
	float       AttackDuration;		//AttackDuration : AttackSpeed이상이 되면 AttackSpeed만큼 깎고 공격을 합니다.
	float       Cooltime;			//Cooltime : 생산 쿨타임입니다.낮을 수록 다음 생산까지 걸리는 시간이 짧습니다.
	float		CooltimeOrgin;		// CooltimeOrgin : 원래 쿨타임입니다.
	float		CooltimeDuration;	// CooltimeDuration : 현재 남은 쿨타임입니다. 0이되면 준비완료입니다.
	float		firstCooltime;		// firstCooltime: 초기 쿨타임입니다. AI전용이며 해당 쿨타임 이후에 소환 가능합니다.
	Vec2		velocity;			//velocity : 이동 속도 입니다. 높을수록 빠릅니다.

};