// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyFactory.h"
#include "EnemyActor.h"
#include "Engine/World.h"

// Sets default values
AEnemyFactory::AEnemyFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyFactory::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 1. 타이머 누적
	CurrentTime += DeltaTime;

	// 2. 간격 체크
	if (CurrentTime >= DelayTime)
	{
		// 3. 널 체크 및 스폰 로직
		if (GetWorld() && Enemy)
		{
			GetWorld()->SpawnActor<AEnemyActor>(
				Enemy,
				GetActorLocation(),
				GetActorRotation()
			);

			// 스폰 후 다음 지연 시간을 랜덤하게 변경 (1~3초)
			DelayTime = FMath::RandRange(1.0f, 3.0f);
		}

		// 4. 타이머 초기화
		CurrentTime = 0.0f;
	}
}
