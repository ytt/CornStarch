#include "DefineEventTable.hpp" // イベントテーブル
#include "MainWindow.hpp"

const wxString CMainWindow::DEFAULT_CHANNEL = "Lobby"; // 主チャンネル

using namespace std;


//////////////////////////////////////////////////////////////////////


CMainWindow::CMainWindow(void) : m_view(NULL), m_logHolder(NULL)
    , m_connect(NULL), m_nickTable(NULL), m_channel(NULL), m_user(NULL)
    , m_persist(NULL)
{
}


CMainWindow::~CMainWindow(void)
{
    delete m_view;
    delete m_logHolder;

    delete m_connect;
    delete m_user;
    delete m_channel;
    delete m_nickTable;

    delete  m_persist;
}


//////////////////////////////////////////////////////////////////////


// 初期化を行う
void CMainWindow::init(void)
{
    // viewの初期化
    m_view = new CMainView();
    m_view->init(this);
    SetMenuBar(m_view->getMenuBar()); // メニューバー

    // 通信の初期化
    m_connect = new CSCConnection();
    m_connect->init();

    // データ保持部の初期化
    m_channel = new CSCChannelHolder(); // チャンネル
    m_channel->init();

    // ユーザ情報の初期化
    m_user = new CSCUser();
    m_user->init();
    m_user->setChannel(DEFAULT_CHANNEL);

    // ニックネームテーブルの初期化
    m_nickTable = new CSCNickTable();

    // ログ保持部の初期化
    m_logHolder = new CMainLogHolder();

    // 永続化を扱う
    m_persist = new CMyPersistent();
    m_persist->init();

    // イベントハンドラの初期化
    initHandle();

    // パスワード情報が保存されていれば
    wxString basicKey = m_user->getBasicKey();
    wxString nameKey = m_user->getNameKey();
    if (m_persist->isKeySaved(basicKey) && m_persist->isKeySaved(nameKey)){

        // パスワード情報を読み込む
        m_user->setUserName(m_persist->loadInfo(nameKey));
        m_user->setBasic(m_persist->loadInfo(basicKey));

        // 認証タスクを開始する
        m_connect->startAuthTask(GetEventHandler(), m_user->getUserName(), m_user->getBasic());
    }
}


//////////////////////////////////////////////////////////////////////


// 画面操作に関するイベントハンドラを設定する
void CMainWindow::initHandle(void)
{
    // エンターキー押下時
    this->Connect(m_view->getPostPaneID(), wxEVT_COMMAND_TEXT_ENTER,
        wxCommandEventHandler(CMainWindow::onEnter));

    // チャンネルリスト選択時
    this->Connect(m_view->getCnPaneID(), wxEVT_COMMAND_LISTBOX_SELECTED,
        wxCommandEventHandler(CMainWindow::onChannel));
}

// Modelがあれば画面を更新する
void CMainWindow::updateAllView(const wxString& channel)
{
    updateMessageView(channel);
    updateMemberView(channel);
    updateChannelView(channel);
}

// メッセージ画面を更新する(Modelがある場合)
void CMainWindow::updateMessageView(const wxString& channel)
{
    // メッセージが受信済みかつメンバーリストを受信済み
    if (m_channel->hasReceivedMessage(channel) && m_channel->hasReceivedMember(channel)){

        // メッセージを表示
        m_view->displayMessages(m_channel->getMessages(channel), *m_nickTable);
    } else {

        // メッセージ取得タスクを開始
        m_connect->startGetMessageTask(GetEventHandler(), channel, m_user->getBasic());
    }
}

// メンバー画面を更新する(Modelがある場合)
void CMainWindow::updateMemberView(const wxString& channel)
{
    // メンバー受信済み
    if (m_channel->hasReceivedMember(channel)){

        // メンバーを表示
        m_view->displayMembers(m_channel->getMembers(channel));

        // メッセージを受信済みならメッセージ表示
        if (m_channel->hasReceivedMessage(channel)){
            m_view->displayMessages(m_channel->getMessages(channel), *m_nickTable);
        }
    } else {

        // メンバー取得タスクを開始
        m_connect->startGetMemberTask(GetEventHandler(), channel, m_user->getBasic());
    }
}

