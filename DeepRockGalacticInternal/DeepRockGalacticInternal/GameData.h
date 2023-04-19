#pragma once

#include <stdint.h>

typedef struct Vector3 { float x, y, z; } Vec3;


enum WeaponType
{
	minigun = 0x7C0,
	autocannon = 0x7F4,
	pistol = 0x744,
	zipline = 0x788,
	shotgun = 0x718,
	grenadelauncher = 0x6F0,
	platformgun = 0x701,
	coilgun = 0xB59,
	piack_axe = 0x6B4,

	sentrygun = 0x488,
	supply_drop = 0x418,
	grenade =	0x4CB,
	//shield_generator =	0x48461C,
};



// Created with ReClass.NET 1.2 by KN4CK3R

class GameData
{
public:
	char pad_0000[2736]; //0x0000
	class PlayerData* pPlayerData; //0x0AB0
	char pad_0AB8[8]; //0x0AB8
	class WeaponData* pWeaponData; //0x0AC0
	char pad_0AC8[568]; //0x0AC8
}; //Size: 0x0D00
static_assert(sizeof(GameData) == 0xD00);

class PlayerData
{
public:
	char pad_0000[32]; //0x0000
	class Body* pBody; //0x0020
	char pad_0028[296]; //0x0028
	float DamageTaken; //0x0150
	char pad_0154[92]; //0x0154
	float Health; //0x01B0
	char pad_01B4[164]; //0x01B4
	float TimeSinceLastHit; //0x0258
	char pad_025C[108]; //0x025C
	float ShieldInversePercentage; //0x02C8
	char pad_02CC[136]; //0x02CC
	float ClassHealth; //0x0354
	float BaseHealth; //0x0358
	float MaxShield; //0x035C
	char pad_0360[144]; //0x0360
}; //Size: 0x03F0
static_assert(sizeof(PlayerData) == 0x3F0);

class WeaponData
{
public:
	char pad_0000[208]; //0x0000
	class Weapon* pCurrentWeapon; //0x00D0
	char pad_00D8[8]; //0x00D8
	class Weapon* pPreviousWeapon; //0x00E0
	char pad_00E8[296]; //0x00E8
	float flairCooldown; //0x0210
	int32_t maxFlairs; //0x0214
	int32_t flairs; //0x0218
	float flairThrowCooldown; //0x021C
	char pad_0220[24]; //0x0220
	class Pickaxe* pPickaxe; //0x0238
	class Grenade* pGrenade; //0x0240
	char pad_0248[8]; //0x0248
	class TerrainScanner* pTerrainScanner; //0x0250
	char pad_0258[56]; //0x0258
}; //Size: 0x0290
static_assert(sizeof(WeaponData) == 0x290);

class Pickaxe
{
public:
	char pad_0000[964]; //0x0000
	float powerCooldown; //0x03C4
	char pad_03C8[208]; //0x03C8
}; //Size: 0x0498
static_assert(sizeof(Pickaxe) == 0x498);

class Grenade
{
public:
	char pad_0000[904]; //0x0000
	int32_t maxGrenades; //0x0388
	int32_t grenades; //0x038C
}; //Size: 0x0390
static_assert(sizeof(Grenade) == 0x390);

class TerrainScanner
{
public:
	char pad_0000[136]; //0x0000
}; //Size: 0x0088
static_assert(sizeof(TerrainScanner) == 0x88);

