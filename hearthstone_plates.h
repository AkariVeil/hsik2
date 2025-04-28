#ifndef HEARTHSTONE_PLATES_H
#define HEARTHSTONE_PLATES_H

#include <memory>
#include <vector>
#include <iostream>

// ���� ���� � Hearthstone
enum class CardType {
    MINION,  // �����-�������
    SPELL,   // ����������
    WEAPON   // ������
};

// ��������� ������������� ����
enum class UseStrategyType {
    PLAY,    // ���������� ����� �� �����
    CAST,    // ���������� ����������
    ATTACK   // ����� �������
};

// ������� ����� ��������� ������������� ����
class UseStrategy {
public:
    virtual ~UseStrategy() = default;
    virtual void Execute() = 0;  // �������� ����� ���������� ���������
};

// ���������� ���������� ���������
class PlayStrategy : public UseStrategy {
public:
    // ���������� ��������� ���� �������
    void Execute() override { std::cout << "Playing card to board"; }
};

class CastStrategy : public UseStrategy {
public:
    // ���������� ��������� ���������� ����������
    void Execute() override { std::cout << "Casting spell"; }
};

class AttackStrategy : public UseStrategy {
public:
    // ���������� ��������� ����� �������
    void Execute() override { std::cout << "Attacking target"; }
};

// ������� ����� �����
class Card {
protected:
    CardType type;                 // ��� �����
    std::unique_ptr<UseStrategy> strategy;  // ��������� �������������

    // ����������� ������, ������� ������ ����������� ���������
    virtual void Prepare() = 0;    // ���������� ����� � �������������
    virtual void Announce() = 0;   // ���������� ���� �����

public:
    explicit Card(CardType t) : type(t) {}
    virtual ~Card() = default;

    // �������� ����� ������������� �����
    void Use() {
        Announce();               // ��� 1: �������� ��� �����
        std::cout << " : ";
        Prepare();                // ��� 2: ����������� �����
        std::cout << " => ";
        strategy->Execute();      // ��� 3: ��������� ���������
        std::cout << "\n";
    }

    // ��������� ���� �����
    CardType GetType() const { return type; }
};

// ��������� ����� ��������� ��� ������ ���������
template<typename T>
class Iterator {
public:
    virtual ~Iterator() = default;
    virtual void First() = 0;       // ������� � ������� ��������
    virtual void Next() = 0;        // ������� � ���������� ��������
    virtual bool IsDone() const = 0;  // �������� ���������� ������
    virtual T GetCurrent() const = 0; // ��������� �������� ��������
};

// ���������� ��������� ��� �������
template<typename T>
class ArrayIterator : public Iterator<T> {
    T* items;     // ��������� �� ������
    size_t size;   // ������ �������
    size_t current; // ������� �������

public:
    ArrayIterator(T* arr, size_t s) : items(arr), size(s), current(0) {}

    void First() override { current = 0; }
    void Next() override { current++; }
    bool IsDone() const override { return current >= size; }
    T GetCurrent() const override { return items[current]; }
};

// ��������� ��������� ��� ���������� �� ���� �����
template<typename T>
class CardTypeDecorator : public Iterator<T> {
    Iterator<T>* it;    // ��������� ��������
    CardType type;       // ��� ����� ��� ����������

public:
    CardTypeDecorator(Iterator<T>* iter, CardType t) : it(iter), type(t) {}
    ~CardTypeDecorator() override { delete it; }

    void First() override {
        it->First();
        SkipNonMatching();
    }

    void Next() override {
        it->Next();
        SkipNonMatching();
    }

    bool IsDone() const override { return it->IsDone(); }
    T GetCurrent() const override { return it->GetCurrent(); }

private:
    // ������� ����, �� ��������������� ����
    void SkipNonMatching() {
        while (!it->IsDone() && it->GetCurrent()->GetType() != type) {
            it->Next();
        }
    }
};

#endif
