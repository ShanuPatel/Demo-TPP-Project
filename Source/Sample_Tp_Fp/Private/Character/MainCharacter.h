// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MainCharacter.generated.h"

UCLASS()
class AMainCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMainCharacter();

	/**Variable for the Component*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "CameraBoom", meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	/**Movement Function and Variables*/
	UPROPERTY(EditDefaultsOnly, Category = "MouseSenstivity")
		float BaseTurnRate;
	void MoveForward(float Val);
	void MoveRight(float Val);
	void LookUp(float Val);
	void LookRight(float Val);

	virtual void Jump() override;
	virtual void StopJumping()override;

	/**Sprinting*/
	UPROPERTY(EditDefaultsOnly, Category = "Sprint")
		UAnimMontage* SprintMont;
	bool bIsSprinting;
	void Sprint();
	void StopSprint();

	/**Weapons*/
	UPROPERTY(VisibleAnywhere)
		class AWeaponBase* CurrentWeapon;
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		TSubclassOf<AWeaponBase> WeaponBaseClass;
	void OnEquip();
	bool bHasWeapon;
	void OnFire();

public:

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FORCEINLINE void SetEquipWeapon(AWeaponBase* Weapon) { CurrentWeapon = Weapon; }

};
