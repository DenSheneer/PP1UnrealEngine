// Fill out your copyright notice in the Description page of Project Settings.


#include "HitableObject.h"
#include <TimerManager.h>

// Sets default values
AHitableObject::AHitableObject()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComponent;

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

bool AHitableObject::OnTakeHit(AActor* thisInstigator, int projectileType)
{
	if (projectileTypeVulnerability == 0 || projectileTypeVulnerability == projectileType)
	{
		MeshComponent->AddImpulseAtLocation(thisInstigator->GetVelocity() * 30.0f, thisInstigator->GetActorLocation());
		if (IsDestructable)
		{
			if (UseTimer)
			{
				FTimerDelegate TimerDelegate;
				TimerDelegate.BindLambda([&]
					{
						Destroy();
					});
				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, DeleteTimer, false);
			}
			else
			{
				Destroy();
			}
		}
		return true;
	}
	return false;
}

