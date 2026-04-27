#include "Daemon.h"
#include <QTimer>

Daemon::Daemon(const CadMouseConfig &config, QObject* parent) : QObject(parent) {
    data = config.serialize();
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Daemon::sendFeatureReport);
    timer->start(2000);
    sendFeatureReport();
}

void Daemon::configure(bool liftOffDetection, bool smartScrolling, bool remapWheelPress, bool remapGestureButton, int frequency) {
    std::wcout << L"configuring mouse:"
            << L" liftOffDetection=" << liftOffDetection
            << L" smartScrolling=" << smartScrolling
            << L" remapWheelPress=" << remapWheelPress
            << L" remapGestureButton=" << remapGestureButton
            << L" frequency=" << frequency
            << std::endl;
    try {
        CadMouseConfig config;
        config.setLiftOffDetection(liftOffDetection);
        config.setSmartScrolling(smartScrolling);
        config.setWheelPressRemapped(remapWheelPress);
        config.setRemapGestureButton(remapGestureButton);
        config.setFrequency(Common::parseFrequency(std::to_wstring(frequency)));
        data = config.serialize();
    } catch (HIDException& e) {
        logException(e);
        sendErrorReply(QDBusError::ErrorType::Failed, QString::fromStdWString(e.getMessage()));
    } catch (CLIException& e) {
        sendErrorReply(QDBusError::ErrorType::Failed, QString::fromStdWString(e.getMessage()));
    }
}

void Daemon::sendFeatureReport()
{
    try {
        std::wcout << L"sending feature report " << std::endl;
        getMouse().sendFeatureReport(data);
    } catch (HIDException& e) {
        logException(e);
    }
}