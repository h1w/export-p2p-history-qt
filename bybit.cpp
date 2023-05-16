#include "bybit.h"

Bybit::Bybit(QString _apiKey, QString _secretKey)
{
    uri = "https://api.bybit.com/v5/lending/history-order";

    apiKey = _apiKey.toStdString();
    secretKey = _secretKey.toStdString();
}

Bybit::~Bybit()
{

}

std::string Bybit::CalculateHmacSHA256(std::string_view decodedKey, std::string_view msg)
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

json Bybit::makeRequest()
{
    try
    {
        // headers
        std::string contentType = "application/json";
        std::string signature = "";
        std::string header_apiKey = apiKey;
        std::string timestamp = QString::number(QDateTime::currentMSecsSinceEpoch()).toStdString();;
        std::string recvWindow = "60000";

        // params
        std::string category = "spot";
        std::string startTimestamp = startTimestamp_s;
        std::string endTimestamp = endTimestamp_s;
        std::string limit = "500";

//        std::string params="category="+category+"&startTime="+startTimestamp+"&endTime="+endTimestamp+"&limit="+limit;
        std::string params=timestamp+header_apiKey+recvWindow+"startTime="+startTimestamp+"&endTime="+endTimestamp+"&limit="+limit;
        std::string_view key_view{secretKey};
        std::string_view msg_view{params};
        signature = CalculateHmacSHA256(key_view, msg_view);

        cpr::Response r = cpr::Get(
            cpr::Url{uri},
            cpr::Header{
                {"content-type", contentType},
                {"X-BAPI-SIGN", signature},
                {"X-BAPI-API-KEY", header_apiKey},
                {"X-BAPI-TIMESTAMP", timestamp},
                {"X-BAPI-RECV-WINDOW", recvWindow}
            },
            cpr::Parameters{
//                {"category", "spot"},
                {"startTime", startTimestamp},
                {"endTime", endTimestamp},
                {"limit", limit},
            }
        );

        switch (r.status_code) {
        case 400:
            qDebug() << "Bad request. Need to send the request with GET / POST (must be capitalized)";
            break;
        case 401:
            qDebug() << "Invalid request. 1. Need to use the correct key to access; 2. Need to put authentication params in the request header";
            break;
        case 403:
            qDebug() << "Forbidden request. Possible causes: 1. IP rate limit breached; 2. You send GET request with an empty json body";
            break;
        case 404:
            qDebug() << "Cannot find path. Possible causes: 1. Wrong path; 2. Category value does not match account mode";
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
            json orders_obj = json::parse(r.text);

            int retCode = orders_obj.at("retCode").get<int>();
            std::string retMsg = orders_obj.at("retMsg").get<std::string>();
            qDebug() << "JSN ret code: " << retCode;
            qDebug() << "JSN ret Msg:" << retMsg;
//            std::cout << orders_obj.dump() << std::endl;
            return orders_obj;
        }

        return json({});
    }
    catch (json::exception& e)
    {
        qDebug() << e.what();
        return json({});
    }
}

json Bybit::getOrdersHistory(qint64 _startTimestamp, qint64 _endTimestamp)
{
    startTimestamp_s = QString::number(_startTimestamp).toStdString();
    endTimestamp_s = QString::number(_endTimestamp).toStdString();

    json test_jsn = makeRequest();

    return json({});
}
