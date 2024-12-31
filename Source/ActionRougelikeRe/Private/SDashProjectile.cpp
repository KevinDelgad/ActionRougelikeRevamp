// Fill out your copyright notice in the Description page of Project Settings.


#include "SDashProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

ASDashProjectile::ASDashProjectile()
{
	ImpactParticle = CreateDefaultSubobject<UParticleSystemComponent>("ImpactEmitter");
	ImpactParticle->SetAutoActivate(false);
	ImpactParticle->SetupAttachment(RootComponent);
}

void ASDashProjectile::TeleportAnim()
{
	MovementComp->Velocity = FVector::ZeroVector;
	ImpactParticle->Activate();
	
	GetWorldTimerManager().SetTimer(AliveTimer, this, &ASDashProjectile::Teleport, 0.2f);
}

void ASDashProjectile::Teleport()
{
	AActor* ProjectileInstigator = this->GetInstigator();
	ProjectileInstigator->TeleportTo(this->GetActorLocation(), FRotator(0, 0, 0));
	
	this->Destroy();
}

void ASDashProjectile::ImmediateTeleport(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	GetWorldTimerManager().ClearTimer(AliveTimer);
	TeleportAnim();
}


void ASDashProjectile::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(AliveTimer, this, &ASDashProjectile::TeleportAnim, 0.2f);
}

void ASDashProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentHit.AddDynamic(this,  &ASDashProjectile::ImmediateTeleport);
}

