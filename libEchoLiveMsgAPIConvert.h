/**
* @file libEchoLiveMsgAPIConvert.h
 * @brief Echo-Live消息API导出头文件
 * @details Echo-Live-Msg-API-Convert导出函数
 * @author 星羽筱夜<AuraElicase>
 * @date 2024-09-25
 * @version 1.0.0
 * @copyright Copyright (c) 2024 AuraElicase
 */


#ifndef LIB_ECHO_LIVE_MSG_API_CONVERT_H
#define LIB_ECHO_LIVE_MSG_API_CONVERT_H

#ifdef __cplusplus
extern "C" {
#endif

 /**
  * @brief C字符串格式化Json
  * @param origin 需要处理的原始字符串
  * @return 处理后的Json字符串
  */
__declspec(dllexport) const char* generateMsgsC(const char* origin);

 /**
 * @brief 消息内存回收
 * @param msg 需要释放的消息地址
 */
__declspec(dllexport) void freeMsg(const char* msg);

#ifdef __cplusplus
}
#endif

#endif // LIB_ECHO_LIVE_MSG_API_CONVERT_H
