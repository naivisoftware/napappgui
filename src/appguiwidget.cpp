
// local includes
#include "appguiwidget.h"
#include "appguiservice.h"

#include <imgui/imgui.h>

RTTI_BEGIN_CLASS(nap::AppGUIMenuItemGroup)
    RTTI_PROPERTY(nap::group::members,	&nap::AppGUIMenuItemGroup::mMembers,	nap::rtti::EPropertyMetaData::Embedded | nap::rtti::EPropertyMetaData::ReadOnly)
    RTTI_PROPERTY(nap::group::children,	&nap::AppGUIMenuItemGroup::mChildren,	nap::rtti::EPropertyMetaData::Embedded | nap::rtti::EPropertyMetaData::ReadOnly)
RTTI_END_CLASS

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::AppGUIWidget)
RTTI_END_CLASS

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::AppGUIWindow)
    RTTI_PROPERTY("Name", &nap::AppGUIWindow::mName, nap::rtti::EPropertyMetaData::Default)
RTTI_END_CLASS

namespace nap
{
    //////////////////////////////////////////////////////////////////////////
    // AppGUIWidget
    //////////////////////////////////////////////////////////////////////////

    void AppGUIWidget::draw(double deltaTime)
    {
        ImGui::PushID(mID.c_str());
        drawContent(deltaTime);
        ImGui::PopID();
    }

    //////////////////////////////////////////////////////////////////////////
    // AppGUIWindow
    //////////////////////////////////////////////////////////////////////////

    void AppGUIWindow::draw(double deltaTime)
    {
        ImGui::PushID(mID.c_str());
        if(ImGui::Begin(mName.c_str()))
        {
            drawContent(deltaTime);
        }
        ImGui::End();
        ImGui::PopID();
    }
}
