#pragma once
#include "ChatLog.hpp"
#include "MessageData.hpp"

// ���b�Z�[�W�̃��O
class CMessageLog : public CChatLog
{
private:
    CMessageData* m_message;
    wxString m_nick;

public:
    CMessageLog(void);
    ~CMessageLog(void);

    // ���������s��
    void init(const CMessageData& message, const wxString& nick);

    // ���O���擾����
    CMessageData getLog(void) const;

    // �j�b�N�l�[�����擾����
    wxString getNickName(void) const;

    // �{���ɑΉ�����j�b�N�l�[�������m��
    bool isUnknownNick(const wxString& name) const;

    // �j�b�N�l�[�����X�V����
    void updateNick(const wxString& nick);
};

