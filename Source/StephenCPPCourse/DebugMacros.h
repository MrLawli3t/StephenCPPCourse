#pragma once
#include "DrawDebugHelpers.h"

#define DRAW_DEBUG_SPHERE(Center, Color) if (GetWorld()) DrawDebugSphere(GetWorld(), Center, 50.f, 16, Color, true);
#define DRAW_DEBUG_LINE_FORWARD(Start, Length) if (GetWorld()) DrawDebugLine(GetWorld(), Start, Start + GetActorForwardVector() * Length, FColor::Red, true);
#define DRAW_DEBUG_POINT(Location) if (GetWorld()) DrawDebugPoint(GetWorld(), Location, 50.f, FColor::Red, true);
#define DRAW_DEBUG_VECTOR(StartLocation, EndLocation) if (GetWorld())\
	{\
	DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, true);\
	DrawDebugPoint(GetWorld(), EndLocation, 50.f, FColor::Red, true);\
	}