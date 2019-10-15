#include "AppSession.h"

AppSession::AppSession()
{

}

AppSession &AppSession::instance()
{
    static AppSession instance;
    return instance;
}

qint64 AppSession::toInt64Time(const QDateTime &dateTime)
{
    // Definition of FILETIME from MSDN:
    // Contains a 64-bit value representing the number of 100-nanosecond intervals since January 1, 1601 (UTC).
    QDateTime origin(QDate(1601, 1, 1), QTime(0, 0, 0, 0), Qt::UTC);
    // Get offset - note we need 100-nanosecond intervals, hence we multiply by
    // 10000. 8 hour offset
    origin = origin.addSecs(-1*60*60*8);
    qint64 _100nanosecs = 10000 * origin.msecsTo(dateTime);
    return _100nanosecs;
}

QDateTime AppSession::toQDateTime(qint64 time64)
{
    QDateTime origin(QDate(1601, 1, 1), QTime(0, 0, 0, 0), Qt::UTC);
    QDateTime time = origin.addMSecs(time64/10000);
    return time;
}
