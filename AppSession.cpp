#include "AppSession.h"

AppSession::AppSession()
{

}

AppSession &AppSession::instance()
{
    static AppSession instance;
    return instance;
}
