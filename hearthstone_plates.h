#ifndef HEARTHSTONE_PLATES_H
#define HEARTHSTONE_PLATES_H

#include <memory>
#include <vector>
#include <iostream>

// Типы карт в Hearthstone
enum class CardType {
    MINION,  // Карты-миньоны
    SPELL,   // Заклинания
    WEAPON   // Оружие
};

// Стратегии использования карт
enum class UseStrategyType {
    PLAY,    // Размещение карты на столе
    CAST,    // Применение заклинания
    ATTACK   // Атака оружием
};

// Базовый класс стратегии использования карт
class UseStrategy {
public:
    virtual ~UseStrategy() = default;
    virtual void Execute() = 0;  // Основной метод выполнения стратегии
};

// Конкретные реализации стратегий
class PlayStrategy : public UseStrategy {
public:
    // Реализация стратегии игры миньона
    void Execute() override { std::cout << "Playing card to board"; }
};

class CastStrategy : public UseStrategy {
public:
    // Реализация стратегии применения заклинания
    void Execute() override { std::cout << "Casting spell"; }
};

class AttackStrategy : public UseStrategy {
public:
    // Реализация стратегии атаки оружием
    void Execute() override { std::cout << "Attacking target"; }
};

// Базовый класс карты
class Card {
protected:
    CardType type;                 // Тип карты
    std::unique_ptr<UseStrategy> strategy;  // Стратегия использования

    // Абстрактные методы, которые должны реализовать подклассы
    virtual void Prepare() = 0;    // Подготовка карты к использованию
    virtual void Announce() = 0;   // Объявление типа карты

public:
    explicit Card(CardType t) : type(t) {}
    virtual ~Card() = default;

    // Основной метод использования карты
    void Use() {
        Announce();               // Шаг 1: Объявить тип карты
        std::cout << " : ";
        Prepare();                // Шаг 2: Подготовить карту
        std::cout << " => ";
        strategy->Execute();      // Шаг 3: Выполнить стратегию
        std::cout << "\n";
    }

    // Получение типа карты
    CardType GetType() const { return type; }
};

// Шаблонный класс итератора для обхода коллекций
template<typename T>
class Iterator {
public:
    virtual ~Iterator() = default;
    virtual void First() = 0;       // Переход к первому элементу
    virtual void Next() = 0;        // Переход к следующему элементу
    virtual bool IsDone() const = 0;  // Проверка завершения обхода
    virtual T GetCurrent() const = 0; // Получение текущего элемента
};

// Реализация итератора для массива
template<typename T>
class ArrayIterator : public Iterator<T> {
    T* items;     // Указатель на массив
    size_t size;   // Размер массива
    size_t current; // Текущая позиция

public:
    ArrayIterator(T* arr, size_t s) : items(arr), size(s), current(0) {}

    void First() override { current = 0; }
    void Next() override { current++; }
    bool IsDone() const override { return current >= size; }
    T GetCurrent() const override { return items[current]; }
};

// Декоратор итератора для фильтрации по типу карты
template<typename T>
class CardTypeDecorator : public Iterator<T> {
    Iterator<T>* it;    // Обернутый итератор
    CardType type;       // Тип карты для фильтрации

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
    // Пропуск карт, не соответствующих типу
    void SkipNonMatching() {
        while (!it->IsDone() && it->GetCurrent()->GetType() != type) {
            it->Next();
        }
    }
};

#endif