// チャンネル画面とタイトルバーを更新する(Modelがある場合)
void CMainWindow::updateChannelView(const wxString& channel)
{
    // チャンネル受信済みなら
    if (m_channel->hasReceivedChannel()){

        // チャンネルを表示
        m_view->displayChannels(m_channel->getChannels());
        m_view->setSelectedChannel(m_user->getChannelString());

        // タイトルを表示
        displayTitle(channel, m_channel->getTopic(channel));
    } else {

        // チャンネル取得タスクを開始
        m_connect->startGetChannelTask(GetEventHandler(),
            m_user->getUserName(), m_user->getBasic());
    }
}

// タイトルバーにタイトルを表示する
void CMainWindow::displayTitle(const wxString& channel, const wxString& topic)
{
    wxString tpc = topic;

    // 改行を消してタイトルを表示
    tpc.Replace("\r\n", " ");
    tpc.Replace("\n", " ");
    this->SetTitle("【" + channel + "】" + tpc);
}


//////////////////////////////////////////////////////////////////////


// 終了
void CMainWindow::onQuit(wxCommandEvent& event)
{
    Close(true);
}

// ユーザ登録
void CMainWindow::onRegister(wxCommandEvent& event)
{
    // ログイン済みの時
    if (m_user->isLogin()){
        wxMessageBox("ログイン済みです");
        return;
    }

    // 認証ダイアログを表示
    if (m_view->showModalAuthDlg() == wxID_OK){

        // ユーザ情報をセット
        m_user->setUserInfo(m_view->getDlgUserNameAsString(), m_view->getDlgPasswordAsString());

        // 認証タスクの開始
        m_connect->startAuthTask(GetEventHandler(), m_user->getUserName(), m_user->getBasic());
    }
}

// ログアウトメニュー
void CMainWindow::onLogout(wxCommandEvent& event)
{
    // ログインしているとき、保存してある情報を削除
    if (m_user->isLogin()){
        m_persist->deleteInfo(m_user->getNameKey());
        m_persist->deleteInfo(m_user->getBasicKey());
    }
    Close();
}

// チャンネルに参加メニュー
void CMainWindow::onJoin(wxCommandEvent& event)
{
    // 未ログインの時
    if (!m_user->isLogin()){
        return;
    }

    // ダイアログを表示
    if (m_view->showModalChannelDlg() == wxID_OK){

        // チャンネル参加タスクの開始
        wxString cn = m_view->getDlgChannelNameAsString();
        m_connect->startJoinTask(GetEventHandler(),
            cn, m_user->getUserName(), m_user->getBasic());
    }
}

// チャンネルから離脱メニュー
void CMainWindow::onPart(wxCommandEvent& event)
{
    // 未ログインの時
    if (!m_user->isLogin()){
        return;
    }

    // ダイアログを表示
    if (m_view->showModalChannelDlg() == wxID_OK){

        // チャンネル離脱タスクを開始
        wxString cn = m_view->getDlgChannelNameAsString();
        m_connect->startPartTask(GetEventHandler(),
            cn, m_user->getUserName(), m_user->getBasic());
    }
}

// 表示を更新
void CMainWindow::onUpdateDisplay(wxCommandEvent& event)
{
    // 通信を初期化
    delete m_connect;
    m_connect = new CSCConnection();
    m_connect->init();
    m_connect->startStreamTask(GetEventHandler(), m_user->getUserName(), m_user->getBasic());

    // データ保持部を初期化
    m_channel->deleteChannels();

    // 表示を更新
    updateAllView(m_user->getChannelString());
}


//////////////////////////////////////////////////////////////////////


// サイズ変更
void CMainWindow::OnSize(wxSizeEvent& event)
{
    wxFrame::OnSize(event);
    m_view->updateSashPos(this->GetSize());
}

// 投稿ペインでEnterキーを押下
void CMainWindow::onEnter(wxCommandEvent& event)
{
    wxString message = event.GetString();

    // 投稿画面をクリア
    m_view->clearPostPaneText();

    // 何も文がないとき
    if (message == ""){
        return;
    }

    // ログイン済みの時
    if (!m_user->isLogin()){
        return;
    }

    // メッセージ投稿タスクの開始
    wxString channel = m_user->getChannelString();
    m_connect->startPostMessageTask(GetEventHandler(), message, channel, m_user->getBasic());

    // メッセージを保存
    CMessageData data(-1, m_user->getUserName(), message, channel, time(NULL));
    m_channel->pushMessage(data.m_channel, data);
    m_logHolder->pushMessageLog(data, m_user->getNickName());

    // 表示の更新
    m_view->displayLogs(m_logHolder->getLogs());
    updateMessageView(channel);
}

