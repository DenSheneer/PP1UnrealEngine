// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HitableObject.generated.h"

UCLASS()
class PP1UNREALENGINE_API AHitableObject : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AHitableObject();

	//	Object mesh
	UPROPERTY(VisibleDefaultsOnly, Category = Projectile)
		UStaticMeshComponent* MeshComponent;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "hit")
	int projectileTypeVulnerability = 0;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//	Boolean for making object hitable
	UPROPERTY(EditAnywhere, Category = "hit")
		bool IsHitable = true;
	UPROPERTY(EditAnywhere, Category = "hit")
		bool IsDestructable = false;
	UPROPERTY(EditAnywhere, Category = "hit")
		bool UseTimer = false;
	UPROPERTY(EditAnywhere, Category = "hit")
		float DeleteTimer = 0.5f;

	bool OnTakeHit(AActor* thisInstigator, int projectileType);
};
