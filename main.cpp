#include <iostream>
#include <cstdlib>
#include <ctime>
#include "hearthstone_plates.h"

// ����� �����-�������
class MinionCard : public Card {
public:
    MinionCard() : Card(CardType::MINION) {
        strategy = std::make_unique<PlayStrategy>();
    }

    // ���������� ������� � �������������
    void Prepare() override { std::cout << "Summoning minion"; }

    // ���������� ���� �����
    void Announce() override { std::cout << "Minion"; }
};

// ����� �����-����������
class SpellCard : public Card {
public:
    SpellCard() : Card(CardType::SPELL) {
        strategy = std::make_unique<CastStrategy>();
    }

    // ���������� ���������� � �������������
    void Prepare() override { std::cout << "Preparing spell"; }

    // ���������� ���� �����
    void Announce() override { std::cout << "Spell"; }
};

// ����� �����-������
class WeaponCard : public Card {
public:
    WeaponCard() : Card(CardType::WEAPON) {
        strategy = std::make_unique<AttackStrategy>();
    }

    // ���������� ������ � �������������
    void Prepare() override { std::cout << "Equipping weapon"; }

    // ���������� ���� �����
    void Announce() override { std::cout << "Weapon"; }
};

// ������� ��� ������������� ���� ���� ����� ��������
void UseAllCards(Iterator<Card*>* it) {
    for(it->First(); !it->IsDone(); it->Next()) {
        it->GetCurrent()->Use();
    }
}

int main() {
    srand(static_cast<unsigned>(time(nullptr)));

    const size_t deckSize = 10;
    Card* deck[deckSize];

    // �������� ������ �� ���������� �������
    for(size_t i = 0; i < deckSize; ++i) {
        switch(rand() % 3) {
            case 0: deck[i] = new MinionCard(); break;
            case 1: deck[i] = new SpellCard(); break;
            case 2: deck[i] = new WeaponCard(); break;
        }
    }

    // ������������ ������������� ���� ����
    std::cout << "=== All cards ===\n";
    Iterator<Card*>* it = new ArrayIterator<Card*>(deck, deckSize);
    UseAllCards(it);
    delete it;

    // ������������ ���������� �� ���� (������ ����������)
    std::cout << "\n=== Only spells ===\n";
    Iterator<Card*>* spellIt = new CardTypeDecorator<Card*>(
        new ArrayIterator<Card*>(deck, deckSize),
        CardType::SPELL
    );
    UseAllCards(spellIt);
    delete spellIt;

    // ������� ������
    for(size_t i = 0; i < deckSize; ++i) {
        delete deck[i];
    }

    return 0;
}
