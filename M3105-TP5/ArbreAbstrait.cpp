#include <stdlib.h>
#include "ArbreAbstrait.h"
#include "Symbole.h"
#include "SymboleValue.h"
#include "Exceptions.h"

////////////////////////////////////////////////////////////////////////////////
// NoeudSeqInst
////////////////////////////////////////////////////////////////////////////////

NoeudSeqInst::NoeudSeqInst() : m_instructions() {
}

int NoeudSeqInst::executer() {
  for (unsigned int i = 0; i < m_instructions.size(); i++)
    m_instructions[i]->executer(); // on exécute chaque instruction de la séquence
  return 0; // La valeur renvoyée ne représente rien !
}

void NoeudSeqInst::ajoute(Noeud* instruction) {
  if (instruction!=nullptr) m_instructions.push_back(instruction);
}

////////////////////////////////////////////////////////////////////////////////
// NoeudAffectation
////////////////////////////////////////////////////////////////////////////////

NoeudAffectation::NoeudAffectation(Noeud* variable, Noeud* expression)
: m_variable(variable), m_expression(expression) {
}

int NoeudAffectation::executer() {
  int valeur = m_expression->executer(); // On exécute (évalue) l'expression
  ((SymboleValue*) m_variable)->setValeur(valeur); // On affecte la variable
  return 0; // La valeur renvoyée ne représente rien !
}

////////////////////////////////////////////////////////////////////////////////
// NoeudOperateurBinaire
////////////////////////////////////////////////////////////////////////////////

NoeudOperateurBinaire::NoeudOperateurBinaire(Symbole operateur, Noeud* operandeGauche, Noeud* operandeDroit)
: m_operateur(operateur), m_operandeGauche(operandeGauche), m_operandeDroit(operandeDroit) {
}

int NoeudOperateurBinaire::executer() {
  int og, od, valeur;
  if (m_operandeGauche != nullptr) og = m_operandeGauche->executer(); // On évalue l'opérande gauche
  if (m_operandeDroit != nullptr) od = m_operandeDroit->executer(); // On évalue l'opérande droit
  // Et on combine les deux opérandes en fonctions de l'opérateur
  if (this->m_operateur == "+") valeur = (og + od);
  else if (this->m_operateur == "-") valeur = (og - od);
  else if (this->m_operateur == "*") valeur = (og * od);
  else if (this->m_operateur == "==") valeur = (og == od);
  else if (this->m_operateur == "!=") valeur = (og != od);
  else if (this->m_operateur == "<") valeur = (og < od);
  else if (this->m_operateur == ">") valeur = (og > od);
  else if (this->m_operateur == "<=") valeur = (og <= od);
  else if (this->m_operateur == ">=") valeur = (og >= od);
  else if (this->m_operateur == "et") valeur = (og && od);
  else if (this->m_operateur == "ou") valeur = (og || od);
  else if (this->m_operateur == "non") valeur = (!og);
  else if (this->m_operateur == "/") {
    if (od == 0) throw DivParZeroException();
    valeur = og / od;
  }
  return valeur; // On retourne la valeur calculée
}

////////////////////////////////////////////////////////////////////////////////
// NoeudInstSi
////////////////////////////////////////////////////////////////////////////////

NoeudInstSi::NoeudInstSi(Noeud* condition, Noeud* sequence)
: m_condition(condition), m_sequence(sequence) {
}

int NoeudInstSi::executer() {
  if (m_condition->executer()){
      m_sequence->executer();
      return 1;
  }
  return 0; // La valeur renvoyée ne représente rien !
}

NoeudInstSiRiche::NoeudInstSiRiche(std::vector<Noeud*> vectNoeuds, std::vector<Noeud*> noeudSinon)
: m_vectNoeuds(vectNoeuds), m_vectNoeudsSinon(noeudSinon) {
}

int NoeudInstSiRiche::executer() {
    
    Noeud* p;
    bool exit =false;
    
    for(int i = 0 ; i<m_vectNoeuds.size()-1 && !exit ;i+=2){//i incremente de 2 pour mettre i seulement sur les conditions
        if(m_vectNoeuds.at(i)->executer()){//condition
            m_vectNoeuds.at(i+1)->executer();//sequence
            exit = true;
        }
    }
    
    if (m_vectNoeudsSinon.size()>0 && !exit){ // si il y a un sinon et aucune séquence exécutée
        m_vectNoeudsSinon.at(0)->executer();//sequencd d'un sinon
    }
    
    return 0;
}

NoeudInstTantQue::NoeudInstTantQue(Noeud* condition, Noeud* sequence)
: m_condition(condition), m_sequence(sequence) {
}

int NoeudInstTantQue::executer() {
  while (m_condition->executer()) m_sequence->executer();
  return 0; // La valeur renvoyée ne représente rien !
}

NoeudInstRepeter::NoeudInstRepeter(Noeud* sequence, Noeud* condition)
: m_sequence(sequence), m_condition(condition) {
}

int NoeudInstRepeter::executer() {
    do {
        m_sequence->executer(); //séquence
    } while (m_condition->executer()); // if condition bonne
    return 0;
}

NoeudInstPour::NoeudInstPour(Noeud* affectationDeb, Noeud* conditionArret, Noeud* affectationFin, Noeud* sequence)
:m_affectationDeb(affectationDeb), m_condition(conditionArret), m_affectationFin(affectationFin), m_sequence(sequence){
}

int NoeudInstPour::executer() {
    for(m_affectationDeb->executer(); m_condition->executer(); m_affectationFin->executer()){
        m_sequence->executer();
    }

    return 0;
}

NoeudInstEcrire::NoeudInstEcrire(Noeud* chaine)
: m_chaine(chaine) {
}

int NoeudInstEcrire::executer() {
    cout<<((SymboleValue*) m_chaine)->getChaine();
  return 0; // La valeur renvoyée ne représente rien !
  
}
