// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyActor.generated.h"

UCLASS()
class VACCINEEXE_API AEnemyActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// 컴포넌트 선언
	UPROPERTY(EditAnywhere, Category = "Components")
	class UBoxComponent* BoxComp;

	UPROPERTY(EditAnywhere, Category = "Components")
	class UStaticMeshComponent* MeshComp;

	// 상태 변수
	UPROPERTY(EditAnywhere, Category = "Status")
	float MoveSpeed = 300.0f;

	UPROPERTY(EditAnywhere, Category = "Status")
	int32 HP = 3;

	UPROPERTY(EditAnywhere, Category = "Status")
	int32 traceRate = 50;

private:
	bool bIsTracking = false;
	FVector Direction;

	// 수정된 충돌 함수 서명 (OnComponentBeginOverlap 대응)
	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
