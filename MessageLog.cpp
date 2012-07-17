#include "MessageLog.hpp"

using namespace std;

CMessageLog::CMessageLog(void) : m_message(NULL)
{
}


CMessageLog::~CMessageLog(void)
{
    delete m_message;
}


//////////////////////////////////////////////////////////////////////


// ���������s��
void CMessageLog::init(const CMessageData& message, const wxString& nick)
{
    if (m_message == NULL){
        m_message = new CMessageData();
    }
    *m_message = message;
    m_nick = nick;

    // �^�C�v�̎w��
    m_type = LOG_MESSAGE;
}

// ���O���擾����
CMessageData CMessageLog::getLog(void) const
{
    return *m_message;
}

// �j�b�N�l�[�����擾����
wxString CMessageLog::getNickName(void) const
{
    return m_nick;
}

// �{���ɑΉ�����j�b�N�l�[�������m��
bool CMessageLog::isUnknownNick(const wxString& name) const
{
    if (m_nick == ""){
        return true;
    }
    return false;
}

// �j�b�N�l�[�����X�V����
void CMessageLog::updateNick(const wxString& nick)
{
    m_nick = nick;
}
