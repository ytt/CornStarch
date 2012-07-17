#include "PartLog.hpp"

using namespace std;

CPartLog::CPartLog(void) : m_sub(NULL)
{
}


CPartLog::~CPartLog(void)
{
    delete m_sub;
}


//////////////////////////////////////////////////////////////////////


// ���������s��
void CPartLog::init(const CSubscribeData& sub, const wxString& nick)
{
    if (m_sub == NULL){
        m_sub = new CSubscribeData();
    }
    *m_sub = sub;
    m_nick = nick;

    // �^�C�v�̎w��
    m_type = LOG_PART;
}

// ���O���擾����
CSubscribeData CPartLog::getLog(void) const
{
    return *m_sub;
}

// �j�b�N�l�[�����擾����
wxString CPartLog::getNickName(void) const
{
    return m_nick;
}

// �{���ɑΉ�����j�b�N�l�[�������m��
bool CPartLog::isUnknownNick(const wxString& name) const
{
    if (m_nick == ""){
        return true;
    }
    return false;
}

// �j�b�N�l�[�����X�V����
void CPartLog::updateNick(const wxString& nick)
{
    m_nick = nick;
}
