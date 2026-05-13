#include "PlayerPawn.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "Bullet.h" // Bullet 클래스 참조
#include "TimerManager.h" // 타이머 사용

APlayerPawn::APlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	SetRootComponent(boxComp);
	// 박스 크기를 50x50x50으로 설정
	boxComp->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f)); 

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	meshComp->SetupAttachment(boxComp);

	firePosition = CreateDefaultSubobject<UArrowComponent>(TEXT("FirePosition"));
	firePosition->SetupAttachment(boxComp);
}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	// 0.5초마다 Fire 함수 반복 실행 (자동 발사)
	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &APlayerPawn::Fire, 0.5f, true);
}

void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void APlayerPawn::Fire()
{
	// GetWorld 널 체크 및 팩토리 등록 여부 확인
	if (!GetWorld() || !bulletFactory) return;

	// 발사 로직
	GetWorld()->SpawnActor<ABullet>(
		bulletFactory,
		firePosition->GetComponentLocation(),
		firePosition->GetComponentRotation()
	);
}