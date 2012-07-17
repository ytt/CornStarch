#pragma once
#include <wx/string.h>

// �`���b�g�̃��O���Ǘ�
class CChatLog
{
public:
    // �`���b�g���O�̃^�C�v
    enum LOG_TYPE{
        LOG_MESSAGE,
        LOG_JOIN,
        LOG_PART,
        LOG_TOPIC,
        LOG_USER,
    };

protected:
    LOG_TYPE m_type; // ���O�̃^�C�v

public:
    CChatLog(void);
    virtual ~CChatLog(void);

    // ���O�̎�ނ��擾
    LOG_TYPE getLogType(void) const;

    // �{���ɑΉ�����j�b�N�l�[�������m��
    virtual bool isUnknownNick(const wxString& name) const;

    // �j�b�N�l�[�����X�V����
    virtual void updateNick(const wxString& nick);
};

