
#ifndef BINANCE_H
#define BINANCE_H

#include <QString>
#include <QDateTime>
#include <string>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>

using json = nlohmann::json;

class Binance
{
public:
    Binance(QString _apiKey, QString _secretKey);
    ~Binance();

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

#endif // BINANCE_H
