#include "Item/CItem_Tool.h"

ACItem_Tool::ACItem_Tool()
{
    SetInteractable();
    SetUsable();
}

bool ACItem_Tool::UseItem()
{
    return false;
}
