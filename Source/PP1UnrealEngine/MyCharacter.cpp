// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include <DrawDebugHelpers.h>
#include <GameFramework/Character.h>
#include <Components/CapsuleComponent.h>
#include <Components/BoxComponent.h>

// Sets default values
AMyCharacter::AMyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Instantiating your class Components
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));

	//Set the location and rotation of the Character Mesh Transform
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -90.0f), FQuat(FRotator(0.0f, -90.0f, 0.0f)));

	// Attaching your class Components to the default character's Skeletal Mesh Component.
	SpringArmComp->SetupAttachment(this->GetMesh());
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);

	//Setting class variables of the spring arm
	SpringArmComp->TargetArmLength = 300.f;
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 265.0f), FRotator(0.0f, 0.0f, 0.0f));

	//Setting class variables of the Character movement component
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bIgnoreBaseRotation = true;
}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AMyCharacter::MoveForward(float InputAxis)
{
	if ((Controller != nullptr) && (InputAxis != 0.0f))
	{
		// Find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// Get forward vector

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		//	Add movement in that direction
		AddMovementInput(Direction, InputAxis);
	}
}

void AMyCharacter::MoveRight(float InputAxis)
{
	if ((Controller != nullptr) && (InputAxis != 0.0f))
	{
		// Find out which way is right.
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//	Get the right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// Add movement in that direction
		AddMovementInput(Direction, InputAxis);
	}
}

void AMyCharacter::BeginSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 1000.0f;
}

void AMyCharacter::EndSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void AMyCharacter::BeginCrouch()
{
	Crouch();
}

void AMyCharacter::EndCrouch()
{
	UnCrouch();
}

void AMyCharacter::Fire()
{
	//	Attempt to fire a projectile
	if (ProjectileClass)
	{
		//	Gets the camera transform.
		FVector CameraLocation = GetMesh()->GetComponentLocation();
		FRotator CameraRotation = GetMesh()->GetComponentRotation();

		//UE_LOG(LogTemp, Warning, TEXT("location: %s"), *CameraLocation.ToString());
		//UE_LOG(LogTemp, Warning, TEXT("rotation: %s"), *CameraRotation.ToString());

		//	GetActorEyesViewPoint documentation:
		//	https://docs.unrealengine.com/4.27/en-US/API/Runtime/Engine/GameFramework/AActor/GetActorEyesViewPoint/
		GetActorEyesViewPoint(CameraLocation, CameraRotation);

		//	Gets left hand socket's location and adds the muzzle offset to it. (offset is set in the editor)
		FVector MuzzleLocation = GetMesh()->GetSocketLocation("spine_01") + FTransform(CameraRotation).TransformVector(MuzzleOffset);

		//	Determine where the projectile is aimed towards
		FHitResult hit;
		FVector outLocation;
		FRotator outRotation;

		//	Get camera's location and rotation and store it in the out- values.
		GetController()->GetPlayerViewPoint(outLocation, outRotation);

		//	Calculate the end point by multiplying the camera's rotation by the zero distance, adding that to the start position.
		FVector TraceStart = outLocation;
		FVector TraceEnd = TraceStart + outRotation.Vector() * ZeroDistance;

		//	Do a raycast to check if the recticle is aiming at an object.
		FCollisionQueryParams Traceparams;
		GetWorld()->LineTraceSingleByChannel(hit, TraceStart, TraceEnd, ECC_Pawn, Traceparams);

		//	If the trace hits something, set the trace end to that distance.
		float CheckedDistance;
		if (hit.Distance > 0.0f) { CheckedDistance = hit.Distance; }
		else { CheckedDistance = ZeroDistance; }

		//	The new end point is calculated using the checked distance.
		FVector checkedEnd = TraceStart + outRotation.Vector() * CheckedDistance;


		//	Visualize the line with a debug drawer.
		DrawDebugLine(GetWorld(), MuzzleLocation, checkedEnd, FColor::Green, false, 1, 0, 1);

		//	Print the distance.
		//UE_LOG(LogTemp, Warning, TEXT("Distance: %f"), CheckedDistance);

		FRotator MuzzleRotation = CameraRotation;


		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();		//	https://docs.unrealengine.com/4.27/en-US/API/Runtime/Engine/Engine/FActorSpawnParameters/Instigator/

			//	Spawn the projectile at the set position.
			AShooterProjectile* Projectile = World->SpawnActor<AShooterProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);

			if (Projectile)
			{
				//	The projectile's trajectory is set using the calculated end point.				
				FVector LaunchDirection = UKismetMathLibrary::GetDirectionUnitVector(MuzzleLocation, checkedEnd);
				Projectile->FireInDirection(LaunchDirection);

				//UE_LOG(LogTemp, Warning, TEXT("LaunchDirection: %s"), *LaunchDirection.ToString());
			}
		}
	}
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	PlayerInputComponent->BindAxis("Forward", this, &AMyCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Right", this, &AMyCharacter::MoveRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &AMyCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &AMyCharacter::EndCrouch);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMyCharacter::BeginSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMyCharacter::EndSprint);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMyCharacter::Fire);

}

