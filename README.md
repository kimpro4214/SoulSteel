# Soul Steel — 액션 RPG

> 기계화된 전사들이 격돌하는 궁전, 살아남아 진실에 닿아라

**Soul Steel**은 Unreal Engine 5.4 기반의 3D 액션 RPG입니다.
플레이어는 메크 전사를 조종해 적들을 처치하고, Gameplay Ability System(GAS)으로 구동되는 다양한 스킬과 무기를 활용해 전투를 이어갑니다.
정교한 히트 판정, 모션 워핑 기반의 타격감, 속성 기반 피해 계산이 맞물린 전투 경험을 제공합니다.

## 팀 정보
- **개발 기간**: 2025.12 ~ 2026.02
- **팀 구성**: 김현규

---

## 주요 기능

### 전투 시스템
- **GAS 기반 어빌리티**: 플레이어·적 모두 `DataAsset_StartUpDataBase`를 통해 게임 시작 시 능력 자동 부여
- **무기 시스템**: `AMechWeaponBase` 기반 무기가 소켓에 부착되며 Gameplay Tag 키로 전환 관리
- **히트 판정**: `PawnCombatComponent`의 `ToggleWeaponCollision`으로 애니메이션 노티파이 단위로 콜리전 제어, `OverlappedActors`로 중복 피격 제거
- **피해 계산**: `GEExecCalc_DamageTaken` — `AttackPower`, `DefensePower`, `DamageTaken` 세 속성을 읽어 최종 피해량 산출

### 플레이어 캐릭터
- Enhanced Input + Gameplay Tag 매핑 기반 조작 (`DataAsset_InputConfig`)
- `MechInputComponent`가 NativeInput / AbilityInput 두 경로를 분리해 바인딩
- `MotionWarpingComponent` 연동으로 어빌리티 실행 중 타겟 방향 자동 보정

### 적 AI 시스템
- **Behavior Tree 기반 AI**: `MechAIController`가 BT와 Blackboard를 소유
- **서비스 / 태스크 노드**:
  - `BTService_OrientToTargetActor` — 매 틱 타겟 방향으로 회전
  - `BTTask_RotateToFaceTarget` — 어택 전 1회성 정렬
- **적 어빌리티**: `UMechEnemyGameplayAbility`로 고유 공격 패턴 구현

### 속성 & UI 시스템
- `MechAttributeSet`이 `PostGameplayEffectExecute`에서 속성 변화를 감지해 `IPawnUIInterface` 델리게이트 방송
- `HeroUIComponent` / `EnemyUIComponent`가 해당 델리게이트를 구독해 HUD 자동 갱신

---

## 기술 스택

### 엔진 & 렌더링
- **Unreal Engine 5.4**
- **Lumen** 글로벌 일루미네이션
- **Nanite** 가상화 지오메트리

### 게임플레이 시스템
- **Gameplay Ability System (GAS)** — 어빌리티, 속성, 게임플레이 이펙트 전 범위 사용
- **Enhanced Input** 1.x — Gameplay Tag 기반 입력 매핑
- **Motion Warping** — 어빌리티 실행 중 루트 모션 목표 보정
- **AI Module + Navigation System** — Behavior Tree, NavMesh 경로 탐색
- **AnimGraph Runtime** — 애니메이션 그래프 런타임 연동

### 아키텍처
- **단일 C++ 모듈** (`PalaceManager`) — 스크립팅 레이어 없이 전 로직 C++ 구현
- **GAS DataAsset 드리븐**: 어빌리티 부여를 코드 변경 없이 DataAsset 편집만으로 제어
- **Gameplay Tag 중앙 관리**: `MechGameplayTags.h` 네임스페이스 + `DefaultGameplayTags.ini`
- **인터페이스 기반 다형성**: `IPawnCombatInterface` / `IPawnUIInterface`로 Hero·Enemy 공통 접근

---

## 아키텍처

### 캐릭터 계층
```
AMechBaseCharacter  (IAbilitySystemInterface, IPawnCombatInterface, IPawnUIInterface)
├── AMechHeroCharacter   — MechHeroController, MechInputComponent
└── AMechEnemyCharacter  — MechAIController, Behavior Tree
```

`AMechBaseCharacter`가 `MechAbilitySystemComponent`, `MechAttributeSet`, `MotionWarpingComponent`를 직접 소유합니다.
캐릭터 클래스별로 `TSoftObjectPtr<UDataAsset_StartUpDataBase>`를 가리키며, 빙의(Possess) 시 어빌리티와 이펙트가 자동 부여됩니다.

