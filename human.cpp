#include "human.h"
//! Метод, который проверяет заданное имя на повторение в других родственных связях
bool human::repetitionCheck(QString name, QString role, human* tmp) {
    bool repet = 0;
    if (!(tmp->grandParent1.isEmpty()) && role != "grandparent") {
        if (name == tmp->grandParent1)
            repet = 1;
        if (repet == 0 && !(tmp->grandParent2.isEmpty())) {
            if (name == tmp->grandParent2)
                repet = 1;
            if (repet == 0 && !(tmp->grandParent3.isEmpty())) {
                if (name == tmp->grandParent3)
                    repet = 1;
                if (repet == 0 && !(tmp->grandParent4.isEmpty())) {
                    if (name == tmp->grandParent4)
                        repet = 1;
                }
            }
        }
    }
    if (repet == 0 && !(tmp->parent1.isEmpty()) && role != "parent") {
        if (name == tmp->parent1)
            repet = 1;
        if (repet == 0 && !(tmp->parent2.isEmpty())) {
            if (name == tmp->parent2)
                repet = 1;
        }
    }
    if (repet == 0 && !(tmp->children.isEmpty()) && role != "child") {
        for (int i = 0; i < tmp->children.count() && repet == 0; i++) {
            if (name == tmp->children[i])
                repet = 1;
        }
    }
    if (repet == 0 && !(tmp->grandChildren.isEmpty()) && role != "grandchild") {
        for (int i = 0; i < tmp->grandChildren.count() && repet == 0; i++) {
            if (name == tmp->grandChildren[i])
                repet = 1;
        }
    }
    if (repet == 0 && !(tmp->sibling.isEmpty()) && role != "sibling") {
        for (int i = 0; i < tmp->sibling.count() && repet == 0; i++) {
            if (name == tmp->sibling[i])
                repet = 1;
        }
    }
    return repet;
}
//! Метод, который проверяет всех родственников заданного человека на повторение в других родственных связях
bool human::humanRepetition(QString ownName, human *tmp) {
    bool repet = 0;
    repet = repetitionCheck(ownName,"",tmp);
    if (repet == 0 && !(tmp->parent1.isEmpty())) {
        repet = repetitionCheck(tmp->parent1, "parent", tmp);
        if (repet == 0 && !(tmp->parent2.isEmpty()))
            repet = repetitionCheck(tmp->parent2, "parent", tmp);
    }
    if (!(tmp->grandParent1.isEmpty()) && repet == 0) {
        repet = repetitionCheck(tmp->grandParent1, "grandparent", tmp);
        if (repet == 0 && !(tmp->grandParent2.isEmpty())) {
            repet = repetitionCheck(tmp->grandParent2, "grandparent", tmp);
            if (repet == 0 && !(tmp->grandParent3.isEmpty())) {
                repet = repetitionCheck(tmp->grandParent3, "grandparent", tmp);
                if (repet == 0 && !(tmp->grandParent4.isEmpty()))
                    repet = repetitionCheck(tmp->grandParent4, "grandparent", tmp);
            }
        }
    }
    if (repet == 0 && !(tmp->children.isEmpty())) {
        for (int i = 0; i < tmp->children.count() && repet == 0; i++) {
            repet = repetitionCheck(tmp->children[i], "child", tmp);
        }
    }
    if (repet == 0 && !(tmp->grandChildren.isEmpty())) {
        for (int i = 0; i < tmp->grandChildren.count() && repet == 0; i++) {
            repet = repetitionCheck(tmp->grandChildren[i], "grandchild", tmp);
        }
    }
    if (repet == 0 && !(tmp->sibling.isEmpty())) {
        for (int i = 0; i < tmp->sibling.count() && repet == 0; i++) {
            repet = repetitionCheck(tmp->sibling[i], "sibling", tmp);
        }
    }
    return repet;
}
//! Метод, который добавляет к родителю дедушку текущего человека, если нужно
void human::buildingParGrandparfRelation(QString parentPatr, human *other) {
    QString grandPaName = this->grandParent1.mid(this->grandParent1.indexOf(" "), this->grandParent1.indexOf(" ", this->grandParent1.indexOf(" ") + 1) - this->grandParent1.indexOf(" "));
    grandPaName.remove(grandPaName.length() - 3, 3);
    if (parentPatr.contains(grandPaName)) {
        if (other->parent1.isEmpty()) {
            other->parent1 = this->grandParent1;
        } else if (other->parent2.isEmpty()) {
            other->parent2 = this->grandParent1;
        }
    }
    if (!(this->grandParent2.isEmpty())) {
        grandPaName = this->grandParent2.mid(this->grandParent2.indexOf(" "), this->grandParent2.indexOf(" ", this->grandParent2.indexOf(" ") + 1) - this->grandParent2.indexOf(" "));
        grandPaName.remove(grandPaName.length() - 3, 3);
        if (parentPatr.contains(grandPaName)) {
            if (other->parent1.isEmpty()) {
                other->parent1 = this->grandParent2;
            } else if (other->parent2.isEmpty()) {
                other->parent2 = this->grandParent2;
            }
        }
        if (!(this->grandParent3.isEmpty())) {
            grandPaName = this->grandParent3.mid(this->grandParent3.indexOf(" "), this->grandParent3.indexOf(" ", this->grandParent3.indexOf(" ") + 1) - this->grandParent3.indexOf(" "));
            grandPaName.remove(grandPaName.length() - 3, 3);
            if (parentPatr.contains(grandPaName)) {
                if (other->parent1.isEmpty()) {
                    other->parent1 = this->grandParent3;
                } else if (other->parent2.isEmpty()) {
                    other->parent2 = this->grandParent3;
                }
            }
            if (!(this->grandParent4.isEmpty())) {
                grandPaName = this->grandParent4.mid(this->grandParent4.indexOf(" "), this->grandParent4.indexOf(" ", this->grandParent4.indexOf(" ") + 1) - this->grandParent4.indexOf(" "));
                grandPaName.remove(grandPaName.length() - 3, 3);
                if (parentPatr.contains(grandPaName)) {
                    if (other->parent1.isEmpty()) {
                        other->parent1 = this->grandParent4;
                    } else if (other->parent2.isEmpty()) {
                        other->parent2 = this->grandParent4;
                    }
                }
            }
        }
    }
}
