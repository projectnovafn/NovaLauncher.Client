#pragma once

#include "Axis.h"
#include "HAL/PlatformMath.h"

struct FQuat;

/**
 * Implements a container for rotation information.
 *
 * All rotation values are stored in degrees.
 */
struct FRotator
{
public:
    /** Rotation around the right axis (around Y axis), Looking up and down (0=Straight Ahead, +Up, -Down) */
    float Pitch;

    /** Rotation around the up axis (around Z axis), Running in circles 0=East, +North, -South. */
    float Yaw;

    /** Rotation around the forward axis (around X axis), Tilting your head, 0=Straight, +Clockwise, -CCW. */
    float Roll;

public:

    /** A rotator of zero degrees on each axis. */
    static const FRotator ZeroRotator;

public:

#if ENABLE_NAN_DIAGNOSTIC
    FORCEINLINE void DiagnosticCheckNaN() const
    {
        if (ContainsNaN())
        {
            logOrEnsureNanError(TEXT("FRotator contains NaN: %s"), *ToString());
            *const_cast<FRotator*>(this) = ZeroRotator;
        }
    }

    FORCEINLINE void DiagnosticCheckNaN(const TCHAR* Message) const
    {
        if (ContainsNaN())
        {
            logOrEnsureNanError(TEXT("%s: FRotator contains NaN: %s"), Message, *ToString());
            *const_cast<FRotator*>(this) = ZeroRotator;
        }
    }
#else
    FORCEINLINE void DiagnosticCheckNaN() const {}
    FORCEINLINE void DiagnosticCheckNaN(const TCHAR* Message) const {}
#endif

    /**
     * Default constructor (no initialization).
     */
    FORCEINLINE FRotator() { }

    /**
     * Constructor
     *
     * @param InF Value to set all components to.
     */
    explicit FORCEINLINE FRotator(float InF);

    /**
     * Constructor.
     *
     * @param InPitch Pitch in degrees.
     * @param InYaw Yaw in degrees.
     * @param InRoll Roll in degrees.
     */
    FORCEINLINE FRotator(float InPitch, float InYaw, float InRoll);

public:

    // Binary arithmetic operators.

    /**
     * Get the result of adding a rotator to this.
     *
     * @param R The other rotator.
     * @return The result of adding a rotator to this.
     */
    FRotator operator+(const FRotator& R) const;

    /**
     * Get the result of subtracting a rotator from this.
     *
     * @param R The other rotator.
     * @return The result of subtracting a rotator from this.
     */
    FRotator operator-(const FRotator& R) const;

    /**
     * Get the result of scaling this rotator.
     *
     * @param Scale The scaling factor.
     * @return The result of scaling.
     */
    FRotator operator*(float Scale) const;

    /**
     * Multiply this rotator by a scaling factor.
     *
     * @param Scale The scaling factor.
     * @return Copy of the rotator after scaling.
     */
    FRotator operator*=(float Scale);

    // Binary comparison operators.

    /**
     * Checks whether two rotators are identical. This checks each component for exact equality.
     *
     * @param R The other rotator.
     * @return true if two rotators are identical, otherwise false.
     * @see Equals()
     */
    bool operator==(const FRotator& R) const;

    /**
     * Checks whether two rotators are different.
     *
     * @param V The other rotator.
     * @return true if two rotators are different, otherwise false.
     */
    bool operator!=(const FRotator& V) const;

    // Assignment operators.

    /**
     * Adds another rotator to this.
     *
     * @param R The other rotator.
     * @return Copy of rotator after addition.
     */
    FRotator operator+=(const FRotator& R);

    /**
     * Subtracts another rotator from this.
     *
     * @param R The other rotator.
     * @return Copy of rotator after subtraction.
     */
    FRotator operator-=(const FRotator& R);

public:

    // Functions.

    /**
     * Checks whether rotator is nearly zero within specified tolerance, when treated as an orientation.
     * This means that FRotator(0, 0, 360) is "zero", because it is the same final orientation as the zero rotator.
     *
     * @param Tolerance Error Tolerance.
     * @return true if rotator is nearly zero, within specified tolerance, otherwise false.
     */
    bool IsNearlyZero(float Tolerance = KINDA_SMALL_NUMBER) const;

    /**
     * Checks whether this has exactly zero rotation, when treated as an orientation.
     * This means that FRotator(0, 0, 360) is "zero", because it is the same final orientation as the zero rotator.
     *
     * @return true if this has exactly zero rotation, otherwise false.
     */
    bool IsZero() const;

