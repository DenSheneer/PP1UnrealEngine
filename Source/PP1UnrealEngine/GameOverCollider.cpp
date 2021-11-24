// Fill out your copyright notice in the Description page of Project Settings.


#include "GameOverCollider.h"
#include <Components/BoxComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include <GameFramework/PlayerController.h>
#include "MyCharacter.h"

// Sets default values
AGameOverCollider::AGameOverCollider()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PickupRoot = CreateDefaultSubobject<USceneComponent>(TEXT("PickupRootComponent"));
	RootComponent = PickupRoot;

	GameOverCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("GameOverCollider"));
	GameOverCollider->SetGenerateOverlapEvents(true);
	GameOverCollider->OnComponentHit.AddDynamic(this, &AGameOverCollider::OnEnter);
	GameOverCollider->BodyInstance.SetCollisionProfileName(TEXT("Pickup"));
	GameOverCollider->SetupAttachment(PickupRoot);

}

// Called when the game starts or when spawned
void AGameOverCollider::BeginPlay()
{
	Super::BeginPlay();
}

void AGameOverCollider::OnEnter(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	UE_LOG(LogTemp, Warning, TEXT("this was true"));
	AMyCharacter* player;
	player = Cast<AMyCharacter>(OtherActor);
	if (player)
	{
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindLambda([&]
			{
				APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
				if (PlayerController) PlayerController->ConsoleCommand("quit");
			});
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerDelegate, 1.5f, false);
	}
}

// Called every frame
void AGameOverCollider::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

