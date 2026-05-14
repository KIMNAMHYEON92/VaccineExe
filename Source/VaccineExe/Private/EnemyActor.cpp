// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Bullet.h"

// Sets default values
AEnemyActor::AEnemyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// 1. BoxComponent 설정 (Root)
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
	BoxComp->SetBoxExtent(FVector(40.0f, 40.0f, 40.0f));

	// 2. MeshComponent 설정
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(BoxComp);
}

// Called when the game starts or when spawned
void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();
	
	// 컴포넌트 레벨에서 오버랩 이벤트 바인딩
	if (BoxComp)
	{
		BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor::OnOverlap);
	}
}

// Called every frame
void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 1. 플레이어 폰 찾기
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
	if (PlayerPawn)
	{
		// 2. 방향 벡터 계산 (목표 위치 - 현재 위치)
		FVector Direction = PlayerPawn->GetActorLocation() - GetActorLocation();
		Direction.Normalize();

		// 3. 이동 처리
		FVector NewLocation = GetActorLocation() + (Direction * MoveSpeed * DeltaTime);
		SetActorLocation(NewLocation);
	}
}

void AEnemyActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 1. 상대방이 Bullet인지 확인
	ABullet* Bullet = Cast<ABullet>(OtherActor);
	if (Bullet)
	{
		// 2. 총알 파괴
		Bullet->Destroy();
		// 3. 자신(적) 파괴
		this->Destroy();
		
		UE_LOG(LogTemp, Warning, TEXT("Enemy and Bullet Destroyed!"));
	}
}
