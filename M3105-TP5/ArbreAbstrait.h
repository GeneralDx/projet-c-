#ifndef ARBREABSTRAIT_H
#define ARBREABSTRAIT_H

// Contient toutes les déclarations de classes nécessaires
//  pour représenter l'arbre abstrait

#include <vector>
#include <iostream>
#include <iomanip>
using namespace std;

#include "Symbole.h"
#include "Exceptions.h"

////////////////////////////////////////////////////////////////////////////////
class Noeud {
// Classe abstraite dont dériveront toutes les classes servant à représenter l'arbre abstrait
// Remarque : la classe ne contient aucun constructeur
  public:
    virtual int  executer() =0 ; // Méthode pure (non implémentée) qui rend la classe abstraite
    virtual void ajoute(Noeud* instruction) { throw OperationInterditeException(); }
    virtual ~Noeud() {} // Présence d'un destructeur virtuel conseillée dans les classes abstraites
};

////////////////////////////////////////////////////////////////////////////////
class NoeudSeqInst : public Noeud {
// Classe pour représenter un noeud "sequence d'instruction"
//  qui a autant de fils que d'instructions dans la séquence
  public:
     NoeudSeqInst();   // Construit une séquence d'instruction vide
    ~NoeudSeqInst() {} // A cause du destructeur virtuel de la classe Noeud
    int executer();    // Exécute chaque instruction de la séquence
    void ajoute(Noeud* instruction);  // Ajoute une instruction à la séquence

  private:
    vector<Noeud *> m_instructions; // pour stocker les instructions de la séquence
};

////////////////////////////////////////////////////////////////////////////////
class NoeudAffectation : public Noeud {
// Classe pour représenter un noeud "affectation"
//  composé de 2 fils : la variable et l'expression qu'on lui affecte
  public:
     NoeudAffectation(Noeud* variable, Noeud* expression); // construit une affectation
    ~NoeudAffectation() {} // A cause du destructeur virtuel de la classe Noeud
    int executer();        // Exécute (évalue) l'expression et affecte sa valeur à la variable

  private:
    Noeud* m_variable;
    Noeud* m_expression;
};

////////////////////////////////////////////////////////////////////////////////
class NoeudOperateurBinaire : public Noeud {
// Classe pour représenter un noeud "opération binaire" composé d'un opérateur
//  et de 2 fils : l'opérande gauche et l'opérande droit
  public:
    NoeudOperateurBinaire(Symbole operateur, Noeud* operandeGauche, Noeud* operandeDroit);
    // Construit une opération binaire : operandeGauche operateur OperandeDroit
   ~NoeudOperateurBinaire() {} // A cause du destructeur virtuel de la classe Noeud
    int executer();            // Exécute (évalue) l'opération binaire)

  private:
    Symbole m_operateur;
    Noeud*  m_operandeGauche;
    Noeud*  m_operandeDroit;
};

////////////////////////////////////////////////////////////////////////////////
class NoeudInstSi : public Noeud {
// Classe pour représenter un noeud "instruction si"
//  et ses 2 fils : la condition du si et la séquence d'instruction associée
  public:
    NoeudInstSi(Noeud* condition, Noeud* sequence);
     // Construit une "instruction si" avec sa condition et sa séquence d'instruction
   ~NoeudInstSi() {} // A cause du destructeur virtuel de la classe Noeud
    int executer();  // Exécute l'instruction si : si condition vraie on exécute la séquence

  private:
    Noeud*  m_condition;
    Noeud*  m_sequence;
};

class NoeudInstSiRiche : public Noeud {
// Classe pour représenter un noeud "instruction Si riche"
//  et ses 2 fils : un vecteur de noeuds qui est composé de condition / séquence ...
//                  un vecteur de noeuds représentant le sinon, ( on a utiliser un noeud vide car c'est plus simple de les manipuler)
  public:
    NoeudInstSiRiche(std::vector<Noeud*> vectNoeuds, std::vector<Noeud*> noeudSinon);
     // Construit une instruction SiRiche avec le vector de noeud mis en paramètre.
   ~NoeudInstSiRiche() {} // A cause du destructeur virtuel de la classe Noeud
    int executer();  // Exécute l'instruction SiRiche : Si la condition est vrai alors, sinonsi la condition est vraie alors ... sinon on excute la séquence
  private:
    std::vector<Noeud*> m_vectNoeuds;
    std::vector<Noeud*> m_vectNoeudsSinon;

};

class NoeudInstTantQue : public Noeud {
    // Classe pour représenter un noeud "instruction tantque"
//  et ses 2 fils : la condition du tantque et la séquence d'instruction associée
  public:
    NoeudInstTantQue(Noeud* condition, Noeud* sequence);
     // Construit une "instruction tantque" avec sa condition et sa séquence d'instruction
   ~NoeudInstTantQue() {} // A cause du destructeur virtuel de la classe Noeud
    int executer();  // Exécute l'instruction tantque : tantque condition vraie on exécute la séquence

  private:
    Noeud*  m_condition;
    Noeud*  m_sequence;
};

class NoeudInstRepeter : public Noeud {
  public:
    NoeudInstRepeter(Noeud* condition, Noeud* sequence);
   ~NoeudInstRepeter() {}
    int executer();

  private:
    Noeud*  m_condition;
    Noeud*  m_sequence;
};

class NoeudInstPour : public Noeud {
  public:
    NoeudInstPour(Noeud* affectationDeb, Noeud* conditionArret, Noeud* affectationFin, Noeud* sequence);
   ~NoeudInstPour() {}
    int executer();

  private:
    Noeud*  m_affectationDeb;
    Noeud*  m_affectationFin;
    Noeud*  m_condition;
    Noeud*  m_sequence;
};

class NoeudInstEcrire : public Noeud {
    // Classe pour représenter un noeud "instruction tantque"
//  et ses 2 fils : la condition du tantque et la séquence d'instruction associée
  public:
    NoeudInstEcrire(Noeud* chaine);
     // Construit une "instruction tantque" avec sa condition et sa séquence d'instruction
   ~NoeudInstEcrire() {} // A cause du destructeur virtuel de la classe Noeud
    int executer();  // Exécute l'instruction tantque : tantque condition vraie on exécute la séquence

  private:
    Noeud*  m_chaine;
};

#endif /* ARBREABSTRAIT_H */
