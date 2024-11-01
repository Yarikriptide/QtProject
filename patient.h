// patient.h
#ifndef PATIENT_H
#define PATIENT_H

#include <QString>
#include <QDate>

class Patient {
public:
    // Конструкторы
    Patient();
    Patient(int id, const QString& name, const QDate& birthDate);

    // Геттеры
    int getId() const;
    QString getName() const;
    QDate getBirthDate() const;

    // Сеттеры
    void setId(int newId);
    void setName(const QString& newName);
    void setBirthDate(const QDate& newBirthDate);

    // Операторы сравнения для работы в бинарном дереве
    bool operator<(const Patient& other) const;
    bool operator>(const Patient& other) const;
    bool operator==(const Patient& other) const;

private:
    int id;            // Уникальный идентификатор пациента
    QString name;      // ФИО пациента
    QDate birthDate;   // Дата рождения
};

#endif // PATIENT_H
