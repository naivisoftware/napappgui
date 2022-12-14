// local includes
#include "appguiwidget.h"
#include "appguiservice.h"

#include <imgui/imgui.h>

RTTI_BEGIN_CLASS(nap::AppGUIWidgetGroup)
    RTTI_PROPERTY(nap::group::members,	&nap::AppGUIWidgetGroup::mMembers,	nap::rtti::EPropertyMetaData::Embedded | nap::rtti::EPropertyMetaData::ReadOnly)
    RTTI_PROPERTY(nap::group::children,	&nap::AppGUIWidgetGroup::mChildren,	nap::rtti::EPropertyMetaData::Embedded | nap::rtti::EPropertyMetaData::ReadOnly)
RTTI_END_CLASS

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::AppGUIWidget)
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
}