// チャンネル選択時
void CMainWindow::onChannel(wxCommandEvent& event)
{
    // ユーザ情報を更新
    m_user->setChannel(event.GetString());

    // タイトルを表示
    wxString ch = m_user->getChannelString();
    displayTitle(ch, m_channel->getTopic(ch));

    // 画面表示を更新
    updateMessageView(m_user->getChannelString());
    updateMemberView(m_user->getChannelString());
}


//////////////////////////////////////////////////////////////////////


// メッセージ投稿終了時
void CMainWindow::onFinishPostMessage(wxThreadEvent& event)
{
}

// 認証情報の受信時
void CMainWindow::onGetAuth(CAuthEvent& event)
{
    // 認証が成功したとき
    if (!event.isAuthSucceeded()){

        wxMessageBox("認証に失敗しました");
        return;
    }

    // ユーザをログイン状態にする
    m_user->setLogin(true);

    // パスワード永続化
    m_persist->saveInfo(m_user->getNameKey(), m_user->getUserName());
    m_persist->saveInfo(m_user->getBasicKey(), m_user->getBasic());

    // ニックネームを取得するためのタスク
    m_connect->startGetMemberInfoTask(GetEventHandler(), m_user->getUserName(),
        m_user->getBasic());

    // ストリーム受信タスク
    m_connect->startStreamTask(GetEventHandler(), m_user->getUserName(), m_user->getBasic());

    // 画面表示の更新
    updateAllView(m_user->getChannelString());

}

// メッセージ一覧受信時
void CMainWindow::onGetMessages(CGetMessageEvent& event)
{
    // メッセージを追加
    m_channel->setMessages(m_user->getChannelString(), event.getMessages());

    // 表示の更新
    updateMessageView(m_user->getChannelString());
}

// メンバー一覧受信時
void CMainWindow::onGetMembers(CGetMemberEvent& event)
{
    // メンバーの追加
    vector<CMemberData*> members = event.getMembers();
    m_channel->setMembers(m_user->getChannelString(), members);
    m_nickTable->addTableFromMembers(members);

    // 表示の更新
    updateMemberView(m_user->getChannelString());
}

// チャンネル一覧受信時
void CMainWindow::onGetChannels(CGetChannelEvent& event)
{
    // チャンネルの追加
    m_channel->setChannels(event.getChannels());

    // 表示の更新
    updateChannelView(m_user->getChannelString());
}

// チャンネル参加時
void CMainWindow::onJoinChannel(CJoinEvent& event)
{
    // ユーザの現在のチャンネルを変更
    m_user->setChannel(event.getChannel().m_name);

    // チャンネル一覧取得タスクの開始
    m_connect->startGetChannelTask(GetEventHandler(),
        m_user->getUserName(), m_user->getBasic());

    // 表示の更新
    updateMemberView(m_user->getChannelString());
    updateMessageView(m_user->getChannelString());
}

// チャンネル離脱時
void CMainWindow::onPartChannel(wxThreadEvent& event)
{
    // チャンネル情報の削除
    wxString ch1(event.GetString().mb_str(wxConvUTF8));

    // チャンネル情報を削除
    m_channel->popChannel(ch1);

    // ユーザの現在のチャンネルを変更
    m_user->setChannel(DEFAULT_CHANNEL);

    // 表示の更新
    updateAllView(m_user->getChannelString());
}

// メンバー情報の受信時
void CMainWindow::onGetMemberInfo(CGetMemberInfoEvent& event)
{
    // データ更新
    CMemberData data = event.getMember();
    (*m_nickTable)[data.m_name] = data.m_nick;

    // 自分の情報だったら
    if (data.m_name == m_user->getUserName()){
        m_user->setNickName(data.m_nick);
        m_user->setKeywords(data.m_keywords);
    }

    m_channel->updateMember(data);
    m_logHolder->onUpdateNickName(data);

    // 表示を更新
    updateMemberView(m_user->getChannelString());
    updateMessageView(m_user->getChannelString());
    m_view->displayLogs(m_logHolder->getLogs());
}

