# MPointSelectorQuery

A configurable Unreal Engine plugin for selecting optimal look-at points in the world based on scoring factors and filters.

## Overview

The MPointSelectorQuery plugin provides a flexible system to query and select points of interest (POIs) that an AI or camera might want to look at. It uses a combination of weighted scoring factors and filters to determine the best point from a reference location and direction.

Key features:
- **Configurable Factors**: Distance, angle, and custom importance scoring
- **Filters**: Line-of-sight checks and extensible filter system
- **Blueprint Integration**: Easy to use in Blueprints with dedicated nodes
- **Extensible**: Create custom factors and filters via inheritance

## Use Cases

- **Look-at Systems**: AI characters or cameras can dynamically select interesting points to focus on, enhancing realism in NPC behavior or cinematic sequences.
- **Interactable Selection**: Games can highlight or prioritize objects that players can interact with, based on proximity, visibility, and importance.
- **Target Selection**: In combat systems, select the best target for attacks (e.g., sword swings) considering distance, angle, and line-of-sight.

## Usage

### 1. Create a Settings Asset
Create a `MPointSelectorQuerySettingsAsset` in your Content folder. Configure:
- **Selector Factors**: Add factors like Distance, Angle, or POI Importance with weights
- **Filters**: Add filters like Line of Sight
- **Limits**: Set max distance and angle

### 2. Add Query Points to Actors
Attach a `MSelectorQueryPoint_GenericSphere` component to actors you want to be selectable as look-at points. Set their importance value.

### 3. Perform a Query
Use the `Point Selector Query` Blueprint node:
- Input: Reference location, direction, settings asset
- Output: The best look-at point interface

### Example Blueprint Setup
```
[Reference Location] --> [Point Selector Query] --> [Look At Point]
[Reference Direction]     [Settings Asset]        [Get Look At Location]
```

This selects the most suitable point based on your configured factors and filters.

### Code Example (C++)
```cpp
#include "MPointSelectorQueryLibrary.h"
#include "MPointSelectorQuerySettingsAsset.h"

// In your actor or component
void AMyCharacter::SelectLookAtPoint()
{
    UMPointSelectorQuerySettingsAsset* Settings = MySettingsAsset; // Your configured asset
    FVector Location = GetActorLocation();
    FVector Direction = GetActorForwardVector();
    
    TScriptInterface<IMSelectorQueryPointInterface> SelectedPoint;
    FCollisionQueryParams QueryParams;
    
    bool bSuccess = UMPointSelectorQueryLibrary::PointSelectorQuery(
        this, Location, Direction, Settings, QueryParams, SelectedPoint);
    
    if (bSuccess && SelectedPoint.GetObject())
    {
        FVector LookAtLocation = SelectedPoint->GetLookAtLocation();
        // Use the selected point, e.g., set camera look-at
    }
}
```
