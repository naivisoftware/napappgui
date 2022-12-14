
// local includes
#include "appgui.h"
#include "appguiservice.h"

// nap includes
#include <entity.h>
#include <imgui/imgui.h>

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::AppGUIComponent)
    RTTI_PROPERTY("Menu Items", &nap::AppGUIComponent::mMenuItems, nap::rtti::EPropertyMetaData::Default)
RTTI_END_CLASS

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::AppGUIComponentInstance)
    RTTI_CONSTRUCTOR(nap::EntityInstance&, nap::Component&)
RTTI_END_CLASS

RTTI_BEGIN_CLASS(nap::AppGUICache)
    RTTI_PROPERTY("OpenWidgets", &nap::AppGUICache::mOpenWidgets, nap::rtti::EPropertyMetaData::Embedded)
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

    bool AppGUIComponentInstance::init(utility::ErrorState &errorState)
    {
        // get resource
        auto* resource = getComponent<AppGUIComponent>();

        // set app gui service pointer
        mAppGUIService = &resource->mAppGUIService;

        std::vector<std::string> group_ids;
        if(!constructMenuRecursive(resource->mMenuItems.get(), group_ids, errorState))
            return false;

        mAppGUIService->registerAppGUIComponentInstance(this);

        return true;
    }


    bool AppGUIComponentInstance::constructMenuRecursive(AppGUIMenuItemGroup* group, std::vector<std::string>& group_ids, utility::ErrorState &errorState)
    {
        for(auto& member : group->mMembers)
        {
            if(member.get()->get_type().is_derived_from<AppGUIWindow>())
            {
                // set appgui service
                member->setAppGUIService(*mAppGUIService);

                auto *window_widget = static_cast<AppGUIWindow *>(member.get());
                auto open_windows_it = mOpenWindows.find(window_widget);
                if (!errorState.check(open_windows_it == mOpenWindows.end(),
                                      "duplicate window %s assigned in AppGUI, check AppGUI configuration",
                                      window_widget->mID.c_str()))
                    return false;

                mOpenWindows.emplace(window_widget, false);
            }
        }
        for(auto& child : group->mChildren)
        {
            if(!constructMenuRecursive(child.get(), group_ids, errorState))
                return false;
        }

        return true;
    }


    void AppGUIComponentInstance::onDestroy()
    {
        mAppGUIService->unregisterAppGUIComponentInstance(this);
    }


    void AppGUIComponentInstance::draw(double deltaTime)
    {
        ImGui::PushID(mID.c_str());
        ImGui::BeginMainMenuBar();
        auto* resource = getComponent<AppGUIComponent>();
        handleWidgetGroup(resource->mMenuItems.get());
        ImGui::EndMainMenuBar();

        for(auto& pair : mOpenWindows)
        {
            if(pair.second)
                pair.first->draw(deltaTime);
        }
        ImGui::PopID();
    }


    void AppGUIComponentInstance::handleWidgetGroup(AppGUIMenuItemGroup* group)
    {
        ImGui::PushID(group->mID.c_str());
        if (ImGui::BeginMenu(group->mID.c_str()))
        {
            for(auto& member : group->mMembers)
            {
                if(member->get_type().is_derived_from<AppGUIWindow>())
                {
                    handleWindowWidget(static_cast<AppGUIWindow *>(member.get()));
                }
            }
            for(auto& child : group->mChildren)
            {
                handleWidgetGroup(child.get());
            }
            ImGui::EndMenu();
        }
        ImGui::PopID();
    }


    void AppGUIComponentInstance::handleWindowWidget(AppGUIWindow* mWidget)
    {
        ImGui::MenuItem(mWidget->mName.c_str(), nullptr, &mOpenWindows[mWidget]);
    }


    std::vector<std::string> AppGUIComponentInstance::getOpenWindowIDs()
    {
        std::vector<std::string> ids;
        for(auto& pair : mOpenWindows)
        {
            if(pair.second)
                ids.emplace_back(pair.first->mID);
        }
        return ids;
    }

    //////////////////////////////////////////////////////////////////////////
    // AppGUICache
    //////////////////////////////////////////////////////////////////////////

    AppGUICache::AppGUICache(AppGUIComponentInstance &gui)
    {
        mID = gui.mID;

        mOpenWidgets = gui.getOpenWindowIDs();
    }


    bool AppGUICache::isOpen(std::string widgetId)
    {
        auto it = std::find(mOpenWidgets.begin(), mOpenWidgets.end(), widgetId);
        return it != mOpenWidgets.end();
    }
}
