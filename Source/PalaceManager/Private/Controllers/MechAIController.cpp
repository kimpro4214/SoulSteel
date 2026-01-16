// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/MechAIController.h"
#include "Navigation/CrowdFollowingComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "MechDebugHelper.h"


AMechAIController::AMechAIController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	{
		if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
		{
			Debug::Print(TEXT("CrowdFollowingComponent valid"),FColor::Green);
		}
		AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>("EnemySenseConfig_Sight");
		AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
		AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
		AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;
		AISenseConfig_Sight->SightRadius = 5000.f;
		AISenseConfig_Sight->LoseSightRadius = 0.f;
		AISenseConfig_Sight->PeripheralVisionAngleDegrees = 360.f;

		EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("EnemyPerceptionComponent");
		EnemyPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
		EnemyPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
		EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this,&ThisClass::OnEnemyPerceptionUpdated);
		
		SetGenericTeamId(FGenericTeamId(1));
	}
}

ETeamAttitude::Type AMechAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* PawnToCheck = Cast<const APawn>(&Other);

	const IGenericTeamAgentInterface* OtherTeamAgent = Cast<IGenericTeamAgentInterface>(PawnToCheck->GetController());

	if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() < GetGenericTeamId())
	{
		return ETeamAttitude::Hostile;
	}

	return ETeamAttitude::Friendly;
}

void AMechAIController::BeginPlay()
{
	Super::BeginPlay();
	
	if (UCrowdFollowingComponent* CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		CrowdComp->SetCrowdSimulationState(bEnableDetourCrowdAvoidance? ECrowdSimulationState::Enabled : ECrowdSimulationState::Disabled);

		switch (DetourCrowdAvoidanceQuality)
		{
		case 1: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Low);    break;
		case 2: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Medium); break;
		case 3: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::Good);   break;
		case 4: CrowdComp->SetCrowdAvoidanceQuality(ECrowdAvoidanceQuality::High);   break;
		default:
			break;
		}

		CrowdComp->SetAvoidanceGroup(1);
		CrowdComp->SetGroupsToAvoid(1);
		CrowdComp->SetCrowdCollisionQueryRange(CollisionQueryRange);
	}
}

void AMechAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	// 생성자 부분 (AMechAIController::AMechAIController)

	AISenseConfig_Sight->SightRadius = 5000.f;

	// [수정] LoseSightRadius를 SightRadius보다 500 정도 크게 잡으세요.
	// 이렇게 하면 적이 5000 거리에서 발견되고, 5500 밖으로 나가야만 시야에서 사라집니다. (버벅임 방지)
	AISenseConfig_Sight->LoseSightRadius = 5500.f;
	
	UBlackboardComponent* BlackboardComponent = GetBlackboardComponent();
	if (BlackboardComponent && Actor)
	{
		// 성공적으로 감지했을 때만 타겟을 갱신합니다.
		if (Stimulus.WasSuccessfullySensed())
		{
			BlackboardComponent->SetValueAsObject(FName("TargetActor"), Actor);
		}
        
		// [삭제] else { ... SetValue(Null) ... } 
		// 놓쳤다고 해서 바로 지우지 마세요! 
		// 죽었을 때 멈추는 건 이미 비헤이비어 트리(BT)의 데코레이터가 처리하고 있습니다.
		// 여기서 지우면 장애물에 스치기만 해도 멈칫거립니다.
	}
}
