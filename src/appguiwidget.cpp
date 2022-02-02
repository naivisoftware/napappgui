
// local includes
#include "AppGUIwidget.h"

#include <imgui/imgui.h>

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::AppGUIWidget)
RTTI_END_CLASS

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::AppGUIWindow)
    RTTI_PROPERTY("Name", &nap::AppGUIWindow::mName, nap::rtti::EPropertyMetaData::Default)
RTTI_END_CLASS

RTTI_BEGIN_CLASS(nap::AppGUIWindowGroup)
    RTTI_PROPERTY("Items", &nap::AppGUIWindowGroup::mItems, nap::rtti::EPropertyMetaData::Embedded)
    RTTI_PROPERTY("Name", &nap::AppGUIWindowGroup::mName, nap::rtti::EPropertyMetaData::Default)
RTTI_END_CLASS

namespace nap
{
    void AppGUIWidget::draw(double deltaTime)
    {
        ImGui::PushID(mID.c_str());
        drawContent(deltaTime);
        ImGui::PopID();
    }


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
