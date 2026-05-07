// Copyright (c) Miknios. All rights reserved.

#include "MPointSelectorQueryModule.h"

#define LOCTEXT_NAMESPACE "FMPointSelectorQueryModule"

DEFINE_LOG_CATEGORY(LogPointSelectorQuery);

void FMPointSelectorQueryModule::StartupModule()
{
}

void FMPointSelectorQueryModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FMPointSelectorQueryModule, MPointSelectorQuery)

