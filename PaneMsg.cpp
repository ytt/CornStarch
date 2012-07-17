#include "PaneMsg.hpp"

using namespace std;

CPaneMsg::CPaneMsg(void)
{
}


CPaneMsg::~CPaneMsg(void)
{
}


//////////////////////////////////////////////////////////////////////


// 初期化を行う
void CPaneMsg::init(wxWindow* parent)
{
    // 画面の初期化
    Create(parent, wxID_ANY, wxEmptyString, wxDefaultPosition, 
        wxDefaultSize, wxTE_MULTILINE|wxTE_READONLY|wxVSCROLL|wxTE_RICH2/*|wxTE_DONTWRAP*/);

    // フォント設定
    this->SetFont(wxFont(10, wxDEFAULT, wxNORMAL, wxNORMAL));
}

// メッセージを表示する
void CPaneMsg::displayMessages(const vector<CMessageData*>& messages,
    const map<wxString, wxString>& nickTable)
{
    wxString result;
    vector<int> first;
    vector<int> second;
    vector<int> third;

    int size = (int)messages.size();
    for (int i = 0; i < size; i++){

        // 時刻
        first.push_back((int)result.size());
        result += messages[i]->getTime("%H:%M");

        // 名前
        second.push_back((int)result.size());
        wxString nick = getNickName(messages[i]->m_username, nickTable);
        result += " (" + nick + "):";

        // 本文(temporary_nick)
        third.push_back((int)result.size());
        if (messages[i]->m_tempNick != ""){
            // temporary_nickがあれば
            result += " (" + messages[i]->m_tempNick + ") ";
        }

        // 文字列を追加
        wxString body = messages[i]->m_body;
        body.Replace("\r", "");
        result += body + "\n";
    }

    // 書き出し
    this->SetValue("");
    this->AppendText(result.substr(0, result.size() - 1));

    // 色づけ
    for (int i = 0; i < size; i++){
        // 時刻を赤
        this->SetStyle(first[i], second[i], wxTextAttr(*wxRED));

        // 名前を青
        this->SetStyle(second[i], third[i], wxTextAttr(*wxBLUE));
    }
}


//////////////////////////////////////////////////////////////////////


// ユーザ名に対応するニックネームを取得する
wxString CPaneMsg::getNickName(const wxString& userName, const map<wxString, wxString>& nickTable)
{
    // テーブルに存在しない時、本名を返す
    if (nickTable.find(userName) == nickTable.end()){
        return userName;
    }

    // ニックネームを返す
    return nickTable.find(userName)->second;
}

