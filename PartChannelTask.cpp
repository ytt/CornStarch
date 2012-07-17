#include "PartChannelTask.hpp"

using namespace std;

CPartChannelTask::CPartChannelTask(void)
{
}


CPartChannelTask::~CPartChannelTask(void)
{
}


//////////////////////////////////////////////////////////////////////


// 行う処理をセット
void CPartChannelTask::init(wxEvtHandler* handler, const wxString& userName,
    const wxString& channel, const wxString& basic)
{
    CSCTask::init(handler, basic);
    m_userName = userName;
    m_channel = channel;
}


//////////////////////////////////////////////////////////////////////


// StarChatに対してリクエストを送信する
void CPartChannelTask::sendRequestToSC(CSCClient* client)
{
    client->sendPartRequest(m_channel, m_userName, m_basic);
}

// HTTPレスポンスを解析してイベントを作成する
wxThreadEvent* CPartChannelTask::parseHttpResponse(const string& responseBody)
{
    // イベントの初期化
    wxThreadEvent* event = new wxThreadEvent();
    event->SetEventType(myEVT_THREAD_DELETE_PART); // イベントの種類をセット
    event->SetString(m_channel);
    return event;
}
