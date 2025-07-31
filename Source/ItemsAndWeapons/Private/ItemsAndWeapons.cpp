#include "ItemsAndWeapons.h"

#define LOCTEXT_NAMESPACE "FItemsAndWeaponsModule"

void FItemsAndWeaponsModule::StartupModule()
{
    UE_LOG(LogTemp, Warning, TEXT("WE'RE IN BITCHES"));
}

void FItemsAndWeaponsModule::ShutdownModule()
{
    
}

#undef LOCTEXT_NAMESPACE
    
IMPLEMENT_MODULE(FItemsAndWeaponsModule, ItemsAndWeapons)