#include "GetMemberInfoTask.hpp"

using namespace std;

CGetMemberInfoTask::CGetMemberInfoTask(void)
{
}


CGetMemberInfoTask::~CGetMemberInfoTask(void)
{
}


//////////////////////////////////////////////////////////////////////


// 初期化を行う
void CGetMemberInfoTask::init(wxEvtHandler* handler, const wxString& username,
    const wxString& basic)
{
    CSCTask::init(handler, basic);
    m_username = username;
}


//////////////////////////////////////////////////////////////////////


// StarChatに対してリクエストを送信する
void CGetMemberInfoTask::sendRequestToSC(CSCClient* client)
{
    // GETリクエストの送信
    client->sendGetMemInfoRequest(m_username, m_basic);
}

// HTTPレスポンスを解析してイベントを作成する
wxThreadEvent* CGetMemberInfoTask::parseHttpResponse(const string& responseBody)
{
    // イベントの初期化
    CGetMemberInfoEvent* event = new CGetMemberInfoEvent();

    CSCJsonParser parser;
    event->setMember(parser.getMember(responseBody)); // 値取得
    event->SetEventType(myEVT_THREAD_GET_MEMBER_INFO); // イベントの種類をセット
    return event;
}
