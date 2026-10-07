// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "NetCharacter.generated.h"

UCLASS()
class AUTHCLIENTSERVERMOD_API ANetCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANetCharacter();

	// Registro obligatorio de variables replicadas
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Disparado por el input local (tecla de habilidad)
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void CastAbility();

	// Modificación de salud autoritativa
	virtual float TakeDamage(float DamageAmount,
		struct FDamageEvent const& DamageEvent,
		class AController* EventInstigator,
		AActor* DamageCauser) override;

protected:
	// ESTADO REPLICADO
	// Salud: visible para todos, ejecuta OnRep al sincronizar
	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth, VisibleAnywhere, Category = "Stats")
	float CurrentHealth;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	float MaxHealth = 100.0f;

	// Maná: solo se replica al cliente propietario (optimización de ancho de banda)
	UPROPERTY(Replicated, VisibleAnywhere, Category = "Stats")
	int32 ManaPoints;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_CurrentHealth(float OldHealth);

	// RPCs
	// El cliente solicita al servidor ejecutar la habilidad con validación estricta
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_CastAbility();

	// El servidor avisa a todos los clientes que reproduzcan los efectos visuales
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayAbilityFX();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
