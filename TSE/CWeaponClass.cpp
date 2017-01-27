//	CWeaponClass.cpp
//
//	CWeaponClass class

#include "PreComp.h"

#define CONFIGURATION_TAG						CONSTLIT("Configuration")
#define MISSILES_TAG							CONSTLIT("Missiles")
#define VARIANTS_TAG							CONSTLIT("Variants")

#define AIM_TOLERANCE_ATTRIB					CONSTLIT("aimTolerance")
#define ALTERNATING_ATTRIB						CONSTLIT("alternating")
#define AMMO_ID_ATTRIB							CONSTLIT("ammoID")
#define ANGLE_ATTRIB							CONSTLIT("angle")
#define CHARGES_ATTRIB							CONSTLIT("charges")
#define CONFIGURATION_ATTRIB					CONSTLIT("configuration")
#define COOLING_RATE_ATTRIB						CONSTLIT("coolingRate")
#define COUNTER_ATTRIB							CONSTLIT("counter")
#define COUNTER_ACTIVATE_ATTRIB					CONSTLIT("counterActivate")
#define COUNTER_UPDATE_ATTRIB					CONSTLIT("counterUpdate")
#define COUNTER_UPDATE_RATE_ATTRIB				CONSTLIT("counterUpdateRate")
#define FAILURE_CHANCE_ATTRIB					CONSTLIT("failureChance")
#define FIRE_RATE_ATTRIB						CONSTLIT("fireRate")
#define HEATING_ATTRIB							CONSTLIT("heating")
#define IDLE_POWER_USE_ATTRIB					CONSTLIT("idlePowerUse")
#define LAUNCHER_ATTRIB							CONSTLIT("launcher")
#define LINKED_FIRE_ATTRIB						CONSTLIT("linkedFire")
#define MAX_FIRE_ARC_ATTRIB						CONSTLIT("maxFireArc")
#define MIN_FIRE_ARC_ATTRIB						CONSTLIT("minFireArc")
#define MULTI_TARGET_ATTRIB						CONSTLIT("multiTarget")
#define OMNIDIRECTIONAL_ATTRIB					CONSTLIT("omnidirectional")
#define POS_ANGLE_ATTRIB						CONSTLIT("posAngle")
#define POS_RADIUS_ATTRIB						CONSTLIT("posRadius")
#define POWER_USE_ATTRIB						CONSTLIT("powerUse")
#define RECOIL_ATTRIB							CONSTLIT("recoil")
#define REPORT_AMMO_ATTRIB						CONSTLIT("reportAmmo")
#define TARGET_STATIONS_ONLY_ATTRIB				CONSTLIT("targetStationsOnly")
#define TYPE_ATTRIB								CONSTLIT("type")

#define CONFIG_TYPE_DUAL						CONSTLIT("dual")
#define CONFIG_TYPE_WALL						CONSTLIT("wall")
#define CONFIG_TYPE_SPREAD2						CONSTLIT("spread2")
#define CONFIG_TYPE_SPREAD3						CONSTLIT("spread3")
#define CONFIG_TYPE_SPREAD5						CONSTLIT("spread5")
#define CONFIG_TYPE_DUAL_ALTERNATING			CONSTLIT("alternating")

#define COUNTER_TYPE_TEMPERATURE				CONSTLIT("temperature")
#define COUNTER_TYPE_CAPACITOR					CONSTLIT("capacitor")

#define ON_FIRE_WEAPON_EVENT					CONSTLIT("OnFireWeapon")

#define FIELD_AMMO_TYPE							CONSTLIT("ammoType")
#define FIELD_AVERAGE_DAMAGE					CONSTLIT("averageDamage")	//	Average damage (1000x hp)
#define FIELD_BALANCE							CONSTLIT("balance")
#define FIELD_BALANCE_DAMAGE					CONSTLIT("balanceDamage")	//	Computed damage (per 180 ticks) based on balance 
#define FIELD_CONFIGURATION						CONSTLIT("configuration")
#define FIELD_DAMAGE_180						CONSTLIT("damage")			//	HP damage per 180 ticks
#define FIELD_DAMAGE_TYPE						CONSTLIT("damageType")
#define FIELD_HP								CONSTLIT("hp")
#define FIELD_IS_ALTERNATING					CONSTLIT("isAlternating")
#define FIELD_MAX_DAMAGE						CONSTLIT("maxDamage")
#define FIELD_MIN_DAMAGE						CONSTLIT("minDamage")
#define FIELD_FIRE_DELAY						CONSTLIT("fireDelay")		//	Delay (ticks)
#define FIELD_FIRE_RATE							CONSTLIT("fireRate")
#define FIELD_POWER								CONSTLIT("power")
#define FIELD_POWER_PER_SHOT					CONSTLIT("powerPerShot")	//	Power used per shot (1000x Megawatt minutes)
#define FIELD_RANGE								CONSTLIT("range")
#define FIELD_RECOIL							CONSTLIT("recoil")
#define FIELD_REPEAT_COUNT						CONSTLIT("repeatCount")
#define FIELD_SPEED								CONSTLIT("speed")
#define FIELD_VARIANT_COUNT						CONSTLIT("variantCount")

#define PROPERTY_AMMO_TYPES						CONSTLIT("ammoTypes")
#define PROPERTY_AVERAGE_DAMAGE					CONSTLIT("averageDamage")	//	Average damage
#define PROPERTY_BALANCE    					CONSTLIT("balance")
#define PROPERTY_BALANCE_DAMAGE					CONSTLIT("balanceDamage")
#define PROPERTY_BALANCE_COST 					CONSTLIT("balanceCost")
#define PROPERTY_BALANCE_EXCLUDE_COST			CONSTLIT("balanceExcludeCost")
#define PROPERTY_DAMAGE_180						CONSTLIT("damage")			//	HP damage per 180 ticks
#define PROPERTY_DAMAGE_PER_PROJECTILE			CONSTLIT("damagePerProjectile")
#define PROPERTY_DAMAGE_WMD_180					CONSTLIT("damageWMD180")
#define PROPERTY_DAMAGED						CONSTLIT("damaged")
#define PROPERTY_EFFECTIVE_RANGE				CONSTLIT("effectiveRange")
#define PROPERTY_FIRE_ARC						CONSTLIT("fireArc")
#define PROPERTY_FIRE_DELAY						CONSTLIT("fireDelay")
#define PROPERTY_FIRE_RATE						CONSTLIT("fireRate")
#define PROPERTY_LINKED_FIRE_OPTIONS			CONSTLIT("linkedFireOptions")
#define PROPERTY_MAX_DAMAGE						CONSTLIT("maxDamage")
#define PROPERTY_MIN_DAMAGE						CONSTLIT("minDamage")
#define PROPERTY_OMNIDIRECTIONAL				CONSTLIT("omnidirectional")
#define PROPERTY_SECONDARY						CONSTLIT("secondary")
#define PROPERTY_STD_COST						CONSTLIT("stdCost")
#define PROPERTY_TRACKING						CONSTLIT("tracking")

#define VARIANT_TYPE_CHARGES					CONSTLIT("charges")
#define VARIANT_TYPE_LEVELS						CONSTLIT("levels")
#define VARIANT_TYPE_MISSILES					CONSTLIT("missiles")

const int MAX_SHOT_COUNT =				100;

const Metric MAX_EXPECTED_PASSTHROUGH = 4.0;
const Metric EXPECTED_FRAGMENT_HITS =   0.2;                //  Fraction of fragments that hit (for balance purposes)
const Metric EXPECTED_TRACKING_FRAGMENT_HITS = 0.67;        //  Fraction of tracking fragments that hit (for balance purposes)
const Metric EXPECTED_SHOCKWAVE_HITS =  0.2;                //  Fraction of shockwave that hits (for balance purposes)
const Metric EXPECTED_RADIUS_DAMAGE =   0.5;                //  Fraction of radius damage (for balance purposes)

const Metric g_DualShotSeparation =		12;					//	Radius of dual shot (pixels)
const int TEMP_DECREASE =				-1;					//	Decrease in temp per cooling rate
const int MAX_TEMP =					120;				//	Max temperature
const int OVERHEAT_TEMP =				100;				//	Weapon overheats
const Metric MAX_TARGET_RANGE =			(24.0 * LIGHT_SECOND);
const int MAX_COUNTER =					100;

const Metric STD_FIRE_RATE_SECS =		16.0;				//	Standard fire rate (secs)
const Metric STD_FIRE_DELAY_TICKS =     8.0;

const Metric STD_AMMO_BALANCE =         -100.0;             //  Balance adj from having ammo
const Metric STD_AMMO_MASS =            10.0;               //  Std ammo mass (kg)
const Metric BALANCE_AMMO_COST_RATIO =  -0.20;              //  Each percent of ammo price above std is
                                                            //      0.25% balance penalty.
const Metric BALANCE_AMMO_MASS_RATIO =  -0.25;              //  Heavier ammo is a balance penalty.

const Metric BALANCE_OMNI_POWER =       0.5;                //  f(x) = factor * (x/360)^power
const Metric BALANCE_OMNI_FACTOR =      100.0;              //      This function maps from a swivel
                                                            //      arc (0-360) to its effect on weapon
                                                            //      balance: 0 = none; 100.0 = +100%

const Metric BALANCE_TRACKING_BONUS =   50.0;               //  Bonus to balance if weapon has tracking.
const Metric BALANCE_LINKED_FIRE_BONUS = 25.0;              //  Bonus to balance if weapon is linked-fire.

const Metric STD_RANGE =                60.0 * LIGHT_SECOND;//  Standard range
const Metric MAX_BALANCE_RANGE =        120.0 * LIGHT_SECOND;   //  Range above this doesn't count for balance purposes
const Metric BALANCE_RANGE_OVER_FACTOR = 0.35;              //  Bonus to balance for each ls above standard
const Metric BALANCE_RANGE_UNDER_FACTOR = 1.0;              //  Penalty to balance for each ls below standard

const Metric BALANCE_SPEED_POWER =      0.5;                //  f(x) = factor * (1 - (x/lightspeed)^power)
const Metric BALANCE_SPEED_FACTOR =     -25.0;

const Metric BALANCE_INTERACTION_FACTOR = 5.0;              //  Bonus to balance for 0 interaction (linear
                                                            //      decrease as interaction reaches 100).
const Metric BALANCE_HP_FACTOR =        10.0;               //  Bonus to balance for projectile HP equal to
                                                            //      standard weapon damage for the level
                                                            //      (linear decrease with fewer HP).
const Metric BALANCE_POWER_AMMO_FACTOR = 0.1;               //  Power used by ammo weapons relative to non-ammo
const Metric BALANCE_POWER_RATIO =      -0.5;               //  Each percent of power consumption above standard
                                                            //      is 0.5% balance penalty.
const Metric BALANCE_COST_RATIO =       -0.5;               //  Each percent of cost above standard is a 0.5%
                                                            //      balance penalty.
const Metric BALANCE_SLOT_FACTOR =      -40.0;              //  Penalty to balance for every extra slot used 
                                                            //      above 1.
const Metric BALANCE_NO_SLOT =          20.0;               //  Bonus to balance if no slot is used.
const Metric BALANCE_EXTERNAL =         -40.0;              //  Penalty to balance if weapon is external
const Metric BALANCE_RECOIL_FACTOR =    -2.5;               //  Penalty to balance per point of recoil
const Metric BALANCE_RADIATION =        30.0;               //  Bonus to balance if we have radiation damage
const Metric BALANCE_DEVICE_DISRUPT_FACTOR = 3.0;           //  Bonus to balance for per % chance of disruption
const Metric BALANCE_DEVICE_DAMAGE_FACTOR = 3.0;            //  Bonus to balance for per % chance of damage
const Metric BALANCE_DISINTEGRATION =   100.0;              //  Bonus to balance for disintegration
const Metric BALANCE_SHATTER_FACTOR =   12.0;               //  Bonus to balance for shatter
const Metric BALANCE_SHIELD_PENETRATE_FACTOR = 0.25;        //  Bonus to balance for each % of shield penetrate chance
const Metric BALANCE_SHIELD_LEVEL_FACTOR =      10;         //  Bonus to balance proportional to shield damage level
const Metric BALANCE_ARMOR_LEVEL_FACTOR =       20;         //  Bonus to balance proportional to armor damage level
const Metric BALANCE_MINING_FACTOR =            0.1;        //  Bonus to balance for each % of mining adj
const Metric BALANCE_WMD_FACTOR =               0.5;        //  Bonust to balance for each % of WMD

const Metric PARTICLE_CLOUD_DAMAGE_FACTOR =		0.75;
const Metric SHOCKWAVE_DAMAGE_FACTOR =			4.0;

static CWeaponClass::SStdStats STD_WEAPON_STATS[MAX_ITEM_LEVEL] =
	{
		//	Damage	Power	        Cost     Ammo Cost   Over	Under
		{	4,		10,		        350.,       0.6,     0,	    0, },
		{	5,		20,		        800.,       1.2,     0,	    -20, },
		{	7,		50,		       1850.,       3.0,     0,     -50, },
		{	9,		100,	       4000.,       6.0,     0,     -100, },
		{	12,		200,	       8500.,      12.0,     5,     -170, },
			
		{	16,		300,	      18500.,      18.0,     5,     -250, },
		{	21,		500,	      39000.,      30.0,     5,     -300, },
		{	27,		1000,	      78500.,      60.0,     10,    -300, },
		{	35,		2000,	     158000.,     120.0,     10,    -300, },
		{	46,		3000,	     320000.,     180.0,     10,    -300, },
			
		{	60,		4000,	     640000.,     240.0,     25,    -300, },
		{	78,		6000,	    1300000.,     360.0,     25,    -300, },
		{	101,	8000,	    2700000.,     480.0,     25,    -300, },
		{	131,	10000,	    5300000.,     600.0,     100,   -300, },
		{	170,	12000,	   10600000.,     720.0,     100,   -300, },
			
		{	221,	15000,	   21300000.,     900.0,     100,   -300, },
		{	287,	20000,	   42600000.,    1200.0,     200,   -300, },
		{	373,	25000,	   85200000.,    1500.0,     200,   -300, },
		{	485,	30000,	  170000000.,    1800.0,     200,   -300, },
		{	631,	35000,	  341000000.,    2100.0,     200,   -300, },
			
		{	820,	40000,	  682000000.,    2400.0,     200,   -300, },
		{	1066,	50000,	 1400000000.,    3000.0,     200,   -300, },
		{	1386,	60000,	 2700000000.,    3600.0,     200,   -300, },
		{	1802,	70000,	 5500000000.,    4200.0,     200,   -300, },
		{	2343,	80000,	10900000000.,    4800.0,     200,   -300, },
	};

static char *CACHED_EVENTS[CWeaponClass::evtCount] =
	{
		"OnFireWeapon",
	};

CWeaponClass::CWeaponClass (void) : 
		m_pConfig(NULL),
		m_bConfigAlternating(false)
	{
	}

CWeaponClass::~CWeaponClass (void)
	{
	int i;

	for (i = 0; i < m_ShotData.GetCount(); i++)
		if (m_ShotData[i].bOwned)
			delete m_ShotData[i].pDesc;

	if (m_pConfig)
		delete [] m_pConfig;
	}

bool CWeaponClass::Activate (CInstalledDevice *pDevice, 
							 CSpaceObject *pSource, 
							 CSpaceObject *pTarget,
							 bool *retbSourceDestroyed,
							 bool *retbConsumedItems)

//	Activate
//
//	Activates the device (in this case, fires the weapon)

	{
	DEBUG_TRY

	CItemCtx Ctx(pSource, pDevice);
	CWeaponFireDesc *pShot = GetWeaponFireDesc(Ctx);

	if (retbSourceDestroyed)
		*retbSourceDestroyed = false;
	if (retbConsumedItems)
		*retbConsumedItems = false;

	//	If not enabled, no firing

	if (pShot == NULL || !pDevice->IsEnabled())
		{
		pDevice->SetLastActivateSuccessful(false);
		return false;
		}

	//	Fire

	bool bSourceDestroyed;

	//	Fire the weapon

	bool bSuccess = FireWeapon(pDevice, pShot, pSource, pTarget, 0, &bSourceDestroyed, retbConsumedItems);

	//	If firing the weapon destroyed the ship, then we bail out

	if (bSourceDestroyed)
		{
		if (retbSourceDestroyed)
			*retbSourceDestroyed = true;
		return false;
		}

	//	Keep track of whether we succeeded or not so that we know whether to consume power

	pDevice->SetLastActivateSuccessful(bSuccess);

	//	If we did not succeed, then we're done

	if (!bSuccess)
		return false;

	//	If this is a continuous fire weapon then set the device data
	//	We set to -1 because we skip the first Update after the call
	//	to Activate (since it happens on the same tick)

	if (pShot->GetContinuous() > 0)
		SetContinuousFire(pDevice, CONTINUOUS_START);

	//	Player-specific code

	if (pSource->IsPlayer())
		{
		//	Track statistics for the player

		CShip *pShip = pSource->AsShip();
		if (pShip)
			{
			CItem WeaponItem(GetItemType(), 1);
			pShip->GetController()->OnItemFired(WeaponItem);

			if ((IsLauncher() || m_bReportAmmo) && pShot->GetAmmoType())
				{
				CItem AmmoItem(pShot->GetAmmoType(), 1);
				pShip->GetController()->OnItemFired(AmmoItem);
				}
			}

		//	Identify the weapon

		GetItemType()->SetKnown();
		}

	//	Consume power

	return true;

	DEBUG_CATCH
	}

int CWeaponClass::CalcActivateDelay (CItemCtx &ItemCtx) const

//	CalcActivateDelay
//
//	Computes the activation delay, optionally dealing with enhancements.

	{
	const CItemEnhancementStack *pEnhancements = ItemCtx.GetEnhancementStack();
	if (pEnhancements)
		return pEnhancements->CalcActivateDelay(ItemCtx);

	return m_iFireRate;
	}

int CWeaponClass::CalcBalance (CItemCtx &ItemCtx, SBalance &retBalance) const

