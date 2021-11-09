// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterProjectile.h"
#include "HitableObject.h"

// Sets default values
AShooterProjectile::AShooterProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//	Setting up all the components.
	setupRootComponent();
	setupCollisionComponent();
	setupProjectileMovementComponent();
	setupProjectileMeshComponent();
	
	//	Delete the projectile after 3 seconds.
	InitialLifeSpan = lifeSpan;
}

// Called when the game starts or when spawned
void AShooterProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AShooterProjectile::setupRootComponent()
{
	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ProjectileSceneComponent"));
	}
}

void AShooterProjectile::setupCollisionComponent()
{
	if (!CollisionComponent)
	{
		//	Use a sphere as a simple collision representation.
		CollisionComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));

		//	Set the sphere's collision component profile to "Projectile".
		CollisionComponent->BodyInstance.SetCollisionProfileName(TEXT("projectile"));

		//	Even called when component hits something
		CollisionComponent->OnComponentHit.AddDynamic(this, &AShooterProjectile::OnHit);

		//	Setting the radius.
		CollisionComponent->InitSphereRadius(15.0f);

		//	Setting the root component to be the sphere component.
		RootComponent = CollisionComponent;
	}
}

void AShooterProjectile::setupProjectileMovementComponent()
{
	if (!ProjectileMovementComponent)
	{
		//	Use this component to drive this projectile's movement.
		ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
		ProjectileMovementComponent->SetUpdatedComponent(CollisionComponent);
		ProjectileMovementComponent->InitialSpeed = 3000.0f;
		ProjectileMovementComponent->MaxSpeed = 3000.0f;
		ProjectileMovementComponent->bRotationFollowsVelocity = true;
		ProjectileMovementComponent->bShouldBounce = true;
		ProjectileMovementComponent->Bounciness = 0.3f;
		ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
	}
}

void AShooterProjectile::setupProjectileMeshComponent()
{
	if (!ProjectileMeshComponent)
	{
		ProjectileMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProjectileMeshComponent"));
		//	In the editor: right-click the static mesh -> 'copy reference'
		static ConstructorHelpers::FObjectFinder<UStaticMesh>Mesh(TEXT("'/Game/Meshes/Sphere.Sphere'"));
		if (Mesh.Succeeded())
		{
			ProjectileMeshComponent->SetStaticMesh(Mesh.Object);
		}
		static ConstructorHelpers::FObjectFinder<UMaterial>Material(TEXT("'/Game/Materials/Red.Red'"));
		if (Material.Succeeded())
		{
			ProjectileMaterialInstance = UMaterialInstanceDynamic::Create(Material.Object, ProjectileMeshComponent);
		}
		ProjectileMeshComponent->SetMaterial(0, ProjectileMaterialInstance);
		ProjectileMeshComponent->SetRelativeScale3D(FVector(0.09f, 0.09f, 0.09f));
		ProjectileMeshComponent->SetupAttachment(RootComponent);

		//	SetupAttachent documentation: https://docs.unrealengine.com/4.26/en-US/API/Runtime/Engine/Components/USceneComponent/SetupAttachment/


	}
}

// Called every frame
void AShooterProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

//	This function initializes the projectile's velocity in the shoot direction.
//	The initial speed is set in the class constructor.
void AShooterProjectile::FireInDirection(const FVector& ShootDirection)
{
	ProjectileMovementComponent->Velocity = ShootDirection * ProjectileMovementComponent->InitialSpeed;
}

//	Function that is called when the projectile hits something
void AShooterProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != this && OtherComponent->IsSimulatingPhysics())
	{
		//	apply effects to other collider.
		//OtherComponent->AddImpulseAtLocation(ProjectileMovementComponent->Velocity * 100.0f, Hit.ImpactPoint);
		AHitableObject* hitActor = Cast<AHitableObject>(OtherActor);
		if (hitActor && hitActor->IsHitable)
		{
			hitActor->OnTakeHit(Cast<AActor>(this));
		}
	}

	Destroy();
}

