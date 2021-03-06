#ifndef _DYNARR_H_
#define _DYNARR_H_

#include <stdlib.h>
#include <assert.h>

/** @defgroup DynArr DynamicArray
 *
 * Tableau dynamique de void*
 * Préfixe des méthodes : da
 * @{
**/

typedef struct
{
	void** First;
	unsigned int Capacity;
	unsigned int Size;
} DynArr;

/** @brief Renvoi un DynArr initialisé
 *
 * @return Nouveau DynArr
**/
DynArr da();

/** @brief Constructeur
 *
 * @return Pointeur vers un nouveau DynArr
**/
DynArr* newDynArr();

/** @brief Initialise un DynArr
 *
 * @param DA Le DynArr à intialiser, != NULL
**/
void daInit(DynArr* DA);

/** @brief Libère le tableau
 *
 * @param DA Le DynArr à libérer, !=NULL
**/
void daFree(DynArr* DA);

/** @brief Destructeur
 *
 * @param DA Le DynArr à détruire
**/
void delDynArr(DynArr* DA);

/** @brief Redimensionne le tableau
 *
 * Si Capacity > newCapa, ne fait rien.
 * Complexité en O(n)
 * @param[in,out] DA Le DynArr à modifier
 * @param[in] newCapa Nouvelle capacité
**/
void daReserve(DynArr* DA, unsigned int newCapa);

/** @brief Ajoute un élément en fin de tableau
 *
 * Modifie la capacité si nécéssaire
 * Complexité en O(log n) (O(1) la pluspart du temps, O(n) dans le pire des cas)
 * @param DA Le DynArr à modifier
 * @param ptr Elément à ajouter
**/
void daAdd(DynArr* DA, void* ptr);

/** @brief Supprime l'élément à la position P
 *
 * Complexité en O(n - P)
 * @param DA Le DynArr à modifier
 * @param Pos Position de l'entrée à supprimer (0 - Size-1)
**/
void daDel(DynArr* DA, unsigned int Pos);

/** @brief Supprime l'élément ptr
 *
 * Complexité en O(n - P)
 * @param DA Le DynArr à modifier
 * @param ptr élément à supprimer (0 - Size-1)
 **/
void daRem(DynArr* DA, void* ptr);

/** @brief Supprime l'élément à la position P d'une manière plus rapide
 *
 * Ne conserve pas l'ordre des objets (important pour certaines applications !)
 * Complexité en O(1)
 * @param DA Le DynArr à modifier
 * @param Pos Position de l'entrée à supprimer (0 - Size-1)
**/
void daFastDel(DynArr* DA, unsigned int Pos);

/** @brief Retourne l'élément en position Pos
 *
 * Complexité en O(1)
 * @param DA Le DynArr à accéder
 * @param Pos Position de l'entrée à renvoyer (0 - Size-1)
**/
void* daGet(const DynArr* DA, unsigned int Pos);

/** @brief Accesseur de la taille utilisée
 *
 * @param[in] DA DynArr auquel accéder
 * @return Taille
**/
unsigned int daGetSize(const DynArr* DA);

/** @brief Accesseur de la capacité
 *
 * @param[in] DA DynArr auquel accéder
 * @return Capacité
**/
unsigned int daGetCapacity(const DynArr* DA);

/** @brief Cherche une valeur et retourne son index
 *
 * @param[in] DA DynArr auquel accéder
 * @param[in] ptr Pointeur vers la donnée recherchée
 * @return Index de la donnée dans le DynArr
 **/
unsigned int daGetID(const DynArr* D,const void* ptr);

/** @brief Test de Regression
**/
void daRegressionTest();

/** @}
**/

#endif

