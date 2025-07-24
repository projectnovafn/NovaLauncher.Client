#pragma once
#include "Actor.h"

class APlayerState;
class APawn;

class AController : public AActor
{
	GENERATED_UCLASS_BODY(AController, Engine)
public:

	UPROPERTY(APlayerState*, PlayerState)
	APlayerState* PlayerState;

	UPROPERTY(APawn*, Pawn)
	APawn* Pawn;
};