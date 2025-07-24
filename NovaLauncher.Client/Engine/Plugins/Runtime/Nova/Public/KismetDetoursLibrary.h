#pragma once
#include "CoreMinimal.h"

enum class EDetourType
{
	Detour,
	Rel32Call,
	VFSwap
};

class UKismetDetoursLibrary
{
private:

	static void AddHookInternal(void** Target, void* Detour, EDetourType Type);
	static void RemoveHookInternal(void** Target, void* Detour, EDetourType Type);

public:

	static void AddHook(void** Target, void* Detour, EDetourType Type)
	{
		return AddHookInternal(Target, Detour, Type);
	}

	static void RemoveHook(void** Target, void* Detour, EDetourType Type)
	{
		return RemoveHookInternal(Target, Detour, Type);
	}

	template <typename TargetType, typename DetourType>
	static void AddHook(TargetType& Target, DetourType Detour, EDetourType Type = EDetourType::Detour)
	{
		return AddHookInternal((void**)&Target, (void**)Detour, Type);
	}

	template <typename TargetType, typename DetourType>
	static void RemoveHook(TargetType& Target, DetourType Detour, EDetourType Type = EDetourType::Detour)
	{
		return RemoveHookInternal((void**)&Target, (void**)Detour, Type);
	}

	static void BeginTransaction();
	static void CommitTransaction();
};

class UDetour
{
	void** Target;
	void* Detour;
	EDetourType Type;

public:

	template <typename TargetType, typename DetourType>
	void Init(TargetType& InTarget, DetourType InDetour, EDetourType InType = EDetourType::Detour)
	{
		Target = (void**)&InTarget;
		Detour = (void*)InDetour;
		Type = InType;
	}

	void Commit()
	{
		UKismetDetoursLibrary::BeginTransaction();
		UKismetDetoursLibrary::AddHook(Target, Detour, Type);
		UKismetDetoursLibrary::CommitTransaction();
	}

	void Remove()
	{
		UKismetDetoursLibrary::BeginTransaction();
		UKismetDetoursLibrary::RemoveHook(Target, Detour, Type);
		UKismetDetoursLibrary::CommitTransaction();
	}
};