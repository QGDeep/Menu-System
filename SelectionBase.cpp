// Fill out your copyright notice in the Description page of Project Settings.


#include "SelectionBase.h"
#include "CommonTextBlock.h"
#include "Logging/StructuredLog.h"

USelectionBase::USelectionBase()
{
	// Initialize base data
	CurrentSelection = 0;
	
	// Enables keyboard and controller interaction with UI
	SetIsFocusable(true);

	// Used to avoid using a virtual function in constructor
	SetVisibilityInternal(ESlateVisibility::Visible);
}

void USelectionBase::NativeConstruct()
{
	//Super::NativeConstruct();

	// Check if options are zero. If true, log message and return
	if(Options.Num() == 0)
	{
		UE_LOGFMT(LogTemp, Log, "USelectionBase: No options were provided");
		return;
	}

	// Used to show first the label (selected labels)
	UpdateCurrentSelection();

	/*
	 * Create a struct for navigation delegate and hold the pointer
	 * with bind dynamic (pointer is for the on navigation function)
	 */
	FCustomWidgetNavigationDelegate NavigationDelegate;
	NavigationDelegate.BindDynamic(this, &USelectionBase::OnNavigation);

	// set custom navigation rules for left and right navigation
	SetNavigationRuleCustom(EUINavigation::Left, NavigationDelegate);
	SetNavigationRuleCustom(EUINavigation::Right, NavigationDelegate);
}

void USelectionBase::Clear()
{
	// Resets all the options
	Options.Reset();
}

void USelectionBase::AddOption(const FSelectionOption& InOption)
{
	/*
	 * Add options to array and update labels
	 */
	Options.Add(InOption);
	UpdateCurrentSelection();
}

void USelectionBase::SetCurrentSelection(int InIndex)
{
	check(InIndex >= 0 && InIndex < Options.Num());
	CurrentSelection = InIndex;
	UpdateCurrentSelection();
}

void USelectionBase::SelectPrevious()
{
	OnNavigation(EUINavigation::Left);
}

void USelectionBase::SelectNext()
{
	OnNavigation(EUINavigation::Right);
}

UWidget* USelectionBase::OnNavigation(EUINavigation InNavigation)
{
	/*
	* Check if navigation parameter is limited to left and right only
	* Top or bottom should not work at all
	 */
	check(InNavigation == EUINavigation::Left || InNavigation == EUINavigation::Right);

	/*
	 * check if navigation index is true
	 * if true, for left decrease index by one
	 * else, increase index by 1 for right
	 */
	const auto Direction = InNavigation == EUINavigation::Left ? -1 : 1;

	// Update current selection
	CurrentSelection += Direction;
	if(CurrentSelection < 0)
	{
		CurrentSelection = Options.Num() - 1;
	}else if(CurrentSelection >= Options.Num())
	{
		CurrentSelection = 0;
	}

	// Update label
	UpdateCurrentSelection();

	OnSelectionChange.ExecuteIfBound(CurrentSelection);

	/*
	 * return the widget as this because the widget is visible
	 * internally. this also helps to keep this widget in focus
	 * not change to another widget.
	 */
	return this;
}

void USelectionBase::UpdateCurrentSelection()
{
	Label->SetText(Options[CurrentSelection].Label);
}

