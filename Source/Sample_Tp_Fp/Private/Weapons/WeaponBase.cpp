// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapons/WeaponBase.h"
#include "Character/MainCharacter.h"
#include "DrawDebugHelpers.h"
#include "Components/BoxComponent.h"
#include "Engine/SkeletalMeshSocket.h"

// Sets default values
AWeaponBase::AWeaponBase()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponSkeltalMesh"));
	RootComponent = WeaponMesh;

	WeaponCollisionComp = CreateDefaultSubobject<UBoxComponent>(TEXT("WeaponCollisionComponent"));
	WeaponCollisionComp->SetupAttachment(WeaponMesh);
	WeaponCollisionComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	WeaponCollisionComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	WeaponCollisionComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	WeaponCollisionComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	WeaponCollisionComp->OnComponentBeginOverlap.AddDynamic(this, &AWeaponBase::OnWeaponOvelapBegin);
	WeaponCollisionComp->OnComponentEndOverlap.AddDynamic(this, &AWeaponBase::OnWeaponOverlapEnd);
}

// Called when the game starts or when spawned
void AWeaponBase::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeaponBase::EquipWeapon(AMainCharacter* MaiCharacter)
{
	if (MaiCharacter)
	{
		const USkeletalMeshSocket* RightHandSocket = MaiCharacter->GetMesh()->GetSocketByName("hand_rSocket");
		RightHandSocket->AttachActor(this, MaiCharacter->GetMesh());
	}
}

void AWeaponBase::Fire(AMainCharacter* Mypawn)
{
	if (Mypawn)
	{
		UE_LOG(LogTemp, Warning, TEXT("WeaponFIRING"));
		FVector EyeLocation;// can be changed to muzzle location
		FRotator EyeRotation;//can be changed to muzzle Rotation
		Mypawn->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		FVector EndTrace = EyeLocation + (EyeRotation.Vector() * 20000);
		TArray<FHitResult> HitResults;
		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(Mypawn);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;

		if (GetWorld()->LineTraceMultiByChannel(OUT HitResults, EyeLocation, EndTrace, ECC_Visibility, QueryParams))
		{

			for (FHitResult& Result : HitResults)
			{
				if (AActor* HitActor = Result.GetActor())
				{
					UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitActor->GetName());
					GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, FString::Printf(TEXT("ActorName:%s"), *HitActor->GetName()));
				}
			}
		}
		DrawDebugLine(GetWorld(), EyeLocation, EndTrace, FColor::Red, false, 2.0f, 0, 3.f);
	}
}

void AWeaponBase::OnWeaponOvelapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
		if (MainCharacter)
		{
			MainCharacter->SetEquipWeapon(this);
		}
	}
}

void AWeaponBase::OnWeaponOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AMainCharacter* MainCharacter = Cast<AMainCharacter>(OtherActor);
		if (MainCharacter)
		{
			UE_LOG(LogTemp, Warning, TEXT("Current End Weapon"))
				MainCharacter->SetEquipWeapon(nullptr);
		}
	}
}

