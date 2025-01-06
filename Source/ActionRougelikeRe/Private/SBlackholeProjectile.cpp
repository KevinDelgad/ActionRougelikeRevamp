// Fill out your copyright notice in the Description page of Project Settings.


#include "SBlackholeProjectile.h"

#include "AssetTypeCategories.h"
#include "GameplayTagContainer.h"
#include "Components/SphereComponent.h"
#include "PhysicsEngine/PhysicsAsset.h"
#include "PhysicsEngine/RadialForceComponent.h"

ASBlackholeProjectile::ASBlackholeProjectile()
{
	PullForceComponent = CreateDefaultSubobject<URadialForceComponent>("PullForceComponent");
	PullForceComponent->SetupAttachment(RootComponent);
	PullForceComponent->AddCollisionChannelToAffect(ECC_PhysicsBody);
	
	PullForceComponent->RemoveObjectTypeToAffect(TEnumAsByte<enum EObjectTypeQuery>(ECC_Pawn));

	BlackHole = CreateDefaultSubobject<USphereComponent>("BlackHole");
	BlackHole->SetupAttachment(RootComponent);

	SphereComp->SetCollisionResponseToChannel(ECC_PhysicsBody, ECR_Overlap);
}

void ASBlackholeProjectile::BeginPlay()
{
	Super::BeginPlay();
	GetWorldTimerManager().SetTimer(DestroyTimerHandle, this, &ASBlackholeProjectile::DestroySelf, 5.0f);
}

void ASBlackholeProjectile::DestroySelf()
{
	this->Destroy();
}

void ASBlackholeProjectile::OnHitHole(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor != this->GetInstigator() && OtherActor != this)
	{
		OtherActor->Destroy();
	}
}


void ASBlackholeProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	BlackHole->OnComponentBeginOverlap.AddDynamic(this, &ASBlackholeProjectile::OnHitHole);
}
