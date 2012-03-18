#ifndef _WORLD_H_
#define _WORLD_H_

#include "Core/List.h"
#include "Physics/Vertex.h"
#include "Physics/Rigid.h"
#include "Physics/Polygon.h"

/** @defgroup World
 *
 * Liste les objets du monde, appelle les méthodes de mise à jour
 * Préfixe des fonctions : wd
 * @{
**/

/** @brief Structure du monde physique
 *
 * Les Rigids des polygons étant considérés comme liés au Polygon
 * ils sont gérés à part, en revanche, leur Vertices doivent être
 * ajoutés manuellement au World pour être mis à jour par les
 * méthodes générales ! En effet un Vertex peut être partagé par
 * plusieurs polygons.
**/
typedef struct
{
	List Vertices; /**< Liste de Vertices **/
	List Rigids; /**< Liste de contraintes isolées **/
	List Polygons; /**< Liste de Polygones **/

	float Width; /**< Largeur du monde **/
	float Height; /**< Hauteur du monde **/

	float prevdt; /**< Intervalle de temps précédent **/
	float dt; /**< Intervalle de temps courant pour la simulation **/
} World;

/** @brief Constructeur
 *
 * Appelle wbInit après l'allocation
 * @param W Largeur du monde
 * @param H Hauteur du monde
**/
World* newWorld(float Width, float Height);

/** @brief Initialise le monde
 *
 * @param W Largeur du monde
 * @param H Hauteur du monde
**/
void wdInit(World* W, float Width, float Height);

/** @brief Ajoute un Vertex au monde
 *
 *
**/
void wdAddVertex(World* W, Vertex* V);

/** @brief Ajoute un Polygon au monde
 *
**/
void wdAddPolygon(World* W, Polygon* P);

/** @brief Ajoute un Rigid au monde
 *
 *
 **/
void wdAddRigid(World* W, Rigid* R);

/** @brief Applique une force à tout les vextices du monde
 *
 * Ne parcours que la liste Vertices, les vertices faisant partie d'un
 * ensemble plus grand du monde (polygon...) doivent aussi y figurer !
 * @param W Monde à mettre à jour
**/
void wdApplyForce(World* W, Vec2 Force);

/** @brief Appelle Resolve pour tout les Vertices du monde
 *
 * Ne parcours que la liste Vertices, les vertices faisant partie d'un
 * ensemble plus grand du monde (polygon...) doivent aussi y figurer !
 * @param W Monde à mettre à jour
**/
void wdResolveVextex(World* W);

/** @brief Appelle Resolve() pour tout les Rigid du monde
 *
 * Resolve() les contraintes rigides isolées et contenues dans des polygones
 * non fixes.
 * @param W World
**/
void wdResolveRigid(World* W);

/** @brief Recherche et réagit aux collisions
 *
 * Gère les collision Polygon/Polygon
 * @param W World
**/
void wdHandleCollision(World* W, Bool DebugDraw);

/** @brief Libère la mémoire occupée
 *
 * Détruit tout les objets contenu en vidant les liste
**/
void wdFree(World *W);

/** @brief Dextructeur
 *
 * Appelle wbFree avant de compléter la libération
**/
void delWorld(World *W);

/** @}
**/


#endif

