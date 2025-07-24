#include "Math/Rotator.h"
#include "Math/Quat.h"

FQuat FRotator::Quaternion()
{
    FQuat Quat;

    auto DEG_TO_RAD = PI / 180;
    auto DIVIDE_BY_2 = DEG_TO_RAD / 2;

    auto SP = FMath::Sin(Pitch * DIVIDE_BY_2);
    auto CP = FMath::Cos(Pitch * DIVIDE_BY_2);

    auto SY = FMath::Sin(Yaw * DIVIDE_BY_2);
    auto CY = FMath::Cos(Yaw * DIVIDE_BY_2);

    auto SR = FMath::Sin(Roll * DIVIDE_BY_2);
    auto CR = FMath::Cos(Roll * DIVIDE_BY_2);

    Quat.X = CR * SP * SY - SR * CP * CY;
    Quat.Y = -CR * SP * CY - SR * CP * SY;
    Quat.Z = CR * CP * SY - SR * SP * CY;
    Quat.W = CR * CP * CY + SR * SP * SY;

    return Quat;
}