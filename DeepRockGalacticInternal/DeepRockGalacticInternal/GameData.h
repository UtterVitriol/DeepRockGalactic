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


/// Created with ReClass.NET 1.2 by KN4CK3R

class Character
{
public:
	char pad_0000[288]; //0x0000
	class Weapons* pEquipables; //0x0120
	char pad_0128[352]; //0x0128
	class CharacterMovement* pCharacterMovement; //0x0288
	char pad_0290[180]; //0x0290
	int32_t jumpMaxCount; //0x0344
	char pad_0348[1824]; //0x0348
	class IntoxicationComponent* pIntoxicationComponent; //0x0A68
	char pad_0A70[64]; //0x0A70
	class HealthComponent* pHealthComponent; //0x0AB0
	char pad_0AB8[8]; //0x0AB8
	class InventoryComponent* pInventoryComponent; //0x0AC0
	char pad_0AC8[112]; //0x0AC8
	float runningSpeed; //0x0B38
	float runBoost; //0x0B3C
	char pad_0B40[168]; //0x0B40
	float carryingMovementSpeedPenalty; //0x0BE8
	float carryingMaxFallVelocity; //0x0BEC
	char pad_0BF0[12]; //0x0BF0
	float carryingThrowMinForce; //0x0BFC
	float carryingThrowMaxForce; //0x0C00
	char pad_0C04[223]; //0x0C04
	int8_t isDanceRange; //0x0CE3
	int8_t isDancing; //0x0CE4
	char pad_0CE5[19]; //0x0CE5
	int32_t danceMove; //0x0CF8
	char pad_0CFC[4]; //0x0CFC
}; //Size: 0x0D00
static_assert(sizeof(Character) == 0xD00);

class HealthComponent
{
public:
	char pad_0000[432]; //0x0000
	float characterDamage; //0x01B0
	char pad_01B4[268]; //0x01B4
	float maxHealth; //0x02C0
	float maxArmor; //0x02C4
	float shieldDamage; //0x02C8
	char pad_02CC[12]; //0x02CC
	float healthPerChrystalVolume; //0x02D8
	char pad_02DC[28]; //0x02DC
	float shieldRegenDelay; //0x02F8
	char pad_02FC[300]; //0x02FC
}; //Size: 0x0428
static_assert(sizeof(HealthComponent) == 0x428);

class InventoryComponent
{
public:
	char pad_0000[504]; //0x0000
	float flareCooldown; //0x01F8
	float flareProductionTime; //0x01FC
	char pad_0200[20]; //0x0200
	int32_t maxFlares; //0x0214
	int32_t flares; //0x0218
	char pad_021C[180]; //0x021C
}; //Size: 0x02D0
static_assert(sizeof(InventoryComponent) == 0x2D0);

class Pickaxe
{
public:
	char pad_0000[264]; //0x0000
	float N00000DF8; //0x0108
	char pad_010C[696]; //0x010C
	float powerCooldown; //0x03C4
	char pad_03C8[284]; //0x03C8
}; //Size: 0x04E4
static_assert(sizeof(Pickaxe) == 0x4E4);

class Grenade
{
public:
	char pad_0000[904]; //0x0000
	int32_t maxGrenades; //0x0388
	int32_t grenades; //0x038C
}; //Size: 0x0390
static_assert(sizeof(Grenade) == 0x390);

class TraversalTool
{
public:
	char pad_0000[16]; //0x0000
	class HasWeaponType* pWeaponType; //0x0010
	uint32_t FNameIndex; //0x0018
	char pad_001C[1548]; //0x001C
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
	char pad_06E0[280]; //0x06E0
}; //Size: 0x07F8
static_assert(sizeof(TraversalTool) == 0x7F8);

class HasWeaponType
{
public:
	char pad_0000[88]; //0x0000
	int16_t weaponType; //0x0058
	char pad_005A[196]; //0x005A
}; //Size: 0x011E
static_assert(sizeof(HasWeaponType) == 0x11E);

