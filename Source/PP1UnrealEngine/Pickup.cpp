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

	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	PickupMesh->AttachToComponent(PickupRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	PickupBox = CreateDefaultSubobject<UBoxComponent>(TEXT("PickupCollider"));
	PickupBox->SetGenerateOverlapEvents(true);
	PickupBox->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	PickupBox->OnComponentHit.AddDynamic(this, &APickup::OnPickup);
	PickupBox->AttachToComponent(PickupRoot, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	PickupBox->BodyInstance.SetCollisionProfileName(TEXT("Pickup"));
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
	UE_LOG(LogTemp, Warning, TEXT("this was true"));
	Destroy();
}

