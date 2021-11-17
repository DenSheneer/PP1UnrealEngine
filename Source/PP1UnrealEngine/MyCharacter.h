// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ShooterProjectile.h"
#include <Sound/SoundCue.h>
#include <Components/AudioComponent.h>
#include "MyCharacter.generated.h"

UCLASS()
class PP1UNREALENGINE_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	int LastPickupType = 0;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//	Called for forwards/backward input
	void MoveForward(float InputAxis);

	//	Called for left/right side input
	void MoveRight(float InputAxis);

	//	Sets Character Movement Speed to Sprint values.
	void BeginSprint();

	//	Sets Character Movement Speed to default speed values.
	void EndSprint();

	//	Request for Character to Crouch.
	void BeginCrouch();

	//	Request for Character to EndCrouch.
	void EndCrouch();

	//	Fires projectile.
	UFUNCTION()
		void Fire();

	//	Spring Arm Component to follow the camera behind the player
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class USpringArmComponent* SpringArmComp;

	//	Player follow camera
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		class UCameraComponent* CameraComp;

	//	Gun muzzle offset from the camera location.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		FVector MuzzleOffset;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
		float ZeroDistance = 2000.0f;

	//	Projectile class to spawn.
	//	https://docs.unrealengine.com/4.27/en-US/ProgrammingAndScripting/ProgrammingWithCPP/UnrealArchitecture/TSubclassOf/
	UPROPERTY(EditDefaultsOnly, Category = Projectile)
		TSubclassOf<class AShooterProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio)
		USoundCue* shootShoundcue;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Audio)
		USoundCue* pickupSoundcue;

private:
	UAudioComponent* shootAudioComponent;
	UAudioComponent* pickupAudioComponent;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void TakePickup(int type);

};
