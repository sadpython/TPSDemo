//© 2024 Andrei Vinnik

#include "TPSItemSettings.h"

#include "TPSItem.h"

TSubclassOf<UTPSItem> UTPSItemSettings::GetItemClass() const
{
	return ItemClass.LoadSynchronous();
}

FText UTPSItemSettings::GetItemName() const
{
	return Name;
}

UTexture2D* UTPSItemSettings::GetIcon() const
{
	return Icon.LoadSynchronous();
}

USkeletalMesh* UTPSItemSettings::GetMesh() const
{
	return Mesh.LoadSynchronous();
}
