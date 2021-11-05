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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//	Boolean for making object hit able
	UPROPERTY(EditAnywhere, Category = "hit")
		bool IsHitable = true;

		void OnTakeHit(AActor* thisInstigator);
};
