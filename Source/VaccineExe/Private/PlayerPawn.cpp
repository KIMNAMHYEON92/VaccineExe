#include "PlayerPawn.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/FloatingPawnMovement.h" // 이동 컴포넌트
#include "EnhancedInputComponent.h"             // 입력 바인딩
#include "EnhancedInputSubsystems.h"            // 서브시스템
#include "Bullet.h"
#include "TimerManager.h"

APlayerPawn::APlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f)); 

	meshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	meshComp->SetupAttachment(boxComp);

	firePosition = CreateDefaultSubobject<UArrowComponent>(TEXT("FirePosition"));
	firePosition->SetupAttachment(boxComp);

	// 이동 컴포넌트 생성 (이것이 있어야 AddMovementInput이 작동함)
	movementComp = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComp"));
}

void APlayerPawn::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(FireTimerHandle, this, &APlayerPawn::Fire, 0.5f, true);
}



void APlayerPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// 1. 매핑 컨텍스트 등록 (가장 안전한 위치)
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings(); // 중복 방지
			if (defaultMappingContext)
			{
				Subsystem->AddMappingContext(defaultMappingContext, 0);
				UE_LOG(LogTemp, Warning, TEXT("[SUCCESS] Mapping Context 등록 완료!"));
			}
		}
	}

	// 2. 액션과 함수 바인딩
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (moveAction)
		{
			EnhancedInputComponent->BindAction(moveAction, ETriggerEvent::Triggered, this, &APlayerPawn::Move);
			UE_LOG(LogTemp, Warning, TEXT("[SUCCESS] MoveAction 바인딩 완료!"));
		}
	}
}

void APlayerPawn::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();
	
	// 키보드를 누를 때마다 이 로그가 미친듯이 올라와야 정상입니다.
	UE_LOG(LogTemp, Warning, TEXT("Move Input: X=%f, Y=%f"), MovementVector.X, MovementVector.Y);

	if (Controller != nullptr)
	{
		// X축(전후: W,S)과 Y축(좌우: A,D)
		FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f);
		AddMovementInput(MoveDirection, 1.0f);
	}
}

void APlayerPawn::Fire()
{
	if (!GetWorld() || !bulletFactory) return;

	GetWorld()->SpawnActor<ABullet>(
		bulletFactory,
		firePosition->GetComponentLocation(),
		firePosition->GetComponentRotation()
	);
}