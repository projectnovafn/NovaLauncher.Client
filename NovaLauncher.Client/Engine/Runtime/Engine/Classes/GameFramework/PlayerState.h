#pragma once
#include "Info.h"

class APlayerState : public AInfo
{
	GENERATED_UCLASS_BODY(APlayerState, Engine)
public:

	UPROPERTY(int, PlayerID)
	int PlayerID;
};