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

    string response = postData(path, body);

    printf("Response from send coordinates:%s\n", response.c_str());
}

void movePen(string direction)
{
    string path = "/machine/pen";

    string body = "";
    body.append("penPosition=").append(direction);

    printf("BODY TO SEND: %s\n", body.c_str());

    string response = postData(path, body);

    printf("Response from move pen:%s\n", response.c_str());
}

void updateAutoHomeStatus(string autoHomeStatus)
{
    string path = "/machine/status";
    string body = "";
    body.append("autoHoming=").append(autoHomeStatus);

    printf("BODY TO SEND: %s\n", body.c_str());

    string response = postData(path, body);

    printf("Response from updateAutoHomeStatus:%s\n", response.c_str());
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

    printf("BODY TO SEND: %s\n", body.c_str());

    string response = postData(path, body);

    printf("Response from updateMovingStatus:%s\n", response.c_str());
}

void updateStartPlottingStatus(string startPlottingStatus)
{
    string path = "/machine/status";
    string body = "";
    body.append("startPlotting=").append(startPlottingStatus);

    printf("BODY TO SEND: %s\n", body.c_str());

    string response = postData(path, body);

    printf("Response from updateStartPlottingStatus:%s\n", response.c_str());
}

void updateMovingPenStatus(string movingStatus)
{
    string path = "/machine/status";
    string body = "";
    body.append("isMovingPen=").append(movingStatus);

    printf("BODY TO SEND: %s\n", body.c_str());

    string response = postData(path, body);

    printf("Response from updateStartPlottingStatus:%s\n", response.c_str());
}
