#include "GetMessageTask.hpp"

using namespace std;

CGetMessageTask::CGetMessageTask(void)
{
}


CGetMessageTask::~CGetMessageTask(void)
{
}


//////////////////////////////////////////////////////////////////////


// 初期化を行う
void CGetMessageTask::init(wxEvtHandler* handler, const wxString& channel,
    const wxString& basic)
{
    CSCTask::init(handler, basic);
    m_channel = channel;
}


//////////////////////////////////////////////////////////////////////


// StarChatに対してリクエストを送信する
void CGetMessageTask::sendRequestToSC(CSCClient* client)
{
    // GETリクエストの送信
    client->sendGetMessagesRequest(m_channel, m_basic);
}

// HTTPレスポンスを解析してイベントを作成する
wxThreadEvent* CGetMessageTask::parseHttpResponse(const string& responseBody)
{
    // イベントの初期化
    CGetMessageEvent* event = new CGetMessageEvent();

    CSCJsonParser parser;
    event->setMessages(parser.getMessages(responseBody)); // 値取得
    event->SetEventType(myEVT_THREAD_GET_MESSAGE); // イベントの種類をセット
    return event;
}
