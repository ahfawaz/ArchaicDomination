/**************************************************
|	File:		Message.h
|	Author:		Evan Murray
|	Course:		1308
|	Purpose:	CMessage class is the concrete base class
|				for messages sent to the Message System.
**************************************************/

#ifndef MESSAGE_H
#define MESSAGE_H


#include "SGD Wrappers/IMessage.h"


/*************************************************/
// Enumeration of Message Types
enum MsgType  {  MSG_CREATE_ABILITY, MSG_DESTROY_ENTITY, MSG_CREATE_ABILITYTWO,  MSG_DESTROY_ABILITY, MSG_DESTROY_DARKCURSE, 
	MSG_CLEAVE,MSG_HEALINGCIRCLE, MSG_BLIZZARD, MSG_DESTROY_GHOUL, MSG_CATAPULT_ATTACK, };


/*************************************************/
// CMessage class
//	- concrete base class for messages
//	- stores the message ID
class CMessage : public IMessage
{
public:
	///////////////////////////////////////////////
	// Constructor REQUIRES the message type
	CMessage( MsgType type )				{	m_eMsgType = type; 	}
	virtual ~CMessage( void )					{	}
	
	///////////////////////////////////////////////
	// Accessor
	virtual MSGID GetMessageID( void ) /*final*/	{	return m_eMsgType;	}

private:
	///////////////////////////////////////////////
	// Message ID
	MsgType		m_eMsgType;
};

#endif //MESSAGE_H