//	CalcBalance
//
//	Calculates weapon balance relative to level. +100 is 100% overpowered

	{
    //  Initialize

    retBalance.rBalance = 0.0;

	CWeaponFireDesc *pShot = GetWeaponFireDesc(ItemCtx);
	if (pShot == NULL)
		return 0;

	//	Compute the level. For launchers we take either the missile level or the
	//	launcher level (whichever is higher).

    retBalance.iLevel = CalcLevel(pShot);

    //  Get the standard stats for this level

    const SStdStats &Stats = STD_WEAPON_STATS[retBalance.iLevel - 1];
    retBalance.rStdDamage180 = Stats.iDamage * (180.0 / STD_FIRE_DELAY_TICKS);

    //  Compute how much damage we do in 180 ticks.

    retBalance.rDamageHP = CalcDamage(pShot);
    retBalance.rDamageMult = CalcConfigurationMultiplier(pShot, false);
    Metric rDamagePerShot = retBalance.rDamageMult * retBalance.rDamageHP;
    Metric rFireDelay = (m_iFireRate > 0 ? m_iFireRate : 1.0);
    retBalance.rDamage180 = rDamagePerShot * 180.0 / rFireDelay;

    //  Compute the number of balance points (BP) of the damage. +100 = double
    //  damage relative to standard. -100 = half-damage relative to standard.

    retBalance.rDamage = 100.0 * mathLog2(retBalance.rDamage180 / retBalance.rStdDamage180);
    retBalance.rBalance = retBalance.rDamage;

    //  If our damage type is higher or lower than appropriate for this level,
    //  adjust our balance. Positive nunmbers mean that the weapon has a lower 
    //  level than the minimum for its damage type.

    DamageTypes iDamageType = pShot->GetDamageType();
    int iTypeDelta = DamageDesc::GetDamageLevel(iDamageType) - retBalance.iLevel;
    retBalance.rDamageType = (iTypeDelta >= 0 ? STD_WEAPON_STATS[iTypeDelta].iOverTierAdj : STD_WEAPON_STATS[-iTypeDelta].iUnderTierAdj);
    retBalance.rBalance += retBalance.rDamageType;

    //  Ammo weapons have the inherent disadvantage of needing ammo

    bool bAmmo = ItemCtx.GetItem().GetType()->IsMissile();
    bool bUsesAmmo;
    if (pShot->GetAmmoType())
        {
        bUsesAmmo = true;

        //  Having ammo is a 100% penalty

        retBalance.rAmmo = STD_AMMO_BALANCE;

        //  Adjust the standard ammo cost and standard ammo mass for fire rate.

        retBalance.rStdAmmoCost = Stats.rAmmoCost * rFireDelay / STD_FIRE_DELAY_TICKS;
        retBalance.rStdAmmoMass = STD_AMMO_MASS * rFireDelay / STD_FIRE_DELAY_TICKS;

        //  Compute the standard ammo cost at this level and figure out the 
        //  percent cost difference. +1 = ammo is 1% more expensive than 
        //  standard. -1 = ammo is 1% cheaper than standard.

        Metric rAmmoCost = (Metric)CEconomyType::ExchangeToCredits(pShot->GetAmmoType()->GetCurrencyAndValue(ItemCtx, true));
        Metric rAmmoCostDelta = 100.0 * (rAmmoCost - retBalance.rStdAmmoCost) / retBalance.rStdAmmoCost;
        retBalance.rAmmo += rAmmoCostDelta * BALANCE_AMMO_COST_RATIO;

        //  Compute the ammo mass bonus

        Metric rAmmoMassDelta = 100.0 * (pShot->GetAmmoType()->GetMassKg(ItemCtx) - retBalance.rStdAmmoMass) / retBalance.rStdAmmoMass;
        retBalance.rAmmo += rAmmoMassDelta * BALANCE_AMMO_MASS_RATIO;

        //  Add up to total balance

        retBalance.rBalance += retBalance.rAmmo;
        }

    //  Weapons with charges count as needing ammo

    else if (m_bCharges || IsLauncher())
        {
        bUsesAmmo = true;
        retBalance.rAmmo = STD_AMMO_BALANCE;
        retBalance.rBalance += retBalance.rAmmo;
        }
    else
        bUsesAmmo = false;

    //  Omni and swivel weapons are a bonus

    Metric rSwivelRange = CalcRotateRange(ItemCtx) / 360.0;
    if (rSwivelRange > 0.0)
        {
        retBalance.rOmni = BALANCE_OMNI_FACTOR * pow(rSwivelRange, BALANCE_OMNI_POWER);
        retBalance.rBalance += retBalance.rOmni;
        }

    //  Tracking weapons are a bonus

    if (pShot->IsTrackingOrHasTrackingFragments())
        {
        retBalance.rTracking = BALANCE_TRACKING_BONUS;
        retBalance.rBalance += retBalance.rTracking;
        }

    //  Range

    Metric rRange = Min(MAX_BALANCE_RANGE, pShot->GetMaxRange());
    if (rRange >= STD_RANGE)
        retBalance.rRange = BALANCE_RANGE_OVER_FACTOR * (rRange - STD_RANGE) / LIGHT_SECOND;
    else
        retBalance.rRange = BALANCE_RANGE_UNDER_FACTOR * (rRange - STD_RANGE) / LIGHT_SECOND;

    retBalance.rBalance += retBalance.rRange;

    //  Speed

    retBalance.rSpeed = BALANCE_SPEED_FACTOR * (1.0 - pow(pShot->GetRatedSpeed() / LIGHT_SPEED, BALANCE_SPEED_POWER));
    retBalance.rBalance += retBalance.rSpeed;

    //  Projectile HP and interaction

    retBalance.rProjectileHP = 0.0;
    if (pShot->GetInteraction() < 100)
        retBalance.rProjectileHP += BALANCE_INTERACTION_FACTOR * (100 - pShot->GetInteraction()) / 100.0;

    if (pShot->GetHitPoints() > 0)
        retBalance.rProjectileHP += BALANCE_HP_FACTOR * pShot->GetHitPoints() / (Metric)Stats.iDamage;

    retBalance.rBalance += retBalance.rProjectileHP;

    //  Power use (ignore if we're balancing a launcher's missile).

    if (!bAmmo || !IsLauncher())
        {
        //  Note: We divide by Stats.iPower (not rStdPower) because we care 
        //  about power use relative to what we generate. That is, launcher
        //  power use is so small that we don't care much if it doubles.

        Metric rStdPower = (bUsesAmmo ? BALANCE_POWER_AMMO_FACTOR * Stats.iPower : Stats.iPower);
        Metric rPowerDelta = 100.0 * (GetPowerRating(ItemCtx) - rStdPower) / (Metric)Stats.iPower;

        retBalance.rPower = BALANCE_POWER_RATIO * rPowerDelta;
        retBalance.rBalance += retBalance.rPower;
        }

    //  Cost

    if (!bAmmo || !IsLauncher())
        {
        Metric rCost = (Metric)CEconomyType::ExchangeToCredits(GetItemType()->GetCurrencyAndValue(ItemCtx, true));
        Metric rCostDelta = 100.0 * (rCost - Stats.rCost) / Stats.rCost;
        retBalance.rCost = BALANCE_COST_RATIO * rCostDelta;
        retBalance.rBalance += retBalance.rCost;
        }

    //  Slots

    if (GetSlotsRequired() == 0)
        retBalance.rSlots = BALANCE_NO_SLOT;
    else if (GetSlotsRequired() > 1)
        retBalance.rSlots = BALANCE_SLOT_FACTOR * (GetSlotsRequired() - 1);
    else
        retBalance.rSlots = 0.0;

    retBalance.rBalance += retBalance.rSlots;

    //  Linked-fire

    if (m_dwLinkedFireOptions)
        {
        retBalance.rLinkedFire = BALANCE_LINKED_FIRE_BONUS;
        retBalance.rBalance += retBalance.rLinkedFire;
        }

    //  Recoil

    if (m_iRecoil)
        {
        retBalance.rRecoil = m_iRecoil * BALANCE_RECOIL_FACTOR;
        retBalance.rBalance += retBalance.rRecoil;
        }

    //  External

    if (IsExternal())
        {
        retBalance.rExternal = BALANCE_EXTERNAL;
        retBalance.rBalance += retBalance.rExternal;
        }

    //  Radiation

    int iDamage;
    if (iDamage = pShot->GetSpecialDamage(specialRadiation))
        {
        retBalance.rRadiation = BALANCE_RADIATION;
        retBalance.rBalance += retBalance.rRadiation;
        }

    //  Device disrupt and damage

    if (iDamage = pShot->GetSpecialDamage(specialDeviceDisrupt, DamageDesc::flagSpecialAdj))
        {
        retBalance.rDeviceDisrupt = BALANCE_DEVICE_DISRUPT_FACTOR * Min(iDamage, 50);
        retBalance.rBalance += retBalance.rDeviceDisrupt;
        }

    if (iDamage = pShot->GetSpecialDamage(specialDeviceDamage, DamageDesc::flagSpecialAdj))
        {
        retBalance.rDeviceDamage = BALANCE_DEVICE_DAMAGE_FACTOR * Min(iDamage, 50);
        retBalance.rBalance += retBalance.rDeviceDamage;
        }

    //  Disintegration

    if (iDamage = pShot->GetSpecialDamage(specialDisintegration))
        {
        retBalance.rDisintegration = BALANCE_DISINTEGRATION;
        retBalance.rBalance += retBalance.rDisintegration;
        }

    //  Shatter

    if (iDamage = pShot->GetSpecialDamage(specialShatter))
        {
        retBalance.rShatter = BALANCE_SHATTER_FACTOR * iDamage;
        retBalance.rBalance += retBalance.rShatter;
        }

    //  Shield penetrate

    if (iDamage = pShot->GetSpecialDamage(specialShieldPenetrator, DamageDesc::flagSpecialAdj))
        {
        retBalance.rShieldPenetrate = BALANCE_SHIELD_PENETRATE_FACTOR * iDamage;
        retBalance.rBalance += retBalance.rShieldPenetrate;
        }

    //  Extra shield damage

    if (iDamage = pShot->GetSpecialDamage(specialShieldDisrupt))
        {
        int iEffectLevel = Max(0, 3 + iDamage - retBalance.iLevel);
        retBalance.rShield = BALANCE_SHIELD_LEVEL_FACTOR * iEffectLevel;
        retBalance.rBalance += retBalance.rShield;
        }

    //  Extra armor damage

    if (iDamage = pShot->GetSpecialDamage(specialArmor))
        {
        int iEffectLevel = Max(0, 3 + iDamage - retBalance.iLevel);
        retBalance.rArmor = BALANCE_ARMOR_LEVEL_FACTOR * iEffectLevel;
        retBalance.rBalance += retBalance.rArmor;
        }

    //  Mining

    if (iDamage = pShot->GetSpecialDamage(specialMining, DamageDesc::flagSpecialAdj))
        {
        retBalance.rMining = BALANCE_MINING_FACTOR * iDamage;
        retBalance.rBalance += retBalance.rMining;
        }

    //  WMD
    //
    //  All weapons have some degree of WMD, but we only count the ones that 
    //  have non-default WMD.

    if (pShot->GetSpecialDamage(specialWMD, DamageDesc::flagSpecialLevel))
        {
        retBalance.rWMD = BALANCE_WMD_FACTOR * pShot->GetSpecialDamage(specialWMD, DamageDesc::flagSpecialAdj);
        retBalance.rBalance += retBalance.rWMD;
        }

    //  Done

    return (int)retBalance.rBalance;
	}

int CWeaponClass::CalcConfiguration (CItemCtx &ItemCtx, CWeaponFireDesc *pShot, int iFireAngle, CVector *ShotPos, int *ShotDir, bool bSetAlternating) const

//	CalcConfiguration
//
//	Returns the number of shots in the configuration and initializes
//	ShotPos and ShotDir appropriately.
//
//	ShotPos and ShotDir must be at least MAX_SHOT_COUNT
//
//	If ItemCtx has pSource and pDevice then ShotPos and ShotDir are absolute (source position
//	and direction has been applied). Otherwise, they are relative (caller must add to source pos and direction)
//
//	If iFireAngle is -1 and we have pSource and pDevice, we compute the current facing of the ship and device.
//
//	Returns the number of shots.

	{
	int i;
	CSpaceObject *pSource = ItemCtx.GetSource();
	CInstalledDevice *pDevice = ItemCtx.GetDevice();

	//	Compute the source position

	CVector vSource;
	if (pSource && pDevice)
		vSource = pDevice->GetPos(pSource);

	//	Compute the fire direction

	if (iFireAngle == -1)
		iFireAngle = GetDefaultFireAngle(pDevice, pSource);

	//	Fill in the arrays

	int iShotCount = 0;
	switch (m_Configuration)
		{
		case ctDual:
			{
			//	Compute a normal perpendicular to the direction of fire

			CVector Perp = PolarToVector(iFireAngle, (g_KlicksPerPixel * g_DualShotSeparation));
			Perp = Perp.Perpendicular();

			//	Create two shots

			iShotCount = 2;
			ShotPos[0] = vSource + Perp;
			ShotDir[0] = iFireAngle;

			ShotPos[1] = vSource - Perp;
			ShotDir[1] = iFireAngle;

			break;
			}

		case ctDualAlternating:
			{
			//	Compute a normal perpendicular to the direction of fire

			CVector Perp = PolarToVector(iFireAngle, (g_KlicksPerPixel * g_DualShotSeparation));
			Perp = Perp.Perpendicular();

			//	If we have a device, the alternate

			if (pDevice)
				{
				//	Figure out our polarity

				int iPolarity = GetAlternatingPos(pDevice);

				//	Create a shot either from the left or from the right depending
				//	on the continuous shot variable.

				iShotCount = 1;
				ShotPos[0] = vSource + (iPolarity ? Perp : -Perp);
				ShotDir[0] = iFireAngle;

				if (bSetAlternating)
					SetAlternatingPos(pDevice, (iPolarity + 1) % 2);
				}

			//	Otherwise, return all shots

			else
				{
				iShotCount = 2;
				ShotPos[0] = vSource + Perp;
				ShotDir[0] = iFireAngle;

				ShotPos[1] = vSource - Perp;
				ShotDir[1] = iFireAngle;
				}
			break;
			}

		case ctWall:
			{
			//	Compute a normal perpendicular to the direction of fire

			CVector Perp = PolarToVector(iFireAngle, (g_KlicksPerPixel * g_DualShotSeparation));
			Perp = Perp.Perpendicular();

			//	Create five shots

			iShotCount = 0;
			for (i = -2; i <= 2; i++)
				{
				ShotPos[iShotCount] = vSource + ((Metric)i * Perp);
				ShotDir[iShotCount] = iFireAngle;
				iShotCount++;
				}

			break;
			}

		case ctSpread5:
			{
			//	Shots at +2 and -2 degrees

			ShotPos[iShotCount] = vSource;
			ShotDir[iShotCount] = (iFireAngle + 2) % 360;
			iShotCount++;

			ShotPos[iShotCount] = vSource;
			ShotDir[iShotCount] = (iFireAngle + 358) % 360;
			iShotCount++;

			//	Fallthrough!
			}

		case ctSpread3:
			{
			//	Shot at 0 degrees

			ShotPos[iShotCount] = vSource;
			ShotDir[iShotCount] = iFireAngle;
			iShotCount++;

			//	Fallthrough!
			}

		case ctSpread2:
			{
			//	Shots at +5 and -5 degrees

			ShotPos[iShotCount] = vSource;
			ShotDir[iShotCount] = (iFireAngle + 5) % 360;
			iShotCount++;

			ShotPos[iShotCount] = vSource;
			ShotDir[iShotCount] = (iFireAngle + 355) % 360;
			iShotCount++;

			break;
			}

		case ctCustom:
			{
			if (m_bConfigAlternating && pDevice)
				{
				//	Figure out which shot we're firing

				int iShot = Max(0, Min(GetAlternatingPos(pDevice), m_iConfigCount));

				iShotCount = 1;
				ShotPos[0] = vSource + PolarToVector(AngleMod(iFireAngle + m_pConfig[iShot].iPosAngle), m_pConfig[iShot].rPosRadius);
				ShotDir[0] = AngleMod(iFireAngle + m_pConfig[iShot].Angle.Roll());

				//	Next shot in sequence

				if (bSetAlternating)
					SetAlternatingPos(pDevice, (iShot + 1) % m_iConfigCount);
				}
			else
				{
				iShotCount = Min(MAX_SHOT_COUNT, m_iConfigCount);

				for (i = 0; i < iShotCount; i++)
					{
					ShotPos[i] = vSource + PolarToVector(AngleMod(iFireAngle + m_pConfig[i].iPosAngle), m_pConfig[i].rPosRadius);
					ShotDir[i] = AngleMod(iFireAngle + m_pConfig[i].Angle.Roll());
					}
				}
			break;
			}

		default:
			{
			iShotCount = 1;
			ShotPos[0] = vSource;
			ShotDir[0] = iFireAngle;
			}
		}

	return iShotCount;
	}

Metric CWeaponClass::CalcConfigurationMultiplier (CWeaponFireDesc *pShot, bool bIncludeFragments) const

