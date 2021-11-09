// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

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

		//	Sets MuzzleOffset. Leaving it at the guide's offset of 100.0f in front of the camera for now.
		MuzzleOffset.Set(100.0f, 0.0f, 0.0f);

		// Transform MuzzleOffset from camera space to world space.
		//	FTransform documentation: https://docs.unrealengine.com/4.27/en-US/API/Runtime/Core/Math/FTransform/
		//	FVector MuzzleLocation = CameraLocation = FTransform(CameraRotation).TransformVector(MuzzleOffset);		//	from the fps game guide
		FVector MuzzleLocation = CameraLocation;

		//	Skews the aim.
		FRotator MuzzleRotation = CameraRotation;		
		MuzzleRotation.Pitch -= 10.0f;		// 10.0f is the angle I set in the editor for the camera component.

		UWorld* World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = GetInstigator();		//	https://docs.unrealengine.com/4.27/en-US/API/Runtime/Engine/Engine/FActorSpawnParameters/Instigator/

			//	Spawn the projectile at the set position. (Preferably the muzzle)
			AShooterProjectile* Projectile = World->SpawnActor<AShooterProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);

			if (Projectile)
			{
				//	Set the projectile's initial trajectory.
				FVector LaunchDirection = MuzzleRotation.Vector();
				Projectile->FireInDirection(LaunchDirection);
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

