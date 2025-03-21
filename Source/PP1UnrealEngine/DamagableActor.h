// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DamagableActor.generated.h"

UCLASS()
class PP1UNREALENGINE_API ADamagableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	void TakeAttack();

	UPROPERTY(EditAnywhere, Category = "attack")
	bool isAttackable = true;

	UFUNCTION(BlueprintImplementableEvent, Category = "attack")
	void onTakeAttack();

	// Sets default values for this actor's properties
	ADamagableActor();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
