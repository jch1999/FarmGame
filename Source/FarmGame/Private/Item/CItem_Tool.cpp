// Fill out your copyright notice in the Description page of Project Settings.


#include "Item/MyCItem_Tool.h"
#include "CItem_Tool.h"

bool AMyCItem_Tool::UseItem()
{
    return false;
}

FName AMyCItem_Tool::GetInteractName()
{
    return FName();
}
