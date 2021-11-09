// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

UCLASS()
class PP1UNREALENGINE_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSCharacter();

	UPROPERTY(EditAnywhere, Category = "weapon")
	float shootRange{ 1000 };

	UFUNCTION(BlueprintCallable, Category = "weapon")
	void fireWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FHitResult instantShot();	
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