class Weapon
{
public:
	char pad_0000[16]; //0x0000
	class HasWeaponType* pWeaponType; //0x0010
	char pad_0018[684]; //0x0018
	float minigunHeatCoolDown; //0x02C4
	char pad_02C8[344]; //0x02C8
	class Stats* pStats; //0x0420
	char pad_0428[520]; //0x0428
	int32_t ammoSubtraction; //0x0630
	float fireRate; //0x0634
	char pad_0638[8]; //0x0638
	float reloadTime; //0x0640
	int32_t reserveAmmo; //0x0644
	int32_t ammo; //0x0648
	char pad_064C[36]; //0x064C
	float shootCooldown; //0x0670
	char pad_0674[20]; //0x0674
	float recoilY; //0x0688
	float recoilYMag; //0x068C
	float recoilX; //0x0690
	float recoilxMag; //0x0694
	char pad_0698[56]; //0x0698
	float zipLineMaxDistance; //0x06D0
	float zipLineMinDistance; //0x06D4
	float zipLineBaseAngle; //0x06D8
	float zipLineMaxAngle; //0x06DC
	char pad_06E0[16]; //0x06E0
	float miniGunSpinUpTime; //0x06F0
	char pad_06F4[76]; //0x06F4
	float coilGunChargeRate; //0x0740
	char pad_0744[4]; //0x0744
	float coilGunCharge; //0x0748
	char pad_074C[4]; //0x074C
	int32_t CoilGunSubtractionRate; //0x0750
	char pad_0754[68]; //0x0754
}; //Size: 0x0798
static_assert(sizeof(Weapon) == 0x798);

class HasWeaponType
{
public:
	char pad_0000[88]; //0x0000
	int16_t weaponType; //0x0058
	char pad_005A[196]; //0x005A
}; //Size: 0x011E
static_assert(sizeof(HasWeaponType) == 0x11E);

class Body
{
public:
	char pad_0000[648]; //0x0000
	class Movement* pMovement; //0x0288
	char pad_0290[180]; //0x0290
	int32_t jumpNumber; //0x0344
}; //Size: 0x0348
static_assert(sizeof(Body) == 0x348);

class Movement
{
public:
	char pad_0000[176]; //0x0000
	class Position* pPosition; //0x00B0
	char pad_00B8[12]; //0x00B8
	Vec3 velocity; //0x00C4
	char pad_00D0[128]; //0x00D0
	float gravit; //0x0150
	float stepHeight; //0x0154
	float jumpHeight; //0x0158
	char pad_015C[48]; //0x015C
	float walkSpeed; //0x018C
	char pad_0190[2436]; //0x0190
	float speedMultiplier; //0x0B14
	char pad_0B18[8]; //0x0B18
	class RunSpeed* pRunSpeed; //0x0B20
}; //Size: 0x0B28
static_assert(sizeof(Movement) == 0xB28);

class RunSpeed
{
public:
	char pad_0000[2872]; //0x0000
	float runSpeed; //0x0B38
	char pad_0B3C[332]; //0x0B3C
}; //Size: 0x0C88
static_assert(sizeof(RunSpeed) == 0xC88);

class Position
{
public:
	char pad_0000[464]; //0x0000
	Vec3 position; //0x01D0
}; //Size: 0x01DC
static_assert(sizeof(Position) == 0x1DC);

class Stats
{
public:
	char pad_0000[312]; //0x0000
	class WeaponDamage* pWeaponDamage; //0x0138
	char pad_0140[228]; //0x0140
	float spreadMultY; //0x0224
	float spreadMultX; //0x0228
	float spreadY; //0x022C
	float spreadX; //0x0230
	char pad_0234[8]; //0x0234
	float ricochetChance; //0x023C
	char pad_0240[4]; //0x0240
	float ricochetMaxRange; //0x0244
	char pad_0248[40]; //0x0248
	float N000049F9; //0x0270
	char pad_0274[524]; //0x0274
}; //Size: 0x0480
static_assert(sizeof(Stats) == 0x480);

class WeaponDamage
{
public:
	char pad_0000[280]; //0x0000
	float damage; //0x0118
	float armorDmgMult; //0x011C
	char pad_0120[48]; //0x0120
	float stunChance; //0x0150
	float stunDuraton; //0x0154
	float fearFactor; //0x0158
	char pad_015C[72]; //0x015C
	float areaDamage; //0x01A4
	char pad_01A8[20]; //0x01A8
	float areaRange; //0x01BC
	char pad_01C0[8]; //0x01C0
}; //Size: 0x01C8
static_assert(sizeof(WeaponDamage) == 0x1C8);