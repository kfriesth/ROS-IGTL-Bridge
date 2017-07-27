/*=========================================================================

  Program:   Converter Class for String
  Language:  C++

  Copyright (c) Brigham and Women's Hospital. All rights reserved.

  This software is distributed WITHOUT ANY WARRANTY; without even
  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
  PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#include "message_converter_string.h"
#include "ros/ros.h"
#include "igtlStringMessage.h"

MessageConverterString::MessageConverterString()
  : MessageConverterBase<ros_igtl_bridge::igtlstring>()
{
}

MessageConverterString::MessageConverterString(ros::NodeHandle *nh)
  : MessageConverterBase<ros_igtl_bridge::igtlstring>(nh)
{
}

MessageConverterString::MessageConverterString(const char* topicPublish, const char* topicSubscribe, ros::NodeHandle *nh)
  : MessageConverterBase<ros_igtl_bridge::igtlstring>(topicPublish, topicSubscribe, nh)
{
}

int MessageConverterString::onIGTLMessage(igtl::MessageHeader * header)
{
  // Create a message buffer to receive string data
  igtl::StringMessage::Pointer stringMsg;
  stringMsg = igtl::StringMessage::New();
  stringMsg->SetMessageHeader(header);
  stringMsg->AllocatePack();

  // Receive string data from the socket
  socket->Receive(stringMsg->GetPackBodyPointer(), stringMsg->GetPackBodySize());

  int b = stringMsg->Unpack(1);
  ros_igtl_bridge::igtlstring msg;
	
  if (b & igtl::MessageHeader::UNPACK_BODY) 
    {
    //std::cout<< "Received String: "<<stringMsg->GetString()<<std::endl;     
    msg.name = stringMsg->GetDeviceName();
    msg.data = stringMsg->GetString();
    //string_pub.publish(msg);
    this->publisher.publish(msg);
    return 1;
    }
  else 
    {
    std::cerr << "CRC Check failed!" << std::endl;
    return 0;
    }
}

void MessageConverterString::onROSMessage(const ros_igtl_bridge::igtlstring::ConstPtr & msg)
{
  //  SendString(msg->name.c_str(), msg->data);
  igtl::StringMessage::Pointer stringMsg = igtl::StringMessage::New();
  stringMsg->SetDeviceName(msg->name.c_str());
  stringMsg->SetString((msg->data).c_str());  
  stringMsg->Pack();
  //std::cout<<stringmsg.c_str()<< " sent"<<std::endl;
  socket->Send(stringMsg->GetPackPointer(), stringMsg->GetPackSize());
}


