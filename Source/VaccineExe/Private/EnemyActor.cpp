// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyActor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "Bullet.h"
#include "PlayerPawn.h"
#include "VaccineGameModeBase.h"

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

	// 추적 확률 결정
	int32 RandomValue = FMath::RandRange(1, 100);
	if (RandomValue <= traceRate)
	{
		bIsTracking = true;
	}
	else
	{
		bIsTracking = false;
		// 직진 모드일 경우 초기 방향 설정
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (PlayerPawn)
		{
			Direction = PlayerPawn->GetActorLocation() - GetActorLocation();
			Direction.Normalize();
		}
	}
}

// Called every frame
void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsTracking)
	{
		// 실시간 추적 모드: 플레이어 폰 방향 매번 계산
		APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
		if (PlayerPawn)
		{
			Direction = PlayerPawn->GetActorLocation() - GetActorLocation();
			Direction.Normalize();
		}
	}

	// 이동 처리
	FVector NewLocation = GetActorLocation() + (Direction * MoveSpeed * DeltaTime);
	NewLocation.Z = 0.0f;
	SetActorLocation(NewLocation, true);

	// 이동 방향 바라보기
	if (!Direction.IsNearlyZero())
	{
		SetActorRotation(Direction.Rotation());
	}
}

void AEnemyActor::OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 1. 상대방이 Bullet인지 확인
	ABullet* Bullet = Cast<ABullet>(OtherActor);
	if (Bullet)
	{
		// 게임 모드 점수 추가
		AVaccineGameModeBase* GM = Cast<AVaccineGameModeBase>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->AddScore(1);
		}
		
		// 폭발 효과 및 사운드
		if (explosionFX)
		{
			UE_LOG(LogTemp, Warning, TEXT("explosionFX is VALID! Spawning Niagara..."));
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), explosionFX, GetActorLocation(), FRotator::ZeroRotator, FVector(10.0f));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("explosionFX is NULL! BP assignment failed."));
		}
		
		if (explosionSound)
		{
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), explosionSound, GetActorLocation());
		}

		// 총알 파괴
		Bullet->Destroy();
		// 자신(적) 파괴
		this->Destroy();
	}
	// 2. 상대방이 PlayerPawn인지 확인
	else if (APlayerPawn* Player = Cast<APlayerPawn>(OtherActor))
	{
		// 플레이어 HP 감소
		Player->hp--;
		UE_LOG(LogTemp, Warning, TEXT("Player Hit! Remaining HP: %d"), Player->hp);

		// 충돌 지점 계산 (중간 지점 + Z축 보정)
		FVector ImpactPoint = (GetActorLocation() + Player->GetActorLocation()) * 0.5f;
		ImpactPoint.Z = 100.0f;

		Player->OnHit(ImpactPoint);
				
		// HP가 0 이하라면 게임 오버 메뉴 표시
		if (Player->hp <= 0)
		{
			AVaccineGameModeBase* GM = Cast<AVaccineGameModeBase>(GetWorld()->GetAuthGameMode());
			if (GM)
			{
				GM->ShowMenu();
			}
		}

		// 자신(적) 파괴
		this->Destroy();
	}
}