    /**
     * Checks whether two rotators are equal within specified tolerance, when treated as an orientation.
     * This means that FRotator(0, 0, 360).Equals(FRotator(0,0,0)) is true, because they represent the same final orientation.
     *
     * @param R The other rotator.
     * @param Tolerance Error Tolerance.
     * @return true if two rotators are equal, within specified tolerance, otherwise false.
     */
    bool Equals(const FRotator& R, float Tolerance = KINDA_SMALL_NUMBER) const;

    /**
     * Adds to each component of the rotator.
     *
     * @param DeltaPitch Change in pitch. (+/-)
     * @param DeltaYaw Change in yaw. (+/-)
     * @param DeltaRoll Change in roll. (+/-)
     * @return Copy of rotator after addition.
     */
    FRotator Add(float DeltaPitch, float DeltaYaw, float DeltaRoll);

    /**
     * Get the rotation, snapped to specified degree segments.
     *
     * @param RotGrid A Rotator specifying how to snap each component.
     * @return Snapped version of rotation.
     */
    FRotator GridSnap(const FRotator& RotGrid) const;

    /**
     * Convert a rotation into a unit vector facing in its direction.
     *
     * @return Rotation as a unit direction vector.
     */
    struct FVector Vector() const;

    /**
     * Get Rotation as a quaternion.
     *
     * @return Rotation as a quaternion.
     */
    struct FQuat Quaternion();

    /**
     * Gets the rotation values so they fall within the range [0,360]
     *
     * @return Clamped version of rotator.
     */
    FRotator Clamp() const;

    /**
     * Create a copy of this rotator and normalize, removes all winding and creates the "shortest route" rotation.
     *
     * @return Normalized copy of this rotator
     */
    FRotator GetNormalized() const;

    /**
     * Create a copy of this rotator and denormalize, clamping each axis to 0 - 360.
     *
     * @return Denormalized copy of this rotator
     */
    FRotator GetDenormalized() const;

    /** Get a specific component of the vector, given a specific axis by enum */
    float GetComponentForAxis(EAxis::Type Axis) const;

    /** Set a specified componet of the vector, given a specific axis by enum */
    void SetComponentForAxis(EAxis::Type Axis, float Component);

    /**
     * In-place normalize, removes all winding and creates the "shortest route" rotation.
     */
    void Normalize();

    /**
    * Return the manhattan distance in degrees between this Rotator and the passed in one.
    * @param Rotator[In] the Rotator we are comparing with.
    * @return Distance(Manhattan) between the two rotators.
    */
    float GetManhattanDistance(const FRotator& Rotator) const;

    /**
    * Return a Rotator that has the same rotation but has different degree values for Yaw, Pitch, and Roll.
    * This rotator should be within -180,180 range,
    * @return A Rotator with the same rotation but different degrees.
    */
    FRotator GetEquivalentRotator() const;

    /**
    * Modify if necessary the passed in rotator to be the closest rotator to it based upon it's equivalent.
    * This Rotator should be within (-180, 180], usually just constructed from a Matrix or a Quaternion.
    *
    * @param MakeClosest[In/Out] the Rotator we want to make closest to us. Should be between
    * (-180, 180]. This Rotator may change if we need to use different degree values to make it closer.
    */
    void SetClosestToMe(FRotator& MakeClosest) const;

    /**
     * Utility to check if there are any non-finite values (NaN or Inf) in this Rotator.
     *
     * @return true if there are any non-finite values in this Rotator, otherwise false.
     */
    bool ContainsNaN() const;

public:

    /**
     * Clamps an angle to the range of [0, 360).
     *
     * @param Angle The angle to clamp.
     * @return The clamped angle.
     */
    static float ClampAxis(float Angle);

    /**
     * Clamps an angle to the range of (-180, 180].
     *
     * @param Angle The Angle to clamp.
     * @return The clamped angle.
     */
    static float NormalizeAxis(float Angle);

    /**
     * Compresses a floating point angle into a byte.
     *
     * @param Angle The angle to compress.
     * @return The angle as a byte.
     */
    static uint8 CompressAxisToByte(float Angle);

    /**
     * Decompress a word into a floating point angle.
     *
     * @param Angle The word angle.
     * @return The decompressed angle.
     */
    static float DecompressAxisFromByte(uint8 Angle);

    /**
     * Compress a floating point angle into a word.
     *
     * @param Angle The angle to compress.
     * @return The decompressed angle.
     */
    static uint16 CompressAxisToShort(float Angle);

    /**
     * Decompress a short into a floating point angle.
     *
     * @param Angle The word angle.
     * @return The decompressed angle.
     */
    static float DecompressAxisFromShort(uint16 Angle);

public:

};


