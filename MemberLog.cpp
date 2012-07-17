#include "MemberLog.hpp"


CMemberLog::CMemberLog(void) : m_member(NULL)
{
}


CMemberLog::~CMemberLog(void)
{
    delete m_member;
}


//////////////////////////////////////////////////////////////////////


// ���������s��
void CMemberLog::init(const CMemberData& member)
{
    if (m_member == NULL){
        m_member = new CMemberData();
    }
    *m_member = member;

    // �^�C�v�̎w��
    m_type = LOG_USER;
}

// ���O���擾����
CMemberData CMemberLog::getLog(void) const
{
    return *m_member;
}