#include <drogon/drogon.h>

int main()
{
    drogon::app().addListener("0.0.0.0", 8848).setThreadNum(2).run();
    return 0;
}
