// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonActivatableWidget.h"
#include "SelectionBase.h"
#include "SettingsWidget.generated.h"

class UCheckBox;
class UComboBoxString;

/*
 * Declare enum for framerate and also
 * define a class to handle the changes
 * being made to the framerate value.
 */
UENUM(BlueprintType)
enum class EFramerate : uint8
{
	FPS_30 UMETA(DisplayName = "30 FPS"),
	FPS_40 UMETA(DisplayName = "40 FPS"),
	FPS_60 UMETA(DisplayName = "60 FPS"),
	FPS_120 UMETA(DisplayName = "120 FPS"),
	FPS_Uncapped UMETA(DisplayName = "Uncapped"),
};

class FFramerateUtils
{
public:
	static int EnumToValue(const EFramerate& InFramerate)
	{
		switch (InFramerate)
		{
			case EFramerate::FPS_30:return 30;
			case EFramerate::FPS_40:return 40;
			case EFramerate::FPS_60:return 60;
			case EFramerate::FPS_120:return 120;
			default:return 0;
		}
	}

	static FString EnumToString(const EFramerate& InFramerate)
	{
		const auto Value = EnumToValue(InFramerate);
		return Value > 0 ? FString::Printf(TEXT("%d FPS"), Value) : FString::Printf(TEXT("Uncapped"));
	}
};


UCLASS()
class MENUSYSTEM_API USettingsWidget : public UCommonActivatableWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	/*
	 * returns a pointer to the widget which tells unreal to focus
	 * on the widget when it becomes active.
	 */
	virtual UWidget* NativeGetDesiredFocusTarget() const override;

protected:
	UFUNCTION()
	void OnResolutionChanged(FString InSelectedItem, ESelectInfo::Type InSelectionType);

	UFUNCTION()
	void OnVSyncChanged(bool InIsChanged);

	
	/*
	 * A class to serve as the entry point for currently set settings
	 * and to apply and save new ones.
	 */
	UPROPERTY()
	TObjectPtr<UGameUserSettings> GameUserSettings;

	// Resolution Setting
	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UComboBoxString> ResolutionComboBox;
	
	/*
	 * function to get values and save values based on changes
	 * made by the player
	 */
	void InitializeResolutionComboBox();

	void InitializeVSync();

	void InitializeFramerate();

	/*
	 * Holds 2 data points for resolutions
	 * Height and Width
	 */
	UPROPERTY()
	TArray<FIntPoint> Resolutions;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<UCheckBox> VSyncCheckBox;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USelectionBase> FramerateSelection;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USelectionBase> ShadingQualitySelection;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USelectionBase> GlobalIlluminationQualitySelection;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USelectionBase> PostProcessingQualitySelection;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USelectionBase> VisualEffectsQualitySelection;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	TObjectPtr<USelectionBase> ShadowQualitySelection;
};
