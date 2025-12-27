// Fill out your copyright notice in the Description page of Project Settings.


#include "MechGameplayTags.h"

namespace MechGameplayTags
{
	/** Input Tags **/
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Move, "InputTag.Move");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_Look, "InputTag.Look");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_EquipPunch, "InputTag.EquipPunch");
	UE_DEFINE_GAMEPLAY_TAG(InputTag_UnequipPunch, "InputTag.UnequipPunch");

	/** Player Tags **/
	UE_DEFINE_GAMEPLAY_TAG(Player_Weapon_Punch, "Player.Weapon.Punch");
	
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Equip_Punch, "Player.Event.Equip.Punch");
	UE_DEFINE_GAMEPLAY_TAG(Player_Event_Unequip_Punch, "Player.Event.Unequip.Punch");
}
