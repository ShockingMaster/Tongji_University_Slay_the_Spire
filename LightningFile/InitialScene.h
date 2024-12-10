#pragma once
#ifndef _INITIAL_SCENE_H_
#define _INITIAL_SCENE_H_

#include "cocos2d.h"

/*
 * Class Name:     InitialScene
 * Class Function: 初始场景类
 */
class InitialScene : public cocos2d::Scene {
public:
    /**
     * 创建并返回一个场景对象。
     * @return 一个指向 cocos2d::Scene 的指针，表示该初始场景。
     */
    static cocos2d::Scene* createScene();

    /**
     * 初始化场景。
     * @return 如果初始化成功返回 true，否则返回 false。
     */
    virtual bool init();

    /**
     * 使用 CREATE_FUNC 宏生成 create 方法。
     * CREATE_FUNC 是 cocos2d-x 提供的宏，用于简化对象创建过程。
     */
    CREATE_FUNC(InitialScene);

private:
    /**
     * 检查单个字符的合法性。
     * @param ch 要检查的字符（支持 Unicode 编码）。
     * @return 如果字符合法返回 true，否则返回 false。
     * @note 可以根据需求定义合法字符的范围，比如只允许字母、数字等。
     */
    bool isValidCharacter(const char32_t ch);

    /**
     * 检查字符串的合法性。
     * @param str 要检查的字符串。
     * @return 如果字符串合法返回 true，否则返回 false。
     * @note 该方法可能调用 isValidCharacter 逐个检查字符串中的每个字符。
     */
    bool isValidString(const std::string& str);
};

#endif // !1