#include <iostream>
#include <string>

using namespace std;

class Logger
{
public:
    virtual void Log(string logText)
    {
        cout << "[Logger] " << logText << endl;
    }
};

class FileLogger : public Logger
{
public:
    void Log(string logText) override
    {
        cout << "[FileLogger] Saved in File: " << logText << endl;
    }
};

class DatabaseLogger : public Logger
{
public:
    void Log(string logText) override
    {
        cout << "[DatabaseLogger] Saved in DataBase: " << logText << endl;
    }
};

class SmtpMailer
{
private:
    Logger* logger;
public:
    SmtpMailer() : logger(nullptr) {}
    
    void SetLogger(Logger* logger)
    {
        this->logger = logger;
    }

    void SendMessage(string message)
    {
        cout << "[SmtpMailer] Sent mail: " << message << endl;
        if (logger)
        {
            logger->Log("Mail will send: " + message);
        }
        else
        {
            cout << "[SmtpMailer] Logger don't set!" << endl;
        }
        
    }
};

int main()
{
    FileLogger fileLogger;
    DatabaseLogger dbLogger;
    SmtpMailer mailer;
    SmtpMailer tester;

    cout << " TEST FileLogger" << endl;
    mailer.SetLogger(&fileLogger);
    mailer.SendMessage("1-st test mail!");

    cout << "\n TEST DatabaseLogger" << endl;
    mailer.SetLogger(&dbLogger);
    mailer.SendMessage("2-nd test mail!");

    cout << "\n TEST setter" << endl;
    //mailer.SetLogger();
    tester.SendMessage("2-nd test mail!");    

    return 0;
}