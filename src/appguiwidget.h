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

    // AppGuiMenuItemGroup group type definition
    using AppGUIMenuItemGroup = Group<AppGUIMenuItem>;

    /*
    // For backwards compatibility reasons, override the default 'Members' and 'Children' property names
    // of the 'nap::AppGUIMenuItemGroup' to the property names introduced before the arrival of the generic nap::Group<T>.
    template<>
    NAPAPI nap::Group<AppGUIMenuItem>::Group();
*/

    class NAPAPI AppGUIWidget : public AppGUIItem
    {
        RTTI_ENABLE(AppGUIItem)
    public:
        void draw(double deltaTime);
    protected:
        virtual void drawContent(double deltaTime) = 0;
    };

    class NAPAPI AppGUIWindow : public AppGUIMenuItem
    {
        RTTI_ENABLE(AppGUIMenuItem)
    public:
        void draw(double deltaTime);

        // properties
        std::string mName;
    protected:
        virtual void drawContent(double deltaTime) = 0;
    };
}
