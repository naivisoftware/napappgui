
// local includes
#include "appguiwindow.h"
#include "appguiservice.h"

#include <imgui/imgui.h>
#include <nap/logger.h>

RTTI_BEGIN_CLASS(nap::AppGUIWindowGroup)
        RTTI_PROPERTY(nap::group::members,	&nap::AppGUIWindowGroup::mMembers,	nap::rtti::EPropertyMetaData::Embedded | nap::rtti::EPropertyMetaData::ReadOnly)
        RTTI_PROPERTY(nap::group::children,	&nap::AppGUIWindowGroup::mChildren,	nap::rtti::EPropertyMetaData::Embedded | nap::rtti::EPropertyMetaData::ReadOnly)
RTTI_END_CLASS

RTTI_BEGIN_CLASS_NO_DEFAULT_CONSTRUCTOR(nap::AppGUIWindow)
        RTTI_PROPERTY("Title", &nap::AppGUIWindow::mTitle, nap::rtti::EPropertyMetaData::Default)
RTTI_END_CLASS

namespace nap
{
    //////////////////////////////////////////////////////////////////////////
    // AppGUIWindow
    //////////////////////////////////////////////////////////////////////////

    void AppGUIWindow::draw(double deltaTime)
    {
        ImGui::PushID(mID.c_str());
        mOpen = true;
        if(ImGui::Begin(mTitle.c_str(), &mOpen))
        {
            drawContent(deltaTime);
        }
        ImGui::End();
        ImGui::PopID();
    }


    bool AppGUIWindow::getIsClosed() const
    {
        return !mOpen;
    }
}
