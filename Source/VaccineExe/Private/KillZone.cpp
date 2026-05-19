#include "KillZone.h"
#include "Components/BoxComponent.h"

AKillZone::AKillZone()
{
	PrimaryActorTick.bCanEverTick = false;

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	SetRootComponent(CollisionComponent);
	
	// 기본 크기 설정
	CollisionComponent->SetBoxExtent(FVector(50.f, 2000.f, 50.f));
	
	// 충돌 설정
	CollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

void AKillZone::BeginPlay()
{
	Super::BeginPlay();
	
	// 델리게이트 바인딩
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AKillZone::OnKillZoneOverlap);
}

void AKillZone::OnKillZoneOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		UE_LOG(LogTemp, Warning, TEXT("KillZone: Destroying actor %s"), *OtherActor->GetName());
		OtherActor->Destroy();
	}
}