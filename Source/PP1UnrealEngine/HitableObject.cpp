// Fill out your copyright notice in the Description page of Project Settings.


#include "HitableObject.h"

// Sets default values
AHitableObject::AHitableObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));

}

// Called when the game starts or when spawned
void AHitableObject::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AHitableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AHitableObject::OnTakeHit(AActor* thisInstigator)
{
	MeshComponent->AddImpulseAtLocation(thisInstigator->GetVelocity() * 30.0f, thisInstigator->GetActorLocation());
}

