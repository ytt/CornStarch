#include "PostMessageTask.hpp"

using namespace std;

CPostMessageTask::CPostMessageTask(void)
{
}


CPostMessageTask::~CPostMessageTask(void)
{
}


//////////////////////////////////////////////////////////////////////

// 初期化を行う
void CPostMessageTask::init(wxEvtHandler* handler, const wxString& channel,
    const wxString& basic)
{
    CSCTask::init(handler, basic);
    m_channel = channel;
}

// 投稿するメッセージをセット
void CPostMessageTask::setMessage(const wxString& message)
{
    m_postMessage = message;
}


//////////////////////////////////////////////////////////////////////


// StarChatに対してリクエストを送信する
void CPostMessageTask::sendRequestToSC(CSCClient* client)
{
    client->postMessageRequest(m_postMessage, m_channel, m_basic);
}

// HTTPレスポンスを解析してイベントを作成する
wxThreadEvent* CPostMessageTask::parseHttpResponse(const string& responseBody)
{
    // 処理終了イベントを送信
    wxThreadEvent* event = new wxThreadEvent();
    event->SetEventType(myEVT_THREAD_POST_MESSAGE);
    return event;
}
