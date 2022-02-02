
// local includes
#include "appgui.h"
#include "appguiservice.h"

// nap includes
#include <entity.h>
#include <imgui/imgui.h>

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::AppGUI)
        RTTI_PROPERTY("Widget Groups", &nap::AppGUI::mWidgetGroups, nap::rtti::EPropertyMetaData::Required)
        RTTI_PROPERTY("Menu Name", &nap::AppGUI::mMenuName, nap::rtti::EPropertyMetaData::Default)
RTTI_END_CLASS

RTTI_BEGIN_CLASS(nap::AppGUICache)
        RTTI_PROPERTY("OpenWidgets", &nap::AppGUICache::mOpenWidgets, nap::rtti::EPropertyMetaData::Embedded)
RTTI_END_CLASS


namespace nap
{
    AppGUI::AppGUI(AppGUIService& appGUIService) : mAppGUIService(appGUIService){}

    bool AppGUI::init(utility::ErrorState &errorState)
    {
        std::vector<std::string> group_ids;
        for(auto& group : mWidgetGroups)
        {
            if(!extractWindowsRecursive(group.get(), group_ids, errorState))
                return false;
        }

        mAppGUIService.registerAppGUI(this);

        return true;
    }


    bool AppGUI::extractWindowsRecursive(AppGUIMenuItems* items, std::vector<std::string>& group_ids, utility::ErrorState &errorState)
    {
        for(auto& item : items->mItems)
        {
            if(item.get()->get_type().is_derived_from<AppGuiWindow>())
            {
                auto* window_widget = static_cast<AppGuiWindow*>(item.get());
                auto open_windows_it = mOpenWindows.find(window_widget);
                if(!errorState.check(open_windows_it==mOpenWindows.end(),
                                     "duplicate window %s assigned in AppGUI, check AppGUI configuration",
                                     window_widget->mID.c_str()))
                    return false;

                mOpenWindows.emplace(window_widget, false);
            }else if(item.get()->get_type().is_derived_from<AppGUIMenuItems>())
            {
                auto* group_item = static_cast<AppGUIMenuItems*>(item.get());

                auto group_ids_it = std::find(group_ids.begin(), group_ids.end(), group_item->mID);
                if(!errorState.check(group_ids_it==group_ids.end(),
                                     "recursive group %s found, check AppGUI configuration",
                                     group_item->mID.c_str()))
                    return false;

                group_ids.emplace_back(group_item->mID);
                if(!extractWindowsRecursive(group_item, group_ids, errorState))
                    return false;
            }
        }

        return true;
    }


    void AppGUI::onDestroy()
    {
        mAppGUIService.unregisterAppGUI(this);
    }


    void AppGUI::draw(double deltaTime)
    {
        ImGui::PushID(mID.c_str());
        ImGui::BeginMainMenuBar();
        for(auto& group : mWidgetGroups)
        {
            handleWidgetGroup(group.get());
        }
        ImGui::EndMainMenuBar();

        for(auto& pair : mOpenWindows)
        {
            if(pair.second)
                pair.first->draw(deltaTime);
        }
        ImGui::PopID();
    }


    void AppGUI::handleWidgetGroup(AppGUIMenuItems* mWidgetGroup)
    {
        ImGui::PushID(mWidgetGroup->mID.c_str());
        if (ImGui::BeginMenu(mWidgetGroup->mName.c_str()))
        {
            for(auto& item : mWidgetGroup->mItems)
            {
                if(item.get()->get_type().is_derived_from<AppGuiWindow>())
                {
                    handleWindowWidget(static_cast<AppGuiWindow*>(item.get()));
                }else if(item.get()->get_type().is_derived_from<AppGUIMenuItems>())
                {
                    handleWidgetGroup(static_cast<AppGUIMenuItems*>(item.get()));
                }
            }
            ImGui::EndMenu();
        }
        ImGui::PopID();
    }


    void AppGUI::handleWindowWidget(AppGuiWindow* mWidget)
    {
        ImGui::MenuItem(mWidget->mName.c_str(), nullptr, &mOpenWindows[mWidget]);
    }


    std::vector<std::string> AppGUI::getOpenWindowIDs()
    {
        std::vector<std::string> ids;
        for(auto& pair : mOpenWindows)
        {
            if(pair.second)
                ids.emplace_back(pair.first->mID);
        }
        return ids;
    }


    AppGUICache::AppGUICache(AppGUI &gui)
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
