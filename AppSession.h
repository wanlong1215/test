#ifndef APPSESSION_H
#define APPSESSION_H

#include <QString>

struct LoginUser
{
    int id;
    QString name;
    int level;
};

class AppSession
{
public:
    static AppSession &instance();

    LoginUser user;

private:
    AppSession();
};

#endif // APPSESSION_H
