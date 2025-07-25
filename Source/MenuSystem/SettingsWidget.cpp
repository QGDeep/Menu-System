// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingsWidget.h"

#include "Components/CheckBox.h"
#include "Components/ComboBoxString.h"
#include "GameFramework/GameUserSettings.h"
#include "Kismet/KismetSystemLibrary.h"

namespace
{
	// define const array for framerate options
	constexpr EFramerate FramerateOptions[] = {
		EFramerate::FPS_30,
		EFramerate::FPS_40,
		EFramerate::FPS_60,
		EFramerate::FPS_120,
		EFramerate::FPS_Uncapped
	};

	/*
	 * Getter and Setter and pointers to functions from
	 * game user settings
	 */
	typedef int32(UGameUserSettings::*GetFunc)() const;
	typedef void(UGameUserSettings::*SetFunc)(int);
	
	struct FSelectionElement
	{
		USelectionBase* Widget;		// pointer to selection component
		GetFunc GetFunc;			// get function typedef
		SetFunc SetFunc;			// set function typedef
	};
}


void USettingsWidget::NativeConstruct()
{
	//Super::NativeConstruct();

	GameUserSettings = UGameUserSettings::GetGameUserSettings();

	InitializeResolutionComboBox();
	InitializeVSync();
	InitializeFramerate();

	const FSelectionElement SelectionElements[] = {
		{ShadingQualitySelection, &UGameUserSettings::GetShadingQuality, &UGameUserSettings::SetShadingQuality},
		{GlobalIlluminationQualitySelection, &UGameUserSettings::GetGlobalIlluminationQuality, &UGameUserSettings::SetGlobalIlluminationQuality},
		{PostProcessingQualitySelection, &UGameUserSettings::GetPostProcessingQuality, &UGameUserSettings::SetPostProcessingQuality},
		{VisualEffectsQualitySelection, &UGameUserSettings::GetVisualEffectQuality, &UGameUserSettings::SetVisualEffectQuality},
		{ShadowQualitySelection, &UGameUserSettings::GetShadowQuality, &UGameUserSettings:: SetShadowQuality},
	};

	for (const auto& [Widget, GetFunc, SetFunc] : SelectionElements)
	{
		const auto CurrentSelection = std::invoke(GetFunc, GameUserSettings);
		Widget->SetCurrentSelection(CurrentSelection);
		Widget->OnSelectionChange.BindLambda([this, SetFunc](int InSelection)
		{
			std::invoke(SetFunc, GameUserSettings, InSelection);
			GameUserSettings->ApplySettings(false);
		});
	}
}

UWidget* USettingsWidget::NativeGetDesiredFocusTarget() const
{
	return ResolutionComboBox;
}

void USettingsWidget::OnResolutionChanged(FString InSelectedItem, ESelectInfo::Type InSelectionType)
{
	const auto SelectedResolution = Resolutions[ResolutionComboBox->GetSelectedIndex()];
	GameUserSettings->SetScreenResolution(SelectedResolution);
	GameUserSettings->ApplySettings(false);
}

void USettingsWidget::OnVSyncChanged(bool InIsChanged)
{
	GameUserSettings->SetVSyncEnabled(InIsChanged);
	GameUserSettings->ApplySettings(false);
}

void USettingsWidget::InitializeResolutionComboBox()
{
	Resolutions.Reset();
	UKismetSystemLibrary::GetSupportedFullscreenResolutions(Resolutions);

	// Set resolution options
	ResolutionComboBox->ClearOptions();
	for(const auto& Resolution : Resolutions)
	{
		// 1920 x 1080
		const auto ResolutionString = FString::Printf(TEXT("%dx%d"), Resolution.X, Resolution.Y);
		ResolutionComboBox->AddOption(ResolutionString);
	}

	// find current resolution
	const auto CurrentResolution = GameUserSettings->GetScreenResolution();
	const auto SelectedIndex = Resolutions.IndexOfByPredicate([&CurrentResolution](const FIntPoint& InResolution)
	{
		return CurrentResolution == InResolution;
	});

	check(SelectedIndex >= 0);
	ResolutionComboBox->SetSelectedIndex(SelectedIndex);

	// listen for changes
	ResolutionComboBox->OnSelectionChanged.Clear();
	ResolutionComboBox->OnSelectionChanged.AddDynamic(this, &USettingsWidget::OnResolutionChanged);
}

void USettingsWidget::InitializeVSync()
{
	VSyncCheckBox->SetIsChecked(GameUserSettings->IsVSyncEnabled());
	VSyncCheckBox->OnCheckStateChanged.Clear();
	VSyncCheckBox->OnCheckStateChanged.AddDynamic(this, &USettingsWidget::OnVSyncChanged);
}


void USettingsWidget::InitializeFramerate()
{
	FramerateSelection->Clear();

	int FramerateOptionIndex = 0;

	const auto CurrentFramerate = GameUserSettings->GetFrameRateLimit();
	// add framerate options to the menu
	for (const auto& Framerate: FramerateOptions)
	{
		FramerateSelection->AddOption({
		FText::FromString(FFramerateUtils::EnumToString(Framerate))
		});

		if (CurrentFramerate == FFramerateUtils::EnumToValue(Framerate))
		{
			FramerateSelection->SetCurrentSelection(FramerateOptionIndex);
		}

		FramerateOptionIndex++;
	}

	FramerateSelection->OnSelectionChange.BindLambda([this](const int InSelection)
	{
		GameUserSettings->SetFrameRateLimit(FFramerateUtils::EnumToValue(
			FramerateOptions[InSelection]
			));
		
		GameUserSettings->ApplySettings(false);
	});
}