//	CalcConfigurationMultiplier
//
//	Calculates multiplier

	{
	if (pShot == NULL)
		{
        pShot = GetWeaponFireDesc(CItemCtx());
        if (pShot == NULL)
			return 1.0;
		}

	Metric rMult = 1.0;
	switch (m_Configuration)
		{
		case ctDual:
		case ctSpread2:
			rMult = 2.0;
			break;

		case ctSpread3:
			rMult = 3.0;
			break;

		case ctWall:
		case ctSpread5:
			rMult = 5.0;
			break;

		case ctCustom:
			if (m_bConfigAlternating)
				rMult = 1.0;
			else
				rMult = m_iConfigCount;
			break;
		}

	if (pShot->GetContinuous() > 0)
		rMult *= (pShot->GetContinuous() + 1);

	//	Include passthrough.
    //
    //  If the probability of passthrough is P, then the expected number of hits
    //  is:
    //
    //  hits = Sum(n=1-infinity) n * (P^(n-1) - P^n)
    //
    //  Empirically this converges to:
    //
    //  hits = 1/(1-P)
    //
    //  Note, however, that since targets are finite size, we put a limit on the
    //  number of passthroughs we expect.

    if (pShot->GetPassthrough() > 0)
        {
        Metric rPassthroughProb = Min(0.99, pShot->GetPassthrough() / 100.0);
        rMult *= Min(1.0 / (1.0 - rPassthroughProb), MAX_EXPECTED_PASSTHROUGH);
        }

	//	Compute fragment count

	if (bIncludeFragments
			&& pShot->HasFragments()
			&& pShot->GetFirstFragment()->Count.GetMaxValue() > 1)
		{
		//	Multiply by the average

		rMult *= pShot->GetFirstFragment()->Count.GetAveValue();

		//	If tracking, assume all will hit. Otherwise, we assume that
		//	only a fraction will hit.

		if (!pShot->GetFirstFragment()->pDesc->IsTracking())
			rMult *= EXPECTED_FRAGMENT_HITS;
		}

	return rMult;
	}

Metric CWeaponClass::CalcDamage (CWeaponFireDesc *pShot, const CItemEnhancementStack *pEnhancements, DWORD dwDamageFlags) const

//	CalcDamage
//
//	Computes damage for the given weapon fire desc.

	{
	//	If we have fragments we need to recurse

	if (pShot->HasFragments())
		{
		Metric rTotal = 0.0;

		CWeaponFireDesc::SFragmentDesc *pFragment = pShot->GetFirstFragment();
		while (pFragment)
			{
			//	By default, 1/8 of fragments hit, unless the fragments are radius type

			Metric rHitFraction;
			switch (pFragment->pDesc->GetType())
				{
				case ftArea:
				case ftRadius:
					rHitFraction = 1.0;
					break;

				default:
                    if (pFragment->pDesc->IsTracking())
                        rHitFraction = EXPECTED_TRACKING_FRAGMENT_HITS;
                    else
                        rHitFraction = EXPECTED_FRAGMENT_HITS;
                    break;
				}

            //  Adjust for passthrough

            if (pFragment->pDesc->GetPassthrough() > 0)
                {
                Metric rPassthroughProb = Min(0.99, pFragment->pDesc->GetPassthrough() / 100.0);
                rHitFraction *= Min(1.0 / (1.0 - rPassthroughProb), MAX_EXPECTED_PASSTHROUGH);
                }

			//	Add up values

			rTotal += rHitFraction * pFragment->Count.GetAveValueFloat() * CalcDamage(pFragment->pDesc, pEnhancements, dwDamageFlags);

			pFragment = pFragment->pNext;
			}

		return rTotal;
		}
	else
		{
		Metric rDamage;

		//	Average damage depends on type

		switch (pShot->GetType())
			{
			case ftArea:
				//	Assume 1/8th damage points hit on average
				rDamage = EXPECTED_SHOCKWAVE_HITS * pShot->GetAreaDamageDensityAverage() * pShot->GetDamage().GetDamageValue(dwDamageFlags);
				break;

			case ftRadius:
				//	Assume average target is far enough away to take half damage
				rDamage = EXPECTED_RADIUS_DAMAGE * pShot->GetDamage().GetDamageValue(dwDamageFlags);
				break;

			default:
				rDamage = pShot->GetDamage().GetDamageValue(dwDamageFlags);
			}

		//	If we have a capacitor, adjust damage

		switch (m_Counter)
			{
			case cntCapacitor:
				{
				//	Compute the number of ticks until we discharge the capacitor

				Metric rFireTime = (MAX_COUNTER / (Metric)-m_iCounterActivate) * m_iFireRate;

				//	Compute the number of ticks to recharge

				Metric rRechargeTime = (MAX_COUNTER / (Metric)m_iCounterUpdate) * m_iCounterUpdateRate;

				//	Adjust damage by the fraction of time that we spend firing.
                //  Remember that we're recharging even while firing, so we 
                //  only care about the ratio.

                if (rRechargeTime > rFireTime)
					rDamage *= rFireTime / rRechargeTime;

				break;
				}

            case cntTemperature:
                {
                //  Compute the number of ticks until we reach max temp

                Metric rFireTime = (MAX_COUNTER / (Metric)m_iCounterActivate) * m_iFireRate;

                //  Compute the number of ticks to cool down

                Metric rCoolDownTime = (MAX_COUNTER / (Metric)-m_iCounterUpdate) * m_iCounterUpdateRate;

                //  Adjust damage by the fraction of time that we spend firing.

                if (rCoolDownTime > rFireTime)
                    rDamage *= rFireTime / rCoolDownTime;

                break;
                }
			}

		//	Adjust for enhancements

		if (pEnhancements)
			rDamage += (rDamage * pEnhancements->GetBonus() / 100.0);

		//	Done

		return rDamage;
		}
	}

Metric CWeaponClass::CalcDamagePerShot (CWeaponFireDesc *pShot, const CItemEnhancementStack *pEnhancements, DWORD dwDamageFlags) const

//	CalcDamagePerShot
//
//	Returns average damage per shot

	{
	return CalcConfigurationMultiplier(pShot, false) * CalcDamage(pShot, pEnhancements, dwDamageFlags);
	}

int CWeaponClass::CalcFireAngle (CItemCtx &ItemCtx, Metric rSpeed, CSpaceObject *pTarget, bool *retbOutOfArc)

//	CalcFireAngle
//
//	Calculates the default fire angle for the weapon

	{
	CSpaceObject *pSource = ItemCtx.GetSource();
	if (pSource == NULL)
		return -1;

	CInstalledDevice *pDevice = ItemCtx.GetDevice();
	if (pDevice == NULL)
		return -1;

	if (pTarget && CanRotate(ItemCtx))
		{
		int iFireAngle;

		CVector vSource = pDevice->GetPos(pSource);

		//	Get the position and velocity of the target

		CVector vTarget = pTarget->GetPos() - vSource;
		CVector vTargetVel = pTarget->GetVel() - pSource->GetVel();

		//	Figure out which direction to fire in

		Metric rTimeToIntercept = CalcInterceptTime(vTarget, vTargetVel, rSpeed);
		CVector vInterceptPoint = (rTimeToIntercept > 0.0 ? vTarget + vTargetVel * rTimeToIntercept : vTarget);
		iFireAngle = VectorToPolar(vInterceptPoint, NULL);

		//	If this is a directional weapon make sure we are in-bounds

		int iMinFireAngle, iMaxFireAngle;
		if (IsDirectional(pDevice, &iMinFireAngle, &iMaxFireAngle))
			{
			int iMin = (pSource->GetRotation() + iMinFireAngle) % 360;
			int iMax = (pSource->GetRotation() + iMaxFireAngle) % 360;

			if (iMin < iMax)
				{
				if (iFireAngle < iMin)
					iFireAngle = iMin;
				else if (iFireAngle > iMax)
					iFireAngle = iMax;
				}
			else
				{
				if (iFireAngle < iMin && iFireAngle > iMax)
					{
					int iToMax = iFireAngle - iMax;
					int iToMin = iMin - iFireAngle;

					if (iToMax > iToMin)
						iFireAngle = iMin;
					else
						iFireAngle = iMax;
					}
				}
			}

		//	Remember the fire angle (we need it later if this is a continuous
		//	fire device)

		pDevice->SetFireAngle(iFireAngle);

		return iFireAngle;
		}
	else
		return GetDefaultFireAngle(pDevice, pSource);
	}

int CWeaponClass::CalcFireSolution (CInstalledDevice *pDevice, CSpaceObject *pSource, CSpaceObject *pTarget)

//	CalcFireSolution
//
//	Computes the direction to fire to hit the target (or -1 if no solution)

	{
	ASSERT(pTarget);

	CItemCtx Ctx(pSource, pDevice);
	CWeaponFireDesc *pShot = GetWeaponFireDesc(Ctx);
	if (pShot == NULL)
		return -1;

	//	Compute source

	CVector vSource = pDevice->GetPos(pSource);

	//	We need to calculate the intercept solution.

	Metric rWeaponSpeed = pShot->GetRatedSpeed();
	CVector vTarget = pTarget->GetPos() - vSource;
	CVector vTargetVel = pTarget->GetVel() - pSource->GetVel();

	//	Figure out intercept time

	Metric rDist;
	Metric rTimeToIntercept = CalcInterceptTime(vTarget, vTargetVel, rWeaponSpeed, &rDist);
	if (rTimeToIntercept < 0.0)
		return -1;

	//	Compute direction

	CVector vInterceptPoint = vTarget + vTargetVel * rTimeToIntercept;
	return VectorToPolar(vInterceptPoint, NULL);
	}

int CWeaponClass::CalcLevel (CWeaponFireDesc *pShot) const

//  CalcLevel
//
//  Returns the level of the given shot.

    {
    return Max(1, Min(pShot->GetLevel(), MAX_ITEM_LEVEL));
    }

int CWeaponClass::CalcPowerUsed (CInstalledDevice *pDevice, CSpaceObject *pSource)

//	CalcPowerUsed
//
//	Returns the power consumed

	{
	CItemCtx Ctx(pSource, pDevice);

	if (!pDevice->IsEnabled())
		return 0;

	//	We consume less power when we are fully charged

	int iPower = m_iPowerUse;
	if (pDevice->IsReady() && (!pDevice->IsTriggered() || !pDevice->IsLastActivateSuccessful()))
		iPower = m_iIdlePowerUse;

	//	Adjust based on power efficiency enhancement

	const CItemEnhancementStack *pEnhancements = Ctx.GetEnhancementStack();
	if (pEnhancements)
		iPower = iPower * pEnhancements->GetPowerAdj() / 100;

	return iPower;
	}

int CWeaponClass::CalcRotateRange (CItemCtx &ItemCtx) const

//  CalcRotateRange
//
//  Calculates the number of degrees of arc that this weapon is able to rotate.
//  E.g., omni weapons = 360; fixed weapons = 0.

    {
    int iMinArc;
    int iMaxArc;
    if (!CanRotate(ItemCtx, &iMinArc, &iMaxArc))
        return 0;

    if (iMinArc == iMaxArc)
        return 360;

    return AngleRange(iMinArc, iMaxArc);
    }

bool CWeaponClass::CanRotate (CItemCtx &Ctx, int *retiMinFireArc, int *retiMaxFireArc) const

//	CanRotate
//
//	Returns TRUE if the weapon is either omnidirectional or directional

	{
	//	If the weapon is natively on a turret

	if (m_bOmnidirectional)
		{
		if (retiMinFireArc)
			*retiMinFireArc = 0;
		if (retiMaxFireArc)
			*retiMaxFireArc = 0;
		return true;
		}
	else if (m_iMinFireArc != m_iMaxFireArc)
		{
		if (retiMinFireArc)
			*retiMinFireArc = m_iMinFireArc;
		if (retiMaxFireArc)
			*retiMaxFireArc = m_iMaxFireArc;
		return true;
		}

	//	If the device slot is a turret

	else if (Ctx.GetDevice())
		{
		if (Ctx.GetDevice()->IsOmniDirectional())
			{
			if (retiMinFireArc)
				*retiMinFireArc = 0;
			if (retiMaxFireArc)
				*retiMaxFireArc = 0;
			return true;
			}
		else if (Ctx.GetDevice()->IsDirectional())
			{
			if (retiMinFireArc)
				*retiMinFireArc = Ctx.GetDevice()->GetMinFireArc();
			if (retiMaxFireArc)
				*retiMaxFireArc = Ctx.GetDevice()->GetMaxFireArc();
			return true;
			}
		else
			return false;
		}

	//	Fixed weapon

	else
		return false;
	}

ALERROR CWeaponClass::CreateFromXML (SDesignLoadCtx &Ctx, CXMLElement *pDesc, CItemType *pType, CDeviceClass **retpWeapon)

//	CreateFromXML
//
//	Creates from an XML element

	{
	ALERROR error;
	int i;
	CWeaponClass *pWeapon;

	pWeapon = new CWeaponClass;
	if (pWeapon == NULL)
		return ERR_MEMORY;

	if (error = pWeapon->InitDeviceFromXML(Ctx, pDesc, pType))
		return error;

	//	Basics

	pWeapon->m_iFireRateSecs = pDesc->GetAttributeIntegerBounded(FIRE_RATE_ATTRIB, 0, -1, 16);
	pWeapon->m_iFireRate = (int)((pWeapon->m_iFireRateSecs / STD_SECONDS_PER_UPDATE) + 0.5);
	pWeapon->m_iPowerUse = pDesc->GetAttributeIntegerBounded(POWER_USE_ATTRIB, 0, -1, 0);
	pWeapon->m_iIdlePowerUse = pDesc->GetAttributeIntegerBounded(IDLE_POWER_USE_ATTRIB, 0, -1, pWeapon->m_iPowerUse / 10);
	pWeapon->m_iRecoil = pDesc->GetAttributeInteger(RECOIL_ATTRIB);
	pWeapon->m_iFailureChance = pDesc->GetAttributeInteger(FAILURE_CHANCE_ATTRIB);
	pWeapon->m_iMinFireArc = AngleMod(pDesc->GetAttributeInteger(MIN_FIRE_ARC_ATTRIB));
	pWeapon->m_iMaxFireArc = AngleMod(pDesc->GetAttributeInteger(MAX_FIRE_ARC_ATTRIB));

	pWeapon->m_bCharges = pDesc->GetAttributeBool(CHARGES_ATTRIB);
	pWeapon->m_bOmnidirectional = pDesc->GetAttributeBool(OMNIDIRECTIONAL_ATTRIB);
	pWeapon->m_bMIRV = pDesc->GetAttributeBool(MULTI_TARGET_ATTRIB);
	pWeapon->m_bReportAmmo = pDesc->GetAttributeBool(REPORT_AMMO_ATTRIB);
	pWeapon->m_bTargetStationsOnly = pDesc->GetAttributeBool(TARGET_STATIONS_ONLY_ATTRIB);

	//	Configuration

	CString sConfig = pDesc->GetAttribute(CONFIGURATION_ATTRIB);
	if (strEquals(sConfig, CONFIG_TYPE_DUAL))
		pWeapon->m_Configuration = ctDual;
	else if (strEquals(sConfig, CONFIG_TYPE_WALL))
		pWeapon->m_Configuration = ctWall;
	else if (strEquals(sConfig, CONFIG_TYPE_SPREAD2))
		pWeapon->m_Configuration = ctSpread2;
	else if (strEquals(sConfig, CONFIG_TYPE_SPREAD3))
		pWeapon->m_Configuration = ctSpread3;
	else if (strEquals(sConfig, CONFIG_TYPE_SPREAD5))
		pWeapon->m_Configuration = ctSpread5;
	else if (strEquals(sConfig, CONFIG_TYPE_DUAL_ALTERNATING))
		pWeapon->m_Configuration = ctDualAlternating;
	else
		{
		CXMLElement *pConfig = pDesc->GetContentElementByTag(CONFIGURATION_TAG);
		if (pConfig && pConfig->GetContentElementCount())
			{
			pWeapon->m_Configuration = ctCustom;

			pWeapon->m_iConfigCount = pConfig->GetContentElementCount();
			pWeapon->m_pConfig = new SConfigDesc [pWeapon->m_iConfigCount];

			for (i = 0; i < pWeapon->m_iConfigCount; i++)
				{
				CXMLElement *pShotConfig = pConfig->GetContentElement(i);
				pWeapon->m_pConfig[i].Angle.LoadFromXML(pShotConfig->GetAttribute(ANGLE_ATTRIB));
				pWeapon->m_pConfig[i].iPosAngle = AngleMod(pShotConfig->GetAttributeInteger(POS_ANGLE_ATTRIB));
				pWeapon->m_pConfig[i].rPosRadius = g_KlicksPerPixel * pShotConfig->GetAttributeInteger(POS_RADIUS_ATTRIB);
				}

			pWeapon->m_iConfigAimTolerance = pConfig->GetAttributeInteger(AIM_TOLERANCE_ATTRIB);
			if (pWeapon->m_iConfigAimTolerance == 0)
				pWeapon->m_iConfigAimTolerance = 5;

			pWeapon->m_bConfigAlternating = pConfig->GetAttributeBool(ALTERNATING_ATTRIB);
			}
		else
			pWeapon->m_Configuration = ctSingle;
		}

	//	Counter

	sConfig = pDesc->GetAttribute(COUNTER_ATTRIB);
	if (!sConfig.IsBlank())
		{
		if (strEquals(sConfig, COUNTER_TYPE_TEMPERATURE))
			pWeapon->m_Counter = cntTemperature;
		else if (strEquals(sConfig, COUNTER_TYPE_CAPACITOR))
			pWeapon->m_Counter = cntCapacitor;
		else
			{
			Ctx.sError = CONSTLIT("Invalid weapon counter type");
			return ERR_FAIL;
			}

		pWeapon->m_iCounterActivate = pDesc->GetAttributeInteger(COUNTER_ACTIVATE_ATTRIB);
		pWeapon->m_iCounterUpdate = pDesc->GetAttributeInteger(COUNTER_UPDATE_ATTRIB);
		pWeapon->m_iCounterUpdateRate = pDesc->GetAttributeInteger(COUNTER_UPDATE_RATE_ATTRIB);
		if (pWeapon->m_iCounterUpdateRate <= 0)
			pWeapon->m_iCounterUpdateRate = 1;
		}
	else if ((pWeapon->m_iCounterActivate = pDesc->GetAttributeInteger(HEATING_ATTRIB)) > 0)
		{
		//	Backward compatibility

		pWeapon->m_Counter = cntTemperature;
		pWeapon->m_iCounterUpdate = TEMP_DECREASE;
		pWeapon->m_iCounterUpdateRate = pDesc->GetAttributeInteger(COOLING_RATE_ATTRIB);
		if (pWeapon->m_iCounterUpdateRate <= 0)
			pWeapon->m_iCounterUpdateRate = 1;
		}
	else
		{
		pWeapon->m_Counter = cntNone;
		pWeapon->m_iCounterActivate = 0;
		pWeapon->m_iCounterUpdate = 0;
		pWeapon->m_iCounterUpdateRate = 0;
		}

	//	Linked fire options

	CString sLinkedFire;
	if (pDesc->FindAttribute(LINKED_FIRE_ATTRIB, &sLinkedFire))
		{
		if (error = ParseLinkedFireOptions(Ctx, sLinkedFire, &pWeapon->m_dwLinkedFireOptions))
			return error;
		}
	else
		pWeapon->m_dwLinkedFireOptions = 0;

	//	Initialize the variants, whether from missiles, level scaling, or 
	//	charges.

	if (error = pWeapon->InitVariantsFromXML(Ctx, pDesc, pType))
		return error;

	//	Done

	*retpWeapon = pWeapon;

	return NOERROR;
	}

