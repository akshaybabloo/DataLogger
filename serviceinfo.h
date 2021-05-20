//
// Created by em8273 on 19/05/2021.
//

#ifndef DATALOGGER_SERVICEINFO_H
#define DATALOGGER_SERVICEINFO_H


#include <QObject>
#include <QLowEnergyService>

class ServiceInfo: public QObject{

    Q_OBJECT

public:
    ServiceInfo() = default;
    explicit ServiceInfo(QLowEnergyService *service);
    QLowEnergyService *service() const;
    QString getUuid() const;
    QString getName() const;
    QString getType() const;

private:
    QLowEnergyService *m_service = nullptr;

};


#endif //DATALOGGER_SERVICEINFO_H
