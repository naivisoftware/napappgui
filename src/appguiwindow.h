#pragma once

// External Includes
#include <nap/resource.h>
#include <nap/resourceptr.h>
#include <rtti/factory.h>
#include <nap/group.h>

// Local Includes
#include "appguiitem.h"

namespace nap
{
    //////////////////////////////////////////////////////////////////////////
    // forward declares
    class AppGUIService;
    class AppGUIWindow;

    // AppGUIWindowGroup group type definition
    using AppGUIWindowGroup = Group<AppGUIWindow>;

    class NAPAPI AppGUIWindow : public AppGUIItem
    {
    RTTI_ENABLE(AppGUIItem)
    public:
        void draw(double deltaTime) override;

        /**
         * Returns true if window is closed during last draw call
         * @return true if window is closed during last draw call
         */
        bool getIsClosed() const;

        // properties
        std::string mTitle;
    protected:
        virtual void drawContent(double deltaTime) = 0;
    private:
        bool mOpen = false;
    };
}
