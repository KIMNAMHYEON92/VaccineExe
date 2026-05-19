#include "PlayerPawn.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/FloatingPawnMovement.h" // 이동 컴포넌트
#include "EnhancedInputComponent.h"             // 입력 바인딩
#include "EnhancedInputSubsystems.h"            // 서브시스템
#include "Bullet.h"
#include "MainWidget.h"
#include "NiagaraFunctionLibrary.h"
#include "TimerManager.h"
#include "VaccineGameModeBase.h"
#include "Kismet/GameplayStatics.h"

APlayerPawn::APlayerPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	boxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollider"));
	SetRootComponent(boxComp);
	boxComp->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f)); 
	boxComp->SetCanEverAffectNavigation(false);

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

	// 화면 이탈 방지 Clamp 적용
	FVector CurrentLocation = GetActorLocation();
	CurrentLocation.X = FMath::Clamp(CurrentLocation.X, -600.0f, 600.0f);
	CurrentLocation.Y = FMath::Clamp(CurrentLocation.Y, -1000.0f, 1000.0f);
	CurrentLocation.Z = 0.0f;
	SetActorLocation(CurrentLocation,true);
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

		// 이동 방향이 존재할 경우 firePosition의 회전값 업데이트
		if (!MoveDirection.IsNearlyZero())
		{
			firePosition->SetWorldRotation(MoveDirection.Rotation());
		}
	}
}

void APlayerPawn::Fire()
{
	if (!GetWorld() || !bulletFactory) return;

	// 기본 발사
	GetWorld()->SpawnActor<ABullet>(
		bulletFactory,
		firePosition->GetComponentLocation(),
		firePosition->GetComponentRotation()
	);

	// 발사 사운드 재생
	if (fireSound)
	{
		UGameplayStatics::PlaySound2D(GetWorld(), fireSound);
	}

	// 산탄 모드 (Level 3)
	if (bIsMultiShot)
	{
		FRotator Rotation = firePosition->GetComponentRotation();
		
		// +15도
		FRotator LeftRot = Rotation;
		LeftRot.Yaw += 15.0f;
		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition->GetComponentLocation(), LeftRot);

		// -15도
		FRotator RightRot = Rotation;
		RightRot.Yaw -= 15.0f;
		GetWorld()->SpawnActor<ABullet>(bulletFactory, firePosition->GetComponentLocation(), RightRot);
	}
}

void APlayerPawn::UpgradeWeapon(int32 Level)
{
	if (Level == 2 && fireRate > 0.2f)
	{
		fireRate = 0.2f;
		// 타이머 재설정
		GetWorldTimerManager().ClearTimer(FireTimerHandle);
		GetWorldTimerManager().SetTimer(FireTimerHandle, this, &APlayerPawn::Fire, fireRate, true);
		UE_LOG(LogTemp, Warning, TEXT("Weapon Upgraded: Level 2 (Rapid Fire)"));
	}
	else if (Level == 3 && !bIsMultiShot)
	{
		bIsMultiShot = true;
		UE_LOG(LogTemp, Warning, TEXT("Weapon Upgraded: Level 3 (Multi Shot)"));
	}
}

void APlayerPawn::OnHit(FVector HitLocation)
{
	if (hitFX)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), hitFX, HitLocation, FRotator::ZeroRotator, FVector(10.0f));
	}

	AVaccineGameModeBase* GM = Cast<AVaccineGameModeBase>(GetWorld()->GetAuthGameMode());
	if (GM && GM->mainUI)
	{
		GM->mainUI->UpdateHP(hp);
	}
}