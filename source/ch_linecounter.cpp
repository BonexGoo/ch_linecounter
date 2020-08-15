#include <boss.hpp>
#include "ch_linecounter.hpp"

#include <resource.hpp>

ZAY_DECLARE_VIEW_CLASS("ch_linecounterView", ch_linecounterData)

ZAY_VIEW_API OnCommand(CommandType type, chars topic, id_share in, id_cloned_share* out)
{
}

ZAY_VIEW_API OnNotify(NotifyType type, chars topic, id_share in, id_cloned_share* out)
{
    if(type == NT_FileContent)
    {
        m->FileName = topic;
        m->Parsing(uint08s(in));
        m->invalidate();
    }
}

ZAY_VIEW_API OnGesture(GestureType type, sint32 x, sint32 y)
{
}

ZAY_VIEW_API OnRender(ZayPanel& panel)
{
    // 배경색
    ZAY_RGB(panel, 255, 255, 255)
        panel.fill();

    // 소개박스
    ZAY_XYWH(panel, 30, 30, panel.w() - 60, 40)
    {
        ZAY_RGBA(panel, 0, 0, 0, 64)
            panel.fill();
        ZAY_RGB(panel, 0, 0, 0)
        {
            auto Text = String::Format("  Welcome everyone! (BuildDate: %s)\n"
                                       "  This is a code line counter.", __DATE__);
            panel.text(Text, UIFA_LeftMiddle, UIFE_Right);
        }

        // 파일오픈 버튼
        ZAY_LTRB_UI(panel, panel.w() - 10 - 80, 10, panel.w() - 10, panel.h() - 10, "btn1",
            ZAY_GESTURE_T(t)
            {
                if(t == GT_InReleased)
                    m->OnBtn1();
            })
        {
            const bool DragPressed = !!(panel.state("btn1") & (PS_Pressed | PS_Dragging));
            const bool Focused = !!(panel.state("btn1") & PS_Focused);
            ZAY_MOVE_IF(panel, 1, 1, DragPressed)
            {
                ZAY_RGBA(panel, 0, 0, 0, (Focused)? 220 : 160)
                    panel.fill();
                ZAY_RGB(panel, 0, 0, 0)
                    panel.rect(1);
                ZAY_RGB(panel, 0, 255, 0)
                    panel.text("File Open");
            }
        }
    }

    // 결과박스
    if(0 < m->Result.Length())
    ZAY_LTRB_SCISSOR(panel, 30, 30 + 40 + 30, panel.w() - 30, panel.h() - 30)
    {
        ZAY_RGBA(panel, 0, 0, 0, 16)
            panel.fill();

        // 파일명
        ZAY_MOVE(panel, 0, 5)
        ZAY_RGB(panel, 0, 0, 128)
        {
            auto Text = String::Format("  Parsing %s...", (chars) m->FileName);
            panel.text(Text, UIFA_LeftTop, UIFE_Right);
        }

        // 결과
        ZAY_MOVE(panel, 0, 25)
        ZAY_RGB(panel, 0, 0, 0)
            panel.text(m->Result, UIFA_LeftTop, UIFE_Right);
    }
}

ch_linecounterData::ch_linecounterData()
{
}

ch_linecounterData::~ch_linecounterData()
{
}

void ch_linecounterData::OnBtn1()
{
    FileName = "*";
    Result = "  Waiting...";
    Platform::Popup::FileContentDialog();
}

void ch_linecounterData::Parsing(uint08s content)
{
    sint32 LineCount = 0;
    if(0 < content.Count())
    for(auto Pos = (chars) &content[0]; *Pos != '\0'; ++Pos)
    {
        if(*Pos == '\n')
            LineCount++;
    }

    // 결과집계
    Result = String::Format("  LineCount: %d\n"
                            "  Parsing complete!", LineCount);
}
