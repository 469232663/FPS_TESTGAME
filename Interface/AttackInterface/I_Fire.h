// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "I_Fire.generated.h"
/**
 * 
 */
UINTERFACE(Blueprintable)
class UI_Fire : public UInterface
{
	GENERATED_BODY()
	
};

class II_Fire
{
	GENERATED_BODY() 

public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "FireInterface")	//BlueprintNativeEventҪ��Ĭ��C++��ʵ��
		bool Fire_Int(bool isFire,float Time);

	/*UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "FireInterface")  //BlueprintImplementableEventC++������ ��ͼʵ��
		void TEST_Inter(bool is);*/

};