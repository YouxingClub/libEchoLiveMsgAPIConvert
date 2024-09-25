/**
 * @file libEchoLiveMsgAPIConvert.cpp
 * @brief Echo-Live消息API处理函数
 * @details 处理Echo-Live API与字符串变量的转换
 * @author 星羽筱夜<AuraElicase>
 * @date 2024-09-25
 * @version 1.0.0
 * @copyright Copyright (c) 2024 AuraElicase
 */

#include <complex>
#include <iostream>
#include <string>
#include <vector>

#include "nlohmann/json.hpp"

#include "echoLiveWsAPIStruct.h"
#include "libEchoLiveMsgAPIConvert.h"

using json = nlohmann::json;


/**
 * @brief 序列化结构体为Json
 * @param jsonStr 需要生成的Json数据
 * @param style 需要序列化json的结构体
 */
void marshalJson(json &jsonStr, const echoLiveAPIStyle &style)
{
    try
    {
        json styleData;
        if (style.style.bold) styleData["bold"] = true;
        if (style.style.italic) styleData["italic"] = true;
        if (style.style.underline) styleData["underline"] = true;
        if (style.style.strikethrough) styleData["strikethrough"] = true;
        if (style.style.emphasis) styleData["emphasis"] = true;
        if (!style.style.color.empty()) styleData["color"] = style.style.color;
        if (!style.style.backgroundColor.empty()) styleData["backgroundColor"] = style.style.backgroundColor;
        if (!style.style.size.empty()) styleData["size"] = style.style.size;
        if (!style.style.weight.empty()) styleData["weight"] = style.style.weight;
        if (!style.style.letterSpacing.empty()) styleData["letterSpacing"] = style.style.letterSpacing;
        if (!style.style.stretch.empty()) styleData["stretch"] = style.style.stretch;
        if (!style.style.css.empty()) styleData["style"] = style.style.css;

        jsonStr["text"] = style.text;
        if (!styleData.empty()) jsonStr["style"] = styleData;

        if (style.data.has_value())
        {
            json externalData;
            externalData["image"]["url"] = style.data->image.url;
            jsonStr["data"] = externalData;
        }
    } catch (std::exception& e)
    {
        std::cout << e.what() << std::endl;
    }
}

/**
 * @brief 分割普通字符串与格式字符串
 * @param text 需要分割的原始文本
 * @param marker 格式字符串匹配前缀
 * @param markerEnd 格式字符串匹配后缀
 * @return 拆分后的普通字符串与格式字符串数组
 */
std::vector<std::string> splitOriginString(const std::string& text, const std::string& marker, const std::string& markerEnd)
{
    std::vector<std::string> results;
    size_t start = 0;

    while (true) {
        // 查找下一个标记的开始位置
        const size_t markerPos = text.find(marker, start);
        if (markerPos == std::string::npos) {
            // 如果没有找到标记，输出剩余部分
            results.push_back(text.substr(start));
            break;
        }

        // 输出标记之前的部分
        if (markerPos > start) {
            results.push_back(text.substr(start, markerPos - start));
        }

        // 找到标记的结束位置
        const size_t endPos = text.find(markerEnd, markerPos);
        if (endPos == std::string::npos) {
            // 如果没有找到结束标记，输出剩余部分并退出
            results.push_back(text.substr(markerPos));
            break;
        }

        // 输出匹配到的部分
        results.push_back(text.substr(markerPos, endPos - markerPos + 1));

        // 更新开始位置
        start = endPos + 1;
    }

    return results;
}

/**
 * @brief 拆分格式字符串
 * @param text 需要拆分的格式字符串
 * @param split 分隔符
 * @return 拆分后的模板内容数组
 */
