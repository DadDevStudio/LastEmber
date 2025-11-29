// © 2025 Dad Dev Studio. All Rights Reserved. Unauthorized distribution or modification is prohibited.


#include "Abilities/LEGA_Shoot.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffect.h"

ULEGA_Shoot::ULEGA_Shoot()
{
    InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
    // Add tags / costs here if needed
}

void ULEGA_Shoot::ActivateAbility(
    const FGameplayAbilitySpecHandle Handle,
    const FGameplayAbilityActorInfo* ActorInfo,
    const FGameplayAbilityActivationInfo ActivationInfo,
    const FGameplayEventData* TriggerEventData)
{
    if (!ActorInfo || !ActorInfo->AvatarActor.IsValid())
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    ACharacter* Character = Cast<ACharacter>(ActorInfo->AvatarActor.Get());
    APlayerController* PC = Cast<APlayerController>(ActorInfo->PlayerController.Get());
    UAbilitySystemComponent* SourceASC = ActorInfo->AbilitySystemComponent.Get();

    if (!Character || !PC || !SourceASC)
    {
        EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
        return;
    }

    FVector Start;
    FRotator ViewRot;
    if (UCameraComponent* Cam = Character->FindComponentByClass<UCameraComponent>())
    {
        Start = Cam->GetComponentLocation();
        ViewRot = Cam->GetComponentRotation();
    }
    else
    {
        PC->GetPlayerViewPoint(Start, ViewRot);
    }

    const FVector End = Start + ViewRot.Vector() * TraceRange;
    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(LEGA_Shoot), true, Character);
    Params.AddIgnoredActor(Character);

    UWorld* World = Character->GetWorld();
    bool bHit = World && World->LineTraceSingleByChannel(Hit, Start, End, TraceChannel, Params);

    if (bHit)
    {
        DrawDebugLine(World, Start, Hit.ImpactPoint, FColor::Red, false, 2.f, 0, 1.f);
        DrawDebugSphere(World, Hit.ImpactPoint, 6.f, 8, FColor::Red, false, 2.f);

        // Server applies damage via GameplayEffect
        if (ActorInfo->IsNetAuthority())
        {
            if (DamageGameplayEffect && SourceASC)
            {
                FGameplayEffectContextHandle Ctx = SourceASC->MakeEffectContext();
                Ctx.AddSourceObject(this);

                FGameplayEffectSpecHandle SpecHandle = SourceASC->MakeOutgoingSpec(DamageGameplayEffect, GetAbilityLevel(), Ctx);
                if (SpecHandle.IsValid() && SpecHandle.Data.IsValid())
                {
                    // Set damage magnitude via SetByCaller with tag "Damage"
                    SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Gameplay.SetByCaller.Damage")), BaseDamage);

                    if (IAbilitySystemInterface* TargetASI = Cast<IAbilitySystemInterface>(Hit.GetActor()))
                    {
                        if (UAbilitySystemComponent* TargetASC = TargetASI->GetAbilitySystemComponent())
                        {
                            SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASC);
                            UE_LOG(LogTemp, Warning, TEXT("[LEGA_Shoot] Applied Damage GE to %s"), *Hit.GetActor()->GetName());
                        }
                    }
                }
            }
            else
            {
                // No GE assigned - just log on server
                if (Hit.GetActor())
                {
                    UE_LOG(LogTemp, Warning, TEXT("[LEGA_Shoot] Server hit actor: %s at %s"), *Hit.GetActor()->GetName(), *Hit.ImpactPoint.ToString());
                }
            }
        }
    }
    else
    {
        DrawDebugLine(World, Start, End, FColor::Green, false, 2.f, 0, 1.f);
    }

    EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
