/*
 * @Descripttion: 配置系统 
 * @version: 
 * @Author: primoxu
 * @Date: 2021-10-01 13:26:41
 * @LastEditors: sueRimn
 * @LastEditTime: 2021-10-02 15:42:21
 */
#ifndef _PRIMO_CONFIG_H_
#define _PRIMO_CONFIG_H_

#include <memory>
#include <string>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <yaml-cpp/yaml.h>
#include <unordered_map>
#include <unordered_set>
#include <list>
#include <functional>

#include "log/Logger.h"
#include "util.h"


namespace primo
{

class CfgVarBase
{
public:
    typedef std::shared_ptr<CfgVarBase> ptr;
    CfgVarBase(const std::string& name, const std::string desc = "")
        : mName(name),
          mDesc(desc)
    {
        std::transform(mName.begin(), mName.end(), mName.begin(), ::tolower);
    }

    virtual ~CfgVarBase(){}

    const std::string& GetName() const
    {
        return mName;
    }

    const std::string& GetDesc() const
    {
        return mDesc;
    }

    virtual std::string ToString() = 0;
    virtual bool FromString(const std::string& val) = 0; 
    virtual std::string GetTypeName() const = 0;
protected:
    std::string mName;
    std::string mDesc;
};


/**
 * @brief 类型转换模板类(F 源类型, T 目标类型)
 */
template<class F, class T>
class LexicalCast 
{
public:
    /**
     * @brief 类型转换/
     * @param[in] v 源类型值
     * @return 返回v转换后的目标类型
     * @exception 当类型不可转换时抛出异常
     */
    T operator()(const F& v) 
    {
        return boost::lexical_cast<T>(v);
    }
};

/**
 * @brief 类型转换模板类片特化(YAML String 转换成 std::vector<T>)
 */
template<class T>
class LexicalCast<std::string, std::vector<T> > 
{
public:
    std::vector<T> operator()(const std::string& v) 
    {
        typename std::vector<T> vec;
        std::stringstream ss;
        YAML::Node node = YAML::Load(v);

        for (size_t i = 0; i < node.size(); ++i)
        {
            ss.str("");
            ss << node[i];
            vec.push_back(LexicalCast<std::string, T>()(ss.str()));
        }
        return vec;
    }
};

/**
 * @brief 类型转换模板类片特化(std::vector<T> 转换成 YAML String)
 */
template<class T>
class LexicalCast<std::vector<T>, std::string> {
public:
    std::string operator()(const std::vector<T>& v) {
        YAML::Node node(YAML::NodeType::Sequence);
        for(auto& i : v) {
            node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

/**
 * @brief 类型转换模板类片特化(YAML String 转换成 std::list<T>)
 */
template<class T>
class LexicalCast<std::string, std::list<T> > {
public:
    std::list<T> operator()(const std::string& v) {
        YAML::Node node = YAML::Load(v);
        typename std::list<T> vec;
        std::stringstream ss;
        for(size_t i = 0; i < node.size(); ++i) {
            ss.str("");
            ss << node[i];
            vec.push_back(LexicalCast<std::string, T>()(ss.str()));
        }
        return vec;
    }
};

/**
 * @brief 类型转换模板类片特化(std::list<T> 转换成 YAML String)
 */
template<class T>
class LexicalCast<std::list<T>, std::string> {
public:
    std::string operator()(const std::list<T>& v) {
        YAML::Node node(YAML::NodeType::Sequence);
        for (auto& i : v)
        {
            node.push_back( YAML::Load(LexicalCast<T, std::string>()(i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

/**
 * @brief 类型转换模板类片特化(YAML String 转换成 std::set<T>)
 */
template<class T>
class LexicalCast<std::string, std::set<T> > {
public:
    std::set<T> operator()(const std::string& v) {
        YAML::Node node = YAML::Load(v);
        typename std::set<T> vec;
        std::stringstream ss;
        for(size_t i = 0; i < node.size(); ++i) {
            ss.str("");
            ss << node[i];
            vec.insert(LexicalCast<std::string, T>()(ss.str()));
        }
        return vec;
    }
};

/**
 * @brief 类型转换模板类片特化(std::set<T> 转换成 YAML String)
 */
template<class T>
class LexicalCast<std::set<T>, std::string> {
public:
    std::string operator()(const std::set<T>& v) {
        YAML::Node node(YAML::NodeType::Sequence);
        for(auto& i : v) {
            node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

/**
 * @brief 类型转换模板类片特化(YAML String 转换成 std::unordered_set<T>)
 */
template<class T>
class LexicalCast<std::string, std::unordered_set<T> > 
{
public:
    std::unordered_set<T> operator()(const std::string& v) 
    {
        YAML::Node node = YAML::Load(v);
        typename std::unordered_set<T> vec;
        std::stringstream ss;
        for(size_t i = 0; i < node.size(); ++i) 
        {
            ss.str("");
            ss << node[i];
            vec.insert(LexicalCast<std::string, T>()(ss.str()));
        }
        return vec;
    }
};

/**
 * @brief 类型转换模板类片特化(std::unordered_set<T> 转换成 YAML String)
 */
template<class T>
class LexicalCast<std::unordered_set<T>, std::string> {
public:
    std::string operator()(const std::unordered_set<T>& v) {
        YAML::Node node(YAML::NodeType::Sequence);
        for(auto& i : v) {
            node.push_back(YAML::Load(LexicalCast<T, std::string>()(i)));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

/**
 * @brief 类型转换模板类片特化(YAML String 转换成 std::map<std::string, T>)
 */
template<class T>
class LexicalCast<std::string, std::map<std::string, T> > {
public:
    std::map<std::string, T> operator()(const std::string& v) {
        YAML::Node node = YAML::Load(v);
        typename std::map<std::string, T> vec;
        std::stringstream ss;
        for(auto it = node.begin();
                it != node.end(); ++it) {
            ss.str("");
            ss << it->second;
            vec.insert(std::make_pair(it->first.Scalar(),
                        LexicalCast<std::string, T>()(ss.str())));
        }
        return vec;
    }
};

/**
 * @brief 类型转换模板类片特化(std::map<std::string, T> 转换成 YAML String)
 */
template<class T>
class LexicalCast<std::map<std::string, T>, std::string> {
public:
    std::string operator()(const std::map<std::string, T>& v) {
        YAML::Node node(YAML::NodeType::Map);
        for(auto& i : v) {
            node[i.first] = YAML::Load(LexicalCast<T, std::string>()(i.second));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

/**
 * @brief 类型转换模板类片特化(YAML String 转换成 std::unordered_map<std::string, T>)
 */
template<class T>
class LexicalCast<std::string, std::unordered_map<std::string, T> > {
public:
    std::unordered_map<std::string, T> operator()(const std::string& v) {
        YAML::Node node = YAML::Load(v);
        typename std::unordered_map<std::string, T> vec;
        std::stringstream ss;
        for(auto it = node.begin();
                it != node.end(); ++it) {
            ss.str("");
            ss << it->second;
            vec.insert(std::make_pair(it->first.Scalar(),
                        LexicalCast<std::string, T>()(ss.str())));
        }
        return vec;
    }
};

/**
 * @brief 类型转换模板类片特化(std::unordered_map<std::string, T> 转换成 YAML String)
 */
template<class T>
class LexicalCast<std::unordered_map<std::string, T>, std::string> {
public:
    std::string operator()(const std::unordered_map<std::string, T>& v) {
        YAML::Node node(YAML::NodeType::Map);
        for(auto& i : v) {
            node[i.first] = YAML::Load(LexicalCast<T, std::string>()(i.second));
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};


template <class T, class FromStr = LexicalCast<std::string, T>, 
                   class ToStr = LexicalCast<T, std::string> >
class CfgVar : public CfgVarBase
{
public:
    typedef std::shared_ptr<CfgVar> ptr;
    typedef std::function<void (const T& old_value, const T& new_value)> OnChangeCallback;

    CfgVar(const std::string& name, 
            const T& default_val,
            const std::string desc = "")
        : CfgVarBase(name, desc),
          mVal(default_val){}

    std::string ToString() override
    {
        try
        {
            return ToStr()(mVal);
        }
        catch(const std::exception& e)
        {
            P_LOG_ERROR(P_LOG_ROOT()) << "CfgVar::ToString exception "
                << e.what() << " convert: " << TypeToName<T>() << " to string"
                << " name=" << mName;
        }
        return "";
    }

    bool FromString (const std::string& val) override
    {
        try
        {
            //std::cout << "to set the string value: " << val << " to config" << std::endl;
            SetValue(FromStr()(val));   
        }
        catch(const std::exception& e)
        {
            P_LOG_ERROR(P_LOG_ROOT()) << "CfgVar::FromString exception "
                << e.what() << " convert: string to " << TypeToName<T>()
                << " name=" << mName
                << " - " << val;
        }
        return false;
    }

    std::string GetTypeName() const override
    {
        return TypeToName<T>();
    }

    const T GetValue() const 
    {
        return mVal;
    }

    void SetValue(const T& val)
    {
        if (mVal == val)
        {
            return;
        }
        
        for (auto& cb : mCbs)
        {
            cb.second(mVal, val);
        }
        mVal = val;
    }

    uint64_t AddListener(OnChangeCallback cbfunc)
    {
        static uint64_t sFuncId= 0;
        ++sFuncId;
        mCbs[sFuncId] = cbfunc;
        return sFuncId;
    }

    void DelListener(uint64_t key)
    {
        mCbs.erase(key);
    }

    OnChangeCallback GetListener(uint64_t key)
    {
        return mCbs[key];
    }

    void ClearListener()
    {
        mCbs.clear();
    }
private:
    T mVal;
    std::map<uint64_t, OnChangeCallback> mCbs;
};


class Config
{ 
public:
    typedef std::unordered_map<std::string, CfgVarBase::ptr> CfgVarMap;

    /**
     * @brief 获取/创建对应参数名的配置参数
     * @param[in] name 配置参数名称
     * @param[in] default_value 参数默认值
     * @param[in] description 参数描述
     * @details 获取参数名为name的配置参数,如果存在直接返回
     *          如果不存在,创建参数配置并用default_value赋值
     * @return 返回对应的配置参数,如果参数名存在但是类型不匹配则返回nullptr
     * @exception 如果参数名包含非法字符[^0-9a-z_.] 抛出异常 std::invalid_argument
     */
    template <typename T>
    static typename CfgVar<T>::ptr LookUp(const std::string& name,
        const T& default_value, const std::string& desc = "")
    {
        //std::cout << " test trace" << std::endl;
        auto itr = GetDatas().find(name);
        if (itr != GetDatas().end())
        {
            auto tmp = std::dynamic_pointer_cast<CfgVar<T> >(itr->second);
            if (tmp)
            {
                P_LOG_INFO(P_LOG_ROOT()) << "Lookup name=" << name << " exists";
                return tmp;
            }
            else
            {
                P_LOG_ERROR(P_LOG_ROOT()) << "Lookup name=" << name << " exists but type not "
                        << TypeToName<T>() << " real_type=" << itr->second->GetTypeName()
                        << " " << itr->second->ToString();
                return nullptr;
            }
        }

        if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyz_.0123456789")
                != std::string::npos)
        {
            P_LOG_ERROR(P_LOG_ROOT()) << "Lookup name invalid";
            throw std::invalid_argument(name);
        }

        typename CfgVar<T>::ptr v(new CfgVar<T>(name, default_value, desc));
        GetDatas()[name] = v;
        return v; 
    }

    /**
     * @brief 查找配置参数
     * @param[in] name 配置参数名称
     * @return 返回配置参数名为name的配置参数
     */
    template<class T>
    static typename CfgVar<T>::ptr Lookup(const std::string& name) 
    {
        //RWMutexType::ReadLock lock(GetMutex());
        auto it = GetDatas().find(name);
        if(it == GetDatas().end())
        {
            return nullptr;
        }
        return std::dynamic_pointer_cast<CfgVar<T> >(it->second);
    }

    /**
     * @brief 查找配置参数
     * @param[in] name 配置参数名称
     * @return 返回配置参数名为name的配置参数
     */
    static CfgVarBase::ptr LookUpBase(const std::string& name) 
    {
        auto it = GetDatas().find(name);
        return it == GetDatas().end() ? nullptr : it->second;
    }

    static void LoadFromYaml(const YAML::Node& root);

private:
    static CfgVarMap& GetDatas()
    {
        static CfgVarMap sDatas;
        return sDatas;
    }
};


}

#endif