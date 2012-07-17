#pragma once
#include "ChatLog.hpp"
#include "MemberData.hpp"

// �����o�[���X�V���O
class CMemberLog : public CChatLog
{
private:
    CMemberData* m_member;

public:
    CMemberLog(void);
    ~CMemberLog(void);

    // ���������s��
    void init(const CMemberData& member);

    // ���O���擾����
    CMemberData getLog(void) const;
};