bool CWeaponClass::FindAmmoDataField (const CItem &Ammo, const CString &sField, CString *retsValue) const

//	FindAmmoDataField
//
//	Returns meta-data

	{
	int i;

	//	Make sure we actually fire the given ammo. We need to check here because
	//	GetWeaponFireDesc doesn't check.

	ASSERT(Ammo.IsEmpty() || GetAmmoVariant(Ammo.GetType()) != -1);

    CWeaponFireDesc *pShot = GetWeaponFireDesc(CItemCtx(), Ammo);
	if (pShot == NULL)
		return false;

	if (strEquals(sField, FIELD_AMMO_TYPE))
		{
		CItemType *pAmmoType = pShot->GetAmmoType();
		*retsValue = (pAmmoType ? strFromInt(pAmmoType->GetUNID()) : NULL_STR);
		}
	else if (strEquals(sField, FIELD_HP))
		*retsValue = strFromInt(pShot->GetHitPoints());
	else if (strEquals(sField, FIELD_MIN_DAMAGE))
		*retsValue = strFromInt((int)(CalcConfigurationMultiplier(pShot) * pShot->GetDamage().GetMinDamage()));
	else if (strEquals(sField, FIELD_MAX_DAMAGE))
		*retsValue = strFromInt((int)(CalcConfigurationMultiplier(pShot) * pShot->GetDamage().GetMaxDamage()));
	else if (strEquals(sField, FIELD_DAMAGE_TYPE))
		*retsValue = strFromInt(pShot->GetDamageType());
	else if (strEquals(sField, FIELD_FIRE_DELAY))
		*retsValue = strFromInt(m_iFireRate);
	else if (strEquals(sField, FIELD_FIRE_RATE))
		{
		if (m_iFireRate)
			*retsValue = strFromInt(1000 / m_iFireRate);
		else
			return false;
		}
	else if (strEquals(sField, FIELD_AVERAGE_DAMAGE))
		*retsValue = strFromInt((int)(CalcDamagePerShot(pShot) * 1000.0));
	else if (strEquals(sField, FIELD_DAMAGE_180))
		{
		Metric rDamagePerShot = CalcDamagePerShot(pShot);
		*retsValue = (m_iFireRate > 0 ? strFromInt((int)((rDamagePerShot * 180.0 / m_iFireRate) + 0.5)) : strFromInt((int)(rDamagePerShot + 0.5)));
		}
	else if (strEquals(sField, FIELD_POWER))
		*retsValue = strFromInt(m_iPowerUse * 100);
	else if (strEquals(sField, FIELD_POWER_PER_SHOT))
		*retsValue = strFromInt((int)(((m_iFireRate * m_iPowerUse * STD_SECONDS_PER_UPDATE * 1000) / 600.0) + 0.5));
    else if (strEquals(sField, FIELD_BALANCE))
        {
        CItem Item(GetItemType(), 1);
        CItemCtx ItemCtx(Item);
        SBalance Balance;
		*retsValue = strFromInt(CalcBalance(ItemCtx, Balance));
        }
	else if (strEquals(sField, FIELD_RANGE))
		*retsValue = strFromInt((int)((pShot->GetMaxRange() / LIGHT_SECOND) + 0.5));
	else if (strEquals(sField, FIELD_RECOIL))
		*retsValue = (m_iRecoil ? strFromInt((int)(m_iRecoil * m_iRecoil * 10 * g_MomentumConstant / g_SecondsPerUpdate)) : NULL_STR);
	else if (strEquals(sField, FIELD_SPEED))
		*retsValue = strFromInt((int)((100.0 * pShot->GetRatedSpeed() / LIGHT_SECOND) + 0.5));
	else if (strEquals(sField, FIELD_VARIANT_COUNT))
		*retsValue = strFromInt(GetSelectVariantCount());
	else if (strEquals(sField, FIELD_REPEAT_COUNT))
		*retsValue = strFromInt(pShot->GetContinuous() + 1);
	else if (strEquals(sField, FIELD_CONFIGURATION))
		{
		CCodeChain &CC = g_pUniverse->GetCC();
		CVector ShotPos[MAX_SHOT_COUNT];
		int ShotDir[MAX_SHOT_COUNT];
		int iShotCount = CalcConfiguration(CItemCtx(), pShot, 0, ShotPos, ShotDir, false);

		CMemoryWriteStream Output(5000);
		if (Output.Create() != NOERROR)
			return false;

		Output.Write("='(", 3);
		for (i = 0; i < iShotCount; i++)
			{
			ICCItem *pPos = CreateListFromVector(CC, ShotPos[i]);
			if (pPos == NULL || pPos->IsError())
				return false;

			Output.Write("(", 1);
			CString sItem = pPos->Print(&CC);
			Output.Write(sItem.GetASCIIZPointer(), sItem.GetLength());
			Output.Write(" ", 1);

			sItem = strFromInt(ShotDir[i]);
			Output.Write(sItem.GetASCIIZPointer(), sItem.GetLength());
			Output.Write(") ", 2);

			pPos->Discard(&CC);
			}
		Output.Write(")", 1);
		Output.Close();

		*retsValue = CString(Output.GetPointer(), Output.GetLength());
		return true;
		}
	else if (strEquals(sField, FIELD_IS_ALTERNATING))
		*retsValue = ((m_Configuration == ctDualAlternating || m_bConfigAlternating) ? CString("True") : NULL_STR);
	else
		return pShot->FindDataField(sField, retsValue);

	return true;
	}

bool CWeaponClass::FindDataField (const CString &sField, CString *retsValue)

//	FindDataField
//
//	Returns meta-data

	{
	CString sRootField = sField;
	int iVariant = 0;

	//	Look for a :nn suffix specifying a variant

	char *pPos = sField.GetASCIIZPointer();
	while (*pPos != '\0')
		{
		if (*pPos == ':')
			{
			sRootField.Truncate(pPos - sField.GetASCIIZPointer());
			iVariant = strParseInt(pPos + 1, 0);
			break;
			}
		pPos++;
		}

    CItem Ammo;
    if (iVariant >= 0 && iVariant < GetAmmoItemCount())
        Ammo = CItem(GetAmmoItem(iVariant), 1);

	return FindAmmoDataField(Ammo, sRootField, retsValue);
	}

CWeaponClass::EOnFireWeaponResults CWeaponClass::FireOnFireWeapon (CItemCtx &ItemCtx, 
																   CWeaponFireDesc *pShot,
																   const CVector &vSource,
																   CSpaceObject *pTarget,
																   int iFireAngle,
																   int iRepeatingCount)

//	FireOnFireWeapon
//
//	Fires OnFireWeapon event
//
//	default (or Nil) = fire weapon as normal
//	noShot = do not consume power or ammo
//	shotFired (or True) = consume power and ammo normally

	{
	SEventHandlerDesc Event;
	if (FindEventHandlerWeaponClass(evtOnFireWeapon, &Event))
		{
		CCodeChainCtx Ctx;
		EOnFireWeaponResults iResult;
		const CItemEnhancementStack *pEnhancement = ItemCtx.GetEnhancementStack();

		Ctx.SaveAndDefineSourceVar(ItemCtx.GetSource());
		Ctx.SaveAndDefineItemVar(ItemCtx);
		Ctx.DefineInteger(CONSTLIT("aFireAngle"), iFireAngle);
		Ctx.DefineVector(CONSTLIT("aFirePos"), vSource);
		Ctx.DefineInteger(CONSTLIT("aFireRepeat"), iRepeatingCount);
		Ctx.DefineSpaceObject(CONSTLIT("aTargetObj"), pTarget);
		Ctx.DefineInteger(CONSTLIT("aWeaponBonus"), (pEnhancement ? pEnhancement->GetBonus() : 0));
		Ctx.DefineItemType(CONSTLIT("aWeaponType"), pShot->GetWeaponType());

		ICCItem *pResult = Ctx.Run(Event);
		if (pResult->IsError())
			ItemCtx.GetSource()->ReportEventError(ON_FIRE_WEAPON_EVENT, pResult);

		if (pResult->IsNil())
			iResult = resDefault;
		else
			{
			CString sValue = pResult->GetStringValue();
			if (strEquals(sValue, CONSTLIT("shotFired")))
				return resShotFired;
			else if (strEquals(sValue, CONSTLIT("noShot")))
				return resNoShot;
			else if (strEquals(sValue, CONSTLIT("default")))
				return resDefault;
			else
				return resShotFired;
			}

		Ctx.Discard(pResult);

		//	Done

		return iResult;
		}
	else
		return resDefault;
	}

bool CWeaponClass::FireWeapon (CInstalledDevice *pDevice, 
							   CWeaponFireDesc *pShot, 
							   CSpaceObject *pSource, 
							   CSpaceObject *pTarget,
							   int iRepeatingCount,
							   bool *retbSourceDestroyed,
							   bool *retbConsumedItems)

//	FireWeapon
//
//	Fires the weapon. Returns TRUE if we should consume power, etc.

	{
	int i;
    CItemCtx ItemCtx(pSource, pDevice);

	//	Figure out the source of the shot

	CVector vSource = pDevice->GetPos(pSource);

	//	Figure out the speed of the shot

	Metric rSpeed = pShot->GetInitialSpeed();

	//	Slight HACK: If we have no target and this is a tracking weapon
	//	then we get the target from the device. We do this here because
	//	it is somewhat expensive to get the target from the device so
	//	we only do it if we really need it.

	if (pTarget == NULL && pShot->IsTrackingOrHasTrackingFragments())
		pTarget = pDevice->GetTarget(pSource);

	//	Get the fire angle from the device (the AI sets it when it has pre-
	//	calculated the target and fire solution).

	int iFireAngle = pDevice->GetFireAngle();

	//	If the fire angle is -1 then we need to calc it ourselves

	if (iFireAngle == -1)
		{
		bool bOutOfArc;
		iFireAngle = CalcFireAngle(ItemCtx, rSpeed, pTarget, &bOutOfArc);
		}

	//	Pre-init

	if (retbConsumedItems)
		*retbConsumedItems = false;
	if (retbSourceDestroyed)
		*retbSourceDestroyed = false;

	//	Figure out if fire is suppressed by some other object
	//	on the source

	bool bFireSuppressed = pSource->AbsorbWeaponFire(pDevice);

	//	If we're prone to failure, then bad things happen

	bool bFailure = false;
	if (m_iFailureChance > 0)
		{
		if (mathRandom(1, 100) <= m_iFailureChance)
			bFailure = true;
		}

	//	If we're damaged, figure out what bad things happen

	if (pDevice->IsDamaged() || pDevice->IsDisrupted())
		bFailure = true;

	bool bMisfire = false;
	bool bExplosion = false;
	if (bFailure)
		{
		int iRoll = mathRandom(1, 100);

		//	10% of the time, everything works OK

		if (iRoll <= 10)
			;

		//	60% of the time, nothing happens

		else if (iRoll <= 70)
			{
			pSource->OnDeviceStatus(pDevice, failWeaponJammed);

			//	Still counts--we should consume power

			return true;
			}

		//	20% of the time, we fire in a random direction

		else if (iRoll <= 90)
			{
			bMisfire = true;
			pSource->OnDeviceStatus(pDevice, failWeaponMisfire);
			}

		//	10% of the time, the shot explodes

		else
			{
			bExplosion = true;
			bFireSuppressed = true;
			pSource->OnDeviceStatus(pDevice, failWeaponExplosion);
			}
		}

	//	Deal with counter

	switch (m_Counter)
		{
		case cntTemperature:
			{
			if (pDevice->GetTemperature() >= OVERHEAT_TEMP)
				{
				if (pDevice->GetTemperature() >= MAX_TEMP)
					{
					pSource->OnDeviceStatus(pDevice, failWeaponJammed);

					//	Still counts--consume power

					return true;
					}
				else
					{
					int iRoll = mathRandom(1, 100);

					//	25% of the time, everything works OK

					if (iRoll <= 25)
						;

					//	25% of the time, the weapon jams

					else if (iRoll <= 50)
						{
						pSource->OnDeviceStatus(pDevice, failWeaponJammed);
						return true;
						}

					//	25% of the time, the weapon is disabled

					else if (iRoll <= 75)
						{
						pSource->DisableDevice(pDevice);
						return true;
						}

					//	25% of the time, the weapon is damaged

					else
						{
						pSource->DamageItem(pDevice);
						pSource->OnDeviceStatus(pDevice, failDeviceOverheat);
						}
					}
				}

			//	Update temperature

			pDevice->IncTemperature(m_iCounterActivate);
			pSource->OnComponentChanged(comDeviceCounter);
			break;
			}

		case cntCapacitor:
			{
			//	If we don't have enough capacitor power, then we can't fire

			if (pDevice->GetTemperature() < m_iCounterActivate)
				return false;

			//	Consume capacitor

			pDevice->IncTemperature(m_iCounterActivate);
			pSource->OnComponentChanged(comDeviceCounter);
			break;
			}
		}

	//	If this is a misfire, adjust the angle

	if (bMisfire)
		iFireAngle = (iFireAngle + mathRandom(-60, 60) + 360) % 360;

	//	Figure out how many shots to create

	CVector ShotPos[MAX_SHOT_COUNT];
	int ShotDir[MAX_SHOT_COUNT];
	int iShotCount = CalcConfiguration(ItemCtx, pShot, iFireAngle, ShotPos, ShotDir, (iRepeatingCount == pShot->GetContinuous()));
    if (iShotCount <= 0)
        return false;

	//	If we're independently targeted, then we generate an array of separate
    //  targets for each shot.

    CSpaceObject *MIRVTarget[MAX_SHOT_COUNT];
	if (m_bMIRV)
		{
        //  The first shot always goes to the current target.

        MIRVTarget[0] = pTarget;

        //  Now initialize the remainder

        if (iShotCount > 1)
            {
		    TArray<CSpaceObject *> TargetList;
		    int iFound = pSource->GetNearestVisibleEnemies(iShotCount, 
				    MAX_TARGET_RANGE, 
				    &TargetList, 
				    pTarget, 
				    FLAG_INCLUDE_NON_AGGRESSORS);

            int iNextTarget = 0;
            for (i = 1; i < iShotCount; i++)
                {
                //  If we've exhausted the nearest target list, then add the
                //  selected target again and loop back to the beginning of
                //  the list.

                if (iNextTarget == iFound)
                    {
                    MIRVTarget[i] = pTarget;
                    iNextTarget = 0;
                    }
                else
                    {
			        CSpaceObject *pNewTarget = TargetList[iNextTarget++ % iFound];

                    //	Calculate direction to fire in

                    CVector vTarget = pNewTarget->GetPos() - ShotPos[i];
                    CVector vTargetVel = pNewTarget->GetVel() - pSource->GetVel();
                    Metric rTimeToIntercept = CalcInterceptTime(vTarget, vTargetVel, rSpeed);
                    CVector vInterceptPoint = (rTimeToIntercept > 0.0 ? vTarget + pNewTarget->GetVel() * rTimeToIntercept : vTarget);

                    ShotDir[i] = VectorToPolar(vInterceptPoint, NULL);
                    MIRVTarget[i] = pNewTarget;
                    }
                }
            }
		}

	//	If the shot requires ammo, check to see that the source has
	//	enough ammo for all shots

	if (iRepeatingCount == 0)
		{
		if (pShot->GetAmmoType())
			{
			//	Select the ammo

			CItemListManipulator ItemList(pSource->GetItemList());
			CItem Item(pShot->GetAmmoType(), iShotCount);
			bool bAmmoSelected = ItemList.SetCursorAtItem(Item);

			//	If we could not select ammo (because we don't have any)
			//	and this is the first shot, then we can't fire at all

			if (!bAmmoSelected)
				{
				return false;
				}
			}
		else if (m_bCharges)
			{
			if (pDevice->GetCharges(pSource) <= 0)
				{
				return false;
				}
			}
		}

	//	Create barrel flash effect

	if (!bFireSuppressed)
		{
		for (i = 0; i < iShotCount; i++)
			pShot->CreateFireEffect(pSource->GetSystem(), pSource, ShotPos[i], CVector(), ShotDir[i]);
		}

	//	Create all the shots

	CVector vRecoil;
	if (!bFireSuppressed)
		{
		bool bNoShotsFired = true;

		if (pShot->GetFireType() == ftContinuousBeam && !pDevice->HasLastShots())
			pDevice->SetLastShotCount(iShotCount);

		for (i = 0; i < iShotCount; i++)
			{
			//	Fire out to event, if the weapon has one.
			//	Otherwise, we create weapon fire

			EOnFireWeaponResults iResult;
			iResult = FireOnFireWeapon(ItemCtx, 
					pShot, 
					ShotPos[i], 
					(m_bMIRV ? MIRVTarget[i] : pTarget),
					ShotDir[i], 
					iRepeatingCount);

			//	Did we destroy the source?

			if (pSource->IsDestroyed())
				*retbSourceDestroyed = true;

			//	If we didn't fire a shot in the event handler, do it now

			if (iResult == resDefault)
				{
				CSpaceObject *pNewObj;
				CDamageSource Source(pSource, killedByDamage);

				//	Flags for the type of shot

				DWORD dwFlags = 0;
				if (i != 0)
					dwFlags = CSystem::CWF_FRAGMENT;
				else
					dwFlags = CSystem::CWF_WEAPON_FIRE;

				if (iRepeatingCount != 0)
					dwFlags |= CSystem::CWF_REPEAT;

				//	If this is a continuous beam, then we need special code.

				if (pShot->GetFireType() == ftContinuousBeam
						&& (pNewObj = pDevice->GetLastShot(pSource, i))
						&& (pShot->IsCurvedBeam() || pNewObj->GetRotation() == ShotDir[i]))
					{
					pNewObj->AddContinuousBeam(ShotPos[i],
							pSource->GetVel() + PolarToVector(ShotDir[i], rSpeed),
							ShotDir[i]);

					//	Fire system events, which are normally fired inside CreateWeaponFireDesc

					pSource->GetSystem()->FireSystemWeaponEvents(pNewObj, pShot, Source, iRepeatingCount, dwFlags);
					}

				//	Otherwise, create a shot

				else
					{
					pSource->GetSystem()->CreateWeaponFire(pShot,
							pDevice->GetEnhancements(),
							Source,
							ShotPos[i],
							pSource->GetVel() + PolarToVector(ShotDir[i], rSpeed),
							ShotDir[i],
							iRepeatingCount,
							(m_bMIRV ? MIRVTarget[i] : pTarget),
							dwFlags,
							&pNewObj);

					//	If this shot was created by automated weapon fire, then set flag

					if (pDevice->IsAutomatedWeapon())
						pNewObj->SetAutomatedWeapon();

					//	Remember the shot, if necessary

					if (pShot->GetFireType() == ftContinuousBeam)
						pDevice->SetLastShot(pNewObj, i);
					}

				bNoShotsFired = false;
				}

			//	This result means that OnFireWeapon fired a shot

			else if (iResult == resShotFired)
				bNoShotsFired = false;

			//	Add up all the shot directions to end up with a recoil dir

			if (m_iRecoil && iResult != resNoShot)
				vRecoil = vRecoil + PolarToVector(ShotDir[i], 1.0);
			}

		//	If no shots were fired, then we're done

		if (bNoShotsFired)
			return false;
		}

	//	Consume ammo

	bool bNextVariant = false;
	if (iRepeatingCount == pShot->GetContinuous())
		{
		if (pShot->GetAmmoType())
			{
			//	Select the ammo

			CItemListManipulator ItemList(pSource->GetItemList());
			CItem Item(pShot->GetAmmoType(), iShotCount);
			bool bAmmoSelected = ItemList.SetCursorAtItem(Item);

			//	If we selected some ammo and this is the last
			//	shot in the series, then consume ammo

			if (bAmmoSelected)
				{
				//	If we've exhausted our ammunition, remember to
				//	select the next variant

				if (ItemList.GetItemAtCursor().GetCount() == 1)
					bNextVariant = true;

				ItemList.DeleteAtCursor(1);

				//	Remember to tell the ship that we've consumed items

				if (retbConsumedItems)
					*retbConsumedItems = true;
				}
			}
		else if (m_bCharges)
			{
			if (pDevice->GetCharges(pSource) > 0)
				{
				pDevice->IncCharges(pSource, -1);

				if (retbConsumedItems)
					*retbConsumedItems = true;
				}
			}
		}

	//	Sound effect

	if (!bFireSuppressed)
		pShot->PlayFireSound(pSource);

	//	Recoil

	if (!bFireSuppressed && m_iRecoil)
		{
		CVector vAccel = vRecoil.Normal() * (Metric)(-10 * m_iRecoil * m_iRecoil);
		pSource->Accelerate(vAccel, g_MomentumConstant);
		pSource->ClipSpeed(pSource->GetMaxSpeed());
		}

	//	Switch to the next variant if necessary

	if (bNextVariant)
		pSource->OnDeviceStatus(pDevice, statusUsedLastAmmo);

	//	Create an explosion if weapon damage

	if (bExplosion && !pSource->IsDestroyed())
		{
		SDamageCtx Ctx;
		Ctx.pObj = pSource;
		Ctx.pDesc = pShot;
		Ctx.Damage = pShot->GetDamage();
		Ctx.Damage.SetCause(killedByWeaponMalfunction);
		Ctx.iDirection = (pDevice->GetPosAngle() + 360 + mathRandom(0, 30) - 15) % 360;
		Ctx.vHitPos = vSource;
		Ctx.pCause = pSource;
		Ctx.Attacker = CDamageSource(pSource, killedByWeaponMalfunction);

		EDamageResults iResult = pSource->Damage(Ctx);

		if (iResult == damageDestroyed 
				|| iResult == damagePassthroughDestroyed)
			*retbSourceDestroyed = true;
		}

	//	Done!

	return true;
	}

