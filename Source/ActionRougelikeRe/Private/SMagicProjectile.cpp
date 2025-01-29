// Fill out your copyright notice in the Description page of Project Settings.
#include "ActionRougelikeRe/Public/SMagicProjectile.h"

#include "SAttributeComponent.h"
#include "SGameplayFunctionLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASMagicProjectile::ASMagicProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Damage = 20.0f;

	
	FlightSound = CreateDefaultSubobject<UAudioComponent>("FlightSoundComp");
	FlightSound->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ASMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

//ActorOverlap Can be used rather than hit to ignore friendly fire
void ASMagicProjectile::OnActorOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this->GetInstigator())
	{
		// USAttributeComponent* AttributeComp = Cast<USAttributeComponent>(OtherActor->GetComponentByClass(USAttributeComponent::StaticClass()));
		// if (AttributeComp)
		// {
		// 	AttributeComp->ApplyHealthChange(this->GetInstigator(),Damage);
		// 	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound , GetActorLocation(), GetActorRotation());
		// 	Destroy();
		// }

		if(USGameplayFunctionLibrary::ApplyDirectionalDamage(GetInstigator(), OtherActor, Damage, SweepResult))
		{
			Explode();
		}
	}
}

void ASMagicProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,	
	FVector NormalImpulse, const FHitResult& Hit)
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound , GetActorLocation(), GetActorRotation());
	UGameplayStatics::PlayWorldCameraShake(this, CameraShakeClass, GetActorLocation(), 25000, 25000);
	Destroy();
	//Super::OnHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);
}

void ASMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ASMagicProjectile::OnActorOverlap);
}

// Called every frame
void ASMagicProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

