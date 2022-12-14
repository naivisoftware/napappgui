
// local includes
#include "appguiwindowmenucomponent.h"
#include "appguiservice.h"

// nap includes
#include <entity.h>
#include <imgui/imgui.h>

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::AppGUIWindowMenuComponent)
    RTTI_PROPERTY("Menu Items", &nap::AppGUIWindowMenuComponent::mWindowGroups, nap::rtti::EPropertyMetaData::Default)
RTTI_END_CLASS

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::AppGUIWindowMenuComponentInstance)
    RTTI_CONSTRUCTOR(nap::EntityInstance&, nap::Component&)
RTTI_END_CLASS

RTTI_BEGIN_CLASS(nap::AppGUICache)
    RTTI_PROPERTY("OpenWidgets", &nap::AppGUICache::mOpenWidgets, nap::rtti::EPropertyMetaData::Embedded)
RTTI_END_CLASS


namespace nap
{
    //////////////////////////////////////////////////////////////////////////
    // AppGUIWindowMenuComponent
    //////////////////////////////////////////////////////////////////////////

    AppGUIWindowMenuComponent::AppGUIWindowMenuComponent(AppGUIService& appGUIService) : mAppGUIService(appGUIService){}

    //////////////////////////////////////////////////////////////////////////
    // AppGUIWindowMenuComponentInstance
    //////////////////////////////////////////////////////////////////////////

    bool AppGUIWindowMenuComponentInstance::init(utility::ErrorState &errorState)
    {
        // get resource
        auto* resource = getComponent<AppGUIWindowMenuComponent>();

        // set app gui service pointer
        mAppGUIService = &resource->mAppGUIService;

        std::vector<std::string> group_ids;
        for(auto& menu_item : resource->mWindowGroups)
        {
            if(!constructMenuRecursive(menu_item.get(), group_ids, errorState))
                return false;
        }

        mAppGUIService->registerAppGUIWindowMenuComponentInstance(this);

        return true;
    }


    bool AppGUIWindowMenuComponentInstance::constructMenuRecursive(AppGUIWindowGroup* group, std::vector<std::string>& group_ids, utility::ErrorState &errorState)
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


    void AppGUIWindowMenuComponentInstance::onDestroy()
    {
        mAppGUIService->unregisterAppGUIWindowMenuComponentInstance(this);
    }


    void AppGUIWindowMenuComponentInstance::draw(double deltaTime)
    {
        auto* resource = getComponent<AppGUIWindowMenuComponent>();

        ImGui::PushID(mID.c_str());
        ImGui::BeginMainMenuBar();
        for(auto& menu_item : resource->mWindowGroups)
        {
            handleWindowGroup(menu_item.get());
        }
        ImGui::EndMainMenuBar();

        for(auto& pair : mOpenWindows)
        {
            if(pair.second)
            {
                pair.first->draw(deltaTime);
                pair.second = !pair.first->getIsClosed();
            }
        }
        ImGui::PopID();
    }


    void AppGUIWindowMenuComponentInstance::handleWindowGroup(AppGUIWindowGroup* group)
    {
        ImGui::PushID(group->mID.c_str());
        if (ImGui::BeginMenu(group->mID.c_str()))
        {
            for(auto& member : group->mMembers)
            {
                if(member->get_type().is_derived_from<AppGUIWindow>())
                {
                    handleWindow(static_cast<AppGUIWindow *>(member.get()));
                }
            }
            for(auto& child : group->mChildren)
            {
                handleWindowGroup(child.get());
            }
            ImGui::EndMenu();
        }
        ImGui::PopID();
    }


    void AppGUIWindowMenuComponentInstance::handleWindow(AppGUIWindow* mWidget)
    {
        ImGui::MenuItem(mWidget->mTitle.c_str(), nullptr, &mOpenWindows[mWidget]);
    }


    std::vector<std::string> AppGUIWindowMenuComponentInstance::getOpenWindowIDs()
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

    AppGUICache::AppGUICache(AppGUIWindowMenuComponentInstance &gui)
    {
        mID = gui.mID;
        mOpenWidgets = gui.getOpenWindowIDs();
    }


    bool AppGUICache::isOpen(const std::string& windowID)
    {
        auto it = std::find(mOpenWidgets.begin(), mOpenWidgets.end(), windowID);
        return it != mOpenWidgets.end();
    }
}
