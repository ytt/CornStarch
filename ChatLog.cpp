#include "ChatLog.hpp"


CChatLog::CChatLog(void)
{
}


CChatLog::~CChatLog(void)
{
}


//////////////////////////////////////////////////////////////////////


// ���O�̎�ނ��擾
CChatLog::LOG_TYPE CChatLog::getLogType(void) const
{
    return m_type;
}

// �{���ɑΉ�����j�b�N�l�[�������m��
bool CChatLog::isUnknownNick(const wxString& name) const
{
    return false;
}

// �j�b�N�l�[�����X�V����
void CChatLog::updateNick(const wxString& nick)
{
}
