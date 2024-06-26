#include "../include/api.h"
#include <curl/curl.h>
using namespace std;

const string BASE_URL = "localhost:3000";

static size_t WriteCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
    ((string *)userp)->append((char *)contents, size * nmemb);
    return size * nmemb;
}

string getData(string path)
{
    string url = BASE_URL;
    url.append(path);
    CURL *curl;
    CURLcode res;
    string readBuffer;

    curl = curl_easy_init();

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
    }

    return readBuffer;
}

string postData(string path, string body)
{
    string url = BASE_URL;
    url.append(path);
    CURL *curl;
    CURLcode res;
    string readBuffer;

    curl = curl_easy_init();

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        res = curl_easy_perform(curl);

        if (res != CURLE_OK)
        {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);
    }

    return readBuffer;
}

void sendCoordinates(double x, double y)
{
    string path = "/machine/coordinates";
    string body = "";
    body.append("x=").append(to_string(x)).append("&y=").append(to_string(y));

    postData(path, body);
}

void movePen(string direction)
{
    string path = "/machine/pen";

    string body = "";
    body.append("penPosition=").append(direction);

    postData(path, body);
}

void updateAutoHomeStatus(string autoHomeStatus)
{
    string path = "/machine/status";
    string body = "";
    body.append("autoHoming=").append(autoHomeStatus);

    postData(path, body);
}

void updateMovingAxisStatus(char axis, string movingStatus)
{
    string path = "/machine/status";
    string body = "";
    if (axis == 'x')
    {
        body.append("isMovingX=");
    }
    else
    {
        body.append("isMovingY=");
    }

    body.append(movingStatus);

    postData(path, body);
}

void updateStartPlottingStatus(string startPlottingStatus)
{
    string path = "/machine/status";
    string body = "";
    body.append("startPlotting=").append(startPlottingStatus);

    postData(path, body);
}

void updateMovingPenStatus(string movingStatus)
{
    string path = "/machine/status";
    string body = "";
    body.append("isMovingPen=").append(movingStatus);

    postData(path, body);
}
