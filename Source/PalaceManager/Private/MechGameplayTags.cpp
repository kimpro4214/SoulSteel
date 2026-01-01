// Fill out your copyright notice in the Description page of Project Settings.


#include "MechGameplayTags.h"

namespace MechGameplayTags
{
	/** Input Tags **/
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Jump, "InputTag.Jump");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_EquipPunch, "InputTag.EquipPunch");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_UnequipPunch, "InputTag.UnequipPunch");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_LightAttack_Punch, "InputTag.LightAttack.Punch");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_HeavyAttack_Punch, "InputTag.HeavyAttack.Punch");

	/** Player Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Equip_Punch, "Player.Ability.Equip.Punch");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Unequip_Punch, "Player.Ability.Unequip.Punch");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Light_Punch, "Player.Ability.Attack.Light.Punch");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Heavy_Punch, "Player.Ability.Attack.Heavy.Punch");

	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Punch, "Player.Weapon.Punch");
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_PunchSecondary, "Player.Weapon.PunchSecondary");
	
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip_Punch, "Player.Event.Equip.Punch");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Unequip_Punch, "Player.Event.Unequip.Punch");
	
	/** Enemy Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Weapon, "Enemy.Weapon");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_WeaponSecondary, "Enemy.WeaponSecondary");
	
	/** Shared tags **/
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_MeleeHit,"Shared.Event.MeleeHit");
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_BaseDamage,"Shared.SetByCaller.BaseDamage");

}
