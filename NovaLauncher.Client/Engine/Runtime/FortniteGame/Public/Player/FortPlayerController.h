#pragma once
#include "GameFramework/PlayerController.h"
#include "Inventory/FortWorldItem.h"
#include "Inventory/FortQuickBars.h"

class AFortPlayerController : public APlayerController
{
	GENERATED_UCLASS_BODY(AFortPlayerController, FortniteGame)
	
public:
    UPROPERTY_BOOL(bWantsToSprint)
    bool bWantsToSprint;

    UPROPERTY_BOOL(bHoldingSprint)
    bool bHoldingSprint;

    UPROPERTY(AActor*, TargetedBuilding)
    AActor* TargetedBuilding;

    UFortWorldItem* GetInventoryItemWithGuid(FGuid ItemGuid);

    void ClientForceUpdateQuickbar(EFortQuickBars QuickbarToRefresh);
    void HandleQuickBarChanged(EFortQuickBars QuickBarType, TArray<int> Slots);
};