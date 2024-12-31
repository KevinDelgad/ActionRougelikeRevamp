// Fill out your copyright notice in the Description page of Project Settings.
#include "SCharacter.h"
#include "DrawDebugHelpers.h"
#include "SInteractionComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArmComp");
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SetupAttachment(RootComponent);
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	CameraComp->SetupAttachment(SpringArm);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>("InteractionComp");
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	
	bUseControllerRotationYaw = false;
}

FRotator ASCharacter::GetProjectileRotationToCrosshair(FVector SpawnLocation)
{
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_PhysicsBody);
	FVector CameraViewPoint = CameraComp->GetComponentLocation();
	
	
	FVector End = CameraViewPoint + (CameraComp->GetForwardVector() * 5000);
	FHitResult Hit;
	bool bBlockingHit = GetWorld()->LineTraceSingleByObjectType(Hit, CameraViewPoint, End, ObjectQueryParams);

	if (bBlockingHit)
	{
		UE_LOG(LogTemp, Log, TEXT("%s was hit!"), *GetNameSafe(Hit.GetActor()));
	}
	
	FVector EndPoint = bBlockingHit ? Hit.ImpactPoint : End ;
	FRotator EndRotation = UKismetMathLibrary::FindLookAtRotation(SpawnLocation, EndPoint);
	FColor LineColor = bBlockingHit ? FColor::Red : FColor::Green;
	DrawDebugLine(GetWorld(), CameraViewPoint, EndPoint, LineColor, false, 2, 0, 2.0f);
	return EndRotation;
	
	//Projectile->SetActorRotation(EndRotation);
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASCharacter::MoveForward(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;
	
	AddMovementInput(ControlRot.Vector(), Value);
}

void ASCharacter::MoveRight(float Value)
{
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	ControlRot.Roll = 0.0f;

	//X = Forward (Red)
	//Y = Right (Green)
	//Z = Up (Blue)

	FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	
	AddMovementInput(RightVector, Value);
}

void ASCharacter::PrimaryAttack_TimeElapsed()
{
	//ensure Macro functions as a breakpoint for first Fail if returns false
	//Ensure does not run on shipped builds
	if (ensure(ProjectileClass))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
		//GetProjectileRotationToCrosshair(HandLocation);
		//Rotation and Position of Projectile Spawn
		FTransform SpawnTM = FTransform(GetProjectileRotationToCrosshair(HandLocation),HandLocation);
		
		FActorSpawnParameters SpawnParams;
		
		// Ensures Object does not move/cancel spawn to avoid overlapping when spawns
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;
		
		GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnTM, SpawnParams);
	}
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
	// -- Rotation Visualization -- //
	const float DrawScale = 100.0f;
	const float Thickness = 5.0f;

	FVector LineStart = GetActorLocation();
	// Offset to the right of pawn
	LineStart += GetActorRightVector() * 100.0f;
	// Set line end in direction of the actor's forward
	FVector ActorDirection_LineEnd = LineStart + (GetActorForwardVector() * 100.0f);
	// Draw Actor's Direction
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ActorDirection_LineEnd, DrawScale, FColor::Yellow, false, 0.0f, 0, Thickness);

	FVector ControllerDirection_LineEnd = LineStart + (GetControlRotation().Vector() * 100.0f);
	// Draw 'Controller' Rotation ('PlayerController' that 'possessed' this character)
	DrawDebugDirectionalArrow(GetWorld(), LineStart, ControllerDirection_LineEnd, DrawScale, FColor::Green, false, 0.0f, 0, Thickness);

}

void ASCharacter::AbiltyAnim(FTimerHandle TimerHandle,  TDelegate<void()>::TMethodPtr<ASCharacter> Func)
{
	PlayAnimMontage(AttackAnim);

	GetWorldTimerManager().SetTimer(TimerHandle, this, Func, 0.2f);
}

void ASCharacter::PrimaryAttack()
{

	AbiltyAnim(TimerHandle_PrimaryAttack, &ASCharacter::PrimaryAttack_TimeElapsed);
	
}

void ASCharacter::PrimaryAbility_TimeElapsed()
{
	//ensureAlways Macro functions as a breakpoint for every Fail if returns false
	// Not Recommended in Tick Function
	if (ensureAlways(BlackHole))
	{
		FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
		//GetProjectileRotationToCrosshair(HandLocation);
		//Rotation and Position of Projectile Spawn
		FTransform SpawnTM = FTransform(GetProjectileRotationToCrosshair(HandLocation),HandLocation);
		
		FActorSpawnParameters SpawnParams;
		
		// Ensures Object does not move/cancel spawn to avoid overlapping when spawns
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Instigator = this;
		
		GetWorld()->SpawnActor<AActor>(BlackHole, SpawnTM, SpawnParams);
	}
}

void ASCharacter::PrimaryAbility()
{
	AbiltyAnim(TimerHandle_PrimaryAbility, &ASCharacter::PrimaryAbility_TimeElapsed);
}

void ASCharacter::SecondaryAbility_TimeElapsed()
{
	FVector HandLocation = GetMesh()->GetSocketLocation("Muzzle_01");
	//GetProjectileRotationToCrosshair(HandLocation);
	//Rotation and Position of Projectile Spawn
	FTransform SpawnTM = FTransform(GetProjectileRotationToCrosshair(HandLocation),HandLocation);
	
	FActorSpawnParameters SpawnParams;
	
	// Ensures Object does not move/cancel spawn to avoid overlapping when spawns
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	SpawnParams.Instigator = this;
	
	GetWorld()->SpawnActor<AActor>(Dash, SpawnTM, SpawnParams);
}

void ASCharacter::SecondaryAbility()
{
	AbiltyAnim(TimerHandle_SecondaryAbility, &ASCharacter::SecondaryAbility_TimeElapsed);
}

void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}

// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	// Name / Owner of Input / Function To Trigger
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);
	
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("PrimaryAttack", IE_Pressed, this, &ASCharacter::PrimaryAttack);
	PlayerInputComponent->BindAction("PrimaryAbility", IE_Pressed, this, &ASCharacter::PrimaryAbility);
	PlayerInputComponent->BindAction("SecondaryAbility", IE_Pressed, this, &ASCharacter::SecondaryAbility);

	PlayerInputComponent->BindAction("PrimaryInteract", IE_Pressed, this, &ASCharacter::PrimaryInteract);
	
}

  