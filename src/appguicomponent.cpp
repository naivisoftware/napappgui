
// local includes
#include "appguicomponent.h"
#include "appguiservice.h"

// nap includes
#include <entity.h>
#include <imgui/imgui.h>
#include <nap/logger.h>

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::AppGUIComponent)
RTTI_END_CLASS

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::AppGUIComponentInstance)
RTTI_END_CLASS

namespace nap
{
    //////////////////////////////////////////////////////////////////////////
    // AppGUIComponent
    //////////////////////////////////////////////////////////////////////////

    AppGUIComponent::AppGUIComponent(AppGUIService& appGUIService) : mAppGUIService(appGUIService){}

    //////////////////////////////////////////////////////////////////////////
    // AppGUIComponentInstance
    //////////////////////////////////////////////////////////////////////////

    AppGUIComponentInstance::AppGUIComponentInstance(EntityInstance& entity, Component& resource) :
        ComponentInstance(entity, resource){}


    bool AppGUIComponentInstance::init(utility::ErrorState &errorState)
    {
        auto* resource = getComponent<AppGUIComponent>();
        mAppGUIService = &resource->mAppGUIService;
        mAppGUIService->registerAppGUIComponentInstance(this);
        return initInternal(errorState);
    }


    void AppGUIComponentInstance::onDestroy()
    {
        mAppGUIService->unregisterAppGUIComponentInstance(this);
        onDestroyInternal();
    }
}
