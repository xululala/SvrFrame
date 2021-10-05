/*
 * @Descripttion: 
 * @version: 
 * @Author: primoxu
 * @Date: 2021-10-02 15:30:37
 * @LastEditors: primoxu
 * @LastEditTime: 2021-10-02 15:31:59
 */

#include "../src/log/Logger.h"
#include "../src/config.h"

primo::CfgVar< std::vector<int> >::ptr test_vec_cfg = 
    primo::Config::LookUp("system.vec", std::vector<int>(2, 3), "test vec cfg");

primo::CfgVar< std::unordered_map<std::string, int> >::ptr test_map_cfg = 
    primo::Config::LookUp("system.map", std::unordered_map<std::string, int>{{"wu", 3}, {"xu", 4}}, "test map cfg");

//primo::CfgVar< std::vector<int> >::ptr test_cfg = 
    //primo::Config::LookUp("system.vec", std::vector<int>(2, 3), "test");

//primo::CfgVar< std::vector<int> >::ptr test_cfg = 
    //primo::Config::LookUp("system.vec", std::vector<int>(2, 3), "test");

int main(int argc, char** argv)
{
    P_LOG_INFO(P_LOG_ROOT) << "Before: " << test_vec_cfg->ToString();
    P_LOG_INFO(P_LOG_ROOT) << "Before: " << test_map_cfg->ToString();
    YAML::Node root = YAML::LoadFile("../config/config.yml");
    primo::Config::LoadFromYaml(root);

    P_LOG_INFO(P_LOG_ROOT) << "After: " << test_vec_cfg->ToString();
    P_LOG_INFO(P_LOG_ROOT) << "After: " << test_map_cfg->ToString();
    return 0;
}