### GAS 어빌리티 부여 흐름
```
DataAsset_StartUpDataBase::GiveToAbilitySystemComponent()
├── ActivateOnGivenAbilities  — 부여 즉시 활성화
├── ReactiveAbilities         — 이벤트 반응형 어빌리티
└── StartUpGameplayEffects    — 초기 속성 이펙트 적용
```

Hero 전용 `DataAsset_HeroStartUpData`, 적 전용 `DataAsset_EnemyStartUpData`로 분리됩니다.

### 컴포넌트 계층
```
UPawnCombatComponent
├── UHeroCombatComponent
└── UEnemyCombatComponent

UPawnUIComponent
├── UHeroUIComponent
└── UEnemyUIComponent
```

두 계층 모두 인터페이스(`IPawnCombatInterface` / `IPawnUIInterface`)를 통해 접근하며,
직접 캐스팅 대신 `MechFunctionLibrary` 헬퍼를 우선 사용합니다.

### Gameplay Tag 명명 규칙
| 접두사 | 용도 |
|--------|------|
| `InputTag_*` | Enhanced Input 액션 바인딩 |
| `Player_Ability_*` / `Enemy_Ability_*` | 어빌리티 식별자 |
| `Player_Weapon_*` / `Enemy_Weapon_*` | 무기 슬롯 키 |
| `Shared_*` | 크로스-폰 이벤트 (HitReact, Death, MeleeHit) |

---

## 프로젝트 구조

```
Source/PalaceManager/
├── AbilitySystem/
│   ├── Abilities/          # MechGameplayAbility, MechHeroGameplayAbility, MechEnemyGameplayAbility
│   ├── AttributeSets/      # MechAttributeSet
│   ├── ExecCalculations/   # GEExecCalc_DamageTaken
│   └── DataAssets/         # DataAsset_StartUpDataBase, Hero/Enemy 서브클래스
├── Characters/
│   ├── MechBaseCharacter   # 공통 베이스
│   ├── Hero/               # MechHeroCharacter, MechHeroController
│   └── Enemy/              # MechEnemyCharacter, MechAIController
├── Components/
│   ├── Combat/             # PawnCombatComponent, Hero/EnemyCombatComponent
│   └── UI/                 # PawnUIComponent, Hero/EnemyUIComponent
├── Weapons/                # MechWeaponBase, MechHeroWeapon
├── AI/                     # BTService_OrientToTargetActor, BTTask_RotateToFaceTarget
├── Input/                  # MechInputComponent, DataAsset_InputConfig
└── GameplayTags/           # MechGameplayTags.h / .cpp
```

---

## 핵심 설계 원칙

### 1. DataAsset 드리븐 어빌리티 부여
어빌리티를 코드에서 직접 부여하지 않고 DataAsset에 배열로 선언합니다.
새 적 유형을 추가할 때 C++ 변경 없이 DataAsset 에디터 편집만으로 어빌리티 구성이 완료됩니다.

```cpp
// DataAsset_StartUpDataBase
UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
TArray<TSubclassOf<UMechGameplayAbility>> ActivateOnGivenAbilities;

UPROPERTY(EditDefaultsOnly, Category = "StartUpData")
TArray<TSubclassOf<UGameplayEffect>> StartUpGameplayEffects;
```

### 2. 인터페이스 기반 크로스-폰 접근
전투 및 UI 컴포넌트는 Hero/Enemy 구분 없이 인터페이스로 접근해 어빌리티 코드의 결합도를 낮춥니다.

```cpp
// 직접 캐스팅 대신 인터페이스 사용
if (IPawnCombatInterface* CombatInterface = Cast<IPawnCombatInterface>(TargetActor))
{
    UPawnCombatComponent* CombatComp = CombatInterface->GetPawnCombatComponent();
    CombatComp->ToggleWeaponCollision(true, WeaponTag);
}
```

### 3. Gameplay Tag 중앙화
모든 태그를 `MechGameplayTags.h` 단일 네임스페이스에서 선언·정의합니다.
문자열 리터럴 태그 사용을 금지하고 `FGameplayTag` 상수로만 참조해 오타를 컴파일 타임에 차단합니다.

```cpp
// MechGameplayTags.h
namespace MechGameplayTags
{
    PALACEMANAGER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(InputTag_LightAttack);
    PALACEMANAGER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Player_Ability_LightAttack);
    PALACEMANAGER_API UE_DECLARE_GAMEPLAY_TAG_EXTERN(Shared_Event_MeleeHit);
}
```

### 4. 무기 콜리전 노티파이 제어
`AnimNotify` 단위로 `ToggleWeaponCollision`을 호출해 정확한 타이밍에만 히트 판정을 활성화합니다.
`OverlappedActors` Set으로 동일 스윙 내 중복 피격을 방지합니다.

