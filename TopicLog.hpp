#pragma once
#include "ChatLog.hpp"
#include "ChannelData.hpp"

// �g�s�b�N�ύX�̃��O
class CTopicLog : public CChatLog
{
private:
    CChannelData* m_channel;

public:
    CTopicLog(void);
    ~CTopicLog(void);

    // ���������s��
    void init(const CChannelData& channel);

    // ���O���擾����
    CChannelData getLog(void) const;
};

