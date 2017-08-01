#pragma once

#include <mutex>
#include <deque>
#include <unordered_map>
#include <string>
#include <vector> 
#include <algorithm> 
#include <iostream>

#include <deque>
#include "Message.h"

using namespace std::chrono;                   // high_resolution_clock
typedef std::deque<MessageObject> MessageList; // Map value in MessageMap

// Internally used in MessageSystem class to unify messages and time infoSendDelayedMessage
struct WaitingMessage
{
	WaitingMessage(MessageObject _msg, high_resolution_clock::time_point _timeStamp, double _delayTime) 
	: msg(_msg), timeStamp(_timeStamp), delayTime(_delayTime) {}
	MessageObject msg;							 // Message to be delivered
	high_resolution_clock::time_point timeStamp; // Time message was sent to the WaitingList
	double delayTime;							 // Time to wait before sending message to MessageMap
};

class MessageSystem
{
public:

	// Singleton for MessageSystem
	static MessageSystem& Instance() 
	{ 
		static MessageSystem Instance;
		return Instance;
	};

	// Sends a message to the MessageMap data structure
	template <typename T>
	void Send(T dataToSend, std::string msgType); 

	// Sends a message to the MessageMap data structure after delayTime (ms) has elapsed
	template <typename T>
	void DelayedSend(T dataToSend, std::string msgType, double delayTime);

	// Grabs a message of msgType from the MessageMap data structure
	MessageObject Receive(std::string msgType);

	// Helper -- appends newMsgType to ValidMessageTypes
	void AddValidMessageType(std::string newMsgType);

	// returns true if MessageList of msgType is empty
	bool IsMessageListEmpty(std::string msgType);

	// MessageSystem Update loop -- handles timing for delayed messages
	void Update(void);

private:

	// Singleton
	MessageSystem(){};

	// For internal use in DelayedSend; not to be called externally
	void Send(MessageObject msg); 

	// key = string indicating messageType of vector of MessageObjects, map = dequeue of MessageObjects                      
	std::unordered_map<std::string, MessageList> MessageMap;
	std::mutex mtx;								// Mutex used for keeping the messaging system thread-safe
	std::vector<std::string> ValidMessageTypes; // List of all valid message types for sending
	std::vector<WaitingMessage> WaitingList;   	// List of delayed Messages waiting to be sent
}; 

// Template implementation for Send
template <typename T>
void MessageSystem::Send(T dataToSend, std::string msgType)
{
	// Check to make sure the user-defined msgType is a valid, registered type before sending
	if (std::find(ValidMessageTypes.begin(), ValidMessageTypes.end(), msgType) == ValidMessageTypes.end())
	{
		std::cout << msgType << " is not a valid message type" << std::endl;
		std::cout << "Message not sent" << std::endl;
		return;
	}

	// Construct our DataObject for sending
	MessageData msgData(dataToSend);
	MessageObject msg(msgType, msgData);

	// Using unique lock for safety -- will free mutex in case of exception throw
	std::unique_lock<std::mutex> msgMutex(mtx);
	MessageMap[msg.GetName()].push_back(msg);
}

// Template implementation for DelayedSend
template <typename T>
void MessageSystem::DelayedSend(T dataToSend, std::string msgType, double delayTime)
{
	// Check to make sure the user-defined msgType is a valid & registered type before sending
	if (std::find(ValidMessageTypes.begin(), ValidMessageTypes.end(), msgType) == ValidMessageTypes.end())
	{
		std::cout << msgType << " is not a valid message type" << std::endl;
		std::cout << "Message not sent" << std::endl;
		return;
	}

	// Construct our DataObject to send to the waiting queue
	MessageData msgData(dataToSend);
	MessageObject msg(msgType, msgData);

	// Construct the message to be pushed to the WaitingList
	WaitingMessage delayedMsg(msg, std::chrono::high_resolution_clock::now(), delayTime);
	WaitingList.push_back(delayedMsg);
}
