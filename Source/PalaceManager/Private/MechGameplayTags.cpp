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
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Roll,"InputTag.Roll");

	/** Player Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Equip_Punch, "Player.Ability.Equip.Punch");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Unequip_Punch, "Player.Ability.Unequip.Punch");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Light_Punch, "Player.Ability.Attack.Light.Punch");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Attack_Heavy_Punch, "Player.Ability.Attack.Heavy.Punch");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_HitPause,"Player.Ability.HitPause");
	UE_DEFINE_GAMEPLAY_TAG(Player_Ability_Roll,"Player.Ability.Roll");

	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Punch, "Player.Weapon.Punch");
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_PunchSecondary, "Player.Weapon.PunchSecondary");
	
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip_Punch, "Player.Event.Equip.Punch");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Unequip_Punch, "Player.Event.Unequip.Punch");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_HitPause,"Player.Event.HitPause");
	
	UE_DEFINE_GAMEPLAY_TAG(Player_Status_Rolling,"Player.Status.Rolling");
	
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_Light,"Player.SetByCaller.AttackType.Light");
	UE_DEFINE_GAMEPLAY_TAG(Player_SetByCaller_AttackType_Heavy,"Player.SetByCaller.AttackType.Heavy");
	
	/** Enemy Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Melee,"Enemy.Ability.Melee");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_Ranged,"Enemy.Ability.Ranged");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_SummonEnemies,"Enemy.Ability.SummonEnemies");
	
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Weapon, "Enemy.Weapon");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_WeaponSecondary, "Enemy.WeaponSecondary");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_WeaponMouth, "Enemy.WeaponMouth");
	
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Event_SummonEnemies,"Enemy.Event.SummonEnemies");
	
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_Strafing,"Enemy.Status.Strafing");
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Status_UnderAttack,"Enemy.Status.UnderAttack");
	
	UE_DEFINE_GAMEPLAY_TAG(Enemy_Ability_BossUI, "Enemy.Ability.BossUI");

	
	/** Shared tags **/
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_HitReact,"Shared.Ability.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Ability_Death,"Shared.Ability.Death");
	
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_MeleeHit,"Shared.Event.MeleeHit");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Event_HitReact,"Shared.Event.HitReact");
	UE_DEFINE_GAMEPLAY_TAG(Shared_SetByCaller_BaseDamage,"Shared.SetByCaller.BaseDamage");
	
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_Dead,"Shared.Status.Dead");
	
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact_Front,"Shared.Status.HitReact.Front");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact_Left,"Shared.Status.HitReact.Left");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact_Right,"Shared.Status.HitReact.Right");
	UE_DEFINE_GAMEPLAY_TAG(Shared_Status_HitReact_Back,"Shared.Status.HitReact.Back");

}
