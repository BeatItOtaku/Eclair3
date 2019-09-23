// Fill out your copyright notice in the Description page of Project Settings.

#include "ButtonBase.h"
#include "Widgets/SNullWidget.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/Input/SButton.h"
#include "Components/ButtonSlot.h"

UButtonBase::UButtonBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SButton::FArguments ButtonDefaults;
	WidgetStyle = *ButtonDefaults._ButtonStyle;

	ColorAndOpacity = FLinearColor::White;
	BackgroundColor = FLinearColor::White;

	ClickMethod = EButtonClickMethod::DownAndUp;
	TouchMethod = EButtonTouchMethod::DownAndUp;

	IsFocusable = true;
}

void UButtonBase::ReleaseSlateResources(bool bReleaseChildren)
{
	Super::ReleaseSlateResources(bReleaseChildren);

	MyButton.Reset();
}

TSharedRef<SWidget> UButtonBase::RebuildWidget()
{
	Super::RebuildWidget();
	MyButton = SNew(SButton)
		.OnClicked(BIND_UOBJECT_DELEGATE(FOnClicked, SlateHandleClicked))
		.OnPressed(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandlePressed))
		.OnReleased(BIND_UOBJECT_DELEGATE(FSimpleDelegate, SlateHandleReleased))
		.OnHovered_UObject(this, &ThisClass::SlateHandleHovered)
		.OnUnhovered_UObject(this, &ThisClass::SlateHandleUnhovered)
		.ButtonStyle(&WidgetStyle)
		.ClickMethod(ClickMethod)
		.TouchMethod(TouchMethod)
		.IsFocusable(IsFocusable)
		;

	return MyButton.ToSharedRef();
}

void UButtonBase::SynchronizeProperties()
{
	Super::SynchronizeProperties();

	MyButton->SetColorAndOpacity(ColorAndOpacity);
	MyButton->SetBorderBackgroundColor(BackgroundColor);
}

void UButtonBase::SetStyle(const FButtonStyle& InStyle)
{
	WidgetStyle = InStyle;
	if (MyButton.IsValid())
	{
		MyButton->SetButtonStyle(&WidgetStyle);
	}
}

void UButtonBase::SetBackgroundColor(FLinearColor InBackgroundColor)
{
	BackgroundColor = InBackgroundColor;
	if (MyButton.IsValid())
	{
		MyButton->SetBorderBackgroundColor(InBackgroundColor);
	}
}

bool UButtonBase::IsPressed() const
{
	if (MyButton.IsValid())
	{
		return MyButton->IsPressed();
	}

	return false;
}

void UButtonBase::SetClickMethod(EButtonClickMethod::Type InClickMethod)
{
	ClickMethod = InClickMethod;
	if (MyButton.IsValid())
	{
		MyButton->SetClickMethod(ClickMethod);
	}
}

void UButtonBase::SetTouchMethod(EButtonTouchMethod::Type InTouchMethod)
{
	TouchMethod = InTouchMethod;
	if (MyButton.IsValid())
	{
		MyButton->SetTouchMethod(TouchMethod);
	}
}

void UButtonBase::PostLoad()
{
	Super::PostLoad();

	if (GetLinkerUE4Version() < VER_UE4_DEPRECATE_UMG_STYLE_ASSETS && Style_DEPRECATED != nullptr)
	{
		const FButtonStyle* StylePtr = Style_DEPRECATED->GetStyle<FButtonStyle>();
		if (StylePtr != nullptr)
		{
			WidgetStyle = *StylePtr;
		}

		Style_DEPRECATED = nullptr;
	}
}

FReply UButtonBase::SlateHandleClicked()
{
	OnClicked.Broadcast();

	return FReply::Handled();
}

void UButtonBase::SlateHandlePressed()
{
	OnPressed.Broadcast();
}

void UButtonBase::SlateHandleReleased()
{
	OnReleased.Broadcast();
}

void UButtonBase::SlateHandleHovered()
{
	OnHovered.Broadcast();
}

void UButtonBase::SlateHandleUnhovered()
{
	OnUnhovered.Broadcast();
}