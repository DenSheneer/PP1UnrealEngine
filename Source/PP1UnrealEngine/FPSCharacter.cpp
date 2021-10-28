// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSCharacter.h"
#include "DamagableActor.h"
#include <iostream>

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPSCharacter::fireWeapon()
{
	FHitResult hit = instantShot();
	ADamagableActor* hitActor = Cast<ADamagableActor>(hit.Actor);
	if (hitActor && hitActor->isAttackable)
	{
		hitActor->TakeAttack();
	}
}

FHitResult AFPSCharacter::instantShot()
{
	FVector cameraPosition;
	FRotator cameraRotation;
	FVector endTrace = FVector(0,0,0);

	APlayerController* const playerController = GetWorld()->GetFirstPlayerController();
	if (playerController)
	{
		playerController->GetPlayerViewPoint(cameraPosition, cameraRotation);
		endTrace = cameraPosition + (cameraRotation.Vector() * shootRange);
	}

	FCollisionQueryParams traceParams(SCENE_QUERY_STAT(instantShot), true, this->GetInstigator());
	FHitResult hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(hit, cameraPosition, endTrace, ECC_Visibility, traceParams);

	return hit;
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

