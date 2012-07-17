#pragma once
#include "ChatLog.hpp"
#include "SubscribeData.hpp"

// �`�����l�����E���O
class CPartLog : public CChatLog
{
private:
    CSubscribeData* m_sub;
    wxString m_nick;

public:
    CPartLog(void);
    ~CPartLog(void);

    // ���������s��
    void init(const CSubscribeData& sub, const wxString& nick);

    // ���O���擾����
    CSubscribeData getLog(void) const;

    // �j�b�N�l�[�����擾����
    wxString getNickName(void) const;

    // �{���ɑΉ�����j�b�N�l�[�������m��
    bool isUnknownNick(const wxString& name) const;

    // �j�b�N�l�[�����X�V����
    void updateNick(const wxString& nick);
};