int CWeaponClass::GetActivateDelay (CInstalledDevice *pDevice, CSpaceObject *pSource)

//	GetActivateDelay
//
//	Returns the number of ticks between shots
//	NOTE: We do not adjust for enhancements.

	{
	return m_iFireRate;
	}

CItemType *CWeaponClass::GetAmmoItem (int iIndex) const

//  GetAmmoItem
//
//  Returns the nth ammo item compatible with this weapon.

    {
    ASSERT(iIndex >= 0 && iIndex < GetAmmoItemCount());
    return m_ShotData[iIndex].pDesc->GetAmmoType();
    }

int CWeaponClass::GetAmmoItemCount (void) const

//  GetAmmoItemCount
//
//  Returns the number of ammo items that we are compatible with.

    {
	if (!UsesAmmo())
		return 0;

    //  Launchers use the m_ShotData array to track each missile. We can assume
	//	that each entry is a missile.

    else if (IsLauncherWithAmmo())
        return m_ShotData.GetCount();

	//	NOTE: We can't assume that m_ShotData only has one entry because we 
	//	could be a scaling weapon.

    else
        return 1;
    }

ICCItem *CWeaponClass::FindAmmoItemProperty (CItemCtx &Ctx, const CItem &Ammo, const CString &sProperty)

//  FindAmmoItemProperty
//
//  Returns a property of the weapon when the given ammo item is selected.

    {
	int i;
	CCodeChain &CC = g_pUniverse->GetCC();

	//	Make sure we actually fire the given ammo. We need to check here because
	//	GetWeaponFireDesc doesn't check.

	ASSERT(Ammo.IsEmpty() || GetAmmoVariant(Ammo.GetType()) != -1);

	//	Get the shot

	CWeaponFireDesc *pShot = GetWeaponFireDesc(Ctx, Ammo);
	if (pShot == NULL)
		return CDeviceClass::FindAmmoItemProperty(Ctx, Ammo, sProperty);

	//	Enhancements

	const CItemEnhancementStack *pEnhancements = Ctx.GetEnhancementStack();

	//	Get the property

	ICCItem *pResult;
	if (strEquals(sProperty, PROPERTY_AMMO_TYPES))
		{
		if (UsesAmmo())
			{
			pResult = CC.CreateLinkedList();
			for (i = 0; i < m_ShotData.GetCount(); i++)
				if (m_ShotData[i].pDesc->GetAmmoType())
					pResult->AppendInteger(CC, m_ShotData[i].pDesc->GetAmmoType()->GetUNID());

			return pResult;
			}
		else
			return CC.CreateNil();
		}

	else if (strEquals(sProperty, PROPERTY_AVERAGE_DAMAGE))
		return CC.CreateDouble(CalcDamagePerShot(pShot, pEnhancements));

    else if (strEquals(sProperty, PROPERTY_BALANCE))
        {
        SBalance Balance;
        CalcBalance(Ctx, Balance);
        if (Balance.iLevel == 0)
            return CC.CreateNil();

        return CC.CreateInteger((int)Balance.rBalance);
        }
    else if (strEquals(sProperty, PROPERTY_BALANCE_DAMAGE))
        {
        SBalance Balance;
        CalcBalance(Ctx, Balance);
        if (Balance.iLevel == 0)
            return CC.CreateNil();

        const SStdStats &Stats = STD_WEAPON_STATS[Balance.iLevel - 1];

        //  Compute the balance assuming that damage is standard.

        Metric rDelta = Balance.rBalance - Balance.rDamage;

        //  Compute cost to compensate for this balance

        Metric rDamage = Balance.rStdDamage180 - (Balance.rStdDamage180 * rDelta / 100.0);

        return CC.CreateInteger((int)(rDamage + 0.5));
        }
    else if (strEquals(sProperty, PROPERTY_BALANCE_COST))
        {
        SBalance Balance;
        CalcBalance(Ctx, Balance);
        if (Balance.iLevel == 0)
            return CC.CreateNil();

        const SStdStats &Stats = STD_WEAPON_STATS[Balance.iLevel - 1];

        //  Compute the balance assuming that cost is standard.

        Metric rBalance = Balance.rBalance - Balance.rCost;

        //  Compute cost to compensate for this balance

        Metric rCostDelta = -rBalance / BALANCE_COST_RATIO;
        Metric rCostCredits = Stats.rCost + (Stats.rCost * rCostDelta / 100.0);

        return CC.CreateInteger((int)GetItemType()->GetCurrencyType()->Exchange(CEconomyType::Default(), (CurrencyValue)rCostCredits));
        }
    else if (strEquals(sProperty, PROPERTY_BALANCE_EXCLUDE_COST))
        {
        SBalance Balance;
        CalcBalance(Ctx, Balance);
        if (Balance.iLevel == 0)
            return CC.CreateNil();

        return CC.CreateInteger((int)(Balance.rBalance - Balance.rCost));
        }
	else if (strEquals(sProperty, PROPERTY_DAMAGE_180))
		{
		Metric rDamagePerShot = CalcDamagePerShot(pShot, pEnhancements);
		int iDelay = CalcActivateDelay(Ctx);
		return CC.CreateInteger(iDelay > 0 ? (int)((rDamagePerShot * 180.0 / iDelay) + 0.5) : (int)(rDamagePerShot + 0.5));
		}

	else if (strEquals(sProperty, PROPERTY_DAMAGE_PER_PROJECTILE))
		return CC.CreateDouble(CalcDamage(pShot, pEnhancements));

	else if (strEquals(sProperty, PROPERTY_DAMAGE_WMD_180))
		{
		Metric rDamagePerShot = CalcDamagePerShot(pShot, pEnhancements, DamageDesc::flagWMDAdj);
		int iDelay = CalcActivateDelay(Ctx);
		return CC.CreateInteger(iDelay > 0 ? (int)((rDamagePerShot * 180.0 / iDelay) + 0.5) : (int)(rDamagePerShot + 0.5));
		}

    else if (strEquals(sProperty, PROPERTY_EFFECTIVE_RANGE))
        return CC.CreateInteger((int)(pShot->GetEffectiveRange() / LIGHT_SECOND));

	else if (strEquals(sProperty, PROPERTY_FIRE_ARC))
		{
		CInstalledDevice *pDevice = Ctx.GetDevice();	//	May be NULL
		int iMinFireArc;
		int iMaxFireArc;

		//	Omnidirectional

		if (IsOmniDirectional(pDevice))
			return CC.CreateString(PROPERTY_OMNIDIRECTIONAL);

		//	Fire arc

		else if (IsDirectional(pDevice, &iMinFireArc, &iMaxFireArc))
			{
			//	Create a list

			ICCItem *pResult = CC.CreateLinkedList();
			if (pResult->IsError())
				return pResult;

			CCLinkedList *pList = (CCLinkedList *)pResult;

			pList->AppendInteger(CC, iMinFireArc);
			pList->AppendInteger(CC, iMaxFireArc);

			return pResult;
			}

		//	Otherwise, see if we are pointing in a particular direction

		else
			{
			int iFacingAngle = AngleMod((pDevice ? pDevice->GetRotation() : 0) + AngleMiddle(m_iMinFireArc, m_iMaxFireArc));
			if (iFacingAngle == 0)
				return CC.CreateNil();
			else
				return CC.CreateInteger(iFacingAngle);
			}
		}

	else if (strEquals(sProperty, PROPERTY_FIRE_DELAY))
		return CC.CreateInteger(CalcActivateDelay(Ctx));

	else if (strEquals(sProperty, PROPERTY_FIRE_RATE))
		{
		Metric rDelay = CalcActivateDelay(Ctx);
		if (rDelay <= 0.0)
			return CC.CreateNil();

		return CC.CreateInteger((int)(1000.0 / rDelay));
		}

	else if (strEquals(sProperty, PROPERTY_LINKED_FIRE_OPTIONS))
		{
		//	Get the options from the device

		DWORD dwOptions = GetLinkedFireOptions(Ctx);
		if (dwOptions == 0)
			return CC.CreateNil();

		//	Create a list

		ICCItem *pResult = CC.CreateLinkedList();
		if (pResult->IsError())
			return pResult;

		CCLinkedList *pList = (CCLinkedList *)pResult;

		//	Add options

		if (dwOptions & CDeviceClass::lkfAlways)
			pList->AppendString(CC, CDeviceClass::GetLinkedFireOptionString(CDeviceClass::lkfAlways));
		else if (dwOptions & CDeviceClass::lkfTargetInRange)
			pList->AppendString(CC, CDeviceClass::GetLinkedFireOptionString(CDeviceClass::lkfTargetInRange));
		else if (dwOptions & CDeviceClass::lkfEnemyInRange)
			pList->AppendString(CC, CDeviceClass::GetLinkedFireOptionString(CDeviceClass::lkfEnemyInRange));

		//	Done

		return pResult;
		}

	else if (strEquals(sProperty, PROPERTY_MAX_DAMAGE))
		return CC.CreateDouble(CalcDamagePerShot(pShot, pEnhancements, DamageDesc::flagMaxDamage));

	else if (strEquals(sProperty, PROPERTY_MIN_DAMAGE))
		return CC.CreateDouble(CalcDamagePerShot(pShot, pEnhancements, DamageDesc::flagMinDamage));

	else if (strEquals(sProperty, PROPERTY_OMNIDIRECTIONAL))
		{
		CInstalledDevice *pDevice = Ctx.GetDevice();	//	May be NULL
		return CC.CreateBool(IsOmniDirectional(pDevice));
		}

    else if (strEquals(sProperty, PROPERTY_STD_COST))
        {
        const SStdStats &Stats = STD_WEAPON_STATS[CalcLevel(pShot) - 1];
        return CC.CreateDouble(Stats.rCost);
        }

	//	See if the shot has the property

	else if (pResult = pShot->FindProperty(sProperty))
		return pResult;

	//	If the item is a missile, then we default to items properties

	else if (Ctx.GetItem().GetType() && Ctx.GetItem().GetType()->IsMissile())
		{
		CString sValue;
        if (FindAmmoDataField(Ctx.GetItem(), sProperty, &sValue))
            return CreateResultFromDataField(CC, sValue);

        else if (Ctx.GetItem().GetType()->FindDataField(sProperty, &sValue))
            return CreateResultFromDataField(CC, sValue);

        else
            return NULL;
		}

	//	Otherwise, just get the property from the base class

	else
		return CDeviceClass::FindAmmoItemProperty(Ctx, Ammo, sProperty);
    }

int CWeaponClass::GetAmmoVariant (const CItemType *pItem) const

//	GetAmmoVariant
//
//	Returns the variant that fires the given ammo (or -1 if the weapons
//	does not fire the ammo)

	{
	int i;

	DWORD dwItemUNID = (pItem ? pItem->GetUNID() : 0);
	if (dwItemUNID == 0)
		return -1;

	//	Find it

	for (i = 0; i < m_ShotData.GetCount(); i++)
		{
		if (m_ShotData[i].pDesc->GetAmmoType()
				&& m_ShotData[i].pDesc->GetAmmoType()->GetUNID() == dwItemUNID)

			//	Launchers take multiple ammo; others do not.

			return (IsLauncher() ? i : 0);
		}

	return -1;
	}

ItemCategories CWeaponClass::GetImplCategory (void) const

//	GetImplCategory
//
//	Returns the weapon class category

	{
	if (IsLauncher())
		return itemcatLauncher;
	else
		return itemcatWeapon;
	}

int CWeaponClass::GetCounter (CInstalledDevice *pDevice, CSpaceObject *pSource, CounterTypes *retiType, int *retiLevel)

//	GetCounter
//
//	Return counter

	{
	//	Return the type

	if (retiType)
		*retiType = m_Counter;

	if (m_Counter == cntNone || pDevice == NULL || pSource == NULL)
        {
        if (retiLevel)
            *retiLevel = 0;

		return 0;
        }

	//	If we're a capacitor, then don't show the counter if we are full

	if (m_Counter == cntCapacitor && pDevice->GetTemperature() >= MAX_COUNTER)
        {
        if (retiLevel)
            *retiLevel = MAX_COUNTER;

		return 0;
        }

	//	Otherwise, return the current value

    if (retiLevel)
        *retiLevel = pDevice->GetTemperature();

	return pDevice->GetTemperature();
	}

int CWeaponClass::GetAlternatingPos (CInstalledDevice *pDevice) const
	{
	return (int)(DWORD)HIBYTE(LOWORD(pDevice->GetData()));
	}

DWORD CWeaponClass::GetContinuousFire (CInstalledDevice *pDevice) const
	{
	return (int)(DWORD)LOBYTE(LOWORD(pDevice->GetData()));
	}

int CWeaponClass::GetCurrentVariant (CInstalledDevice *pDevice) const
	{
	return (int)(short)HIWORD(pDevice->GetData()); 
	}

const DamageDesc *CWeaponClass::GetDamageDesc (CItemCtx &Ctx)

//	GetDamageDesc
//
//	Returns the kind of damage caused by this weapon

	{
	CWeaponFireDesc *pShot = GetWeaponFireDesc(Ctx);
	if (pShot == NULL)
		return NULL;

	return &pShot->GetDamage();
	}

DamageTypes CWeaponClass::GetDamageType (CItemCtx &Ctx, const CItem &Ammo) const

