#pragma once
#include "SCTask.hpp"
#include "GetMemberEvent.hpp"

// イベントの宣言
wxDECLARE_EVENT(myEVT_THREAD_GET_MEMBER, CGetMemberEvent);

// メンバー取得を別スレッドで取得するためのタスク
class CGetMemberTask : public CSCTask
{
private:
    wxString m_channel; // チャンネル名

public:
    CGetMemberTask(void);
    ~CGetMemberTask(void);

    // 初期化を行う
    void init(wxEvtHandler* handler, const wxString& channel, 
        const wxString& basic);

private:

    // StarChatに対してリクエストを送信する
    void sendRequestToSC(CSCClient* client);

    // HTTPレスポンスを解析してイベントを作成する
    wxThreadEvent* parseHttpResponse(const std::string& responseBody);
};

