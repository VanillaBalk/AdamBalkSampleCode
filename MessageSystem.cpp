#include "MessageSystem.h"
#include <algorithm>
#include <chrono>

using namespace std::chrono; // high_resolution_clock

MessageObject MessageSystem::Receive(std::string msgType)
{
	// Using unique lock for safety -- will free mutex in case of exception throw
	std::unique_lock<std::mutex> msgMutex(mtx);
	MessageObject msg = MessageMap[msgType].front();
	MessageMap[msgType].pop_front();
	msg.SetDeliveryStatus(true); // Message has been delivered
	return msg;
}
	
bool MessageSystem::IsMessageListEmpty(std::string msgType)
{
	return MessageMap[msgType].empty();
}

void MessageSystem::AddValidMessageType(std::string newMsgType)
{
	ValidMessageTypes.push_back(newMsgType);
}

void MessageSystem::Send(MessageObject msg)
{
	// Using unique lock for safety -- will free mutex in case of exception throw
	std::unique_lock<std::mutex> msgMutex(mtx);
	MessageMap[msg.GetName()].push_back(msg);
}

void MessageSystem::Update(void)
{
	// Iterate through all messages waiting to be sent
	for (unsigned i = 0; i < WaitingList.size(); ++i)
	{
		// If enough time has passed for according to the time stamp
		if ((duration_cast<milliseconds>(high_resolution_clock::now() - WaitingList[i].timeStamp).count()) >= WaitingList[i].delayTime)
		{
			// Send the to the Map to be received by the proper system
			Send(WaitingList[i].msg);
			// Remove the message from the Waiting Queue
			WaitingList.erase(WaitingList.begin() + i);
		}
	}
}
