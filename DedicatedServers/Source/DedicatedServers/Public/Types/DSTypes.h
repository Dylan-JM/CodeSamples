#pragma once

#include "DSTypes.generated.h"

UENUM(BlueprintType)
enum class ECountdownTimerState : uint8
{
	NotStarted UMETA(DisplayName = "NotStarted"),
	Started UMETA(DisplayName = "Started"),
	Paused UMETA(DisplayName = "Paused"),
	Stopped UMETA(DisplayName = "Stopped")
};

UENUM()
enum class ECountdownTimerType : uint8
{
	LobbyCountdown UMETA(DisplayName = "LobbyCountdown"),
	PreMatch UMETA(DisplayName = "PreMatch"),
	Match UMETA(DisplayName = "Match"),
	PostMatch UMETA(DisplayName = "PostMatch"),
	None UMETA(DisplayName = "None")
};

UENUM()
enum class EMatchStatus : uint8
{
	WaitingForPlayers,
	PreMatch,
	Match,
	PostMatch,
	SeamlessTravelling
};

UENUM()
enum class ELobbyStatus : uint8
{
	WaitingForPlayers,
	CountdownToSeamlessTravel,
	SeamlessTravelling
};

USTRUCT(BlueprintType)
struct FCountdownTimerHandle
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	ECountdownTimerState State = ECountdownTimerState::NotStarted;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	ECountdownTimerType Type = ECountdownTimerType::None;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CountdownTime = 0.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CountdownUpdateInterval = 1.f;
	
	FTimerHandle TimerFinishedHandle{};
	FTimerHandle TimerUpdateHandle{};
	FTimerDelegate TimerFinishedDelegate;
	FTimerDelegate TimerUpdateDelegate;
	
};

inline bool operator==(const FCountdownTimerHandle& lhs, const FCountdownTimerHandle& rhs)
{
	return lhs.Type == rhs.Type;
}