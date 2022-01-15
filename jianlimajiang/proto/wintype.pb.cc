// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: wintype.proto

#define INTERNAL_SUPPRESS_PROTOBUF_FIELD_DEPRECATION
#include "wintype.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)

namespace proto {
namespace game {
namespace wintype {

namespace {

const ::google::protobuf::EnumDescriptor* WinType_descriptor_ = NULL;

}  // namespace


void protobuf_AssignDesc_wintype_2eproto() {
  protobuf_AddDesc_wintype_2eproto();
  const ::google::protobuf::FileDescriptor* file =
    ::google::protobuf::DescriptorPool::generated_pool()->FindFileByName(
      "wintype.proto");
  GOOGLE_CHECK(file != NULL);
  WinType_descriptor_ = file->enum_type(0);
}

namespace {

GOOGLE_PROTOBUF_DECLARE_ONCE(protobuf_AssignDescriptors_once_);
inline void protobuf_AssignDescriptorsOnce() {
  ::google::protobuf::GoogleOnceInit(&protobuf_AssignDescriptors_once_,
                 &protobuf_AssignDesc_wintype_2eproto);
}

void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
}

}  // namespace

void protobuf_ShutdownFile_wintype_2eproto() {
}

void protobuf_AddDesc_wintype_2eproto() {
  static bool already_here = false;
  if (already_here) return;
  already_here = true;
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
    "\n\rwintype.proto\022\022proto.game.wintype*\321\t\n\007"
    "WinType\022\020\n\014WINTYPE_LOSE\020\000\022\032\n\026WINTYPE_JIU"
    "LIANBAODENG\020\001\022\022\n\016WINTYPE_DASIXI\020\002\022\024\n\020WIN"
    "TYPE_XIAOSIXI\020\003\022\025\n\021WINTYPE_DASANYUAN\020\004\022\022"
    "\n\016WINTYPE_SIANKE\020\005\022\027\n\023WINTYPE_SHIBALUOHA"
    "N\020\006\022\026\n\022WINTYPE_QINGYAOJIU\020\007\022\022\n\016WINTYPE_Z"
    "IYISE\020\010\022\027\n\023WINTYPE_XIAOSANYUAN\020\t\022\025\n\021WINT"
    "YPE_HUNYAOJIU\020\n\022\023\n\017WINTYPE_QINGDUI\020\013\022\023\n\017"
    "WINTYPE_QINPENG\020\014\022\023\n\017WINTYPE_HUNPENG\020\r\022\024"
    "\n\020WINTYPE_QINGYISE\020\016\022\023\n\017WINTYPE_HUNYISE\020"
    "\017\022\026\n\022WINTYPE_PENGPENGHU\020\020\022\022\n\016WINTYPE_PIN"
    "GHU\020\021\022\020\n\014WINTYPE_JIHU\020\022\022\022\n\016WINTYPE_TIANH"
    "U\020\023\022\020\n\014WINTYPE_DIHU\020\024\022\021\n\rWINTYPE_RENHU\020\025"
    "\022\027\n\023WINTYPE_QIANGGANGHU\020\026\022\027\n\023WINTYPE_HAI"
    "DILAOYUE\020\027\022\030\n\024WINTYPE_GANGSHANGHUA\020\030\022\025\n\021"
    "WINTYPE_LONGQIDUi\020\031\022\031\n\025WINTYPE_QINGLONGQ"
    "IDUI\020\032\022\024\n\020WINTYPE_QINQIDUI\020\033\022\026\n\022WINTYPE_"
    "QUANQIUREN\020\034\022\030\n\024WINTYPE_GANGSHANGPAO\020\035\022\023"
    "\n\017WINTYPE_MENQING\020\036\022\024\n\020WINTYPE_FENGYISE\020"
    "\037\022\033\n\027WINTYPE_MENQINGQINGYISE\020 \022\026\n\022WINTYP"
    "E_QINGSIANKE\020!\022\023\n\017WINTYPE_HAIDPAI\020\"\022\025\n\021W"
    "INTYPE_SHISANYAO\020#\022\021\n\rWINTYPE_QIDUI\020$\022\025\n"
    "\021WINTYPE_DAIYAOJIU\020%\022\024\n\020WINTYPE_JIANGDUI"
    "\020&\022\026\n\022WINTYPE_YITIAOLONG\020\'\022\031\n\025WINTYPE_QI"
    "NYITIAOTONG\020(\022\022\n\016WINTYPE_4JOKER\020)\022\023\n\017WIN"
    "TYPE_NOJOKER\020*\022\032\n\026WINTYPE_QINGQUANQIUREN"
    "\020+\022\032\n\025WINTYPE_CHANGSHA_SIXI\020\200\004\022\036\n\031WINTYP"
    "E_CHANGSHA_BANBANHU\020\201\004\022\036\n\031WINTYPE_CHANGS"
    "HA_QUEYIMEN\020\202\004\022 \n\033WINTYPE_CHANGSHA_LIULI"
    "USHUN\020\203\004\022 \n\033WINTYPE_CHANGSHA_PENGPENGHU\020"
    "\204\004\022\"\n\035WINTYPE_CHANGSHA_JIANGJIANGHU\020\205\004\022\037"
    "\n\032WINTYPE_CHENZHOU_SIZHONGHU\020\206\004", 1271);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "wintype.proto", &protobuf_RegisterTypes);
  ::google::protobuf::internal::OnShutdown(&protobuf_ShutdownFile_wintype_2eproto);
}

// Force AddDescriptors() to be called at static initialization time.
struct StaticDescriptorInitializer_wintype_2eproto {
  StaticDescriptorInitializer_wintype_2eproto() {
    protobuf_AddDesc_wintype_2eproto();
  }
} static_descriptor_initializer_wintype_2eproto_;
const ::google::protobuf::EnumDescriptor* WinType_descriptor() {
  protobuf_AssignDescriptorsOnce();
  return WinType_descriptor_;
}
bool WinType_IsValid(int value) {
  switch(value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
    case 26:
    case 27:
    case 28:
    case 29:
    case 30:
    case 31:
    case 32:
    case 33:
    case 34:
    case 35:
    case 36:
    case 37:
    case 38:
    case 39:
    case 40:
    case 41:
    case 42:
    case 43:
    case 512:
    case 513:
    case 514:
    case 515:
    case 516:
    case 517:
    case 518:
      return true;
    default:
      return false;
  }
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace wintype
}  // namespace game
}  // namespace proto

// @@protoc_insertion_point(global_scope)