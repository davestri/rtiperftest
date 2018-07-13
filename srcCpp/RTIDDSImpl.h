#ifndef __RTIDDSIMPL_H__
#define __RTIDDSIMPL_H__

/*
 * (c) 2005-2017  Copyright, Real-Time Innovations, Inc. All rights reserved.
 * Subject to Eclipse Public License v1.0; see LICENSE.md for details.
 */

#include <string>
#include <algorithm>
#include <map>
#include "MessagingIF.h"
#include "perftestSupport.h"
#include "PerftestTransport.h"
#include "RTIDDSLoggerDevice.h"

#define RTIPERFTEST_MAX_PEERS 1024

/* Class for the DDS_DynamicDataMemberId of the type of RTI Perftest*/
class DynamicDataMembersId
{
  private:
    std::map<std::string, int> membersId;
    DynamicDataMembersId();

  public:
    ~DynamicDataMembersId();
    static DynamicDataMembersId &GetInstance();
    int at(std::string key);
};

template <typename T>
class RTIDDSImpl : public IMessaging
{
  public:

    RTIDDSImpl() :
        _transport(),
        _loggerDevice()
    {
        _IsMulticast = false;
        _InstanceMaxCountReader = DDS_LENGTH_UNLIMITED;
        _InstanceHashBuckets = -1;
        _KeepDurationUsec = -1;
        _IsDebug = false;
        _isLargeData = false;

      #ifdef RTI_SECURE_PERFTEST
        _secureUseSecure = false;
      #endif

        _HeartbeatPeriod.sec = 0;
        _HeartbeatPeriod.nanosec = 0;
        _FastHeartbeatPeriod.sec = 0;
        _FastHeartbeatPeriod.nanosec = 0;

        _factory = NULL;
        _participant = NULL;
        _subscriber = NULL;
        _publisher = NULL;
        _reader = NULL;
        _typename = T::TypeSupport::get_type_name();

        _pongSemaphore = NULL;
    }

    ~RTIDDSImpl()
    {
        Shutdown();
    }

    void PrintCmdLineHelp();

    bool ParseConfig(int argc, char *argv[]);

    std::string PrintConfiguration();

    bool Initialize(int argc, char *argv[]);

    void Shutdown();

    unsigned long GetInitializationSampleCount();

    IMessagingWriter *CreateWriter(const char *topic_name);

    // Pass null for callback if using IMessagingSubscriber.ReceiveMessage()
    // to get data
    IMessagingReader *CreateReader(const char *topic_name, IMessagingCB *callback);

    DDSTopicDescription *CreateCft(const char *topic_name, DDSTopic *topic);


  private:

    // Specific functions to configure the Security plugin
  #ifdef RTI_SECURE_PERFTEST
    bool configureSecurePlugin(DDS_DomainParticipantQos& dpQos);
    std::string printSecureArgs();
    bool validateSecureArgs();
  #endif

    bool         _IsMulticast;
    long _InstanceMaxCountReader;
    int          _InstanceHashBuckets;
    int          _KeepDurationUsec;
    bool         _IsDebug;
    bool         _isLargeData;
    PerftestTransport _transport;

  #ifdef RTI_SECURE_PERFTEST
    bool _secureUseSecure;

    static const std::string SECURE_PRIVATEKEY_FILE_PUB;
    static const std::string SECURE_PRIVATEKEY_FILE_SUB;
    static const std::string SECURE_CERTIFICATE_FILE_PUB;
    static const std::string SECURE_CERTIFICATE_FILE_SUB;
    static const std::string SECURE_CERTAUTHORITY_FILE;
    static const std::string SECURE_PERMISION_FILE_PUB;
    static const std::string SECURE_PERMISION_FILE_SUB;
    static const std::string SECURE_LIBRARY_NAME;
  #endif

    DDS_Duration_t   _HeartbeatPeriod;
    DDS_Duration_t   _FastHeartbeatPeriod;

    DDSDomainParticipantFactory *_factory;
    DDSDomainParticipant        *_participant;
    DDSSubscriber               *_subscriber;
    DDSPublisher                *_publisher;
    DDSDataReader               *_reader;
    const char                  *_typename;

    RTIOsapiSemaphore *_pongSemaphore;
    RTIDDSLoggerDevice _loggerDevice;
};


#endif // __RTIDDSIMPL_H__

