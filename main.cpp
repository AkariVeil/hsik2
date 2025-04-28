#include <iostream>
#include <cstdlib>
#include <ctime>
#include "hearthstone_plates.h"

// Класс карты-миньона
class MinionCard : public Card {
public:
    MinionCard() : Card(CardType::MINION) {
        strategy = std::make_unique<PlayStrategy>();
    }

    // Подготовка миньона к использованию
    void Prepare() override { std::cout << "Summoning minion"; }

    // Объявление типа карты
    void Announce() override { std::cout << "Minion"; }
};

// Класс карты-заклинания
class SpellCard : public Card {
public:
    SpellCard() : Card(CardType::SPELL) {
        strategy = std::make_unique<CastStrategy>();
    }

    // Подготовка заклинания к использованию
    void Prepare() override { std::cout << "Preparing spell"; }

    // Объявление типа карты
    void Announce() override { std::cout << "Spell"; }
};

// Класс карты-оружия
class WeaponCard : public Card {
public:
    WeaponCard() : Card(CardType::WEAPON) {
        strategy = std::make_unique<AttackStrategy>();
    }

    // Подготовка оружия к использованию
    void Prepare() override { std::cout << "Equipping weapon"; }

    // Объявление типа карты
    void Announce() override { std::cout << "Weapon"; }
};

// Функция для использования всех карт через итератор
void UseAllCards(Iterator<Card*>* it) {
    for(it->First(); !it->IsDone(); it->Next()) {
        it->GetCurrent()->Use();
    }
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    const size_t deckSize = 10;
    Card* deck[deckSize];

    // Создание колоды со случайными картами
    for(size_t i = 0; i < deckSize; ++i) {
        switch(rand() % 3) {
            case 0: deck[i] = new MinionCard(); break;
            case 1: deck[i] = new SpellCard(); break;
            case 2: deck[i] = new WeaponCard(); break;
        }
    }

    // Демонстрация использования всех карт
    std::cout << "=== All cards ===\n";
    Iterator<Card*>* it = new ArrayIterator<Card*>(deck, deckSize);
    UseAllCards(it);
    delete it;

    // Демонстрация фильтрации по типу (только заклинания)
    std::cout << "\n=== Only spells ===\n";
    Iterator<Card*>* spellIt = new CardTypeDecorator<Card*>(
        new ArrayIterator<Card*>(deck, deckSize),
        CardType::SPELL
    );
    UseAllCards(spellIt);
    delete spellIt;

    // Очистка памяти
    for(size_t i = 0; i < deckSize; ++i) {
        delete deck[i];
    }

    return 0;
}