//	GetDamageType
//
//	Returns the kind of damage caused by this weapon

	{
	CWeaponFireDesc *pShot = GetWeaponFireDesc(Ctx, Ammo);

	//	OK if we don't find shot--could be a launcher with no ammo

	if (pShot == NULL)
		return damageGeneric;

	//	Get the damage type

	return pShot->GetDamageType();
	}

int CWeaponClass::GetDefaultFireAngle (CInstalledDevice *pDevice, CSpaceObject *pSource) const

//	GetDefaultFireAngle
//
//	Gets the natural fire direction (not counting omni or swivel mounts)

	{
	if (pDevice && pSource)
		return (pSource->GetRotation() + pDevice->GetRotation() + AngleMiddle(m_iMinFireArc, m_iMaxFireArc)) % 360;
	else
		return AngleMiddle(m_iMinFireArc, m_iMaxFireArc);
	}

ICCItem *CWeaponClass::FindItemProperty (CItemCtx &Ctx, const CString &sName)

//	FindItemProperty
//
//	Returns the item property. Subclasses should call this if they do not
//	understand the property.

	{
    //  See if we specified a specific variant. If so, get the ammo for it.

	CString sProperty = sName;
    CItem Ammo;
    int iAmmoIndex = ParseVariantFromPropertyName(sName, &sProperty);
    if (iAmmoIndex != -1 && iAmmoIndex < GetAmmoItemCount())
        Ammo = CItem(GetAmmoItem(iAmmoIndex), 1);

	//	Get the property

    return FindAmmoItemProperty(Ctx, Ammo, sProperty);
	}

Metric CWeaponClass::GetMaxEffectiveRange (CSpaceObject *pSource, CInstalledDevice *pDevice, CSpaceObject *pTarget)

//	GetMaxEffectiveRange
//
//	Returns the greatest range at which the weapon is still
//	effective.

	{
	CItemCtx Ctx(pSource, pDevice);
	CWeaponFireDesc *pShot = GetWeaponFireDesc(Ctx);
	if (pShot == NULL)
		return 0.0;

	if (pTarget && !pTarget->CanThrust())
		{
        Metric rSpeed = pShot->GetAveSpeed();
		return (rSpeed * (pShot->GetMaxLifetime() * 90 / 100)) + (128.0 * g_KlicksPerPixel);
		}
	else
		return pShot->GetEffectiveRange();
	}

Metric CWeaponClass::GetMaxRange (CItemCtx &ItemCtx)

//	GetMaxRange
//
//	Returns the greatest range for the weapon.

	{
	CWeaponFireDesc *pShot = GetWeaponFireDesc(ItemCtx);
	if (pShot == NULL)
		return 0.0;

	return pShot->GetMaxRange();
	}

DWORD CWeaponClass::GetLinkedFireOptions (CItemCtx &Ctx)

//	GetLinkedFireOptions
//
//	Returns linked fire options for the weapon
	
	{
	//	If the device has linked fire, then honor that.

	DWORD dwOptions;
	if (Ctx.GetDevice() && (dwOptions = Ctx.GetDevice()->GetLinkedFireOptions()))
		return dwOptions;

	//	Otherwise, take native linked fire setting from weapon.

	return m_dwLinkedFireOptions; 
	}

int CWeaponClass::GetPowerRating (CItemCtx &Ctx) const

//	GetPowerRating
//
//	Returns the rated power

	{
	int iPower = m_iPowerUse;

	const CItemEnhancementStack *pEnhancements = Ctx.GetEnhancementStack();
	if (pEnhancements)
		iPower = iPower * pEnhancements->GetPowerAdj() / 100;

	return iPower;
	}

CString GetReferenceFireRate (int iFireRate)
	{
	if (iFireRate <= 0)
		return NULL_STR;

	int iRate = (int)((10.0 * g_TicksPerSecond / iFireRate) + 0.5);
	if (iRate == 0)
		return CONSTLIT(" @ <0.1 shots/sec");
	else if ((iRate % 10) == 0)
		{
		if ((iRate / 10) == 1)
			return strPatternSubst(CONSTLIT(" @ %d shot/sec"), iRate / 10);
		else
			return strPatternSubst(CONSTLIT(" @ %d shots/sec"), iRate / 10);
		}
	else
		return strPatternSubst(CONSTLIT(" @ %d.%d shots/sec"), iRate / 10, iRate % 10);
	}

bool CWeaponClass::GetReferenceDamageType (CItemCtx &Ctx, const CItem &Ammo, DamageTypes *retiDamage, CString *retsReference) const

//	GetReferenceDamageType
//
//	Returns the damage type done by the weapon

	{
	const CItem &Item = Ctx.GetItem();
	CSpaceObject *pSource = Ctx.GetSource();
	CInstalledDevice *pDevice = Ctx.GetDevice();

	DamageTypes iDamageType;
	CString sReference;
	CItemEnhancement Mods = Item.GetMods();

	//	Fire rate

	CString sFireRate = GetReferenceFireRate(CalcActivateDelay(Ctx));

	//	Compute the damage string and special string

	if (IsLauncherWithAmmo() && Ammo.IsEmpty())
		{
		sReference = CONSTLIT("missile launcher");

		sReference.Append(sFireRate);
		iDamageType = damageGeneric;
		}
	else
		{
		//	Get the damage

		int iFragments;
        CWeaponFireDesc *pRootShot = GetWeaponFireDesc(Ctx, Ammo);
		CWeaponFireDesc *pShot = GetReferenceShotData(pRootShot, &iFragments);
		DamageDesc Damage = pShot->GetDamage();
		iDamageType = Damage.GetDamageType();

		//	Modify the damage based on any enhancements that the ship may have

		const CItemEnhancementStack *pEnhancements = Ctx.GetEnhancementStack();
		if (pEnhancements)
			Damage.AddEnhancements(pEnhancements);

		//	For shockwaves...

		if (pShot->GetType() == ftArea)
			{
			//	Compute total damage. NOTE: For particle weapons the damage 
			//	specified is the total damage if ALL particle were to hit.

			Metric rDamage = SHOCKWAVE_DAMAGE_FACTOR * Damage.GetDamageValue(DamageDesc::flagIncludeBonus);

			//	Calculate the radius of the shockwave

			int iRadius = (int)(((pShot->GetAveExpansionSpeed() * pShot->GetAveLifetime() * g_SecondsPerUpdate) / LIGHT_SECOND) + 0.5);

			//	Compute result

			int iDamage10 = (int)((rDamage * 10.0) + 0.5);
			int iDamage = iDamage10 / 10;
			int iDamageTenth = iDamage10 % 10;

			if (iDamageTenth == 0)
				sReference = strPatternSubst(CONSTLIT("%s shockwave %d hp in %d ls radius%s"), GetDamageShortName(Damage.GetDamageType()), iDamage, iRadius, sFireRate);
			else
				sReference = strPatternSubst(CONSTLIT("%s shockwave %d.%d hp in %d ls radius%s"), GetDamageShortName(Damage.GetDamageType()), iDamage, iDamageTenth, iRadius, sFireRate);
			}

		//	For area weapons...

		else if (pShot->GetType() == ftRadius)
			{
			CString sDamage = Damage.GetDesc(DamageDesc::flagAverageDamage);

			//	Calculate the radius

			int iRadius = (int)((pShot->GetMaxRadius() / LIGHT_SECOND) + 0.5);

			//	Compute result

			sReference = strPatternSubst(CONSTLIT("%s in %d ls radius%s"), sDamage, iRadius, sFireRate);
			}

		//	For particles...

		else if (pShot->GetType() == ftParticles)
			{
			//	Some weapons fire multiple shots (e.g., Avalanche cannon)

			CString sMult;
			int iMult = (int)CalcConfigurationMultiplier(pRootShot, false);
			if (iMult != 1)
				sMult = strPatternSubst(CONSTLIT(" (x%d)"), iMult);

			//	Compute total damage. NOTE: For particle weapons the damage 
			//	specified is the total damage if ALL particle were to hit.

			Metric rDamage = PARTICLE_CLOUD_DAMAGE_FACTOR * Damage.GetDamageValue(DamageDesc::flagIncludeBonus);

			//	Compute result

			int iDamage10 = (int)((rDamage * 10.0) + 0.5);
			int iDamage = iDamage10 / 10;
			int iDamageTenth = iDamage10 % 10;

			if (iDamageTenth == 0)
				sReference = strPatternSubst(CONSTLIT("%s cloud %d hp%s%s"), GetDamageShortName(Damage.GetDamageType()), iDamage, sMult, sFireRate);
			else
				sReference = strPatternSubst(CONSTLIT("%s cloud %d.%d hp%s%s"), GetDamageShortName(Damage.GetDamageType()), iDamage, iDamageTenth, sMult, sFireRate);
			}

		//	For large number of fragments, we have a special description

		else if (iFragments >= 8 && !pShot->IsTracking())
			{
			//	Compute total damage

			Metric rDamage = EXPECTED_FRAGMENT_HITS * iFragments * Damage.GetDamageValue(DamageDesc::flagIncludeBonus);

			//	Compute radius

			int iRadius = (int)((pShot->GetRatedSpeed() * pShot->GetAveLifetime() * g_SecondsPerUpdate / LIGHT_SECOND) + 0.5);

			//	Compute result

			int iDamage10 = (int)((rDamage * 10.0) + 0.5);
			int iDamage = iDamage10 / 10;
			int iDamageTenth = iDamage10 % 10;

			if (iDamageTenth == 0)
				sReference = strPatternSubst(CONSTLIT("%s fragmentation %d hp in %d ls radius%s"), GetDamageShortName(Damage.GetDamageType()), iDamage, iRadius, sFireRate);
			else
				sReference = strPatternSubst(CONSTLIT("%s fragmentation %d.%d hp in %d ls radius%s"), GetDamageShortName(Damage.GetDamageType()), iDamage, iDamageTenth, iRadius, sFireRate);
			}

		//	Otherwise, a normal description

		else
			{
			CString sDamage = Damage.GetDesc(DamageDesc::flagAverageDamage);

			//	Add the multiplier

			int iMult = (int)CalcConfigurationMultiplier(pRootShot);
			if (iMult > 1)
				sDamage.Append(strPatternSubst(CONSTLIT(" (x%d)"), iMult));

			sReference.Append(sDamage);

			//	Compute fire rate

			sReference.Append(sFireRate);
			}
		}

	//	Done

	if (retiDamage)
		*retiDamage = iDamageType;

	if (retsReference)
		*retsReference = sReference;

	return true;
	}

CWeaponFireDesc *CWeaponClass::GetReferenceShotData (CWeaponFireDesc *pShot, int *retiFragments) const

//	GetReferenceShotData
//
//	If a fragment of pShot does more damage than the shot itself, then
//	return the fragment (since it is more representative)

	{
	CWeaponFireDesc *pBestShot = pShot;
	Metric rBestDamage = 0.0;	//	Fragments always take precedence
	int iBestFragments = 1;
	DamageTypes iBestDamageType = damageLaser;

	CWeaponFireDesc::SFragmentDesc *pFragDesc = pShot->GetFirstFragment();
	while (pFragDesc)
		{
		int iOriginalFragments = pFragDesc->Count.GetAveValue();
		int iFragments = iOriginalFragments;
		switch (pFragDesc->pDesc->GetType())
			{
			//	Area damage counts as multiple hits

			case ftArea:
				iFragments = (int)(iFragments * SHOCKWAVE_DAMAGE_FACTOR);
				break;

			//	Radius damage always hits (if in range)

			case ftRadius:
				break;

			//	For others, only some of the fragments will hit

			default:
				if (!pFragDesc->pDesc->IsTracking())
					iFragments = (int)(iFragments * SHOCKWAVE_DAMAGE_FACTOR);
				break;
			}

		Metric rDamage = pFragDesc->pDesc->GetDamage().GetDamageValue() * iFragments;
		DamageTypes iDamageType = pFragDesc->pDesc->GetDamage().GetDamageType();
		if (iDamageType > iBestDamageType 
				|| (iDamageType == iBestDamageType && rDamage >= rBestDamage))
			{
			pBestShot = pFragDesc->pDesc;
			rBestDamage = rDamage;
			iBestFragments = iOriginalFragments;
			iBestDamageType = iDamageType;
			}

		pFragDesc = pFragDesc->pNext;
		}

	if (retiFragments)
		*retiFragments = iBestFragments;

	return pBestShot;
	}

int CWeaponClass::GetSelectVariantCount (void) const

//	GetSelectVariantCount
//
//	Returns the number of selectable variants for this weapon. For example, for
//	missile launchers, this is the number of compatible missiles. This isn't
//	always the same as m_ShotData.GetCount().

	{
	switch (m_iVariantType)
		{
		case varLauncher:
			return m_ShotData.GetCount();

		default:
			return (m_ShotData.GetCount() > 0 ? 1 : 0);
		}
	}

void CWeaponClass::GetSelectedVariantInfo (CSpaceObject *pSource, 
										   CInstalledDevice *pDevice,
										   CString *retsLabel,
										   int *retiAmmoLeft,
										   CItemType **retpType)

//	GetSelectedVariantInfo
//
//	Returns information about the selected variant

	{
	CItemCtx Ctx(pSource, pDevice);
	CWeaponFireDesc *pShot = GetWeaponFireDesc(Ctx);

    //  Short-circuit if we don't have enough data

	if (pShot == NULL || pSource == NULL || pDevice == NULL)
		{
		if (retsLabel)
			*retsLabel = CString();
		if (retiAmmoLeft)
			*retiAmmoLeft = 0;
		if (retpType)
			*retpType = NULL;
		}

    //  If we don't have ammo, then we return info based on charges

	else if (pShot->GetAmmoType() == NULL)
		{
		if (retsLabel)
			*retsLabel = CString();
		if (retiAmmoLeft)
			{
			if (m_bCharges)
				*retiAmmoLeft = pDevice->GetCharges(pSource);
			else
				*retiAmmoLeft = -1;
			}
		if (retpType)
			*retpType = GetItemType();
		}

    //  Otherwise, we return ammo counts

	else
		{
    	CItemListManipulator ItemList(pSource->GetItemList());
		CItem Item(pShot->GetAmmoType(), 1);

		if (retiAmmoLeft)
			{
			if (ItemList.SetCursorAtItem(Item))
				*retiAmmoLeft = ItemList.GetItemAtCursor().GetCount();
			else
				*retiAmmoLeft = 0;
			}

		if (retsLabel)
			{
			if (GetCategory() == itemcatLauncher)
				*retsLabel = Item.GetNounPhrase(Ctx, nounCapitalize);
			else
				*retsLabel = CString();
			}

		if (retpType)
			*retpType = pShot->GetAmmoType();
		}
	}

int CWeaponClass::GetStdDamage (int iLevel)

//	GetStdDamage
//
//	Returns standard damage at this level.

	{
    return GetStdStats(iLevel).iDamage;
	}

const CWeaponClass::SStdStats &CWeaponClass::GetStdStats (int iLevel)

//  GetStdStats
//
//  Returns standard stats for level.

    {
    if (iLevel >= 1 && iLevel <= MAX_ITEM_LEVEL)
	    return STD_WEAPON_STATS[iLevel - 1];
    else
        {
        ASSERT(false);
	    return STD_WEAPON_STATS[0];
        }
    }

int CWeaponClass::GetValidVariantCount (CSpaceObject *pSource, CInstalledDevice *pDevice)

//	GetValidVariantCount
//
//	Returns the number of valid variants for this weapons

	{
	//	Special handling for scalable weapons

	if (m_iVariantType == varLevelScaling || m_iVariantType == varCharges)
		{
		CWeaponFireDesc *pShot = GetWeaponFireDesc(CItemCtx(pSource, pDevice));
		if (pShot && VariantIsValid(pSource, pDevice, *pShot))
			return 1;
		else
			return 0;
		}

	//	Otherwise, count them

	else
		{
		int iCount = 0;

		for (int i = 0; i < m_ShotData.GetCount(); i++)
			{
			if (VariantIsValid(pSource, pDevice, *m_ShotData[i].pDesc))
				iCount++;
			}

		return iCount;
		}
	}

int CWeaponClass::GetWeaponEffectiveness (CSpaceObject *pSource, CInstalledDevice *pDevice, CSpaceObject *pTarget)

//	GetWeaponEffectiveness
//
//	Returns:
//
//	< 0		If the weapon is ineffective against the target
//	0		If the weapon has normal effect on target
//	1-100	If the weapon is particularly effective against the target
//
//	This call is used to figure out whether we should use an EMP or blinder
//	cannon against the target.

	{
	int iScore = 0;

	CItemCtx Ctx(pSource, pDevice);
	CWeaponFireDesc *pShot = GetWeaponFireDesc(Ctx);
	if (pShot == NULL)
		return -100;

	//	If we don't enough ammo, clearly we will not be effective

	if (pShot->GetAmmoType())
		{
		CItemListManipulator ItemList(pSource->GetItemList());
		CItem Item(pShot->GetAmmoType(), 1);
		if (!ItemList.SetCursorAtItem(Item))
			return -100;
		}

	//	Check some targeting options

	if (m_bTargetStationsOnly 
			&& (pTarget == NULL || pTarget->GetCategory() != CSpaceObject::catStation))
		return -100;

	//	Check our state

	switch (m_Counter)
		{
		//	If we're overheating, we will not be effective

		case cntTemperature:
			if (pDevice->IsWaiting() && pDevice->GetTemperature() > 0)
				return -100;

			if (pDevice->GetTemperature() + m_iCounterActivate >= MAX_COUNTER)
				{
				pDevice->SetWaiting(true);
				return -100;
				}

			pDevice->SetWaiting(false);
			break;

		//	If our capacitor is discharged, we will not be effective

		case cntCapacitor:
			if (pDevice->IsWaiting() && pDevice->GetTemperature() < MAX_COUNTER)
				return -100;

			if (pDevice->GetTemperature() < m_iCounterActivate)
				{
				pDevice->SetWaiting(true);
				return -100;
				}

			pDevice->SetWaiting(false);
			break;
		}

	//	If the weapon has EMP damage and the target is not paralysed then
	//	this is very effective.

	if (pTarget && pShot->GetDamage().GetEMPDamage() > 0)
		{
		//	If the target is already ionized, or if the target is a station
		//	(which cannot be paralyzed) then don't use this weapon.

		if (pTarget->IsParalyzed() 
				|| pTarget->GetCategory() != CSpaceObject::catShip)
			return -100;

		iScore += 100;
		}

	//	If the weapon has blinding damage and the target is not blind then
	//	this is very effective

	if (pTarget && pShot->GetDamage().GetBlindingDamage() > 0)
		{
		//	If the target is already blind, or if the target is a station, then
		//	don't bother with this weapon.

		if (pTarget->IsBlind()
				|| pTarget->GetCategory() != CSpaceObject::catShip)
			return -100;

		iScore += 100;
		}

	//	Score

	return iScore;
	}