/* FRotator inline functions
 *****************************************************************************/

 /**
  * Scale a rotator and return.
  *
  * @param Scale scale to apply to R.
  * @param R rotator to be scaled.
  * @return Scaled rotator.
  */
FORCEINLINE FRotator operator*(float Scale, const FRotator& R)
{
    return R.operator*(Scale);
}


FORCEINLINE FRotator::FRotator(float InF)
    : Pitch(InF), Yaw(InF), Roll(InF)
{
    DiagnosticCheckNaN();
}


FORCEINLINE FRotator::FRotator(float InPitch, float InYaw, float InRoll)
    : Pitch(InPitch), Yaw(InYaw), Roll(InRoll)
{
    DiagnosticCheckNaN();
}

FORCEINLINE FRotator FRotator::operator+(const FRotator& R) const
{
    return FRotator(Pitch + R.Pitch, Yaw + R.Yaw, Roll + R.Roll);
}


FORCEINLINE FRotator FRotator::operator-(const FRotator& R) const
{
    return FRotator(Pitch - R.Pitch, Yaw - R.Yaw, Roll - R.Roll);
}


FORCEINLINE FRotator FRotator::operator*(float Scale) const
{
    return FRotator(Pitch * Scale, Yaw * Scale, Roll * Scale);
}


FORCEINLINE FRotator FRotator::operator*= (float Scale)
{
    Pitch = Pitch * Scale; Yaw = Yaw * Scale; Roll = Roll * Scale;
    DiagnosticCheckNaN();
    return *this;
}


FORCEINLINE bool FRotator::operator==(const FRotator& R) const
{
    return Pitch == R.Pitch && Yaw == R.Yaw && Roll == R.Roll;
}


FORCEINLINE bool FRotator::operator!=(const FRotator& V) const
{
    return Pitch != V.Pitch || Yaw != V.Yaw || Roll != V.Roll;
}


FORCEINLINE FRotator FRotator::operator+=(const FRotator& R)
{
    Pitch += R.Pitch; Yaw += R.Yaw; Roll += R.Roll;
    DiagnosticCheckNaN();
    return *this;
}


FORCEINLINE FRotator FRotator::operator-=(const FRotator& R)
{
    Pitch -= R.Pitch; Yaw -= R.Yaw; Roll -= R.Roll;
    DiagnosticCheckNaN();
    return *this;
}


FORCEINLINE bool FRotator::IsNearlyZero(float Tolerance) const
{
#if PLATFORM_ENABLE_VECTORINTRINSICS
    const VectorRegister RegA = VectorLoadFloat3_W0(this);
    const VectorRegister Norm = VectorNormalizeRotator(RegA);
    const VectorRegister AbsNorm = VectorAbs(Norm);
    return !VectorAnyGreaterThan(AbsNorm, VectorLoadFloat1(&Tolerance));
#else
    return
        FMath::Abs(NormalizeAxis(Pitch)) <= Tolerance
        && FMath::Abs(NormalizeAxis(Yaw)) <= Tolerance
        && FMath::Abs(NormalizeAxis(Roll)) <= Tolerance;
#endif
}


FORCEINLINE bool FRotator::IsZero() const
{
    return (ClampAxis(Pitch) == 0.f) && (ClampAxis(Yaw) == 0.f) && (ClampAxis(Roll) == 0.f);
}


FORCEINLINE bool FRotator::Equals(const FRotator& R, float Tolerance) const
{
#if PLATFORM_ENABLE_VECTORINTRINSICS
    const VectorRegister RegA = VectorLoadFloat3_W0(this);
    const VectorRegister RegB = VectorLoadFloat3_W0(&R);
    const VectorRegister NormDelta = VectorNormalizeRotator(VectorSubtract(RegA, RegB));
    const VectorRegister AbsNormDelta = VectorAbs(NormDelta);
    return !VectorAnyGreaterThan(AbsNormDelta, VectorLoadFloat1(&Tolerance));
#else
    return (FMath::Abs(NormalizeAxis(Pitch - R.Pitch)) <= Tolerance)
        && (FMath::Abs(NormalizeAxis(Yaw - R.Yaw)) <= Tolerance)
        && (FMath::Abs(NormalizeAxis(Roll - R.Roll)) <= Tolerance);
#endif
}


FORCEINLINE FRotator FRotator::Add(float DeltaPitch, float DeltaYaw, float DeltaRoll)
{
    Yaw += DeltaYaw;
    Pitch += DeltaPitch;
    Roll += DeltaRoll;
    DiagnosticCheckNaN();
    return *this;
}


