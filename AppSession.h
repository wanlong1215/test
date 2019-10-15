#ifndef APPSESSION_H
#define APPSESSION_H

#include <QString>
#include <QDateTime>

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

    qint64 toInt64Time(const QDateTime &dateTime);
    QDateTime toQDateTime(qint64 time64);

    LoginUser user;

private:
    AppSession();
};

#endif // APPSESSION_H