CWeaponFireDesc *CWeaponClass::GetWeaponFireDesc (CItemCtx &ItemCtx, const CItem &Ammo) const

//  GetWeaponFireDesc
//
//  Get weapon fire descriptor for the weapon when shooting the given ammo.
//  ItemCtx may optionally refer to the installed weapon (it must not contain
//  a different item, though it may be empty).
//
//  Ammo may be specified either through the ammo item or through ItemCtx
//  (as an installed device selection).
//
//	NOTE: If Ammo is passed in, we expect it to be fired by this weapon. Check
//	before calling if not sure.

    {
    if (m_ShotData.GetCount() == 0)
        return NULL;

	//	Handle scalable levels

	if (m_iVariantType == varLevelScaling)
		{
		//	We assume that all levels are represented in m_ShotData.

        int iIndex = (ItemCtx.GetItem().IsEmpty() ? 0 : Min(Max(0, ItemCtx.GetItem().GetLevel() - m_ShotData[0].pDesc->GetLevel()), m_ShotData.GetCount() - 1));
		return m_ShotData[iIndex].pDesc;
		}

	//	Handle charge variants

	else if (m_iVariantType == varCharges)
		{
		//	We assume that all charge values are represented in m_ShotData.

        int iIndex = Min(Max(0, ItemCtx.GetDeviceCharges()), m_ShotData.GetCount() - 1);
		return m_ShotData[iIndex].pDesc;
		}

    //  If we need ammo, then we have extra work to do.
    //  NOTE: Currently, if one variant uses ammo, all need to use ammo.
	//	NOTE 2: This only applies to launchers. By definition, non-launchers
	//	never have more than one type of ammo. [But some launchers do not
	//	have ammo, so we need to check that they use ammo.]

    else if (IsLauncherWithAmmo())
        {
		CWeaponFireDesc *pRoot = NULL;
		int iLevel = -1;

        //  If we have ammo, use it (this overrides whatever item is selected 
        //  in ItemCtx).

        if (!Ammo.IsEmpty())
            {
            int iSelection = GetAmmoVariant(Ammo.GetType());
            if (iSelection != -1)
                pRoot = m_ShotData[iSelection].pDesc;
            else
                return NULL;
            }

        //  If we have a device, we ask it for the variant
        //
        //  NOTE: We need to make sure we check that the source exists because
        //  some code (like CStationType::OnBindDesign) will call this with a
        //  valid CInstalledDevice but a NULL source.

        else if (ItemCtx.GetDevice() && ItemCtx.GetSource())
            {
            int iSelection = GetCurrentVariant(ItemCtx.GetDevice());
            if (iSelection != -1 && iSelection < m_ShotData.GetCount())
                pRoot = m_ShotData[iSelection].pDesc;
            else
                return NULL;
            }

        else if (!ItemCtx.GetVariantItem().IsEmpty())
            {
            int iSelection = GetAmmoVariant(ItemCtx.GetVariantItem().GetType());
            if (iSelection != -1)
                pRoot = m_ShotData[iSelection].pDesc;
            else
                return NULL;
            }

        else if (ItemCtx.GetVariant() != -1 && ItemCtx.GetVariant() < m_ShotData.GetCount())
            {
            pRoot = m_ShotData[ItemCtx.GetVariant()].pDesc;
            }

        //  Otherwise, just return the first entry

        else
            pRoot = m_ShotData[0].pDesc;

        //  For now, the scaling for ammo weapons always comes from the weapon
        //  (we can't have scalable ammo).

        iLevel = (ItemCtx.GetItem().IsEmpty() ? -1 : ItemCtx.GetItem().GetLevel());
		return (iLevel == -1 ? pRoot : pRoot->GetScaledDesc(iLevel));
        }
    else
        {
        CWeaponFireDesc *pRoot = m_ShotData[0].pDesc;
        int iLevel = (ItemCtx.GetItem().IsEmpty() ? -1 : ItemCtx.GetItem().GetLevel());
	    return (iLevel == -1 ? pRoot : pRoot->GetScaledDesc(iLevel));
        }
    }

ALERROR CWeaponClass::InitVariantsFromXML (SDesignLoadCtx &Ctx, CXMLElement *pDesc, CItemType *pType)

//	InitVariantsFromXML
//
//	Loads the variant descriptions, whether from missiles, scaling, or charges.

	{
	int i;
	ALERROR error;

	//	If we have a Missiles tag then this weapon has ammunition; otherwise,
	//	if we have a Variants tag, then we have multiple descriptors.

	CXMLElement *pVariants = pDesc->GetContentElementByTag(MISSILES_TAG);
	if (pVariants == NULL)
		pVariants = pDesc->GetContentElementByTag(VARIANTS_TAG);

	if (pVariants)
		{
		//	Get the type of variants

		CString sType = pVariants->GetAttribute(TYPE_ATTRIB);
		if (sType.IsBlank())
			{
			if (pVariants->GetContentElementCount() > 1
					|| GetDefinedSlotCategory() == itemcatLauncher
					|| pDesc->GetAttributeBool(LAUNCHER_ATTRIB))
				m_iVariantType = varLauncher;
			else
				m_iVariantType = varSingle;
			}
		else if (strEquals(sType, VARIANT_TYPE_CHARGES))
			m_iVariantType = varCharges;
		else if (strEquals(sType, VARIANT_TYPE_MISSILES))
			m_iVariantType = varLauncher;
		else if (strEquals(sType, VARIANT_TYPE_LEVELS))
			m_iVariantType = varLevelScaling;
		else
			{
			Ctx.sError = strPatternSubst(CONSTLIT("Unknown variant type: %s"), sType);
			return ERR_FAIL;
			}

		//	Load each variant

		m_ShotData.InsertEmpty(pVariants->GetContentElementCount());
		for (i = 0; i < m_ShotData.GetCount(); i++)
			{
			CXMLElement *pItem = pVariants->GetContentElement(i);

			//	If this entry defines the missile, then we own it.

			if (pItem->FindAttribute(TYPE_ATTRIB))
				{
				m_ShotData[i].bOwned = true;
				m_ShotData[i].pDesc = new CWeaponFireDesc;

				//	For scalable levels, we assume each entry is a level

				int iLevel = GetLevel();
				if (m_iVariantType == varLevelScaling)
					iLevel += i;

				//	Initialize

				CString sUNID = strPatternSubst(CONSTLIT("%d/%d"), GetUNID(), i);
				if (error = m_ShotData[i].pDesc->InitFromXML(Ctx, pItem, sUNID, iLevel))
					return error;
				}

			//	Otherwise we expect the definition to be in the ItemType.
			//	This is best for missiles.

			else
				{
			    //	In this case we need to load the ammo type because it we 
                //  will later load the missile (at bind time).

			    if (error = m_ShotData[i].pAmmoType.LoadUNID(Ctx, pItem->GetAttribute(AMMO_ID_ATTRIB)))
				    return error;

				//	AmmoID is required in this case.

				if (m_ShotData[i].pAmmoType.GetUNID() == 0)
					{
					Ctx.sError = CONSTLIT("Expected either type= or ammoID= definition.");
					return ERR_FAIL;
					}

				m_ShotData[i].bOwned = false;
				m_ShotData[i].pDesc = NULL;
				}
			}
		}
	else
		{
		//	Figure out the type of variants

		if (pType->IsScalable())
			m_iVariantType = varSingleLevelScaling;

		else if (GetDefinedSlotCategory() == itemcatLauncher
				|| pDesc->GetAttributeBool(LAUNCHER_ATTRIB))
			m_iVariantType = varLauncher;

		else
			m_iVariantType = varSingle;

		//	Add a single entry

		m_ShotData.InsertEmpty(1);
		m_ShotData[0].bOwned = true;
		m_ShotData[0].pDesc = new CWeaponFireDesc;

		//	Load the definition from the root element

		CString sUNID = strPatternSubst(CONSTLIT("%d/0"), GetUNID());
		if (error = m_ShotData[0].pDesc->InitFromXML(Ctx, pDesc, sUNID, GetLevel()))
			return error;

        //  Initialize scaled stats, if necessary

        if (pType->IsScalable())
            {
            if (error = m_ShotData[0].pDesc->InitScaledStats(Ctx, pDesc, pType))
                return error;
            }
		}

	//	Done

	return NOERROR;
	}

bool CWeaponClass::IsAmmoWeapon (void)

//	IsAmmoWeapon
//
//	Returns TRUE if weapon uses ammo or other expendables

	{
	return (UsesAmmo() || m_bCharges);
	}

bool CWeaponClass::IsAreaWeapon (CSpaceObject *pSource, CInstalledDevice *pDevice)

//	IsAreaWeapon
//
//	Is this a weapon with an area of effect

	{
	CItemCtx Ctx(pSource, pDevice);
	CWeaponFireDesc *pShot = GetWeaponFireDesc(Ctx);
	if (pShot == NULL)
		return false;

	if (pShot->GetType() == ftArea)
		return true;

	if (pShot->HasFragments() && pShot->GetFirstFragment()->pDesc->GetType() == ftArea)
		return true;

	return false;
	}

bool CWeaponClass::IsDirectional (CInstalledDevice *pDevice, int *retiMinFireArc, int *retiMaxFireArc)

//	IsDirectional
//
//	Returns TRUE if the weapon can turn but is not omni

	{
	//	If the weapon is omnidirectional then we don't need directional 
	//	calculations.

	if (m_bOmnidirectional || (pDevice && pDevice->IsOmniDirectional()))
		return false;

	//	If we have a device, combine the fire arcs of device slot and weapon

	if (pDevice)
		{
		//	If the device is directional then we always take the fire arc from
		//	the device slot.

		if (pDevice->IsDirectional())
			{
			if (retiMinFireArc)
				*retiMinFireArc = pDevice->GetMinFireArc();
			if (retiMaxFireArc)
				*retiMaxFireArc = pDevice->GetMaxFireArc();

			return true;
			}

		//	Otherwise, see if the weapon is directional.

		else if (m_iMinFireArc != m_iMaxFireArc)
			{
			//	If the device points in a specific direction then we offset the
			//	weapon's fire arc.

			int iDeviceSlotOffset = pDevice->GetMinFireArc();

			if (retiMinFireArc)
				*retiMinFireArc = (m_iMinFireArc + iDeviceSlotOffset) % 360;
			if (retiMaxFireArc)
				*retiMaxFireArc = (m_iMaxFireArc + iDeviceSlotOffset) % 360;

			return true;
			}

		//	Otherwise, we are not directional

		else
			return false;
		}
	else
		{
		//	Otherwise, just check the weapon

		if (retiMinFireArc)
			*retiMinFireArc = m_iMinFireArc;
		if (retiMaxFireArc)
			*retiMaxFireArc = m_iMaxFireArc;

		return (m_iMinFireArc != m_iMaxFireArc);
		}
	}

bool CWeaponClass::IsOmniDirectional (CInstalledDevice *pDevice)

//	IsOmniDirectional
//
//	Returns TRUE if the weapon is omnidirectional (not limited)

	{
	//	The device slot improves the weapon. If the device slot is directional, then
	//	the weapon is directional. If the device slot is omni directional, then the
	//	weapon is omnidirectional.

	if (pDevice && pDevice->IsOmniDirectional())
		return true;

	return m_bOmnidirectional;
	}

bool CWeaponClass::IsStdDamageType (DamageTypes iDamageType, int iLevel)

//	IsStdDamageType
//
//	Returns TRUE if the given damage type is standard at the given level.

	{
	int iTierLevel = ::GetDamageTypeLevel(iDamageType);

	return (iLevel >= iTierLevel && iLevel < iTierLevel + 3);
	}

bool CWeaponClass::IsTrackingWeapon (CItemCtx &Ctx)

//	IsTrackingWeapon
//
//	Returns TRUE if we're a tracking weapon
	
	{
	CWeaponFireDesc *pShot = GetWeaponFireDesc(Ctx);
	if (pShot == NULL)
		return false;

	return pShot->IsTrackingOrHasTrackingFragments();
	}

bool CWeaponClass::IsVariantSelected (CSpaceObject *pSource, CInstalledDevice *pDevice)

//	IsVariantSelected
//
//	Returns TRUE if we've selected some variant (i.e., we haven't selected 0xffff)

	{
	return (GetCurrentVariant(pDevice) != -1);
	}

bool CWeaponClass::IsWeaponAligned (CSpaceObject *pShip, 
									CInstalledDevice *pDevice, 
									CSpaceObject *pTarget, 
									int *retiAimAngle, 
									int *retiFireAngle)

//	IsWeaponAligned
//
//	Return TRUE if weapon is aligned on target.
//
//	Note: If the weapon is invalid, we return an aim angle of -1

	{
	CItemCtx Ctx(pShip, pDevice);
	CWeaponFireDesc *pShot = GetWeaponFireDesc(Ctx);
	if (pShot == NULL || pShip == NULL || pDevice == NULL)
		{
		if (retiAimAngle)
			*retiAimAngle = -1;
		if (retiFireAngle)
			*retiFireAngle = -1;
		return false;
		}

	ASSERT(pTarget);

	//	Compute source

	CVector vSource = pDevice->GetPos(pShip);

	//	We need to calculate the intercept solution.

	Metric rWeaponSpeed = pShot->GetRatedSpeed();
	CVector vTarget = pTarget->GetPos() - vSource;
	CVector vTargetVel = pTarget->GetVel() - pShip->GetVel();

	//	Figure out which direction to fire in

	Metric rDist;
	Metric rTimeToIntercept = CalcInterceptTime(vTarget, vTargetVel, rWeaponSpeed, &rDist);
	CVector vInterceptPoint = (rTimeToIntercept > 0.0 ? vTarget + vTargetVel * rTimeToIntercept : vTarget);
	int iAim = VectorToPolar(vInterceptPoint, NULL);
	if (retiAimAngle)
		*retiAimAngle = iAim;

	//	Omnidirectional weapons are always aligned

	if (IsOmniDirectional(pDevice))
		{
		if (retiFireAngle)
			*retiFireAngle = iAim;
		return true;
		}

	int iFacingAngle = (pShip->GetRotation() + pDevice->GetRotation() + AngleMiddle(m_iMinFireArc, m_iMaxFireArc)) % 360;

	//	Area weapons are always aligned

	if (pShot->GetType() == ftArea)
		{
		if (retiFireAngle)
			*retiFireAngle = iFacingAngle;
		return true;
		}

	//	Figure out our aim tolerance

	int iAimTolerance;
	switch (m_Configuration)
		{
		case ctDual:
		case ctDualAlternating:
			iAimTolerance = 10;
			break;

		case ctSpread2:
			iAimTolerance = 20;
			break;

		case ctSpread3:
		case ctSpread5:
			iAimTolerance = 30;
			break;

		case ctWall:
			iAimTolerance = 45;
			break;

		case ctCustom:
			iAimTolerance = 2 * m_iConfigAimTolerance;
			break;

		default:
			{
			if (m_iFireRate > 10)
				iAimTolerance = 2;
			else if (m_iFireRate > 4)
				iAimTolerance = 4;
			else
				iAimTolerance = 6;
			}
		}

	//	If this is a directional weapon, figure out whether the target
	//	is in the fire arc

	int iMinFireAngle;
	int iMaxFireAngle;
	bool bDirectional = IsDirectional(pDevice, &iMinFireAngle, &iMaxFireAngle);

	//	Tracking weapons behave like directional weapons with 120 degree field

	if (!bDirectional && pShot->IsTracking())
		{
		iMinFireAngle = 300;
		iMaxFireAngle = 60;
		bDirectional = true;
		}

	if (bDirectional)
		{
		int iMin = (pShip->GetRotation() + iMinFireAngle - iAimTolerance + 360) % 360;
		int iMax = (pShip->GetRotation() + iMaxFireAngle + iAimTolerance) % 360;

		//	Are we in the fire arc?

		bool bInArc;
		if (iMin < iMax)
			{
			//	e.g., iMin == 0; iMax == 180

			bInArc = (iAim >= iMin && iAim <= iMax);
			}
		else
			{
			//	e.g., iMin == 315; iMax == 45

			bInArc = (iAim >= iMin || iAim <= iMax);
			}

		//	Compute the fire direction

		if (retiFireAngle)
			{
			if (bInArc)
				*retiFireAngle = iAim;
			else if (AngleOffset(iAim, iMin) < AngleOffset(iAim, iMax))
				*retiFireAngle = iMin;
			else
				*retiFireAngle = iMax;
			}

		return bInArc;
		}

	//	Fire angle

	if (retiFireAngle)
		*retiFireAngle = iFacingAngle;

	//	Compute the angular size of the target

	int iHalfAngularSize = (int)(20.0 * pTarget->GetHitSize() / rDist);

	//	Figure out how far off we are from the direction that we
	//	want to fire in.

	int iAimOffset = AngleOffset(iFacingAngle, iAim);

	//	If we're facing in the direction that we want to fire, 
	//	then we're aligned...

	if (iAimOffset <= Max(iAimTolerance, iHalfAngularSize))
		return true;

	//	Otherwise, we're not and we need to return the aim direction

	else
		return false;
	}

bool CWeaponClass::NeedsAutoTarget (CItemCtx &Ctx, int *retiMinFireArc, int *retiMaxFireArc)

