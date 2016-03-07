// Generated by gencpp from file vrep_common/simRosSetJointForce.msg
// DO NOT EDIT!


#ifndef VREP_COMMON_MESSAGE_SIMROSSETJOINTFORCE_H
#define VREP_COMMON_MESSAGE_SIMROSSETJOINTFORCE_H

#include <ros/service_traits.h>


#include <vrep_common/simRosSetJointForceRequest.h>
#include <vrep_common/simRosSetJointForceResponse.h>


namespace vrep_common
{

struct simRosSetJointForce
{

typedef simRosSetJointForceRequest Request;
typedef simRosSetJointForceResponse Response;
Request request;
Response response;

typedef Request RequestType;
typedef Response ResponseType;

}; // struct simRosSetJointForce
} // namespace vrep_common


namespace ros
{
namespace service_traits
{


template<>
struct MD5Sum< ::vrep_common::simRosSetJointForce > {
  static const char* value()
  {
    return "fbe07bacf4f20c3b14d370f9a1bdae4e";
  }

  static const char* value(const ::vrep_common::simRosSetJointForce&) { return value(); }
};

template<>
struct DataType< ::vrep_common::simRosSetJointForce > {
  static const char* value()
  {
    return "vrep_common/simRosSetJointForce";
  }

  static const char* value(const ::vrep_common::simRosSetJointForce&) { return value(); }
};


// service_traits::MD5Sum< ::vrep_common::simRosSetJointForceRequest> should match 
// service_traits::MD5Sum< ::vrep_common::simRosSetJointForce > 
template<>
struct MD5Sum< ::vrep_common::simRosSetJointForceRequest>
{
  static const char* value()
  {
    return MD5Sum< ::vrep_common::simRosSetJointForce >::value();
  }
  static const char* value(const ::vrep_common::simRosSetJointForceRequest&)
  {
    return value();
  }
};

// service_traits::DataType< ::vrep_common::simRosSetJointForceRequest> should match 
// service_traits::DataType< ::vrep_common::simRosSetJointForce > 
template<>
struct DataType< ::vrep_common::simRosSetJointForceRequest>
{
  static const char* value()
  {
    return DataType< ::vrep_common::simRosSetJointForce >::value();
  }
  static const char* value(const ::vrep_common::simRosSetJointForceRequest&)
  {
    return value();
  }
};

// service_traits::MD5Sum< ::vrep_common::simRosSetJointForceResponse> should match 
// service_traits::MD5Sum< ::vrep_common::simRosSetJointForce > 
template<>
struct MD5Sum< ::vrep_common::simRosSetJointForceResponse>
{
  static const char* value()
  {
    return MD5Sum< ::vrep_common::simRosSetJointForce >::value();
  }
  static const char* value(const ::vrep_common::simRosSetJointForceResponse&)
  {
    return value();
  }
};

// service_traits::DataType< ::vrep_common::simRosSetJointForceResponse> should match 
// service_traits::DataType< ::vrep_common::simRosSetJointForce > 
template<>
struct DataType< ::vrep_common::simRosSetJointForceResponse>
{
  static const char* value()
  {
    return DataType< ::vrep_common::simRosSetJointForce >::value();
  }
  static const char* value(const ::vrep_common::simRosSetJointForceResponse&)
  {
    return value();
  }
};

} // namespace service_traits
} // namespace ros

#endif // VREP_COMMON_MESSAGE_SIMROSSETJOINTFORCE_H