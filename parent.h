#ifndef PARENT_H
#define PARENT_H

#include <QString>

class Parent
{
public:
    QString name;
    QString type;
    int hp;
    int health;
    int armor[3] = {0, 0, 0};

    QString element;
    int mana = 0;

    QString rarity;
    int damage = 0;

    Parent() = default;

    Parent(const QString& n, const QString& t, int h, int he, int a0, int a1, int a2,
           const QString& el = "", const QString& r = "", int m = 0, int d = 0);
};

#endif
