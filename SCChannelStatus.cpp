#include "SCChannelStatus.hpp"

using namespace std;

CSCChannelStatus::CSCChannelStatus(void)
{
}


CSCChannelStatus::~CSCChannelStatus(void)
{
    delete m_messages;
    delete m_members;
}


//////////////////////////////////////////////////////////////////////


// ���������s��
void CSCChannelStatus::init(void)
{
    m_messages = new CMessageVec();
    m_members = new CMemberVec();
}

// �g�s�b�N���Z�b�g����
void CSCChannelStatus::setTopic(const wxString& topic)
{
    m_topic = topic;
}

// �g�s�b�N���擾����
wxString CSCChannelStatus::getTopic(void) const
{
    return m_topic;
}

// ���b�Z�[�W�ꗗ���擾����
vector<CMessageData*> CSCChannelStatus::getMessages(void) const
{
    return *m_messages;
}

// �����o�[�ꗗ���擾����
vector<CMemberData*> CSCChannelStatus::getMembers(void) const
{
    return *m_members;
}

// ���b�Z�[�W��ǉ�����
void CSCChannelStatus::pushMessage(const CMessageData& message)
{
    m_messages->push(message);
}

// �����o�[��ǉ�����
void CSCChannelStatus::pushMember(const CMemberData& member)
{
    m_members->push(member);
}

// ���b�Z�[�W�ꗗ���Z�b�g����
void CSCChannelStatus::setMessages(const vector<CMessageData*>& messages)
{
    m_messages->setMessages(messages);
}

// �����o�[�ꗗ���Z�b�g����
void CSCChannelStatus::setMembers(const vector<CMemberData*>& members)
{
    m_members->setMembers(members);
}

// ���b�Z�[�W���擾���I������
bool CSCChannelStatus::hasReceivedMessage(void) const
{
    return m_messages->hasSetMessage();
}

// �����o�[���擾���I������
bool CSCChannelStatus::hasReceivedMember(void) const
{
    return m_members->hasSetMember();
}

// ID�s�����������e���e�̃��b�Z�[�W�����邩
bool CSCChannelStatus::hasSameMessage(const CMessageData& message) const
{
    return m_messages->hasSameMessage(message);
}

// �������e�̃��b�Z�[�W�ɂ���ID���X�V
void CSCChannelStatus::updateMessageId(const CMessageData& message)
{
    if (!m_messages->hasSameMessage(message)){
        return;
    }
    m_messages->updateMessageId(message);
}

// �����o�[�����X�V����
void CSCChannelStatus::updateMember(const CMemberData& member)
{
    m_members->updateStatus(member);
}

// �����o�[��������
void CSCChannelStatus::popMember(const wxString& userName)
{
    m_members->pop(userName);
}
