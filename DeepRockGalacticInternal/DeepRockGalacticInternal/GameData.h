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
	char pad_0000[264]; //0x0000
	float N00000DF8; //0x0108
	char pad_010C[696]; //0x010C
	float powerCooldown; //0x03C4
	char pad_03C8[96]; //0x03C8
	float N00000E5C; //0x0428
	float N00002684; //0x042C
	float N00000E5D; //0x0430
	char pad_0434[112]; //0x0434
	float N000026BD; //0x04A4
	char pad_04A8[240]; //0x04A8
}; //Size: 0x0598
static_assert(sizeof(Pickaxe) == 0x598);

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
	uint32_t FNameIndex; //0x0018
	char pad_001C[680]; //0x001C
	float minigunHeatCoolDown; //0x02C4
	char pad_02C8[344]; //0x02C8
	class WeaponFire* pWeaponFire; //0x0420
	char pad_0428[512]; //0x0428
	int32_t maxAmmo; //0x0628
	int32_t clipSize; //0x062C
	int32_t shotCost; //0x0630
	float fireRate; //0x0634
	float burstCount; //0x0638
	float burstCycleTime; //0x063C
	float reloadDuration; //0x0640
	int32_t reserveAmmo; //0x0644
	int32_t ammo; //0x0648
	char pad_064C[36]; //0x064C
	float shootCooldown; //0x0670
	char pad_0674[12]; //0x0674
	float recoilRollMin; //0x0680
	float recoilRollMax; //0x0684
	float recoilPitchMin; //0x0688
	float recoilPitchMax; //0x068C
	float recoilYawMin; //0x0690
	float recoilYawMax; //0x0694
	char pad_0698[16]; //0x0698
	int8_t hasAutomaticFire; //0x06A8
	char pad_06A9[39]; //0x06A9
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

class WeaponFire
{
public:
	char pad_0000[304]; //0x0000
	float spreadPerShot; //0x0130
	char pad_0134[4]; //0x0134
	class DamageComponent* pDamageComponent; //0x0138
	int8_t useDamageComponent; //0x0140
	char pad_0141[27]; //0x0141
	int32_t maxPenetrations; //0x015C
	int32_t impactDecalSize; //0x0160
	char pad_0164[4]; //0x0164
	int32_t useDynamicSpread; //0x0168
	char pad_016C[20]; //0x016C
	float minSpead; //0x0180
	float minSpreadWhileMoving; //0x0184
	float minSpreadWhileSprinting; //0x0188
	float maxSpread; //0x018C
	char pad_0190[148]; //0x0190
	float verticalSpreadMultiplier; //0x0224
	float horizontalSpreadMultiplier; //0x0228
	float maxVerticalSpread; //0x022C
	float maxHorizontalSpread; //0x0230
	char pad_0234[4]; //0x0234
	int32_t ricochetBehavior; //0x0238
	float ricochetChance; //0x023C
	char pad_0240[4]; //0x0240
	float ricochetMaxRange; //0x0244
	char pad_0248[17]; //0x0248
	int8_t bulletsCanCarve; //0x0259
	char pad_025A[14]; //0x025A
	float carveDiameter; //0x0268
	char pad_026C[532]; //0x026C
}; //Size: 0x0480
static_assert(sizeof(WeaponFire) == 0x480);

class DamageComponent
{
public:
	char pad_0000[280]; //0x0000
	float damage; //0x0118
	float armorDamageMultiplier; //0x011C
	float armorPenetration; //0x0120
	float shattersArmor; //0x0124
	char pad_0128[8]; //0x0128
	float armorDamageType; //0x0130
	float weakpointMultiplier; //0x0134
	float frozenDamageBonusScale; //0x0138
	float friendlyFireModifier; //0x013C
	float selfFriendlyFireModifier; //0x0140
	char pad_0144[8]; //0x0144
	float staggerOnlyOnWeakpointHit; //0x014C
	float staggerChance; //0x0150
	float staggerDuration; //0x0154
	float fearFactor; //0x0158
	char pad_015C[68]; //0x015C
	int32_t userAreaOfEffect; //0x01A0
	float radialDamage; //0x01A4
	int32_t noFriendlyFireFromRadial; //0x01A8
	char pad_01AC[12]; //0x01AC
	float minimumDamagePercentage; //0x01B8
	float damageRadius; //0x01BC
	float maxDamageRadius; //0x01C0
	char pad_01C4[4]; //0x01C4
}; //Size: 0x01C8
static_assert(sizeof(DamageComponent) == 0x1C8);


///////////////////////////////////////////////////////////

// 
// Created with ReClass.NET 1.2 by KN4CK3R

class Player
{
public:
	char pad_0000[792]; //0x0000
	class Season* PtrSeason; //0x0318
	char pad_0320[504]; //0x0320
	int32_t Credits; //0x0518
	char pad_051C[924]; //0x051C
	class Resources* Resources; //0x08B8
	char pad_08C0[3960]; //0x08C0
}; //Size: 0x1838
static_assert(sizeof(Player) == 0x1838);

class Resources
{
public:
	char pad_0000[16]; //0x0000
	float Phazyonite; //0x0010
	char pad_0014[24]; //0x0014
	float StarchNut; //0x002C
	char pad_0030[24]; //0x0030
	float BarleyBulb; //0x0048
	char pad_004C[24]; //0x004C
	float YeastCone; //0x0064
	char pad_0068[24]; //0x0068
	float Bismor; //0x0080
	char pad_0084[24]; //0x0084
	float Jadiz; //0x009C
	char pad_00A0[24]; //0x00A0
	float EnorPearl; //0x00B8
	char pad_00BC[24]; //0x00BC
	float Umanite; //0x00D4
	char pad_00D8[24]; //0x00D8
	float MaltStar; //0x00F0
	char pad_00F4[24]; //0x00F4
	float Croppa; //0x010C
	char pad_0110[24]; //0x0110
	float Magnite; //0x0128
	char pad_012C[24]; //0x012C
	float SomeResource1; //0x0144
	char pad_0148[24]; //0x0148
	float SomeResource2; //0x0160
	char pad_0164[24]; //0x0164
	float SomeResource3; //0x017C
	char pad_0180[8]; //0x0180
}; //Size: 0x0188
static_assert(sizeof(Resources) == 0x188);

class Season
{
public:
	char pad_0000[16]; //0x0000
	int32_t XP; //0x0010
	int32_t Scrip; //0x0014
	char pad_0018[1136]; //0x0018
}; //Size: 0x0488
static_assert(sizeof(Season) == 0x488);