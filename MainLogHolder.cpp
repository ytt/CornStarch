#include "MainLogHolder.hpp"

using namespace std;

CMainLogHolder::CMainLogHolder(void)
{
}


CMainLogHolder::~CMainLogHolder(void)
{
    size_t size = m_logs.size();
    for (size_t i = 0; i < size; i++){
        delete m_logs[i];
    }
}


//////////////////////////////////////////////////////////////////////


// ���O�ꗗ���擾����
vector<CChatLog*> CMainLogHolder::getLogs(void) const
{
    return m_logs;
}

// ���b�Z�[�W���O��ǉ�����
void CMainLogHolder::pushMessageLog(const CMessageData& message, const wxString& nick)
{
    CMessageLog* log = new CMessageLog();
    log->init(message, nick);

    // ���O�̒ǉ�
    pushLog(log);
}

// �`�����l���Q�����O��ǉ�����
void CMainLogHolder::pushJoinLog(const CSubscribeData& sub, const wxString& nick)
{
    CJoinLog* log = new CJoinLog();
    log->init(sub, nick);

    // ���O�̒ǉ�
    pushLog(log);
}

// �`�����l���ޏo���O��ǉ�����
void CMainLogHolder::pushPartLog(const CSubscribeData& sub, const wxString& nick)
{
    CPartLog* log = new CPartLog();
    log->init(sub, nick);

    // ���O�̒ǉ�
    pushLog(log);
}

// �j�b�N�l�[���ύX���O��ǉ�����
void CMainLogHolder::pushChangeNickLog(const CMemberData& member)
{
    CMemberLog* log = new CMemberLog();
    log->init(member);

    // ���O�̒ǉ�
    pushLog(log);
}

// �g�s�b�N�ύX���O��ǉ�����
void CMainLogHolder::pushTopicLog(const CChannelData& channel)
{
    CTopicLog* log = new CTopicLog();
    log->init(channel);

    // ���O�̒ǉ�
    pushLog(log);
}

// �j�b�N�l�[���̍X�V���s��
void CMainLogHolder::onUpdateNickName(const CMemberData& member)
{
    vector<CChatLog*>::iterator it = m_logs.begin();
    for (; it != m_logs.end(); it++){

        // �j�b�N�l�[�������m�̎��A�X�V���s��
        if ((*it)->isUnknownNick(member.m_name)){
            (*it)->updateNick(member.m_nick);
        }
    }
}


//////////////////////////////////////////////////////////////////////


// ���O��ǉ�����
void CMainLogHolder::pushLog(CChatLog* log)
{
    // ���O�̒ǉ�
    m_logs.push_back(log);

    // ����������擪�폜
    if (m_logs.size() > MAX_LENGTH){
        vector<CChatLog*>::iterator it = m_logs.begin();
        m_logs.erase(it);
    }
}