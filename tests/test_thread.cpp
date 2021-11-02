#include "../primo/thread.h"
#include "../primo/log/Logger.h"
#include <vector>
#include "../primo/config.h"
primo::Logger::ptr logger = P_LOG_NAME("root");

int count = 0;
primo::RWMutex mutex;
void func1()
{
    std::cout << "run func1" <<std::endl;
    P_LOG_INFO(logger) << "name: " << primo::Thread::GetStaticThreadName()
                       << " this.name: " << primo::Thread::GetThis()->GetName()
                       << " id: " << primo::GetThreadId()
                       << " this.id: " << primo::Thread::GetThis()->GetId(); 
    for (int i = 0; i < 100000000; ++i)
    {
        //primo::RWMutex::WriteLock lk(mutex);
        ++ count;
    }
}

void func2()
{
    while (true)
    {
        P_LOG_INFO(logger) << "func2 log ";
    }
}


int main(int argc, char** argv)
{
    YAML::Node root = YAML::LoadFile("../config/config.yml");
    primo::Config::LoadFromYaml(root);
    std::vector<primo::Thread::ptr> thrs;
    P_LOG_INFO(logger) << " thread test start";
    for (int i = 0; i < 2; ++i)
    {
        primo::Thread::ptr thr(new primo::Thread(func2, "thread " + std::to_string(i)));
        thrs.push_back(thr);
    }

    for (int i = 0; i < 2; ++i)
    {
        thrs[i]->join();
    } 

    P_LOG_INFO(logger) << " thread test quit, count value: " << count;
    return 0;
}
