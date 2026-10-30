#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameSettingsSave.generated.h"

UCLASS()
class BEBRA_API UGameSettingsSave : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	bool bQuickCast = false;
};