// Fill out your copyright notice in the Description page of Project Settings.

#include "NetCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

// Sets default values
ANetCharacter::ANetCharacter()
{
    // Habilitar replicación de red en el Actor
    bReplicates = true;

    // Optimización: tasa de sincronización de red
    NetUpdateFrequency = 66.0f;

    CurrentHealth = 100.0f;
    ManaPoints = 50;
}

void ANetCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    // Replicar salud a todas las máquinas conectadas
    DOREPLIFETIME(ANetCharacter, CurrentHealth);

    // [OPTIMIZACIÓN]: Maná solo viaja al dueño de este pawn (OwnerOnly)
    DOREPLIFETIME_CONDITION(ANetCharacter, ManaPoints, COND_OwnerOnly);
}

void ANetCharacter::CastAbility()
{
    // 1. Verificación de propiedad: solo el cliente dueño puede enviar este input
    if (!IsLocallyControlled())
    {
        return;
    }

    // 2. Predicción de UI del lado del cliente: chequeo rápido antes de emitir tráfico
    if (ManaPoints < 10)
    {
        if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Yellow, TEXT("[Cliente] Maná insuficiente."));
        return;
    }

    // 3. Invocar RPC Server (Unreal lo encamina automáticamente al servidor)
    Server_CastAbility();
}

// VALIDACIÓN ANTITRAMPAS (Corre en el Servidor antes de la implementación)
bool ANetCharacter::Server_CastAbility_Validate()
{
    const int32 AbilityCost = 10;

    // El servidor comprueba si la petición es físicamente y lógicamente posible
    if (CurrentHealth <= 0.0f || ManaPoints < AbilityCost)
    {
        // Retornar false indica paquete anómalo / intento de exploit
        return false;
    }

    return true;
}


// EJECUCIÓN AUTORITATIVA (Corre exclusivamente en el Servidor)
void ANetCharacter::Server_CastAbility_Implementation()
{
    const int32 AbilityCost = 10;

    // Mutación autoritativa del estado
    ManaPoints -= AbilityCost;

    // Ordenar a todos los clientes que reproduzcan el cosmético
    Multicast_PlayAbilityFX();

    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Green,
            FString::Printf(TEXT("[Servidor] Habilidad aprobada. Maná restante: %d"), ManaPoints));
    }
}

// EFECTO COSMÉTICO (Corre en Servidor y todos los Clientes remotos)
void ANetCharacter::Multicast_PlayAbilityFX_Implementation()
{
    // Al ser Unreliable, si hay congestión de red no bloqueará datos críticos
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 2.5f, FColor::Cyan,
            FString::Printf(TEXT("[FX] Partículas y sonido reproducidos en NetMode: %d"), (int32)GetNetMode()));
    }
}

float ANetCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent,
    AController* EventInstigator, AActor* DamageCauser)
{
    // Solo el Servidor tiene potestad de reducir salud
    if (!HasAuthority())
    {
        return 0.0f;
    }

    const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    const float OldHealth = CurrentHealth;

    CurrentHealth = FMath::Clamp(CurrentHealth - ActualDamage, 0.0f, MaxHealth);

    // En el Servidor, OnRep no se invoca automáticamente; lo llamamos manualmente para coherencia de eventos
    OnRep_CurrentHealth(OldHealth);

    return ActualDamage;
}

void ANetCharacter::OnRep_CurrentHealth(float OldHealth)
{
    // Se ejecuta en clientes receptores cuando llega la variable modificada
    if (GEngine)
    {
        GEngine->AddOnScreenDebugMessage(-1, 4.0f, FColor::Red,
            FString::Printf(TEXT("[OnRep] Salud actualizada de %.1f a %.1f"), OldHealth, CurrentHealth));
    }

    // Aquí actualizarías el porcentaje de tu ProgressBar en UMG:
    // HealthProgressBar->SetPercent(CurrentHealth / MaxHealth);
}

// Called when the game starts or when spawned
void ANetCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANetCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ANetCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

