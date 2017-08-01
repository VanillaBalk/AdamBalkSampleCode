#pragma once

#include "Vector.h"
#include <cassert>
#include <string>

// Unique, user-defined Identifiers for messages
typedef std::string MessageName;

// Union of different valid data types to be used for message data payloads
union MessageDataUnion
{
	int intValue;
	float floatValue;
	char charValue;
	bool boolValue;
	void* objectValue;
	struct // nameless struct used for Vector data
	{	
		float x, y, z, w; 
	};
};

// Identifiers for valid data types of Messages
enum class MessageType
{
	MESSAGE_TYPE_ERR = -1,
	MESSAGE_TYPE_INT,
	MESSAGE_TYPE_FLOAT,
	MESSAGE_TYPE_CHAR,
	MESSAGE_TYPE_BOOL,
	MESSAGE_TYPE_OBJECT,
	MESSAGE_TYPE_VEC2,
	MESSAGE_TYPE_VEC3,
	MESSAGE_TYPE_VEC4
};

// Message data package to be stored in the message object sent between systems
class MessageData
{
public:

	// Constructors covering commonly used data types
	MessageData(){ mType = MessageType::MESSAGE_TYPE_ERR; }
	MessageData(const MessageData& data) = default; // C++11, use the default copy constructor (copies each element piece by piece)
	MessageData(int data)      { mType = MessageType::MESSAGE_TYPE_INT;    mData.intValue = data; }
	MessageData(float data)    { mType = MessageType::MESSAGE_TYPE_FLOAT;  mData.floatValue = data; }
	MessageData(char data) 	   { mType = MessageType::MESSAGE_TYPE_CHAR;   mData.charValue = data; }
	MessageData(bool data)     { mType = MessageType::MESSAGE_TYPE_BOOL;   mData.boolValue = data; }
	MessageData(void* data)    { mType = MessageType::MESSAGE_TYPE_OBJECT; mData.objectValue = data; }
	MessageData(Vector2f data) { mType = MessageType::MESSAGE_TYPE_VEC2;   mData.x = data.x; mData.y = data.y; }
	MessageData(Vector3f data) { mType = MessageType::MESSAGE_TYPE_VEC3;   mData.x = data.x; mData.y = data.y; mData.z = data.z; }
	MessageData(Vector4f data) { mType = MessageType::MESSAGE_TYPE_VEC4;   mData.x = data.x; mData.y = data.y; mData.z = data.z; mData.w = data.w; }
	~MessageData(){};
	
	// Boolean checks to examine data types of Messages
	bool IsValid() const    { return mType != MessageType::MESSAGE_TYPE_ERR; }
	bool IsInt() const      { return mType == MessageType::MESSAGE_TYPE_INT; }
	bool IsFloat() const    { return mType == MessageType::MESSAGE_TYPE_FLOAT; }
	bool IsChar() const     { return mType == MessageType::MESSAGE_TYPE_CHAR; }
	bool IsBool() const     { return mType == MessageType::MESSAGE_TYPE_BOOL; }
	bool IsObject() const   { return mType == MessageType::MESSAGE_TYPE_OBJECT; }
	bool IsVector2f() const { return mType == MessageType::MESSAGE_TYPE_VEC2; }
	bool IsVector3f() const { return mType == MessageType::MESSAGE_TYPE_VEC3; }
	bool IsVector4f() const { return mType == MessageType::MESSAGE_TYPE_VEC4; }

	// Getters for Message data (includes error checking asserts)
	int      GetInt() const       { assert(mType == MessageType::MESSAGE_TYPE_INT && "Message data is incorrect type"); return(mData.intValue); }
	float    GetFloat() const     { assert(mType == MessageType::MESSAGE_TYPE_FLOAT && "Message data is incorrect type"); return(mData.floatValue); }
	char     GetChar() const      { assert(mType == MessageType::MESSAGE_TYPE_CHAR && "Message data is incorrect type"); return(mData.charValue); }
	bool     GetBool() const      { assert(mType == MessageType::MESSAGE_TYPE_BOOL && "Message data is incorrect type"); return(mData.boolValue); }
	void*    GetObject() const    { assert(mType == MessageType::MESSAGE_TYPE_OBJECT && "Message data is incorrect type"); return(mData.objectValue); }
	Vector2f GetVector2f() const  { assert(mType == MessageType::MESSAGE_TYPE_VEC2 && "Message data is incorrect type"); return{ mData.x, mData.y }; }
	Vector3f GetVector3f() const  { assert(mType == MessageType::MESSAGE_TYPE_VEC2 && "Message data is incorrect type"); return{ mData.x, mData.y, mData.z }; }
	Vector4f GetVector4f() const  { assert(mType == MessageType::MESSAGE_TYPE_VEC2 && "Message data is incorrect type"); return{ mData.x, mData.y, mData.z, mData.w }; }

	// Get data type of message
	MessageType GetMessageType() { return mType;}
	
private:

	MessageType mType;      // indicates data type for message
	MessageDataUnion mData; // data union to write varying types of data to  
};

// Actual Message object sent between systems
class MessageObject
{
public:

	// Constructs an empty message with no name and no type 
	MessageObject();
	// Constructs a valid message given the constructor parameters
	MessageObject(MessageName name, const MessageData& data) : mName(name), mData(data), isDelivered(false){}

	// Get or Set name of the message 
	MessageName GetName(){ return mName; }
	void SetName(MessageName msgName){ mName = msgName; }

	// Get or Set the the delivery status of the message
	bool IsDelivered(){ return isDelivered; }
	void SetDeliveryStatus(bool deliveryStatus){ isDelivered = deliveryStatus; }

	// Boolean checks for message payload data types
	bool IsDataValid()    { return mData.IsValid(); }
	bool IsDataInt()      { return mData.IsInt(); }
	bool IsDataFloat()    { return mData.IsFloat(); }
	bool IsDataChar()     { return mData.IsChar(); }
	bool IsDataBool()     { return mData.IsBool(); }
	bool IsDataObject()   { return mData.IsObject(); }
	bool IsDataVector2f() { return mData.IsVector2f(); }
	bool IsDataVector3f() { return mData.IsVector3f(); }
	bool IsDataVector4f() { return mData.IsVector4f(); }

	// Getters and Setters for data payload of message
	int      GetDataInt()    { return mData.GetInt(); }
	float    GetDataFloat()  { return mData.GetFloat(); }
	char     GetDataChar()   { return mData.GetChar(); }
	bool     GetDataBool()   { return mData.GetBool(); }
	void*    GetDataObject() { return mData.GetObject(); }
	Vector2f GetDataVec2()   { return mData.GetVector2f(); }
	Vector3f GetDataVec3()   { return mData.GetVector3f(); }
	Vector4f GetDataVec4()   { return mData.GetVector4f(); }

private:

	MessageName mName;    // Identifier for message type
	MessageData mData;    // Data payload of message
	bool isDelivered;     // Has the message been delivered
};