```cpp
// PawnCombatComponent
void UPawnCombatComponent::ToggleWeaponCollision(bool bEnabled, FGameplayTag WeaponTag)
{
    AMechWeaponBase* Weapon = GetWeaponByTag(WeaponTag);
    if (!Weapon) return;

    if (bEnabled)
        OverlappedActors.Empty();   // 스윙 시작 시 초기화

    Weapon->GetWeaponCollisionBox()->SetCollisionEnabled(
        bEnabled ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision
    );
}
```

### 5. 속성 변화 → UI 델리게이트 방송
`MechAttributeSet::PostGameplayEffectExecute`에서 직접 위젯을 참조하지 않고 `IPawnUIInterface` 델리게이트만 방송합니다.
UI 레이어는 델리게이트를 구독해 분리된 의존성을 유지합니다.

```cpp
void UMechAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
    if (Data.EvaluatedData.Attribute == GetHealthAttribute())
    {
        OnHealthChanged.Broadcast(GetHealth(), GetMaxHealth());
    }
}
```

---

## 해결한 기술 문제

### GAS Loose 태그 생명주기 불일치로 인한 리스폰 버그

사망 시 `AddLooseGameplayTag`로 부여한 `Shared.Status.Dead` 태그가 리스폰 후에도 남아 어빌리티가 차단되는 버그가 있었습니다.

원인은 Loose 태그가 GameplayEffect 시스템 외부에 존재하기 때문에, `RemoveActiveEffectsWithTags()`만으로는 제거되지 않는다는 점이었습니다.

```cpp
// 문제: Effect 제거만 하고 Loose 태그는 그대로 남음
MechAbilitySystemComponent->RemoveActiveEffectsWithTags(DeadTags);
// → Shared.Status.Dead 태그가 여전히 존재 → 어빌리티 차단 지속

// 해결: Loose 태그를 별도로 명시적 제거
MechAbilitySystemComponent->CancelAllAbilities();
MechAbilitySystemComponent->RemoveActiveEffectsWithTags(DeadTags);
MechAbilitySystemComponent->RemoveLooseGameplayTag(MechGameplayTags::Shared_Status_Dead);
```

이를 파악한 뒤 리스폰 함수(`ServerRevive`) 안에 `CancelAllAbilities → RemoveActiveEffectsWithTags → RemoveLooseGameplayTag` 순서를 고정해 완전한 상태 초기화를 보장했습니다.

이 경험을 통해 GAS에서 Loose 태그와 Effect 태그의 생명주기가 분리되어 있다는 점, 그리고 상태 정리는 "무엇을 추가했는가"와 동일한 경로로 "무엇을 제거할 것인가"를 명시해야 한다는 점을 체득했습니다.

---

## 빌드 & 실행

### 필수 환경
- **Unreal Engine**: 5.4.x
- **Visual Studio 2022** (v143 toolset, C++20)
- **플랫폼**: Windows 64-bit

### 빌드
```bash
# Development Editor 빌드 (에디터에서 반복 작업 시)
"<UE5_ROOT>/Engine/Build/BatchFiles/Build.bat" PalaceManagerEditor Win64 Development \
  "C:/Users/qnfgh/PalaceManager/PalaceManager.uproject"
```

또는 `PalaceManager.sln`을 Visual Studio 2022에서 열고 `Development Editor` 구성으로 빌드합니다.
RiderLink 플러그인을 통한 Live Coding도 지원합니다.

### 에디터 실행
```
1. PalaceManager.uproject 더블클릭 → Unreal Editor 실행
2. Content Browser에서 시작 레벨 열기
3. 에디터 상단 ▶ Play 버튼으로 즉시 플레이 테스트
```

---

## 조작법

| 입력 | 동작 |
|------|------|
| WASD | 이동 |
| Shift | 달리기 |
| Space | 점프 |
| LMB | 일반 공격 |
| RMB | 강공격 / 스킬 |
| 1 ~ 4 | 무기 전환 |
| ESC | 인게임 메뉴 |

---

## 주요 모듈 의존성

| 모듈 | 용도 |
|------|------|
| `EnhancedInput` | 입력 처리 |
| `GameplayAbilities` | GAS 핵심 (어빌리티, 이펙트, 속성) |
| `GameplayTags`, `GameplayTasks` | GAS 인프라 |
| `AIModule`, `NavigationSystem` | Behavior Tree, NavMesh |
| `AnimGraphRuntime`, `MotionWarping` | 애니메이션 시스템 |
