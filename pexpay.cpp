#include "pexpay.h"

Pexpay::Pexpay(QString _apiKey, QString _secretKey)
{
    uri = "https://api.pexpay.com/sapi/v1/c2c/orderMatch/listUserOrderHistory";

    apiKey = _apiKey.toStdString();
    secretKey = _secretKey.toStdString();
}

Pexpay::~Pexpay()
{

}

std::string Pexpay::CalculateHmacSHA256(std::string_view decodedKey, std::string_view msg)
{
    std::array<unsigned char, EVP_MAX_MD_SIZE> hash;
    unsigned int hashLen;
    HMAC(
        EVP_sha256(),
        decodedKey.data(),
        static_cast<int>(decodedKey.size()),
        reinterpret_cast<unsigned char const*>(msg.data()),
        static_cast<int>(msg.size()),
        hash.data(),
        &hashLen
        );
    std::stringstream out;
    for (unsigned int i=0; i < hashLen; i++) {
        out << std::setfill('0') << std::setw(2) << std::right << std::hex << (int)hash.data()[i];
    }
    return out.str();
}

json Pexpay::makeRequest(std::string _tradeType, int _page)
{
    try
    {
        // headers
        std::string contentType = "application/json";
        std::string header_apiKey = apiKey;

        // params
        std::string tradeType = _tradeType;
        std::string startTimestamp = startTimestamp_s;
        std::string endTimestamp = endTimestamp_s;
        std::string page = QString::number(_page).toStdString();
        std::string rows = "100";
        std::string recvWindow = "5000";
        std::string timestamp = QString::number(QDateTime::currentMSecsSinceEpoch()).toStdString();
        std::string signature = "";

        std::string params="tradeType="+tradeType+"&startTimestamp="+startTimestamp+"&endTimestamp="+endTimestamp+"&page="+page+"&rows="+rows+"&recvWindow="+recvWindow+"&timestamp="+timestamp;
        std::string_view key_view{secretKey};
        std::string_view msg_view{params};
        signature = CalculateHmacSHA256(key_view, msg_view);

        cpr::Response r = cpr::Get(
            cpr::Url{uri},
            cpr::Header{
                {"content-type", contentType},
                {"X-MBX-APIKEY", apiKey}
            },
            cpr::Parameters{
                {"tradeType", tradeType},
                {"startTimestamp", startTimestamp},
                {"endTimestamp", endTimestamp},
                {"page", page},
                {"rows", rows},
                {"recvWindow", recvWindow},
                {"timestamp", timestamp},
                {"signature", signature}
            }
            );

        switch (r.status_code) {
        case 403:
            qDebug() << "HTTP 403 return code is used when the WAF Limit (Web Application Firewall) has been violated.";
            break;
        case 409:
            qDebug() << "HTTP 409 return code is used when a cancelReplace order partially succeeds. (e.g. if the cancellation of the order fails but the new order placement succeeds.)";
            break;
        case 429:
            qDebug() << "HTTP 429 return code is used when breaking a request rate limit.";
            break;
        case 418:
            qDebug() << "HTTP 418 return code is used when an IP has been auto-banned for continuing to send requests after receiving 429 codes.";
            break;
        default:
            break;
        }

        if (r.status_code >= 400 && r.status_code <= 499)
        {
            qDebug() << "HTTP 4XX return codes are used for malformed requests; the issue is on the sender's side.";
        }
        else if (r.status_code >= 500 && r.status_code <= 599)
        {
            qDebug() << "HTTP 5XX return codes are used for internal errors; the issue is on Binance's side. It is important to NOT treat this as a failure operation; the execution status is UNKNOWN and could have been a success.";
        }

        // work with response
        //    qDebug() << "Status code: " << r.status_code;
        if (r.status_code == 200)
        {
            return json::parse(r.text);
        }

        return json({});
    }
    catch(json::exception& e)
    {
        qDebug() << e.what();
        return json({});
    }
}

json Pexpay::getOrdersHistory(qint64 _startTimestamp, qint64 _endTimestamp)
{
    startTimestamp_s = QString::number(_startTimestamp).toStdString();
    endTimestamp_s = QString::number(_endTimestamp).toStdString();

    json orderHistory = json::parse(R"(
        {
            "buy": [],
            "sell": []
        }
    )");

    // BUY
    for (int page_num = 1; page_num < 100; ++page_num)
    {
        json res = makeRequest("BUY", page_num);
        if (!res.empty())
        {
            if (!res["data"].empty())
            {
                for (const auto& order_obj : res["data"].items())
                {
                    orderHistory["buy"].push_back(order_obj.value());
                }
            }
            else
            {
                break;
            }
        }
        else
        {
            qDebug() << "Problem with getting binance order history, response json is empty, type: BUY";
            break;
        }
    }

    // SELL
    for (int page_num = 1; page_num < 100; ++page_num)
    {
        json res = makeRequest("SELL", page_num);
        if (!res.empty())
        {
            if (!res["data"].empty())
            {
                for (const auto& order_obj : res["data"].items())
                {
                    orderHistory["sell"].push_back(order_obj.value());
                }
            }
            else
            {
                break;
            }
        }
        else
        {
            qDebug() << "Problem with getting binance order history, response json is empty, type: SELL";
            break;
        }
    }

    return orderHistory;
}
