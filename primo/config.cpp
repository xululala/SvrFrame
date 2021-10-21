/*
 * @Descripttion: 
 * @version: 
 * @Author: primo
 * @Date: 2021-10-02 15:33:12
 * @LastEditors: sueRimn
 * @LastEditTime: 2021-10-02 15:33:12
 */
#include "config.h"

namespace primo
{

static Logger::ptr g_logger = P_LOG_NAME("system");

static void ListAllMember(const std::string& prefix, const YAML::Node& node,
                    std::list<std::pair<std::string, const YAML::Node> >& output)
{
    if (prefix.find_first_not_of("abcdefghijklmnopqrstuvwxyz_.0123456789")
        != std::string::npos)
    {
        P_LOG_ERROR(g_logger) << "Config invalid name, prefix: " << prefix << " node: " << node;
        return;
    }
    output.push_back(std::make_pair(prefix, node));
    if (node.IsMap())
    {
        for (auto it = node.begin(); it != node.end(); ++it)
        {
            ListAllMember(prefix.empty()? it->first.Scalar() : prefix + "." + it->first.Scalar(), 
                it->second, output);
        }
    }

}

void Config::LoadFromYaml(const YAML::Node& root)
{
    std::list<std::pair<std::string, const YAML::Node> > AllNodes;
    ListAllMember("", root, AllNodes);

    for (auto& i : AllNodes)
    {
        std::string key = i.first;
        if (key.empty())
        {
            continue;
        }

        std::transform(key.begin(), key.end(), key.begin(), ::tolower);
        CfgVarBase::ptr var = LookUpBase(key);
        if (var)
        {
            if (i.second.IsScalar())
            {
                var->FromString(i.second.Scalar());
            }
            else
            {
                std::stringstream ss;
                ss << i.second;
                var->FromString(ss.str());
            }
        }
    }
}

}