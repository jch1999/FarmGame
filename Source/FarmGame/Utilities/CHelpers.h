#pragma once

#include "CoreMinimal.h"

#define CheckNull(p) {if(p==nullptr)return;}
#define CheckResult(p, result) {if(p==nullptr)return result;}

#define CheckTrue(p) {if(p==true) return; }
#define CheckTrueResult(p, result) {if(p==true) return result; }

#define CheckFalse(p) {if(p==false) return; }
#define CheckFalseResult(p, result) {if(p==false) return result; }

class CHelpers
{
public:
	template<typename T>
	static void GetAsset(T** OutAsset,FString InPath)
	{
		ConstructorHelpers::FObjectFinder<T> Asset(*InPath);
		/*check(Asset.Succeeded());
		varifyF(Asset.Succeeded(),"Asset Failed");
		ensureMsgf(Asset.Succeeded());*/
		ensureMsgf(Asset.Succeeded(), TEXT("Asset not found"));

		*OutAsset = Asset.Object;
	}

	template<typename T>
	static void GetAssetDynamic(T** OutAsset, FString InPath)
	{
		T* Asset = Cast<T>(StaticLoadObject(T::StaticClass(), nullptr, *InPath));
		ensureMsgf(Asset, TEXT("Asset not found"));

		*OutAsset = Asset;
	}

	template<typename T>
	static void GetClass(TSubclassOf<T>* OutClass, FString InPath)
	{

		ConstructorHelpers::FClassFinder<T> AssetClass(*InPath);
		ensureMsgf(AssetClass.Succeeded(), TEXT("Class not found"));

		*OutClass = AssetClass.Class;
	}

	template<typename T>
	static void CreateSceneComponent(AActor* InObject, T** OutComp, FName InName, USceneComponent* InParent=nullptr)
	{
		(*OutComp) = InObject->CreateDefaultSubobject<T>(InName);
		if (InParent != nullptr)
		{
			(*OutComp)->SetupAttachment(InParent);
			return;
		}
		//else
		{
			//InObject->RootComponent = (*OutComp);
			InObject->SetRootComponent(*OutComp);
		}
	}

	template<typename T>
	static void CreateActorComponent(AActor* InObject, T** OutComp, FName InName)
	{
		(*OutComp) = InObject->CreateDefaultSubobject<T>(InName);
	}

	template<typename T>
	static T* GetComponent(AActor* InActor)
	{
		return Cast<T>(InActor->GetComponentByClass(T::StaticClass()));
	}
};