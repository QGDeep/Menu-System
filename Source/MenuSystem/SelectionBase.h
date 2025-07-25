// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "SelectionBase.generated.h"

class UCommonTextBlock;

USTRUCT(BlueprintType)
struct FSelectionOption
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Option)
	FText Label;
};


UCLASS(Blueprintable, BlueprintType)
class MENUSYSTEM_API USelectionBase : public UCommonUserWidget
{
	GENERATED_BODY()
	
public:
	USelectionBase();
	// C++ equivalent for blueprint construct event
	virtual void NativeConstruct() override;

	void Clear();
	void AddOption(const FSelectionOption& InOption);
	void SetCurrentSelection(int InIndex);

	// left and right buttons(arrows)
	UFUNCTION(BlueprintCallable)
	void SelectPrevious();

	UFUNCTION(BlueprintCallable)
	void SelectNext();

	// delegate (event) that use an int when the selection is changed
	DECLARE_DELEGATE_OneParam(FOnSelectionChange, int)
	FOnSelectionChange OnSelectionChange;
	
protected:
	/*
	 * A function that defines navigation rules for the widget
	 * Can override the behaviour for keyboard keys and controller inputs
	 * for UI navigation
	 */
	UFUNCTION()
	UWidget* OnNavigation(EUINavigation InNavigation);

	// Function update to label
	void UpdateCurrentSelection();

	// Array of selection Options
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FSelectionOption> Options;

	// Pointer to selection options
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UCommonTextBlock> Label;

	int CurrentSelection;
};
