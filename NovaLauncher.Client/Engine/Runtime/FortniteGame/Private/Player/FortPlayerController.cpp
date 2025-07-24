#include "Player/FortPlayerController.h"

UFortWorldItem* AFortPlayerController::GetInventoryItemWithGuid(FGuid ItemGuid)
{
	struct
	{
		FGuid ItemGuid;
		UFortWorldItem* ReturnValue;
	} params(ItemGuid);

	this->ProcessEvent(L"BP_GetInventoryItemWithGuid", &params);

	return params.ReturnValue;
}

void AFortPlayerController::ClientForceUpdateQuickbar(EFortQuickBars QuickbarToRefresh)
{
	this->ProcessEvent(L"ClientForceUpdateQuickbar", &QuickbarToRefresh);
}

void AFortPlayerController::HandleQuickBarChanged(EFortQuickBars QuickBarType, TArray<int> Slots)
{
	struct
	{
		EFortQuickBars QuickBarType;
		TArray<int> Slots;
	} params(QuickBarType, Slots);
	
	this->ProcessEvent(L"HandleQuickBarChanged", &params);
}