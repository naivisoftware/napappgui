
// local includes
#include "AppGUIwidget.h"

#include <imgui/imgui.h>

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::AppGuiWidget)
RTTI_END_CLASS

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::AppGuiWindow)
    RTTI_PROPERTY("Name", &nap::AppGuiWindow::mName, nap::rtti::EPropertyMetaData::Default)
RTTI_END_CLASS

RTTI_BEGIN_CLASS(nap::AppGUIMenuItems)
    RTTI_PROPERTY("Items", &nap::AppGUIMenuItems::mItems, nap::rtti::EPropertyMetaData::Default)
    RTTI_PROPERTY("Name", &nap::AppGUIMenuItems::mName, nap::rtti::EPropertyMetaData::Default)
RTTI_END_CLASS

namespace nap
{
    void AppGuiWidget::draw(double deltaTime)
    {
        ImGui::PushID(mID.c_str());
        drawContent(deltaTime);
        ImGui::PopID();
    }


    void AppGuiWindow::draw(double deltaTime)
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
