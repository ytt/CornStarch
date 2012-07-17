#include "JoinLog.hpp"

using namespace std;

CJoinLog::CJoinLog(void) : m_sub(NULL)
{
}


CJoinLog::~CJoinLog(void)
{
    delete m_sub;
}


//////////////////////////////////////////////////////////////////////


// ���������s��
void CJoinLog::init(const CSubscribeData& sub, const wxString& nick)
{
    if (m_sub == NULL){
        m_sub = new CSubscribeData();
    }
    *m_sub = sub;
    m_nick = nick;

    // �^�C�v�̎w��
    m_type = LOG_JOIN;
}

// ���O���擾����
CSubscribeData CJoinLog::getLog(void) const
{
    return *m_sub;
}

// �j�b�N�l�[�����擾����
wxString CJoinLog::getNickName(void) const
{
    return m_nick;
}

// �{���ɑΉ�����j�b�N�l�[�������m��
bool CJoinLog::isUnknownNick(const wxString& name) const
{
    if (m_nick == ""){
        return true;
    }
    return false;
}

// �j�b�N�l�[�����X�V����
void CJoinLog::updateNick(const wxString& nick)
{
    m_nick = nick;
}
