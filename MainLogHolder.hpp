#pragma once
#include "MessageLog.hpp"
#include "MemberLog.hpp"
#include "JoinLog.hpp"
#include "PartLog.hpp"
#include "TopicLog.hpp"
#include <vector>

// �`���b�g�̃��O��ێ�����N���X
class CMainLogHolder
{
private:
    std::vector<CChatLog*> m_logs; // ���O��ۑ�����
    static const int MAX_LENGTH = 10; //���O�̍ő��

public:
    CMainLogHolder(void);
    ~CMainLogHolder(void);

    // ���O�ꗗ���擾����
    std::vector<CChatLog*> getLogs(void) const;

    // ���b�Z�[�W���O��ǉ�����
    void pushMessageLog(const CMessageData& message, const wxString& nick = "");

    // �`�����l���Q�����O��ǉ�����
    void pushJoinLog(const CSubscribeData& sub, const wxString& nick = "");

    // �`�����l���ޏo���O��ǉ�����
    void pushPartLog(const CSubscribeData& sub, const wxString& nick = "");

    // �j�b�N�l�[���ύX���O��ǉ�����
    void pushChangeNickLog(const CMemberData& member);

    // �g�s�b�N�ύX���O��ǉ�����
    void pushTopicLog(const CChannelData& channel);

    // �j�b�N�l�[���̍X�V���s��
    void onUpdateNickName(const CMemberData& member);

private:

    // ���O��ǉ�����
    void pushLog(CChatLog* log);
};