// メッセージストリーム受信時
void CMainWindow::onMsgStream(CMsgStreamEvent& event)
{
    CMessageData data = event.getMessage();

    // 別クライアントからのメッセージだったら、データ更新のみ
    if (m_channel->hasSameMessage(data)){
        m_channel->onUpdateMessageId(data);
        return;
    }

    // 通知があったとき
    if (m_user->isCalled(data.m_body)){
        wxMessageBox("通知", "呼ばれました", wxOK);
    }

    // データ更新
    wxString nick = m_nickTable->getNickname(data.m_username);
    m_logHolder->pushMessageLog(data, nick);

    // ニックネームが未知の場合、メンバー情報取得タスクの開始
    if (!m_nickTable->isExist(data.m_username)){

        m_connect->startGetMemberInfoTask(GetEventHandler(), data.m_username, m_user->getBasic());
        return;
    }

    // データ追加
    m_channel->pushMessage(data.m_channel, data);

    // メッセージをログ一覧に表示
    // 現在見ているページなら、メッセージ表示部を更新
    m_view->displayLogs(m_logHolder->getLogs()); // ログペイン
    if (m_user->getChannelString() == data.m_channel){
        updateMessageView(data.m_channel);
    }
}

// チャンネル参加ストリーム受信時
void CMainWindow::onJoinStream(CJoinStreamEvent& event)
{
    // 処理待ちに追加
    CSubscribeData data (event.getChannelName(), event.getUserName());

    wxString nick = m_nickTable->getNickname(data.m_username);
    m_logHolder->pushJoinLog(data, nick);

    // ニックネームが未知の場合、メンバー情報取得タスクの開始
    if (!m_nickTable->isExist(data.m_username)){
        m_connect->startGetMemberInfoTask(GetEventHandler(),
            event.getUserName(), m_user->getBasic());
        return;
    }

    // メンバーを追加
    m_channel->pushMember(data.m_channel, CMemberData(data.m_username, nick));

    // 自分が参加したとき(別クライアントソフトから)
    if (data.m_username == m_user->getUserName()){

        // チャンネル情報取得タスクの開始
        m_connect->startGetChannelTask(
            GetEventHandler(), m_user->getUserName(), m_user->getBasic());
    }

    // 表示の更新
    m_view->displayLogs(m_logHolder->getLogs()); // ログペイン
    if (data.m_channel == m_user->getChannelString()){
        updateMemberView(data.m_channel);
    }

}

// チャンネル離脱ストリーム受信時
void CMainWindow::onPartStream(CPartStreamEvent& event)
{
    CSubscribeData data (event.getChannelName(), event.getUserName());
    wxString name = event.getUserName();
    wxString channel = event.getChannelName();

    // データ更新
    wxString nick = m_nickTable->getNickname(name);
    m_logHolder->pushPartLog(data, nick);
    m_channel->popMember(data.m_username);

    // ニックネームが未知の時、メンバー情報取得タスクの開始
    if (!m_nickTable->isExist(name)){
        m_connect->startGetMemberInfoTask(GetEventHandler(),
            name, m_user->getBasic());
        return;
    }

    // 表示の更新
    m_view->displayLogs(m_logHolder->getLogs()); // ログペイン
    if (channel == m_user->getChannelString()){
        updateMemberView(channel);
    }
}

// チャンネル更新ストリーム受信時
void CMainWindow::onChannelStream(CChannelStreamEvent& event)
{
    // データ更新
    CChannelData channel = event.getChannel();
    m_channel->setChannel(channel);
    m_logHolder->pushTopicLog(channel);

    // 表示の更新
    m_view->displayLogs(m_logHolder->getLogs()); // ログペイン

    // 現在のチャンネルならばタイトルを更新
    if (channel.m_name == m_user->getChannelString()){
        displayTitle(channel.m_name, channel.m_topic);
    }
}

// ユーザ情報更新ストリーム受信時
void CMainWindow::onUserStream(CUserStreamEvent& event)
{
    // データ更新
    CMemberData member = event.getMember();
    m_logHolder->pushChangeNickLog(member);
    m_channel->updateMember(member);
    (*m_nickTable)[member.m_name] = member.m_nick;

    // 表示の更新
    updateMemberView(m_user->getChannelString()); // メンバーペイン
    updateMessageView(m_user->getChannelString()); // メッセージペイン
    m_view->displayLogs(m_logHolder->getLogs()); // ログペイン
}
