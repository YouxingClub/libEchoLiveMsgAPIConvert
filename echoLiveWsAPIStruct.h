/**
 * @file echoLiveWsAPIStruct.h
 * @brief Echo-Live Websocket API定义文件
 * @details 包含Echo-Live消息主类型和拓展类型\n
 *      参见：https://sheep-realms.github.io/Echo-Live-Doc/message/data/#image
 * @author 星羽筱夜<AuraElicase>
 * @date 2024-09-25
 * @version 1.0.0
 * @copyright Copyright (c) 2024 AuraElicase
 */

#ifndef ECHOLIVEWSAPISTRUCT_H
#define ECHOLIVEWSAPISTRUCT_H

#endif //ECHOLIVEWSAPISTRUCT_H

#include <string>
#include <optional>


/**
 * @struct echoLiveExternalData
 * @brief EchoLive扩展数据类型
 */
struct echoLiveExternalData
{
    struct image
    {
        std::string url; ///< 图片网络地址或者Base64数据 */
        struct margin
        {
            std::string left = "0em"; ///< 图片左外边距 */
            std::string right = "0em"; ///< 图片右外边距 */
        } margin; ///< 图片外边距 */
        std::string rendering;///< 重采样方式 */
        struct size
        {
            struct height
            {
                std::string max; ///< 最大值 */
                std::string min; ///< 最小值 */
            } height; ///< 高度 */
            struct width
            {
                std::string max; ///< 最大值 */
                std::string min; ///< 最小值 */
            } width; ///< 宽度 */
        } size; ///< 图片尺寸 */
    } image; ///< 图片结构 */
};

struct echoLiveAPIStyle
{
    std::string text; ///< 带样式原始文本 */
    struct style
    {
        bool bold = false; ///< 加粗 */
        bool italic = false; ///< 斜体 */
        bool underline = false; ///< 下划线 */
        bool strikethrough = false; ///< 删除线 */
        bool emphasis = false; ///< 着重号 */
        std::string color; ///< 字体颜色 */
        std::string backgroundColor; ///< 背景颜色 */
        std::string size; ///< 字号 */
        std::string weight; ///< 字重 */
        std::string letterSpacing; ///< 字间距 */
        std::string stretch; ///< 字形伸缩 */
        std::string css; ///< 自定义样式 */
    } style; ///< 文本样式 */
    std::optional<echoLiveExternalData> data = std::nullopt; ///< Echo-Live拓展类型 */
};