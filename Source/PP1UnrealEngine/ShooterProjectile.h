// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ShooterProjectile.generated.h"

UCLASS()
class PP1UNREALENGINE_API AShooterProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShooterProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//	Sphere collision component
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		USphereComponent* CollisionComponent;

	//	Projectile movement component
	//	https://docs.unrealengine.com/4.27/en-US/API/Runtime/Engine/GameFramework/UProjectileMovementComponent/
	UPROPERTY(VisibleAnywhere, Category = Movement)
		UProjectileMovementComponent* ProjectileMovementComponent;

	//	Projectile mesh
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
	UStaticMeshComponent* ProjectileMeshComponent;

	//	Projectile material
	UPROPERTY(VisibleDefaultsOnly, Category = Movement)
	UMaterialInstanceDynamic* ProjectileMaterialInstance;

	//	This function will be responsible for launching the projectile.
	//	It initializes the projectile's velocity in the shoot direction.
	void FireInDirection(const FVector& ShootDirection);		
};