std::vector<std::string> splitTemplateString(const std::string& text, const char split)
{
    std::vector<std::string> results;
    try
    {
        if (text.empty())
        {
            throw std::invalid_argument("empty string");
        }

        std::string text2 = text + split;
        size_t pos = text2.find(split);

        while (pos != std::string::npos)
        {
            std::string tmp = text2.substr(0, pos);
            results.push_back(tmp);
            text2 = text2.substr(pos + 1, text2.length());
            pos = text2.find(split);
        }
    } catch (std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    return results;
}

/**
 * @brief 分析模板字符串
 * @param text 原始模板字符串
 * @param marker 模板字符串匹配前缀
 * @return 拆分后的模板内容数组
 */
std::vector<std::string> analysisTemplateString(const std::string& text, const std::string& marker)
{
    if (const size_t markerPos = text.find(marker, 0); markerPos == std::string::npos) {
       throw std::invalid_argument("no string type template");
    }
    // substr拆除非必要内容
    const std::string validString = text.substr(marker.size(), text.size() - 1 - marker.size());
    std::vector<std::string> results = splitTemplateString(validString, ',');
    return results;
}

/**
 * @brief 格式化消息结构体
 * @param originText 需要格式化的文本
 * @param args 格式化参数数组
 * @param marker 格式化参数匹配前缀
 * @return 格式化后的结构体
 */
echoLiveAPIStyle generateMsgJsonStruct(const std::string& originText, const std::vector<std::string>& args, const std::string& marker)
{
    echoLiveAPIStyle style;
    style.text = originText;
    for (const auto& arg : args)
    {
        std::vector<std::string> styleList = analysisTemplateString(arg, marker);
        if (styleList[0] == "Format")
        {
            if (styleList[1] == "Boldface")
            {
                style.style.bold = true;
            } else if (styleList[1] == "Italics")
            {
                style.style.italic = true;
            } else if (styleList[1] == "Underline")
            {
                style.style.underline = true;
            } else if (styleList[1] == "Strikethrough")
            {
                style.style.strikethrough = true;
            } else if (styleList[1] == "Emphasis")
            {
                style.style.emphasis = true;
            } else if (styleList[1] == "Custom")
            {
                style.style.css = styleList[2];
            } else if (styleList[1] == "Reset")
            {
                style.style.bold = false;
                style.style.italic = false;
                style.style.underline = false;
                style.style.strikethrough = false;
                style.style.emphasis = false;
                style.style.css = "";
                style.style.color = "";
                style.style.backgroundColor = "";
                style.style.size = "";
                style.style.weight = "";
                style.style.letterSpacing = "";
                style.style.stretch = "";
                style.data = std::nullopt;

            } else
            {
                throw std::invalid_argument("invalid argument");
            }
        } else if (styleList[0] == "TypeSize")
        {
            style.style.size = styleList[1];
        } else if (styleList[0] == "TypeWeight")
        {
            style.style.weight = styleList[1];
        } else if (styleList[0] == "FontColor")
        {
            if (styleList[1] == "hex")
            {
                style.style.color = styleList[2];
            } else if (styleList[1] == "rgba")
            {
                std::string rgbaColor = "rgba(" + styleList[2] + "," + styleList[3] + "," + styleList[4] + ")";
                style.style.color = rgbaColor;
            } else
            {
                throw std::invalid_argument("invalid color type");
            }
        } else if (styleList[0] == "BgColor")
        {
            if (styleList[1] == "hex")
            {
                style.style.backgroundColor = styleList[2];
            } else if (styleList[1] == "rgba")
            {
                std::string rgbaColor = "rgba(" + styleList[2] + "," + styleList[3] + "," + styleList[4] + ")";
                style.style.backgroundColor = rgbaColor;
            } else
            {
                throw std::invalid_argument("invalid color type");
            }
        } else if (styleList[0] == "Image")
        {
            echoLiveExternalData eDataImage;
            eDataImage.image.url = styleList[1];
            // eDataImage.image.margin.left = "0em";
            // eDataImage.image.margin.right = "0em";
            // eDataImage.image.size.height.max = "5em";
            // eDataImage.image.size.height.min = "0em";
            // eDataImage.image.size.width.max = "5em";
            // eDataImage.image.size.width.min = "0em";
            style.data = eDataImage;

        } else if (styleList[0] == "Emoji")
        {
            echoLiveExternalData eDataEmoji;
            eDataEmoji.image.url = styleList[1];
            // eDataEmoji.image.margin.left = "0em";
            // eDataEmoji.image.margin.right = "0em";
            // eDataEmoji.image.size.height.max = "5em";
            // eDataEmoji.image.size.height.min = "0em";
            // eDataEmoji.image.size.width.max = "5em";
            // eDataEmoji.image.size.width.min = "0em";
            style.data = eDataEmoji;

        } else
        {
            throw std::invalid_argument("find unhandled expression:" + styleList[0]);
        }
    }
    return style;
}

/**
 * @brief 格式字符串列表处理Json
 * @param msgList 初次拆分后的普通字符串与格式字符串列表
 * @param msgsJson 需要返回的json
 * @param marker 格式字符串匹配前缀
 */
void generateMsgList(const std::vector<std::string>& msgList, json &msgsJson, const std::string& marker)
{
    msgsJson = {
        {"message", json::array()}
    };
    std::vector<std::string> markerTempData;
    // 循环读取拆分出来的数据
    for (const auto& arg : msgList)
    {
        if (arg.find(marker) != std::string::npos)
        {
            markerTempData.push_back(arg);
        }
        else
        {
            json msgJson;
            echoLiveAPIStyle styleData = generateMsgJsonStruct(arg, markerTempData, marker);
            marshalJson(msgJson, styleData);
            msgsJson["message"].push_back(msgJson);
        }
    }
}

/**
 * @brief C++字符串格式化Json
 * @param origin 需要处理的原始字符串
 * @return 处理后的Json字符串
 */
std::string generateMsgs(const std::string& origin)
{
    json a;
    const std::string marker = "[CHAT:";
    const std::string markerEnd = "]";
    const std::vector<std::string> result = splitOriginString(origin, marker, markerEnd);
    generateMsgList(result, a, marker);
    return to_string(a);
}

extern "C" {
    /**
     * @brief C字符串格式化Json
     * @param origin 需要处理的原始字符串
     * @return 处理后的Json字符串
     */
    __declspec(dllexport) const char *generateMsgsC(const char *origin)
    {
        const std::string result = generateMsgs(origin);
        char* cstr = new char[result.length() + 1];
        strcpy_s(cstr,strlen(result.c_str()) + 1, result.c_str());
        return cstr;
    }

    /**
     * @brief 消息内存回收
     * @param msg 需要释放的消息地址
     */
    __declspec(dllexport) void freeMsg(const char* msg)
    {
        delete[] msg; // 释放内存
    }
}