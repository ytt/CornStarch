#include "GetChannelTask.hpp"

using namespace std;

CGetChannelTask::CGetChannelTask(void)
{
}


CGetChannelTask::~CGetChannelTask(void)
{
}


//////////////////////////////////////////////////////////////////////


// 初期化を行う
void CGetChannelTask::init(wxEvtHandler* handler, const wxString& userName,
    const wxString& basic)
{
    CSCTask::init(handler, basic);
    m_userName = userName;
}


//////////////////////////////////////////////////////////////////////


// StarChatに対してリクエストを送信する
void CGetChannelTask::sendRequestToSC(CSCClient* client)
{
    // GETリクエストの送信
    client->sendGetChannelsRequest(m_userName, m_basic);
}

// HTTPレスポンスを解析してイベントを作成する
wxThreadEvent* CGetChannelTask::parseHttpResponse(const string& responseBody)
{
    // イベントの初期化
    CGetChannelEvent* event = new CGetChannelEvent();

    CSCJsonParser parser;
    event->setChannels(parser.getChannels(responseBody)); // 値取得
    event->SetEventType(myEVT_THREAD_GET_CHANNEL); // イベントの種類をセット
    return event;
}