//	NeedsAutoTarget
//
//	Returns TRUE if this weapon is either a swivel weapon, an omniweapon, or a
//	tracking weapon.
//
//	If necessary we return the (absolute) fire arc where we should look for 
//	targets (if we're a swivel weapon).

	{
	//	If we're a tracking weapon then we have no swivel restrictions

	if (IsTrackingWeapon(Ctx))
		{
		if (retiMinFireArc) *retiMinFireArc = 0;
		if (retiMaxFireArc) *retiMaxFireArc = 0;
		return true;
		}

	//	If we're an omni or swivel weapon, adjust our fire arc

	int iMinFireArc = 0;
	int iMaxFireArc = 0;
	if (CanRotate(Ctx, &iMinFireArc, &iMaxFireArc))
		{
		//	Adjust arc based on player rotation

		if (iMinFireArc != iMaxFireArc
				&& Ctx.GetSource())
			{
			iMinFireArc = AngleMod(iMinFireArc + Ctx.GetSource()->GetRotation());
			iMaxFireArc = AngleMod(iMaxFireArc + Ctx.GetSource()->GetRotation());
			}

		if (retiMinFireArc) *retiMinFireArc = iMinFireArc;
		if (retiMaxFireArc) *retiMaxFireArc = iMaxFireArc;

		return true;
		}

	//	Otherwise, we don't need a target

	return false;
	}

void CWeaponClass::OnAccumulateAttributes (CItemCtx &ItemCtx, const CItem &Ammo, TArray<SDisplayAttribute> *retList)

//	OnAccumulateAttributes
//
//	Adds attributes of the weapon type

	{
	//	Add omnidirectional and arc attributes

	int iMinArc;
	int iMaxArc;
	if (CanRotate(ItemCtx, &iMinArc, &iMaxArc))
		{
		//	Omni

		if (iMinArc == iMaxArc)
			retList->Insert(SDisplayAttribute(attribPositive, CONSTLIT("omnidirectional")));
		else
			{
			int iArc = AngleRange(iMinArc, iMaxArc);
			if (iArc >= 150)
				retList->Insert(SDisplayAttribute(attribPositive, CONSTLIT("hemi-directional")));
			else
				retList->Insert(SDisplayAttribute(attribPositive, CONSTLIT("swivel")));
			}
		}

	//	These properties are valid either for an ammo-less weapon, or a specific
	//	ammo/missile.

	CWeaponFireDesc *pRootShot;
	if ((GetSelectVariantCount() == 1 || !Ammo.IsEmpty())
			&& (pRootShot = GetWeaponFireDesc(ItemCtx, Ammo)))
		{
		//	Sometimes the fragments do all the damage. In that case, we take 
		//	the properties from the fragment.

		int iFragments;
		CWeaponFireDesc *pShot = GetReferenceShotData(pRootShot, &iFragments);
		DamageDesc Damage = pShot->GetDamage();

		//	Compute special abilities.

		if (pRootShot->IsTracking() || pShot->IsTracking())
			retList->Insert(SDisplayAttribute(attribPositive, CONSTLIT("tracking")));

		//	Special damage delivery

		if (pShot->GetType() == ftArea)
			retList->Insert(SDisplayAttribute(attribPositive, CONSTLIT("shockwave")));

		else if (pShot->GetType() == ftRadius)
			retList->Insert(SDisplayAttribute(attribPositive, CONSTLIT("radius")));

		//	For continuous beam

		else if (pShot->GetType() == ftContinuousBeam)
			retList->Insert(SDisplayAttribute(attribPositive, CONSTLIT("beam")));

		//	For particles...

		else if (pShot->GetType() == ftParticles)
			retList->Insert(SDisplayAttribute(attribPositive, CONSTLIT("cloud")));

		//	For large number of fragments, we have a special description

		else if (iFragments >= 8 && !pShot->IsTracking())
			retList->Insert(SDisplayAttribute(attribPositive, CONSTLIT("fragmentation")));

		//	Passthrough
		//	
		//	(We ignore passthrough for shockwaves, since that's already a 
		//	property of them.)

		if (pShot->GetPassthrough() >= 20 && pShot->GetType() != ftArea)
			retList->Insert(SDisplayAttribute(attribPositive, CONSTLIT("passthrough")));

		//	Stealth

		if (pRootShot->GetStealth() > CSpaceObject::stealthNormal)
			retList->Insert(SDisplayAttribute(attribPositive, CONSTLIT("stealth")));

		//	Blinding

		if (Damage.GetBlindingDamage() > 0)
			retList->Insert(SDisplayAttribute(attribPositive, CONSTLIT("blinding")));

		//	Radiation

		if (Damage.GetRadiationDamage() > 0)
			retList->Insert(SDisplayAttribute(attribPositive, CONSTLIT("radiation")));

		//	EMP

		if (Damage.GetEMPDamage() > 0)
			retList->Insert(SDisplayAttribute(attribPositive, CONSTLIT("EMP")));

		//	Device damage

		if (Damage.GetDeviceDamage() > 0)
			retList->Insert(SDisplayAttribute(attribPositive, CONSTLIT("device damage")));

		if (Damage.GetDeviceDisruptDamage() > 0)
			retList->Insert(SDisplayAttribute(attribPositive, CONSTLIT("device ionize")));

		//	Disintegration

		if (Damage.GetDisintegrationDamage() > 0)
			retList->Insert(SDisplayAttribute(attribPositive, CONSTLIT("disintegration")));

		//	Shields

		if (Damage.GetShieldDamageLevel() > 0)
			retList->Insert(SDisplayAttribute(attribPositive, CONSTLIT("shield buster")));

		//	Armor

		if (Damage.GetArmorDamageLevel() > 0)
			retList->Insert(SDisplayAttribute(attribPositive, CONSTLIT("armor penetrate")));

		//	WMD

		if (Damage.GetMassDestructionLevel() > 0)
			retList->Insert(SDisplayAttribute(attribPositive, strPatternSubst(CONSTLIT("WMD %d"), Damage.GetMassDestructionLevel())));
		}
	}

void CWeaponClass::OnAddTypesUsed (TSortMap<DWORD, bool> *retTypesUsed)

//	OnAddTypesUsed
//
//	Adds types used by this class

	{
	int i;

	for (i = 0; i < m_ShotData.GetCount(); i++)
		if (m_ShotData[i].bOwned)
			m_ShotData[i].pDesc->AddTypesUsed(retTypesUsed);
	}

ALERROR CWeaponClass::OnDesignLoadComplete (SDesignLoadCtx &Ctx)

//	OnDesignLoadComplete
//
//	Done loading all design elements

	{
	DEBUG_TRY

	ALERROR error;

	//	Shots

	for (int i = 0; i < m_ShotData.GetCount(); i++)
		{
		//	If we own this definition, then we need to bind it.

		if (m_ShotData[i].bOwned)
			{
			if (error = m_ShotData[i].pDesc->OnDesignLoadComplete(Ctx))
				return error;
			}

		//	Otherwise, all we have to do is point to the descriptor from
		//	the item.

		else
			{
		    //	Bind the ammoID

		    if (error = m_ShotData[i].pAmmoType.Bind(Ctx))
			    return error;

			//	Must be valid

			if (m_ShotData[i].pAmmoType == NULL)
				{
				Ctx.sError = strPatternSubst(CONSTLIT("Unable to find ItemType for ammoID: %x"), m_ShotData[i].pAmmoType.GetUNID());
				return ERR_FAIL;
				}

			//	Get the descriptor

			m_ShotData[i].pDesc = m_ShotData[i].pAmmoType->GetMissileDesc();
			if (m_ShotData[i].pDesc == NULL)
				{
				Ctx.sError = strPatternSubst(CONSTLIT("ItemType must have <Missile> definition: %x"), m_ShotData[i].pAmmoType.GetUNID());
				return ERR_FAIL;
				}
			}
		}

	return NOERROR;

	DEBUG_CATCH
	}

ALERROR CWeaponClass::OnFinishBind (SDesignLoadCtx &Ctx)

//  OnFinishBind
//
//  Finish binding

    {
	int i;
	ALERROR error;

	//	Events

	GetItemType()->InitCachedEvents(evtCount, CACHED_EVENTS, m_CachedEvents);

	//	Shots

	TSortMap<CItemType *, bool> UniqueAmmo;
	for (i = 0; i < m_ShotData.GetCount(); i++)
		{
		//	If we own this definition, then we need to bind it.

		if (m_ShotData[i].bOwned)
			{
			if (error = m_ShotData[i].pDesc->FinishBindDesign(Ctx))
				return error;
			}

		//	Make a list of all unique ammo items

		if (m_ShotData[i].pDesc->GetAmmoType())
			UniqueAmmo.SetAt(m_ShotData[i].pDesc->GetAmmoType(), true);
		}

	//	For each ammo item that we fire, give it a pointer to us so that it can
	//	refer back.

	for (i = 0; i < UniqueAmmo.GetCount(); i++)
		{
		CItemType *pAmmoType = UniqueAmmo.GetKey(i);
		pAmmoType->AddWeapon(this);
		}

	return NOERROR;
    }

CEffectCreator *CWeaponClass::OnFindEffectCreator (const CString &sUNID)

//	OnFindEffectCreator
//
//	Find the effect. We start after the weapon class UNID.

	{
	//	We start after the weapon class UNID

	char *pPos = sUNID.GetASCIIZPointer();
	if (*pPos != '/')
		return NULL;

	pPos++;

	//	Parse the variant

	int iOrdinal = strParseInt(pPos, 0, &pPos);
	if (iOrdinal < 0 || iOrdinal >= m_ShotData.GetCount())
		return NULL;

	//	Now ask the weapon fire desc to parse the remaining UNID

    CItem Ammo;
    if (iOrdinal < GetAmmoItemCount())
        Ammo = CItem(GetAmmoItem(iOrdinal), 1);

    CWeaponFireDesc *pDesc = GetWeaponFireDesc(CItemCtx(), Ammo);
	return pDesc->FindEffectCreator(CString(pPos));
	}

CString CWeaponClass::OnGetReference (CItemCtx &Ctx, const CItem &Ammo, DWORD dwFlags)

//	OnGetReference
//
//	Return reference

	{
	CString sReference;

	//	For weapons

	if (Ammo.IsEmpty())
		{
		//	For ammo weapons, we describe the kind of ammo we need.

		CItemType *pAmmo;
		if (!IsLauncher()
				&& GetAmmoItemCount() == 1
				&& (pAmmo = GetAmmoItem(0)))
			{
			AppendReferenceString(&sReference, strPatternSubst(CONSTLIT("Requires %s"), pAmmo->GetNounPhrase(0x02)));
			}
		}

	return sReference;
	}

void CWeaponClass::OnMarkImages (void)

//	OnMarkImages
//
//	Load all images that we need

	{
	int i;

	for (i = 0; i < m_ShotData.GetCount(); i++)
		m_ShotData[i].pDesc->MarkImages();
	}

bool CWeaponClass::RequiresItems (void) const

//	RequiresItems
//
//	Returns TRUE if this weapon requires ammo

	{
	return UsesAmmo();
	}

bool CWeaponClass::SelectFirstVariant (CSpaceObject *pSource, CInstalledDevice *pDevice)

//	SelectFirstVariant
//
//	Selects the first valid variant for this weapon

	{
	SetCurrentVariant(pDevice, -1);
	return SelectNextVariant(pSource, pDevice);
	}

bool CWeaponClass::SelectNextVariant (CSpaceObject *pSource, CInstalledDevice *pDevice, int iDir)

//	SelectNextVariant
//
//	Selects the next valid variant for this weapon

	{
	//	For scaling, the descriptor to use is based on something other than the
	//	selection, so we only one selection.

	if (m_iVariantType == varLevelScaling || m_iVariantType == varCharges)
		{
		CWeaponFireDesc *pShot = GetWeaponFireDesc(CItemCtx(pSource, pDevice));
		if (pShot && VariantIsValid(pSource, pDevice, *pShot))
			{
			SetCurrentVariant(pDevice, 0);
			return true;
			}
		else
			{
			SetCurrentVariant(pDevice, -1);
			return false;
			}
		}
	else
		{
		int iStart = GetCurrentVariant(pDevice);
		if (iStart == -1)
			iStart = m_ShotData.GetCount() + (iDir == 1 ? 0 : -1);
		else
			iStart = m_ShotData.GetCount() + iStart + iDir;

		//	Loop over all variants

		int iSelection = -1;
		if (m_ShotData.GetCount() > 0)
			{
			for (int i = 0; i < m_ShotData.GetCount(); i++)
				{
				int iTry = (iStart + (i * iDir)) % m_ShotData.GetCount();

				//	If this variant is valid, then we're done

				if (VariantIsValid(pSource, pDevice, *m_ShotData[iTry].pDesc))
					{
					iSelection = iTry;
					break;
					}
				}
			}

		//	If we found a selection, then select the variant

		SetCurrentVariant(pDevice, iSelection);
		return (iSelection != -1);
		}
	}

void CWeaponClass::SetAlternatingPos (CInstalledDevice *pDevice, int iAlternatingPos) const

//	SetAlternatingPos
//
//	Sets the alternating position

	{
	pDevice->SetData((pDevice->GetData() & 0xFFFF00FF) | (((DWORD)iAlternatingPos & 0xFF) << 8));
	}

void CWeaponClass::SetContinuousFire (CInstalledDevice *pDevice, DWORD dwContinuous) const

//	SetContinuousFire
//
//	Sets the continuous fire counter for the device

	{
	pDevice->SetData((pDevice->GetData() & 0xFFFFFF00) | (dwContinuous & 0xFF));
	}

bool CWeaponClass::SetCounter (CInstalledDevice *pDevice, CSpaceObject *pSource, CounterTypes iCounter, int iLevel)

//  SetCounter
//
//  Sets the counter to the given level. Returns FALSE if we cannot set it.

    {
    if (m_Counter != iCounter || pDevice == NULL || pSource == NULL)
        return false;

    pDevice->SetTemperature(Max(0, Min(iLevel, MAX_COUNTER)));
	pSource->OnComponentChanged(comDeviceCounter);

    return true;
    }

void CWeaponClass::SetCurrentVariant (CInstalledDevice *pDevice, int iVariant) const

//	SetCurrentVariant
//
//	Sets the current variant for the device

	{
	pDevice->SetData((((DWORD)(WORD)(short)iVariant) << 16) | LOWORD(pDevice->GetData()));
	}

void CWeaponClass::Update (CInstalledDevice *pDevice, CSpaceObject *pSource, SDeviceUpdateCtx &Ctx)

//	Update
//
//	Update

	{
	DEBUG_TRY

	CItemCtx ItemCtx(pSource, pDevice);

	//	Change counter on update

	if (IsCounterEnabled() && (Ctx.iTick % m_iCounterUpdateRate) == 0)
		{
		if (m_iCounterUpdate > 0)
			{
			if (pDevice->GetTemperature() < MAX_COUNTER)
				{
				pDevice->IncTemperature(Min(m_iCounterUpdate, MAX_COUNTER - pDevice->GetTemperature()));
				pSource->OnComponentChanged(comDeviceCounter);
				}
			}
		else
			{
			if (pDevice->GetTemperature() > 0)
				{
				pDevice->IncTemperature(Max(m_iCounterUpdate, -pDevice->GetTemperature()));
				pSource->OnComponentChanged(comDeviceCounter);
				}
			}
		}

	//	Done if we're disabled

	if (!pDevice->IsEnabled())
		return;

	//	See if we continue to fire

	DWORD dwContinuous = GetContinuousFire(pDevice);
	if (dwContinuous == CONTINUOUS_START)
		{
		//	-1 is used to skip the first update cycle
		//	(which happens on the same tick as Activate)

		CWeaponFireDesc *pShot = GetWeaponFireDesc(ItemCtx);
		SetContinuousFire(pDevice, (pShot ? pShot->GetContinuous() : 0));
		}
	else if (dwContinuous > 0)
		{
		CWeaponFireDesc *pShot = GetWeaponFireDesc(ItemCtx);
		if (pShot)
			{
			FireWeapon(pDevice, 
					pShot, 
					pSource, 
					NULL, 
					1 + pShot->GetContinuous() - dwContinuous,
					&Ctx.bSourceDestroyed,
					&Ctx.bConsumedItems);

			if (Ctx.bSourceDestroyed)
				return;
			}

		dwContinuous--;
		SetContinuousFire(pDevice, dwContinuous);
		}
	else if (pDevice->HasLastShots()
			&& (!pDevice->IsTriggered() || pDevice->GetTimeUntilReady() > 1))
		pDevice->SetLastShotCount(0);

	DEBUG_CATCH
	}

bool CWeaponClass::ValidateSelectedVariant (CSpaceObject *pSource, CInstalledDevice *pDevice)

//	ValidateSelectedVariant
//
//	If the selected variant is valid, then it returns TRUE. Otherwise,
//	it selects a different valid variant. If not valid variants are found,
//	it returns FALSE

	{
	CItemCtx Ctx(pSource, pDevice);
	CWeaponFireDesc *pShot = GetWeaponFireDesc(Ctx);
	if (pShot && VariantIsValid(pSource, pDevice, *pShot))
		return true;

	if (SelectNextVariant(pSource, pDevice))
		return true;

	if (SelectFirstVariant(pSource, pDevice))
		return true;

	return false;
	}

bool CWeaponClass::VariantIsValid (CSpaceObject *pSource, CInstalledDevice *pDevice, CWeaponFireDesc &ShotData)

//	VariantIsValid
//
//	Returns TRUE if the variant is valid

	{
	//	If we have charges, then we're only valid if we have charges left.

	if (m_bCharges)
		{
		if (pDevice->GetCharges(pSource) == 0)
			return false;
		}

	//	If we do not need ammo, then we're always valid

	if (ShotData.GetAmmoType() == NULL)
		return true;

	//	Otherwise, check to see if we have enough ammo

	CItemListManipulator ItemList(pSource->GetItemList());
	CItem Item(ShotData.GetAmmoType(), 1);
	if (ItemList.SetCursorAtItem(Item))
		return true;

	//	Not valid

	return false;
	}

