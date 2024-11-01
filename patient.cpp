// patient.cpp
#include "patient.h"

Patient::Patient() : id(0) {}

Patient::Patient(int id, const QString& name, const QDate& birthDate)
    : id(id), name(name), birthDate(birthDate) {}

// Реализация геттеров
int Patient::getId() const {
    return id;
}

QString Patient::getName() const {
    return name;
}

QDate Patient::getBirthDate() const {
    return birthDate;
}

// Реализация сеттеров
void Patient::setId(int newId) {
    id = newId;
}

void Patient::setName(const QString& newName) {
    name = newName;
}

void Patient::setBirthDate(const QDate& newBirthDate) {
    birthDate = newBirthDate;
}

// Реализация операторов сравнения
bool Patient::operator<(const Patient& other) const {
    return id < other.id;
}

bool Patient::operator>(const Patient& other) const {
    return id > other.id;
}

bool Patient::operator==(const Patient& other) const {
    return id == other.id;
}
