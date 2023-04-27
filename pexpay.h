
#ifndef PEXPAY_H
#define PEXPAY_H

#include <QString>
#include <QDateTime>
#include <string>
#include <nlohmann/json.hpp>
#include <cpr/cpr.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <string>
#include <string_view>
#include <array>
#include <sstream>
#include <iomanip>
#include <iostream>

using json = nlohmann::json;

class Pexpay
{
public:
    Pexpay(QString _apiKey, QString _secretKey);
    ~Pexpay();

    json getOrdersHistory(qint64 _startTimestamp, qint64 _endTimestamp);

private:
    json makeRequest(std::string _tradeType, int _page);

    std::string CalculateHmacSHA256(std::string_view decodedKey, std::string_view msg);

private:
    std::string uri;

    std::string apiKey;
    std::string secretKey;

    std::string startTimestamp_s;
    std::string endTimestamp_s;
};

#endif // PEXPAY_H
