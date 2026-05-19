// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyFactory.generated.h"

UCLASS()
class VACCINEEXE_API AEnemyFactory : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyFactory();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 스폰할 적 클래스 (에디터에서 BP_Enemy 등 할당)
	UPROPERTY(EditAnywhere, Category = "Spawn Settings")
	TSubclassOf<class AEnemyActor> Enemy;

	// 스폰 간격
	UPROPERTY(EditAnywhere, Category = "Spawn Settings")
	float DelayTimeBase = 4.0f;
	UPROPERTY(EditAnywhere, Category = "Spawn Settings")
	float DelayTime = 4.0f;

private:
	// 내부 타이머 변수
	float CurrentTime = 0.0f;
};