class CharacterMovement
{
public:
	char pad_0000[176]; //0x0000
	class Position* pPosition; //0x00B0
	char pad_00B8[12]; //0x00B8
	Vec3 velocity; //0x00C4
	char pad_00D0[128]; //0x00D0
	float gravityScale; //0x0150
	float maxStepHeight; //0x0154
	float jumpHeight; //0x0158
	char pad_015C[12]; //0x015C
	int8_t movementMode; //0x0168
	char pad_0169[3]; //0x0169
	float groundFriction; //0x016C
	char pad_0170[28]; //0x0170
	float walkSpeed; //0x018C
	char pad_0190[8]; //0x0190
	float flySpeed; //0x0198
	char pad_019C[4]; //0x019C
}; //Size: 0x01A0
static_assert(sizeof(CharacterMovement) == 0x1A0);

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

class Weapons
{
public:
	class Pickaxe* pPickaxe; //0x0000
	class Weapon* pPrimaryGun; //0x0008
	class Weapon* pSecondayGun; //0x0010
	class TraversalTool* pTraversalTool; //0x0018
	class SupportTool* pSupportTool; //0x0020
	class Grenade* pGrenade; //0x0028
	class LaserPointer* pLaserPointer; //0x0030
	class TerrainScanner* pTerrainScanner; //0x0038
	class SupplyDrop* pSupplyDrop; //0x0040
	char pad_0048[64]; //0x0048
}; //Size: 0x0088
static_assert(sizeof(Weapons) == 0x88);

class SupportTool
{
public:
	char pad_0000[136]; //0x0000
}; //Size: 0x0088
static_assert(sizeof(SupportTool) == 0x88);

class LaserPointer
{
public:
	char pad_0000[136]; //0x0000
}; //Size: 0x0088
static_assert(sizeof(LaserPointer) == 0x88);

class TerrainScanner
{
public:
	char pad_0000[136]; //0x0000
}; //Size: 0x0088
static_assert(sizeof(TerrainScanner) == 0x88);

class SupplyDrop
{
public:
	char pad_0000[136]; //0x0000
}; //Size: 0x0088
static_assert(sizeof(SupplyDrop) == 0x88);

class IntoxicationComponent
{
public:
	char pad_0000[136]; //0x0000
}; //Size: 0x0088
static_assert(sizeof(IntoxicationComponent) == 0x88);

class Weapon
{
public:
	char pad_0000[16]; //0x0000
	class HasWeaponType* pWeaponType; //0x0010
	int32_t FNameIndex; //0x0018
	char pad_001C[1028]; //0x001C
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
	char pad_064C[52]; //0x064C
	float recoilRollMin; //0x0680
	float recoilRollMax; //0x0684
	float recoilPitchMin; //0x0688
	float recoilPitchMax; //0x068C
	float recoilYawMin; //0x0690
	float recoilYawMax; //0x0694
	char pad_0698[16]; //0x0698
	int8_t hasAutomaticFire; //0x06A8
	char pad_06A9[39]; //0x06A9
	float startingFireRate; //0x06D0
	float maxFireRate; //0x06D4
	char pad_06D8[272]; //0x06D8
	float splashDamage; //0x07E8
	float splashRadius; //0x07EC
	char pad_07F0[152]; //0x07F0
}; //Size: 0x0888
static_assert(sizeof(Weapon) == 0x888);

class N00006039
{
public:
	char pad_0000[136]; //0x0000
}; //Size: 0x0088
static_assert(sizeof(N00006039) == 0x88);

class N000060DC
{
public:
	char pad_0000[136]; //0x0000
}; //Size: 0x0088
static_assert(sizeof(N000060DC) == 0x88);

///////////////////////////////////////////////////////////

// Created with ReClass.NET 1.2 by KN4CK3R

class FSDSavedGame
{
public:
	char pad_0000[792]; //0x0000
	class Season* PtrSeason; //0x0318
	char pad_0320[176]; //0x0320
	int32_t perkPoints; //0x03D0
	char pad_03D4[324]; //0x03D4
	int32_t Credits; //0x0518
	char pad_051C[4]; //0x051C
	int32_t forgingXP; //0x0520
	char pad_0524[916]; //0x0524
	class Resources* Resources; //0x08B8
	char pad_08C0[3960]; //0x08C0
}; //Size: 0x1838
static_assert(sizeof(FSDSavedGame) == 0x1838);

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