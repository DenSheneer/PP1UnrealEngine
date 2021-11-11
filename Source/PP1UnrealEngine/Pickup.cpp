// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include <Components/SphereComponent.h>
#include "MyCharacter.h"
#include <Components/BoxComponent.h>
#include <Components/PrimitiveComponent.h>
#include "Components/StaticMeshComponent.h"
#include <Logging/LogMacros.h>

// Sets default values
APickup::APickup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickupRoot = CreateDefaultSubobject<USceneComponent>(TEXT("PickupRootComponent"));
	RootComponent = PickupRoot;

	PickupCollider = CreateDefaultSubobject<USphereComponent>(TEXT("PickupCollider"));
	PickupCollider->SetGenerateOverlapEvents(true);
	PickupCollider->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	PickupCollider->OnComponentHit.AddDynamic(this, &APickup::OnPickup);
	PickupCollider->AttachToComponent(PickupRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	PickupCollider->BodyInstance.SetCollisionProfileName(TEXT("Pickup"));
}

// Called when the game starts or when spawned
void APickup::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APickup::OnPickup(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	AMyCharacter* player;
	player = Cast<AMyCharacter>(OtherActor);
	if (player)
	{
		player->TakePickup(thisPickupType);
		Destroy();
	}
}

