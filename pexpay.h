
#ifndef PEXPAY_H
#define PEXPAY_H

#include <QString>
#include <QDateTime>
#include <string>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

using json = nlohmann::json;

class Pexpay
{
public:
    Pexpay(QString _apiKey, QString _secretKey);
    ~Pexpay();

    json getOrdersHistory(qint64 _startTimestamp, qint64 _endTimestamp);

private:
    json makeRequest(std::string _tradeType, int _page);

private:
    std::string uri;

    std::string apiKey;
    std::string secretKey;

    std::string startTimestamp_s;
    std::string endTimestamp_s;
};

#endif // PEXPAY_H