FORCEINLINE FRotator FRotator::GridSnap(const FRotator& RotGrid) const
{
    return FRotator
    (
        FMath::GridSnap(Pitch, RotGrid.Pitch),
        FMath::GridSnap(Yaw, RotGrid.Yaw),
        FMath::GridSnap(Roll, RotGrid.Roll)
    );
}


FORCEINLINE FRotator FRotator::Clamp() const
{
    return FRotator(ClampAxis(Pitch), ClampAxis(Yaw), ClampAxis(Roll));
}


FORCEINLINE float FRotator::ClampAxis(float Angle)
{
    // returns Angle in the range (-360,360)
    Angle = FMath::Fmod(Angle, 360.f);

    if (Angle < 0.f)
    {
        // shift to [0,360) range
        Angle += 360.f;
    }

    return Angle;
}


FORCEINLINE float FRotator::NormalizeAxis(float Angle)
{
    // returns Angle in the range [0,360)
    Angle = ClampAxis(Angle);

    if (Angle > 180.f)
    {
        // shift to (-180,180]
        Angle -= 360.f;
    }

    return Angle;
}


FORCEINLINE uint8 FRotator::CompressAxisToByte(float Angle)
{
    // map [0->360) to [0->256) and mask off any winding
    return FMath::RoundToInt(Angle * 256.f / 360.f) & 0xFF;
}


FORCEINLINE float FRotator::DecompressAxisFromByte(uint8 Angle)
{
    // map [0->256) to [0->360)
    return (Angle * 360.f / 256.f);
}


FORCEINLINE uint16 FRotator::CompressAxisToShort(float Angle)
{
    // map [0->360) to [0->65536) and mask off any winding
    return FMath::RoundToInt(Angle * 65536.f / 360.f) & 0xFFFF;
}


FORCEINLINE float FRotator::DecompressAxisFromShort(uint16 Angle)
{
    // map [0->65536) to [0->360)
    return (Angle * 360.f / 65536.f);
}


FORCEINLINE FRotator FRotator::GetNormalized() const
{
    FRotator Rot = *this;
    Rot.Normalize();
    return Rot;
}


FORCEINLINE FRotator FRotator::GetDenormalized() const
{
    FRotator Rot = *this;
    Rot.Pitch = ClampAxis(Rot.Pitch);
    Rot.Yaw = ClampAxis(Rot.Yaw);
    Rot.Roll = ClampAxis(Rot.Roll);
    return Rot;
}


FORCEINLINE void FRotator::Normalize()
{
#if PLATFORM_ENABLE_VECTORINTRINSICS
    VectorRegister VRotator = VectorLoadFloat3_W0(this);
    VRotator = VectorNormalizeRotator(VRotator);
    VectorStoreFloat3(VRotator, this);
#else
    Pitch = NormalizeAxis(Pitch);
    Yaw = NormalizeAxis(Yaw);
    Roll = NormalizeAxis(Roll);
#endif
    DiagnosticCheckNaN();
}

FORCEINLINE float FRotator::GetComponentForAxis(EAxis::Type Axis) const
{
    switch (Axis)
    {
    case EAxis::X:
        return Roll;
    case EAxis::Y:
        return Pitch;
    case EAxis::Z:
        return Yaw;
    default:
        return 0.f;
    }
}

FORCEINLINE void FRotator::SetComponentForAxis(EAxis::Type Axis, float Component)
{
    switch (Axis)
    {
    case EAxis::X:
        Roll = Component;
        break;
    case EAxis::Y:
        Pitch = Component;
        break;
    case EAxis::Z:
        Yaw = Component;
        break;
    }
}


FORCEINLINE bool FRotator::ContainsNaN() const
{
    return (!FMath::IsFinite(Pitch) ||
        !FMath::IsFinite(Yaw) ||
        !FMath::IsFinite(Roll));
}

FORCEINLINE float FRotator::GetManhattanDistance(const FRotator& Rotator) const
{
    return FMath::Abs<float>(Yaw - Rotator.Yaw) + FMath::Abs<float>(Pitch - Rotator.Pitch) + FMath::Abs<float>(Roll - Rotator.Roll);
}

FORCEINLINE FRotator FRotator::GetEquivalentRotator() const
{
    return FRotator(180.0f - Pitch, Yaw + 180.0f, Roll + 180.0f);
}

FORCEINLINE void FRotator::SetClosestToMe(FRotator& MakeClosest) const
{
    FRotator OtherChoice = MakeClosest.GetEquivalentRotator();
    float FirstDiff = GetManhattanDistance(MakeClosest);
    float SecondDiff = GetManhattanDistance(OtherChoice);
    if (SecondDiff < FirstDiff)
        MakeClosest = OtherChoice;
}