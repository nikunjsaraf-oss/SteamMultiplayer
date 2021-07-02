// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuSystem/ServerRow.h"

#include "Components/Button.h"
#include "MenuSystem/MainMenu.h"

void UServerRow::Setup(UMainMenu* InParent, uint32 InIndex)
{
	Parent = InParent;
	Index = InIndex;
	RowButton->OnClicked.AddDynamic(this, &UServerRow::OnClickHandler);	
}

void UServerRow::OnClickHandler()
{
	Parent->SelectIndex(Index);
}
