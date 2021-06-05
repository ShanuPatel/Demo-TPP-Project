// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapons/WeaponBase.h"
#include "DrawDebugHelpers.h"

// Sets default values
AMainCharacter::AMainCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	/**Camera Boom Default Initialiser*/
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bUsePawnControlRotation = true;
	/**Camera Component Default Initialiser*/
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(CameraBoom);
	Camera->bUsePawnControlRotation = false;

	BaseTurnRate = 45.0f;
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	/**Charcter Configuration*/
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->bUseControllerDesiredRotation = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = .5f;

	bIsSprinting = false;
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void AMainCharacter::MoveForward(float Val)
{
	if (Controller && Val != 0.0f)
	{
		if (Val < 0)StopSprint();
		const FRotator NewRotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, NewRotation.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		AddMovementInput(Direction, Val);
	}
}
void AMainCharacter::MoveRight(float Val)
{
	if (Controller && Val != 0.0f)
	{
		if (Val < 0 || Val>0)StopSprint();
		const FRotator NewRotator = Controller->GetControlRotation();
		const FRotator YawRotation(0, NewRotator.Yaw, 0);
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		AddMovementInput(Direction, Val);
	}
}

void AMainCharacter::LookUp(float Val)
{
	AddControllerYawInput(Val * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::LookRight(float Val)
{
	AddControllerPitchInput(Val * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMainCharacter::Jump()
{
	Super::Jump();
}

void AMainCharacter::StopJumping()
{
	Super::StopJumping();
}

void AMainCharacter::OnEquip()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->EquipWeapon(this);
		UE_LOG(LogTemp, Warning, TEXT("Has Weapon"));
		bHasWeapon = true;
	}
}

void AMainCharacter::OnFire()
{
	if (CurrentWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("FIRING"));
		CurrentWeapon->Fire(this);
	}
}

void AMainCharacter::Sprint()
{
	if (!bIsSprinting && !GetVelocity().IsZero())
	{
		bIsSprinting = true;
		GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		UE_LOG(LogTemp, Warning, TEXT("Start Running"));
		UAnimInstance* AniInstance = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
		if (AniInstance)
		{
			AniInstance->Montage_Play(SprintMont, 1.0f);
		}
	}
}

void AMainCharacter::StopSprint()
{
	bIsSprinting = false;
	GetCharacterMovement()->MaxWalkSpeed = 275.0f;
	UAnimInstance* AniInstance = Cast<UAnimInstance>(GetMesh()->GetAnimInstance());
	if (AniInstance)
	{
		AniInstance->StopAllMontages(0.2f);
	}
	UE_LOG(LogTemp, Warning, TEXT("Stop Running"));
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("LookUp", this, &AMainCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &AMainCharacter::LookRight);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::Jump);
	PlayerInputComponent->BindAction("StopJump", IE_Released, this, &AMainCharacter::StopJumping);
	PlayerInputComponent->BindAction("Equip", IE_Pressed, this, &AMainCharacter::OnEquip);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AMainCharacter::OnFire);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMainCharacter::Sprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMainCharacter::StopSprint);
}
