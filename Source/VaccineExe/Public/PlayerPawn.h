#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h" // Enhanced Input 값 처리를 위해 추가
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

	// 기존 컴포넌트들
	UPROPERTY(EditAnywhere)
	class UBoxComponent* boxComp;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* meshComp;

	UPROPERTY(EditAnywhere)
	class UArrowComponent* firePosition;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class ABullet> bulletFactory;

	// 1. 폰 이동을 전담할 컴포넌트 추가
	UPROPERTY(VisibleAnywhere)
	class UFloatingPawnMovement* movementComp;

	// 2. Enhanced Input 변수 선언
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* defaultMappingContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* moveAction; // Axis2D (IA_Move) 사용

	void Move(const FInputActionValue& Value);

private:
	void Fire();
	FTimerHandle FireTimerHandle;
};