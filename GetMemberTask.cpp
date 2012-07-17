#include "GetMemberTask.hpp"

using namespace std;

CGetMemberTask::CGetMemberTask(void)
{
}


CGetMemberTask::~CGetMemberTask(void)
{
}


//////////////////////////////////////////////////////////////////////


// 初期化を行う
void CGetMemberTask::init(wxEvtHandler* handler, const wxString& channel,
    const wxString& basic)
{
    CSCTask::init(handler, basic);
    m_channel = channel;
}


//////////////////////////////////////////////////////////////////////


// StarChatに対してリクエストを送信する
void CGetMemberTask::sendRequestToSC(CSCClient* client)
{
    // GETリクエストの送信
    client->sendGetMembersRequest(m_channel, m_basic);
}

// HTTPレスポンスを解析してイベントを作成する
wxThreadEvent* CGetMemberTask::parseHttpResponse(const string& responseBody)
{
    // イベントの初期化
    CGetMemberEvent* event = new CGetMemberEvent();

    CSCJsonParser parser;
    event->setMembers(parser.getMembers(responseBody)); // 値取得
    event->SetEventType(myEVT_THREAD_GET_MEMBER); // イベントの種類をセット
    return event;
}