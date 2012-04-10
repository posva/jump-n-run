#ifndef _MENUOFITEMS_H
#define _MENUOFITEMS_H

#include "MenuItem.h"

/** @defgroup menu Menu
 *@{
 */

typedef unsigned short ItemID;
typedef unsigned char MenuDirection;

#define MENU_GO_UP 0x00
#define MENU_GO_DOWN 0x01
#define INVALID_ITEM_ID 0xfffe /**< Valeur invalide pour un item non selectionné (ex curseur) **/


/**
 * @brief structure qui défini un menu d'items
 */

typedef struct
{
	unsigned short ItemCount; /**< nombre d'items dans le menu **/
	unsigned short ItemsAdded; /**< nombre d'item qui ont déjà été ajoutés **/
	ItemID ItemSelected; /**< ID de l'item selectionné dans le menu **/
	MenuItem* Items; /**< tableau d'items **/
	float* ItemsZoomspd; /**< tableau de variables utilisé pour le wobble du zoom factor **/
	Vec2 Size, SizeTo; /**< Taille de la boîte pour le menu **/
	float SizeSpd[2]; /**< variable utilisée dans wobble **/
	char* Text; /**< texte du menu (optionel) **/
	
} MenuOfItems;

/**
 * @brief Constructeur
 * @param M MenuOfItem auquel s'applique la fonction
 * @param Text texte du menu
 * @param ItemCount nombre d'item du menu
 */
void moiInit(MenuOfItems* M, const char* Text, unsigned short ItemCount);

/**
 * @brief Destructeur
 * @param M MenuOfItem auquel s'applique la fonction
 */
void moiFree(MenuOfItems* M);

/**
 * @brief Ajoute un item au menu
 * @param M MenuOfItem auquel s'applique la fonction
 * @param Text texte de l'item
 * @param Type Type de l'item
 * @param Function fonction associée à l'item
 * @param Data pointeur vers des données
 */
void moiAddItem(MenuOfItems* M, const char* Text, ItemType Type, void (*Function)(void), void* Data);

/**
 * @brief Bouger dans le menu
 * @param M MenuOfItem auquel s'applique la fonction
 * @param Direction aller ver le haut ou ver le bas dans le menu
 */
void moiMoveCursor(MenuOfItems* M, MenuDirection Direction);

/**
 * @brief Accède à l'item qui est sélectionné
 * @param M MenuOfItem auquel s'applique la fonction
 * @return Pointeur vers l'item selectionné
 */
MenuItem* moiGetItemSelected(MenuOfItems* M);

/**
 * @brief Met à jour le zoom des items
 * @param SelectedFactor Facteur de zoom pour un item sélectioné
 */
void moiUpdateVisuals(MenuOfItems* M, float SelectedFactor);

/**
 * @brief Mutateur de SizeTo
 * @param Size taille de la boîte du menu
 */
void moiSetSize(MenuOfItems* M, Vec2 Size);

/**
 * @brief Accesseur de SizeTo
 * @return taille de la boîte du menu
 */
Vec2 moiGetSize(const MenuOfItems* M);

/**
 * @brief Mutateur de Size
 *
 * Ici on change la valeur qui est utilisé pour dessiner, le changement sera instantané et non fluide
 * @param Size taille de la boîte du menu
 */
void moiSetActualSize(MenuOfItems* M, Vec2 Size);

/**@}*/

#endif