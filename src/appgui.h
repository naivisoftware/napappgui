#pragma once

// Internal Includes
#include "appguiwidget.h"
#include "appguiitem.h"

// External Includes
#include <component.h>
#include <componentptr.h>
#include <nap/resourceptr.h>
#include <rtti/factory.h>

namespace nap
{
    //////////////////////////////////////////////////////////////////////////
    class AppGUIService;

    /**
     * The EgoGUI draws all widgets inside an EgoGUIWidgetGroup.
     * Draw needs to be called manually.
     */
    class NAPAPI AppGUI : public Resource
    {
        friend class AppGUICache;
        friend class AppGUIService;

        RTTI_ENABLE(Resource)
    public:
        AppGUI(AppGUIService& appGUIService);

        ~AppGUI() = default;

        virtual bool init(utility::ErrorState& errorState) override;

        virtual void onDestroy() override;

        void draw(double deltaTime);

        // properties
        std::vector<ResourcePtr<AppGUIMenuItems>> mWidgetGroups;
        std::string mMenuName = "Windows";
    protected:
    private:
        bool extractWindowsRecursive(AppGUIMenuItems* items, std::vector<std::string>& group_ids, utility::ErrorState &errorState);

        void handleWidgetGroup(AppGUIMenuItems* mWidgetGroup);

        void handleWindowWidget(AppGuiWindow* mWidget);

        std::vector<std::string> getOpenWindowIDs();

        std::unordered_map<AppGuiWindow*, bool> mOpenWindows;

        AppGUIService& mAppGUIService;
    };


    class NAPAPI AppGUICache : public Resource
    {
        RTTI_ENABLE(Resource)
    public:
        AppGUICache() = default;
        AppGUICache(AppGUI& gui);

        ~AppGUICache() = default;

        bool isOpen(std::string widgetId);

        std::vector<std::string> mOpenWidgets;
    };

    using AppGUIObjectCreator = rtti::ObjectCreator<AppGUI, AppGUIService>;
}
