#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "PlayerPawn.generated.h"

UCLASS()
class VACCINEEXE_API APlayerPawn : public APawn
{
	GENERATED_BODY()

public:
	APlayerPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 충돌체 및 외형
	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;

	// 총구 위치 및 방향
	UPROPERTY(EditAnywhere)
	class UArrowComponent* firePosition;

	// 발사체 원본 BP 클래스를 담을 변수
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABullet> bulletFactory;

private:
	// 자동 발사 함수 및 타이머 핸들
	void Fire();
	FTimerHandle FireTimerHandle;
};