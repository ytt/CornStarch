#include "TopicLog.hpp"


CTopicLog::CTopicLog(void) : m_channel(NULL)
{
}


CTopicLog::~CTopicLog(void)
{
    delete m_channel;
}


//////////////////////////////////////////////////////////////////////


// ���������s��
void CTopicLog::init(const CChannelData& channel)
{
    if (m_channel == NULL){
        m_channel = new CChannelData();
    }
    *m_channel = channel;

    // �^�C�v�̎w��
    m_type = LOG_TOPIC;
}

// ���O���擾����
CChannelData CTopicLog::getLog(void) const
{
    return *m_channel;
}