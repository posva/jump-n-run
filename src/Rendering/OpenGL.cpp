#include "OpenGL.h"

#include <Objects/Flag.h>
#include <Level/Level.h>


void glDrawLine(float X1, float Y1, float X2, float Y2, float R, float G, float B, float A)
{
	glColor4f(R, G, B, A);
	glBegin(GL_LINES);
	glVertex2f(X1, Y1);
	glVertex2f(X2, Y2);
	glEnd();
}

void glDrawVertex(Vertex* V, float R, float G, float B, float A)
{
	unsigned int i;
	glBegin(GL_TRIANGLE_FAN);
	glColor4f(R, G, B, A);
	glVertex2f(vxGetPosition(V).x, vxGetPosition(V).y);
	for (i = 0; i <= 16; i++)
	{
		glVertex2f(1*4.0*cos((2.0*M_PI)*(i/static_cast<double>(16))) + vxGetPosition(V).x,
				1*4.0*sin((2.0*M_PI)*(i/static_cast<double>(16))) + vxGetPosition(V).y);
	}
	glEnd();
}

void glDrawElastic(Elastic* E)
{
	glColor4f(fabs(vec2SqLength(elVector(E)) - elGetLength(E)*elGetLength(E))/vec2SqLength(elVector(E)) + 0.2f, 0.f, 0.f, 1.f);
	glBegin(GL_LINES);
	glVertex2f(vxGetPosition(elGetV1(E)).x, vxGetPosition(elGetV1(E)).y);
	glVertex2f(vxGetPosition(elGetV2(E)).x, vxGetPosition(elGetV2(E)).y);
	glEnd();
}

void glDrawRigid(Rigid* R)
{
	glColor4f(0.f, 0.f, 1.f, 1.f);
	glBegin(GL_LINES);
	glVertex2f(vxGetPosition(rdGetV1(R)).x, vxGetPosition(rdGetV1(R)).y);
	glVertex2f(vxGetPosition(rdGetV2(R)).x, vxGetPosition(rdGetV2(R)).y);
	glEnd();
}

void glDrawPolygon(Polygon* P)
{
	unsigned int i;

	if(polyIsConvexe(P)) glColor4f(1.f, 1.f, 1.f, 0.2f);
		else glColor4f(1.f, 0.f, 0.f, 0.2f);
	glBegin(GL_POLYGON);
	for(i = 0; i < daGetSize(&P->Vertices); i++)
		glVertex2f(vxGetPosition((Vertex*)daGet(&P->Vertices, i)).x, vxGetPosition((Vertex*)daGet(&P->Vertices, i)).y);
	glEnd();

	for(i = 0; i < daGetSize(&P->Rigids); i++)
	{
		glDrawRigid((Rigid*)daGet(&P->Rigids, i));
	}

	for(i = 0; i < daGetSize(&P->InternalRigids); i++)
	{
		glDrawRigid((Rigid*)daGet(&P->InternalRigids, i));
	}

	if(polyIsFixed(P)) glColor4f(1.f, 0.f, 0.f, 1.f); else glColor4f(0.f, 0.f, 1.f, 1.f);
	Vec2 Center = polyComputeCenter(P);
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(Center.x, Center.y);
	for (i = 0; i <= 16; i++)
	{
		glVertex2f(1*4.0*cos((2.0*M_PI)*(i/static_cast<double>(16))) + Center.x,
				1*4.0*sin((2.0*M_PI)*(i/static_cast<double>(16))) + Center.y);
	}
	glEnd();

	/* BBox
	BBox B = polyGetBBox(P);
	glColor4f(1.f, 0.f, 0.f, 0.5f);
	glBegin(GL_LINE_LOOP);
		glVertex2f(B.Left, B.Top);
		glVertex2f(B.Right, B.Top);
		glVertex2f(B.Right, B.Bottom);
		glVertex2f(B.Left, B.Bottom);
	glEnd(); */
}

void glDrawCircle(float x, float y, float radius)
{
	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(x, y);
	for (int i=0; i<=16; i++)
	{
		glVertex2f(radius*cos((2.0*M_PI)*(i/static_cast<double>(16))) + x,
				   radius*sin((2.0*M_PI)*(i/static_cast<double>(16))) + y);
	}
	glEnd();
}

Texture glTexLoad(const char* Path)
{
	sf::Image image;
	Bool LoadSuccess = 0;
	GLuint texture = 0;

	if(Path[0] != '\0')
	{
		#ifdef SFML_SYSTEM_MACOS
		LoadSuccess = image.loadFromFile(ResourcePath()+Path); //À rajouter ResourcePath selon compilation
		#else
		LoadSuccess = image.loadFromFile(Path);
		#endif
	}

	glEnable(GL_TEXTURE_2D);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	if(LoadSuccess)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.getSize().x, image.getSize().y, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		// Génération de Mipmaps... Pas très utile en 2D, non ?
		//gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGBA, image.getWidth(), image.getHeight(), GL_RGBA, GL_UNSIGNED_BYTE, image.getPixelsPtr());
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	} else {
		/* Texture transparente si le fichier n'a pas pu être chargé */
		GLubyte TexNull[4] =
		{
			0,0,0,0
		};
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, TexNull);
	}
	glDisable(GL_TEXTURE_2D);

	return texture;
}

void glTexFree(Texture T)
{
	glDeleteTextures(1, &T);
}

void glDisplayTex(Texture T, Vec2 TexUL, Vec2 TexUR, Vec2 TexDR, Vec2 TexDL,
				Vec2 UL, Vec2 UR, Vec2 DR, Vec2 DL)
{
	glColor4f(1.f, 1.f, 1.f, 1.f); /* Chaque pixel de la texture est multiplié par cette couleur */
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, T);
	glBegin(GL_QUADS);
	glTexCoord2d(TexUL.x, TexUL.y);  glVertex2f(UL.x, UL.y);
	glTexCoord2d(TexUR.x, TexUR.y);  glVertex2f(UR.x, UR.y);
	glTexCoord2d(TexDR.x, TexDR.y);  glVertex2f(DR.x, DR.y);
	glTexCoord2d(TexDL.x, TexDL.y);  glVertex2f(DL.x, DL.y);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void glDispTexPoly(Texture T, Polygon* P, List* L)
{
	Node* it;
	unsigned int i = 0;
	Vec2 VPos, CoordTex;
	if(polyGetVxCount(P) != lstCount(L)) return;

	glColor4f(1.f, 1.f, 1.f, 1.f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, T);
	glBegin(GL_POLYGON);
	it = lstFirst(L);
	while(!nodeEnd(it))
	{
		VPos = vxGetPosition(polyGetVertex(P, i));
		CoordTex = *((Vec2*) nodeGetData(it));
		glTexCoord2d(CoordTex.x, CoordTex.y);  glVertex2f(VPos.x, VPos.y);
		it = nodeGetNext(it);
		i++;
	}
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void glDrawMenuItems(SharedResources* SR, sf::RenderTarget& win, Menu* M, float ViewX, float ViewY, float ViewWidth, float ViewHeight)
{
	MenuOfItems* moi = mnGetCurrentMenu(M);
	Vec2 Size = moiGetSize(moi),
	Position = mnGetPosition(M);

	if (Position.y+Size.y+20.f <= 0.f) // Pas besoin de dessiner
		return;

	unsigned short i;
	MenuItem* I;
	float MaxTextWidth = 0.f;
	sf::Text ItemText;
	ItemText.setFont(shFntMenu(SR));
	ItemText.setString(std::string(moiGetText(moi)));
	float TitleWidth = (ItemText.findCharacterPos(ItemText.getString().getSize()-1)-ItemText.findCharacterPos(0)).x+ItemText.getCharacterSize();
	TitleWidth *= mnGetItemSelectedZoomFactor(M);



	float heigth = 5.f; //offset
	for (i=0; i<moiGetItemCount(moi); i++)
	{
		I = moiGetItem(moi, i);
		heigth+=mnGetItemHeight(M)*(*mniGetZoom(I));
	}


	Size.x = MAX(TitleWidth+10.f, Size.x);

	Vec2 TitleSize = vec2(TitleWidth, 60.f);
	Vec2 TitlePos = vec2(Position.x+Size.x/2.f-TitleSize.x/2.f, Position.y-TitleSize.y);



	win.pushGLStates();



	 if (strcmp(moiGetText(moi), "")!=0)
	 {

		 ItemText.setScale(mnGetItemSelectedZoomFactor(M), mnGetItemSelectedZoomFactor(M));
		 ItemText.setPosition(TitlePos.x, TitlePos.y+13.f);


		 ItemText.setColor(sf::Color(0,0,0));
		 win.draw(ItemText);
		 ItemText.move(0.f, 1.5f);
		 ItemText.setColor(sf::Color(255,255,255));
		 win.draw(ItemText);

	 }


	 float yoffset = 5.f, selOffset = 0.f;
	 for (i=0; i<moiGetItemCount(moi); i++)
	 {
		 I = moiGetItem(moi, i);
		 if (strcmp(mniGetText(I), "")==0)
			 ItemText.setString(" "); // une chaîne vide donne une erreur
		 else
			 ItemText.setString(std::string(mniGetText(I)));

		 switch (mniGetType(I))
		 {
			 case ITEM_CHECKBOX:
				 if (*(Bool*)mniGetData(I))
					 ItemText.setString(ItemText.getString() + ": Yes");
				 else
					 ItemText.setString(ItemText.getString() + ": No");
				 break;

			 case ITEM_INPUT:
			 case ITEM_INPUT_MULTILINE:
				 ItemText.setString(ItemText.getString() + ": " + *(std::string*)mniGetData(I));
				 break;
			 case ITEM_INPUT_VALUE:
				 ItemText.setString(ItemText.getString() + ": " + *(I->Str));
				 break;
			 case ITEM_VALUE:
				 char ValueText[300], tmp[20];
				 sprintf(tmp, ": a.%uf", mniGetFloatPrecision(I));
				 tmp[2]='%';
				 sprintf(ValueText, tmp, *(float*)mniGetData(I));
				 ItemText.setString(ItemText.getString() + std::string(ValueText));
				 break;
			 default:
				 break;
	 }

	 ItemText.setScale(1.f, 1.f);
	 ItemText.setPosition(Position.x+5.f, Position.y+yoffset+5.f);

	 ItemText.setScale(*mniGetZoom(I), *mniGetZoom(I));

	 if (i != moiGetItemSelectedID(moi))
	 {
		 //win.pushGLStates();
		 ItemText.setColor(sf::Color(0,0,0));
		 win.draw(ItemText);
		 ItemText.move(0.f, 1.5f);
		 ItemText.setColor(sf::Color(255,255,255));
		 win.draw(ItemText);
		 //win.popGLStates();
	 }
	 else
	 selOffset = yoffset;


	 yoffset+=((ItemText.findCharacterPos(ItemText.getString().getSize()-1)-ItemText.findCharacterPos(0)).y+ItemText.getCharacterSize())*(*mniGetZoom(I));

	 float Width;
	 if (mniGetType(I) != ITEM_INPUT_MULTILINE)
	 Width = (ItemText.findCharacterPos(ItemText.getString().getSize()-1)-ItemText.findCharacterPos(0)).x+ItemText.getCharacterSize();
	 else
	 {
		 float extrax = 0.f, calc;
		 //On cherche tous les sauts à la ligne pour trouver la ligne la plus longue
		 std::size_t pos = ItemText.getString().find("\n"), last_pos = 0;
		 while (pos != sf::String::InvalidPos)
		 {
			 calc = ((ItemText.findCharacterPos(pos)-ItemText.findCharacterPos(0)).x+ItemText.getCharacterSize());
			 if (calc > extrax)
				 extrax = calc;
			 last_pos = pos;
			 pos = ItemText.getString().find("\n", last_pos+1);
			 if (pos == last_pos)
				 pos = sf::String::InvalidPos;

		 }

		 calc = ((ItemText.findCharacterPos(ItemText.getString().getSize()-1)-ItemText.findCharacterPos(0)).x+ItemText.getCharacterSize());
		 if (calc > extrax)
			 extrax = calc;

		 if (last_pos == 0 && pos == sf::String::InvalidPos) //Pas de saut à la ligne
			 Width = ((ItemText.findCharacterPos(ItemText.getString().getSize()-1)-ItemText.findCharacterPos(0)).x+ItemText.getCharacterSize());
		 else
			 Width = extrax;
	 }
	 if (Width>MaxTextWidth)
		 MaxTextWidth=Width;

	 mnSetItemHeight(M, ItemText.getCharacterSize());



	 }
	 Size.y = yoffset+10.f;

	 if (moiGetItemSelectedID(moi) != INVALID_ITEM_ID)
	 {
		 I = moiGetItemSelected(moi);
		 if (strcmp(mniGetText(I), "")==0)
			 ItemText.setString(" "); // une chaîne vide donne une erreur
		 else
			 ItemText.setString(std::string(mniGetText(I)));

		 switch (mniGetType(I))
		 {
			 case ITEM_CHECKBOX:
				 if (*(Bool*)mniGetData(I))
					 ItemText.setString(ItemText.getString() + ": Yes");
				 else
					 ItemText.setString(ItemText.getString() + ": No");
				 break;

			 case ITEM_INPUT:
			 case ITEM_INPUT_MULTILINE:
				 ItemText.setString(ItemText.getString() + ": " + *(std::string*)mniGetData(I));
				 break;
			 case ITEM_INPUT_VALUE:
				 ItemText.setString(ItemText.getString() + ": " + *(I->Str));
				 break;
			 case ITEM_VALUE:
				 char ValueText[300], tmp[20];
				 sprintf(tmp, ": a.%uf", mniGetFloatPrecision(I));
				 tmp[2]='%';
				 sprintf(ValueText, tmp, *(float*)mniGetData(I));
				 ItemText.setString(ItemText.getString() + std::string(ValueText));
				 break;
			 default:
				 break;
		 }

		 ItemText.setScale(1.f, 1.f);
		 ItemText.setPosition(Position.x+5.f, Position.y+selOffset+5.f);

		 ItemText.setScale(*mniGetZoom(I), *mniGetZoom(I));

		 yoffset+=((ItemText.findCharacterPos(ItemText.getString().getSize()-1)-ItemText.findCharacterPos(0)).y+ItemText.getCharacterSize())*(*mniGetZoom(I));


		 //win.pushGLStates();
		 ItemText.setColor(sf::Color(0,0,0));
		 win.draw(ItemText);
		 ItemText.move(0.f, 1.5f);
		 ItemText.setColor(sf::Color(255,255,255));
		 win.draw(ItemText);
		 //win.popGLStates();



		 float Width;
		 if (mniGetType(I) != ITEM_INPUT_MULTILINE)
			 Width = (ItemText.findCharacterPos(ItemText.getString().getSize()-1)-ItemText.findCharacterPos(0)).x+ItemText.getCharacterSize();
	 else
	 {
		 float extrax = 0.f, calc;
		 //On cherche tous les sauts à la ligne pour trouver la ligne la plus longue
		 std::size_t pos = ItemText.getString().find("\n"), last_pos = 0;
		 while (pos != sf::String::InvalidPos)
		 {
			 calc = ((ItemText.findCharacterPos(pos)-ItemText.findCharacterPos(0)).x+ItemText.getCharacterSize());
			 if (calc > extrax)
				 extrax = calc;
			 last_pos = pos;
			 pos = ItemText.getString().find("\n", last_pos+1);
			 if (pos == last_pos)
				 pos = sf::String::InvalidPos;

		 }

		 calc = ((ItemText.findCharacterPos(ItemText.getString().getSize()-1)-ItemText.findCharacterPos(0)).x+ItemText.getCharacterSize());
		 if (calc > extrax)
			 extrax = calc;

		 if (last_pos == 0 && pos == sf::String::InvalidPos) //Pas de saut à la ligne
			 Width = ((ItemText.findCharacterPos(ItemText.getString().getSize()-1)-ItemText.findCharacterPos(0)).x+ItemText.getCharacterSize());
		 else
			 Width = extrax;
	 }
		 if (Width>MaxTextWidth)
			 MaxTextWidth=Width;

	 }


	moiSetSize(moi, vec2(MaxTextWidth+5.f, Size.y));

	win.popGLStates();

}

void glDrawMenuBox(SharedResources* SR, sf::RenderTarget& win, Menu* M, float ViewX, float ViewY, float ViewWidth, float ViewHeight)
{
	MenuOfItems* moi = mnGetCurrentMenu(M);
	Vec2 Size = moiGetSize(moi),
	Position = mnGetPosition(M);

	if (Position.y+Size.y+20.f <= 0.f) // Pas besoin de dessiner
		return;

	unsigned short i;
	MenuItem* I;
	sf::Text ItemText;
	ItemText.setFont(shFntMenu(SR));
	ItemText.setString(std::string(moiGetText(moi)));
	float TitleWidth = (ItemText.findCharacterPos(ItemText.getString().getSize()-1)-ItemText.findCharacterPos(0)).x+ItemText.getCharacterSize();
	TitleWidth *= mnGetItemSelectedZoomFactor(M);



	glPushMatrix();

	float heigth = 5.f; //offset
	for (i=0; i<moiGetItemCount(moi); i++)
	{
		I = moiGetItem(moi, i);
		heigth+=mnGetItemHeight(M)*(*mniGetZoom(I));
	}


	glTranslatef(ViewX, ViewY, 0.f);
	glScalef(ViewWidth/win.getSize().x, ViewHeight/win.getSize().y, 1.f);



	Size.x = MAX(TitleWidth+10.f, Size.x);

	Vec2 TitleSize = vec2(TitleWidth, 60.f);
	Vec2 TitlePos = vec2(Position.x+Size.x/2.f-TitleSize.x/2.f, Position.y-TitleSize.y);

	if (strcmp(moiGetText(moi), "")!=0)
		glDrawTitleBox(SR, TitlePos, TitleSize);

	glDrawBox(SR, Position, Size, (int)M->SubAnim);

	glPopMatrix();
}

void glDrawCursor(sf::RenderTarget& win, float ViewWidth, float ViewHeight, float MouseX, float MouseY, sf::Sprite& sprCursor)
{
	//Les states de OpenGl ont déjà été push car le curseur c'est ce qui se dessine en dernier
	win.pushGLStates();
	sprCursor.setPosition(MouseX, MouseY);
	win.draw(sprCursor);
	win.popGLStates();
}


void glDrawCloth(Cloth* C, Texture T)
{
	glColor4f(1.f, 1.f, 1.f, 1.f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, T);
	//vxGetPosition(C->Points
	glBegin(GL_QUADS);
	unsigned int i, j;

	for (i=0; i<C->HCells-1; i++)
		for (j=0; j<C->VCells-1; j++)
		{
			//if (i>=C->HCells-1 || j>=C->VCells-1) continue;

			//left top
			glTexCoord2f(static_cast<float>(i)/static_cast<float>(C->HCells-1), static_cast<float>(j)/static_cast<float>(C->VCells-1));
			glVertex2f(vxGetPosition(C->Points[i][j]).x, vxGetPosition(C->Points[i][j]).y);

			//right top
			glTexCoord2f(static_cast<float>(i+1)/static_cast<float>(C->HCells-1), static_cast<float>(j)/static_cast<float>(C->VCells-1));
			glVertex2f(vxGetPosition(C->Points[i+1][j]).x, vxGetPosition(C->Points[i+1][j]).y);

			//right bottom
			glTexCoord2f(static_cast<float>(i+1)/static_cast<float>(C->HCells-1), static_cast<float>(j+1)/static_cast<float>(C->VCells-1));
			glVertex2f(vxGetPosition(C->Points[i+1][j+1]).x, vxGetPosition(C->Points[i+1][j+1]).y);

			//Left bottom
			glTexCoord2f(static_cast<float>(i)/static_cast<float>(C->HCells-1), static_cast<float>(j+1)/static_cast<float>(C->VCells-1));
			glVertex2f(vxGetPosition(C->Points[i][j+1]).x, vxGetPosition(C->Points[i][j+1]).y);
		}

	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void glDispFlag(Flag* F, float X, float Y)
{
	glPushMatrix();
	glTranslatef(X, Y - F->H*F->cellH, 0.f);
	glColor4f(1.f, 1.f, 1.f, 1.f);
	glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, F->TexPole);
	glBegin(GL_QUADS);
	glTexCoord2f(0.f, 0.f);
	Vec2 Pos = vxGetPosition((Vertex*) daGet(&F->Vertices, 0));
	glVertex2f((float) Pos.x - 5.f, (float) Pos.y);
	glTexCoord2f(1.f, 0.f);
	glVertex2f((float) Pos.x + 5.f, (float) Pos.y);
	glTexCoord2f(1.f, 1.f);
	glVertex2f((float) Pos.x + 5.f, (float) Pos.y + F->H*F->cellH*2);
	glTexCoord2f(0.f, 1.f);
	glVertex2f((float) Pos.x - 5.f, (float) Pos.y + F->H*F->cellH*2);
	glEnd();

	glBindTexture(GL_TEXTURE_2D, F->TexFlag);

	for(unsigned int i = 0; i < F->H - 1; i++)
	{
		glBegin(GL_TRIANGLE_STRIP);
		for(unsigned int j = 0; j < F->W; j++)
		{
			Vec2 Pos = vxGetPosition((Vertex*) daGet(&F->Vertices, i*F->W + j));
			glTexCoord2f((float) j/(F->W-1), (float) i/(F->H-1));
			glVertex2f((float) Pos.x, (float) Pos.y);

			Pos = vxGetPosition((Vertex*) daGet(&F->Vertices, (i + 1)*F->W + j));
			glTexCoord2f((float) j/(F->W-1), (float) (i + 1)/(F->H-1));
			glVertex2f((float) Pos.x, (float) Pos.y);

		}
		glEnd();
	}

	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}

void glDrawFPS(SharedResources* SR, sf::RenderTarget& win, const std::string& FPS)
{
	sf::Text Text;
	Text.setFont(shFntMenu(SR));
	Text.setString(FPS);
	Text.setPosition(5.f, 5.f);
	Text.setColor(sf::Color::Black);
	win.pushGLStates();
	Text.move(-1.5f, 0.f);
	win.draw(Text);
	Text.move(3.f, 0.f);
	win.draw(Text);
	Text.move(-1.5f, -1.5f);
	win.draw(Text);
	Text.move(0.f, 1.5f);
	win.draw(Text);

	Text.setColor(sf::Color::White);
	Text.move(0.f, -1.5f);
	win.draw(Text);
	win.popGLStates();
}

void glDrawTime(SharedResources* SR, sf::RenderTarget& win, unsigned int Time)
{
	std::ostringstream oss;
	oss << "Time : ";
	if(Time/6000 < 10) oss << "0";
	oss << Time/6000 << ":";
	if((Time/100)%60 < 10) oss << "0";
	oss << (Time/100)%60 << ":";
	if(Time%100 < 10) oss << "0";
	oss << Time%100;
	sf::Text Text;
	Text.setFont(shFntMenu(SR));
	Text.setString(oss.str());
	Text.setPosition(win.getSize().x - 250.f, 5.f);
	Text.setColor(sf::Color::Black);
	win.pushGLStates();
	Text.move(-1.5f, 0.f);
	win.draw(Text);
	Text.move(3.f, 0.f);
	win.draw(Text);
	Text.move(-1.5f, -1.5f);
	win.draw(Text);
	Text.move(0.f, 1.5f);
	win.draw(Text);

	Text.setColor(sf::Color::White);
	Text.move(0.f, -1.5f);
	win.draw(Text);
	win.popGLStates();
}

void glDrawPolyFromList(List* L, Vec2 MousePos)
{
	if (lstCount(L) <= 0) return;

	Node* it = lstFirst(L);

	if (lstCount(L) > 1)
	{
		glBegin(GL_POLYGON);
		while (!nodeEnd(it))
		{
			Vertex* V = (Vertex*) nodeGetData(it);
			glVertex2f(vxGetPosition(V).x, vxGetPosition(V).y);
			it = nodeGetNext(it);
		}
		glVertex2f(MousePos.x, MousePos.y);
		glEnd();
	}
	else
	{
		glBegin(GL_LINES);
		Vertex* V = (Vertex*) nodeGetData(it);
		glVertex2f(vxGetPosition(V).x, vxGetPosition(V).y);
		glVertex2f(MousePos.x, MousePos.y);
		glEnd();
	}

}


void glDrawBox(SharedResources* SR, Vec2 Position, Vec2 Size, int SubAnim)
{
	//n dessine les 4 corners
	glColor4f(1.f, 1.f, 1.f, 1.f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, shGetTexture(SR, "mn_corner"));

	//TopLeft
	glBegin(GL_QUADS);

	glTexCoord2f(0.f, 0.f);
	glVertex2f(Position.x-13.f, Position.y);

	glTexCoord2f(1.f, 0.f);
	glVertex2f(Position.x - 2.f, Position.y);

	glTexCoord2f(1.f, 1.f);
	glVertex2f(Position.x - 2.f, ceilf(Position.y + 11.f));

	glTexCoord2f(0.f, 1.f);
	glVertex2f(Position.x-13.f, ceilf(Position.y + 11.f));


	//TopRight

	glTexCoord2f(-1.f, 0.f);
	glVertex2f(Position.x + Size.x , Position.y);

	glTexCoord2f(0.f, 0.f);
	glVertex2f(Position.x + Size.x - 11.f, Position.y);

	glTexCoord2f(0.f, 1.f);
	glVertex2f(Position.x + Size.x - 11.f, ceilf(Position.y + 11.f));

	glTexCoord2f(-1.f, 1.f);
	glVertex2f(Position.x + Size.x, ceilf(Position.y + 11.f));


	//BottomLeft

	glTexCoord2f(0.f, -1.f);
	glVertex2f(Position.x-13.f, Position.y+Size.y+13.f);

	glTexCoord2f(1.f, -1.f);
	glVertex2f(Position.x - 2.f, Position.y+Size.y+13.f);

	glTexCoord2f(1.f, 0.f);
	glVertex2f(Position.x - 2.f, Position.y + Size.y+2.f);

	glTexCoord2f(0.f, 0.f);
	glVertex2f(Position.x-13.f, Position.y +Size.y+2.f);


	//BottomRight

	glTexCoord2f(-1.f, -1.f);
	glVertex2f(Position.x+Size.x, Position.y+Size.y+13.f);

	glTexCoord2f(0.f, -1.f);
	glVertex2f(Position.x +Size.x-11.f, Position.y+Size.y+13.f);

	glTexCoord2f(0.f, 0.f);
	glVertex2f(Position.x +Size.x-11.f, Position.y + Size.y+2.f);

	glTexCoord2f(-1.f, 0.f);
	glVertex2f(Position.x+Size.x, Position.y +Size.y+2.f);

	glEnd();


	//On dessine le Side
	glBindTexture(GL_TEXTURE_2D, shGetTexture(SR, "mn_side"));

	//Left
	glBegin(GL_QUADS);
	glTexCoord2f(0.f, 0.f);
	glVertex2f(Position.x-13.f, Position.y+11.f);

	glTexCoord2f(1.f, 0.f);
	glVertex2f(Position.x - 2.f, Position.y+11.f);

	glTexCoord2f(1.f, 1.f);
	glVertex2f(Position.x - 2.f, ceilf(Position.y +Size.y +2.f));

	glTexCoord2f(0.f, 1.f);
	glVertex2f(Position.x-13.f, ceilf(Position.y +Size.y+2.f));

	//Right
	glTexCoord2f(1.f, 0.f);
	glVertex2f(Position.x-11.f+Size.x, Position.y+11.f);

	glTexCoord2f(0.f, 0.f);
	glVertex2f(Position.x +Size.x, Position.y+11.f);

	glTexCoord2f(0.f, 1.f);
	glVertex2f(Position.x +Size.x, ceilf(Position.y +Size.y +2.f));

	glTexCoord2f(1.f, 1.f);
	glVertex2f(Position.x-11.f+Size.x, ceilf(Position.y +Size.y+2.f));


	//Top
	glTexCoord2f(0.f, 1.f);
	glVertex2f(Position.x-2.f, Position.y);

	glTexCoord2f(0.f, 0.f);
	glVertex2f(ceilf(Position.x - 11.f+Size.x), Position.y);

	glTexCoord2f(1.f, 0.f);
	glVertex2f(ceilf(Position.x - 11.f+Size.x), ceilf(Position.y +11.f));

	glTexCoord2f(1.f, 1.f);
	glVertex2f(Position.x-2.f, ceilf(Position.y +11.f));

	//Bottom
	glTexCoord2f(1.f, 0.f);
	glVertex2f(Position.x-2.f, Position.y+Size.y+2.f);

	glTexCoord2f(1.f, 1.f);
	glVertex2f(ceilf(Position.x - 11.f+Size.x), Position.y+Size.y+2.f);

	glTexCoord2f(0.f, 1.f);
	glVertex2f(ceilf(Position.x - 11.f+Size.x), Position.y +13.f+Size.y);

	glTexCoord2f(0.f, 0.f);
	glVertex2f(Position.x-2.f, Position.y +13.f+Size.y);

	//Shine
	glEnd();

	//back anim 800x40

	glColor4f(0.4f, 0.4f, 0.4f, 0.6f);

	glBindTexture(GL_TEXTURE_2D, shGetTexture(SR, "mn_anim"));

	glBegin(GL_QUADS);
	// when texture area is large, bilinear filter the original
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	glTexCoord2f((SubAnim)/20.f, 0.f);
	glVertex2f(Position.x-2.f, Position.y+11.f);

	glTexCoord2f((SubAnim+1)/20.f, 0.f);
	glVertex2f(Position.x - 11.f+Size.x, Position.y+11.f);

	glTexCoord2f((SubAnim+1)/20.f, (Size.y-9.f)/40.f);
	glVertex2f(Position.x - 11.f+Size.x, ceilf(Position.y +Size.y+2.f));

	glTexCoord2f((SubAnim)/20.f, (Size.y-9.f)/40.f);
	glVertex2f(Position.x-2.f, ceilf(Position.y +Size.y+2.f));

	glEnd();

	//Back
	glBindTexture(GL_TEXTURE_2D, shGetTexture(SR, "mn_shadow"));

	glBegin(GL_QUADS);

	glTexCoord2f(0.f, 0.f);
	glVertex2f(Position.x-2.f, Position.y+11.f);

	glTexCoord2f(1.f, 0.f);
	glVertex2f(Position.x - 11.f+Size.x, Position.y+11.f);

	glTexCoord2f(1.f, 1.f);
	glVertex2f(Position.x - 11.f+Size.x, Position.y +Size.y+2.f);

	glTexCoord2f(0.f, 1.f);
	glVertex2f(Position.x-2.f, Position.y +Size.y+2.f);


	glEnd();
	//gloss
	glColor4f(1.f, 1.f, 1.f, 1.f);
	glBindTexture(GL_TEXTURE_2D, shGetTexture(SR, "mn_gloss"));

	glBegin(GL_QUADS);

	glTexCoord2f(0.f, 0.f);
	glVertex2f(Position.x-2.f, Position.y+11.f);

	glTexCoord2f(1.f, 0.f);
	glVertex2f(Position.x - 11.f+Size.x, Position.y+11.f);

	glTexCoord2f(1.f, 1.f);
	glVertex2f(Position.x - 11.f+Size.x, Position.y + MIN(50.f, Size.y-11.f) +11.f);

	glTexCoord2f(0.f, 1.f);
	glVertex2f(Position.x-2.f, Position.y + MIN(50.f, Size.y-11.f)+ 11.f);


	glEnd();


	glDisable(GL_TEXTURE_2D);

}

void glDrawTitleBox(SharedResources* SR, Vec2 Position, Vec2 Size)
{
	//n dessine les 4 corners
	glColor4f(1.f, 1.f, 1.f, 1.f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, shGetTexture(SR, "mn_corner"));

	//TopLeft
	glBegin(GL_QUADS);

	glTexCoord2f(0.f, 0.f);
	glVertex2f(Position.x-13.f, Position.y);

	glTexCoord2f(1.f, 0.f);
	glVertex2f(Position.x - 2.f, Position.y);

	glTexCoord2f(1.f, 1.f);
	glVertex2f(Position.x - 2.f, ceilf(Position.y + 11.f));

	glTexCoord2f(0.f, 1.f);
	glVertex2f(Position.x-13.f, ceilf(Position.y + 11.f));


	//TopRight

	glTexCoord2f(-1.f, 0.f);
	glVertex2f(Position.x + Size.x , Position.y);

	glTexCoord2f(0.f, 0.f);
	glVertex2f(Position.x + Size.x - 11.f, Position.y);

	glTexCoord2f(0.f, 1.f);
	glVertex2f(Position.x + Size.x - 11.f, ceilf(Position.y + 11.f));

	glTexCoord2f(-1.f, 1.f);
	glVertex2f(Position.x + Size.x, ceilf(Position.y + 11.f));



	glEnd();

	//On dessine le Side
	glBindTexture(GL_TEXTURE_2D, shGetTexture(SR, "mn_side"));

	//Left
	glBegin(GL_QUADS);

	glTexCoord2f(0.f, 0.f);
	glVertex2f(Position.x-13.f, Position.y+11.f);

	glTexCoord2f(1.f, 0.f);
	glVertex2f(Position.x - 2.f, Position.y+11.f);

	glTexCoord2f(1.f, 1.f);
	glVertex2f(Position.x - 2.f, Position.y +Size.y +2.f);

	glTexCoord2f(0.f, 1.f);
	glVertex2f(Position.x-13.f, Position.y +Size.y+2.f);

	//Right
	glTexCoord2f(1.f, 0.f);
	glVertex2f(Position.x-11.f+Size.x, Position.y+11.f);

	glTexCoord2f(0.f, 0.f);
	glVertex2f(Position.x +Size.x, Position.y+11.f);

	glTexCoord2f(0.f, 1.f);
	glVertex2f(Position.x +Size.x, Position.y +Size.y +2.f);

	glTexCoord2f(1.f, 1.f);
	glVertex2f(Position.x-11.f+Size.x, Position.y +Size.y+2.f);


	//Top
	glTexCoord2f(0.f, 1.f);
	glVertex2f(Position.x-2.f, Position.y);

	glTexCoord2f(0.f, 0.f);
	glVertex2f(ceilf(Position.x - 11.f+Size.x), Position.y);

	glTexCoord2f(1.f, 0.f);
	glVertex2f(ceilf(Position.x - 11.f+Size.x), ceilf(Position.y +11.f));

	glTexCoord2f(1.f, 1.f);
	glVertex2f(Position.x-2.f, ceilf(Position.y +11.f));

	glEnd();

	//back anim 800x40

	glColor4f(0.4f, 0.4f, 0.4f, 0.6f);

	//Back
	glBindTexture(GL_TEXTURE_2D, shGetTexture(SR, "mn_shadow"));

	glBegin(GL_QUADS);

	glTexCoord2f(0.f, 0.f);
	glVertex2f(Position.x-2.f, Position.y+11.f);

	glTexCoord2f(1.f, 0.f);
	glVertex2f(Position.x - 11.f+Size.x, Position.y+11.f);

	glTexCoord2f(1.f, 1.f);
	glVertex2f(Position.x - 11.f+Size.x, Position.y +Size.y+2.f);

	glTexCoord2f(0.f, 1.f);
	glVertex2f(Position.x-2.f, Position.y +Size.y+2.f);


	glEnd();

	//gloss
	glColor4f(1.f, 1.f, 1.f, 1.f);
	glBindTexture(GL_TEXTURE_2D, shGetTexture(SR, "mn_gloss"));

	glBegin(GL_QUADS);

	glTexCoord2f(0.f, 0.f);
	glVertex2f(Position.x-2.f, Position.y+11.f);

	glTexCoord2f(1.f, 0.f);
	glVertex2f(Position.x - 11.f+Size.x, Position.y+11.f);

	glTexCoord2f(1.f, 1.f);
	glVertex2f(Position.x - 11.f+Size.x, Position.y + MIN(50.f, Size.y-11.f) +11.f);

	glTexCoord2f(0.f, 1.f);
	glVertex2f(Position.x-2.f, Position.y + MIN(50.f, Size.y-11.f)+ 11.f);


	glEnd();


	glDisable(GL_TEXTURE_2D);
}


void glDrawMinimap(s_Level* Lvl, SharedResources* SR, const sf::RenderTarget& win, float ViewX, float ViewY, float ViewWidth, float ViewHeight, float sc)
{
	//float sc = 0.05f;
	glPushMatrix();
	glTranslatef(ViewX+ViewWidth, ViewY, 0.f);

	glScalef(ViewWidth/win.getSize().x, ViewHeight/win.getSize().y, 1.f);

	glTranslatef(-20.f-wdGetWidth(lvlGetWorld(Lvl))*sc, 20.f, 0.f);

	glScalef(sc, sc, 1.f);

	glColor4f(1.f, 1.f, 1.f, 0.2f);
	glBegin(GL_QUADS);
	glVertex2f(0.f, 0.f);
	glVertex2f(wdGetWidth(lvlGetWorld(Lvl)), 0.f);
	glVertex2f(wdGetWidth(lvlGetWorld(Lvl)), wdGetHeight(lvlGetWorld(Lvl)));
	glVertex2f(0.f, wdGetHeight(lvlGetWorld(Lvl)));
	glEnd();

	lvlDispGoalFlag(Lvl);
	lvlDispAllObj(Lvl);
	lvlDisplayGrass(Lvl, SR);
	lvlDisplayElastics(Lvl, SR);
	lvlDisplayRigids(Lvl, SR);

	glColor4f(0.5f, 0.5f, 0.5f, 1.f);
	glLineStipple(1, 0xCCCC);
	glEnable(GL_LINE_STIPPLE);
	glBegin(GL_LINE_LOOP);
	glVertex2f(ViewX, ViewY);
	glVertex2f((ViewX + ViewWidth), ViewY);
	glVertex2f((ViewX + ViewWidth), (ViewY + ViewHeight));
	glVertex2f(ViewX, (ViewY + ViewHeight));
	glEnd();
	glDisable(GL_LINE_STIPPLE);

}

void glDispPlayer(Player* P, SharedResources* SR)
{
	glPushMatrix();

	//glRotatef(vec2Angle(vec2Sub(vxGetPosition(plGetVxUL(P)), vxGetPosition(plGetVxDL(P)))), 0.f, 0.f, 1.f);
	//marche pas! je resous ça demain, c'ets juste un changement d'origine

	Vec2 N, C, offset;
	float ang = -RAD2DEG(vec2Angle(vec2Sub(vxGetPosition(plGetVxUL(P)), vxGetPosition(plGetVxDL(P)))))+90.f+180.f*(P->Dir == DIR_RIGHT);
	//C = polyComputeCenter(plGetShape(P));
	C = vxGetPosition(P->vxBodyParts[bpBase]);
	float size = 4.f;
	Vertex* from, *to;
	//float txCoord[4][2] ={{0,0},{1,0},{1,1},{0,1}};

	glColor3f(1.f, 1.f, 1.f);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, shGetTexture(SR, "pl_leg"));

	float txCoord[10][4][2] = { {{0,0},{0.5f,0},{0.5f,1},{0,1}}, //Neck
		{{0,0},{0.5f,0},{0.5f,1},{0,1}},
		{{0,0},{0.5f,0},{0.5f,1},{0,1}},
		{{0,0},{0.5f,0},{0.5f,1},{0,1}},
		{{0,0},{0.5f,0},{0.5f,1},{0,1}},
		{{0,0},{0.5f,0},{0.5f,1},{0,1}}, //lefleg1
		{{0.5f,0},{1,0},{1,1},{0.5f,1}},
		{{0,0},{0.5f,0},{0.5f,1},{0,1}}, //lefleg1
		{{0.5f,0},{1,0},{1,1},{0.5f,1}},
		{{0,0},{1,0},{1,1},{0,1}}, //head (unused)
	};


	for (int i=0; i<9; i++)
	{

		switch (i) {
			case bpNeck:
				from = P->vxBodyParts[bpBase];
				to = P->vxBodyParts[i];
				size = 4.f;
				break;
			case bpLeftArm1:
			case bpRightArm1:
				from = P->vxBodyParts[bpNeck];
				to = P->vxBodyParts[i];
				size = 2.5f;
				break;
			case bpLeftLeg1:
			case bpRightLeg1:
				from = P->vxBodyParts[bpBase];
				to = P->vxBodyParts[i];
				size = 2.5f;
				break;
			case bpLeftArm2:
			case bpRightArm2:
			case bpLeftLeg2:
			case bpRightLeg2:
				from = P->vxBodyParts[i-1];
				to = P->vxBodyParts[i];
				size = 2.5f;
				break;
			case bpHeadLeft:
				size = 10.f;
				break;

			default:
				break;
		}

		N = vec2Sub(vxGetPosition(to), vxGetPosition(from));
		N = vec2Normalized(vec2Ortho(N));

		Vec2 Pos = vec2Sub(vxGetPosition(to), vxGetPosition(from));
		//if (P->Dir == DIR_RIGHT) Pos.x*=-1.f, Pos.y*=1.f;
		offset = vec2Sub(vxGetPosition(from), C);
		glPushMatrix();
		//printf("ang:%f\n", ang);


		glTranslatef(vxGetPosition(from).x, vxGetPosition(from).y, 0.f);


		glTranslatef(-offset.x, -offset.y, 0.f);
		if (P->Dir == DIR_RIGHT) glRotatef(180.f, 1.f, 0.f, 0.f);
		glRotatef(ang*((P->Dir == DIR_RIGHT) ? -1.f : 1.f), 0.f, 0.f, 1.f);

		glTranslatef(offset.x, offset.y, 0.f);
		//if (P->Dir == DIR_RIGHT) glScalef(-1.f, 1.f, 1.f);
		//glTranslatef(vxGetPosition(from).x, vxGetPosition(from).y, 0.f);


		glBegin(GL_QUADS);
		glTexCoord2f(txCoord[i][0][0], txCoord[i][0][1]);
		glVertex2f(-N.x*size, -N.y*size);
		glTexCoord2f(txCoord[i][1][0], txCoord[i][1][1]);
		glVertex2f(Pos.x-N.x*size, Pos.y-N.y*size);
		glTexCoord2f(txCoord[i][2][0], txCoord[i][2][1]);
		glVertex2f(Pos.x+N.x*size, Pos.y+N.y*size);
		glTexCoord2f(txCoord[i][3][0], txCoord[i][3][1]);
		glVertex2f(N.x*size, N.y*size);
		glEnd();

		glPopMatrix();
	}


	//tete
	/*size = 10.f;
	from = P->vxBodyParts[bpNeck];
	N = vec2Rotate(vec2(1.f, 0.f), vec2(0.f, 0.f), -DEG2RAD(P->Angles.Angles[bpHeadLeft]));
	N2 = vec2Ortho(N);
	printf("N : %f, %f\n", N.x, N.y);

	glTexCoord2i(0, 0);
	glVertex2f(vxGetPosition(from).x-N2.x*size, vxGetPosition(from).y-N2.y*size);
	glTexCoord2i(1, 0);
	glVertex2f(vxGetPosition(from).x-N2.x*size+N.x*size, vxGetPosition(from).y-N2.y*size);
	glTexCoord2i(1, 1);
	glVertex2f(vxGetPosition(from).x+N2.x*size+N.x*size, vxGetPosition(from).y+N2.y*size+N.y*size);
	glTexCoord2i(0, 1);
	glVertex2f(vxGetPosition(from).x+N2.x*size, vxGetPosition(from).y+N2.y*size+N.y*size);
	*/
	from = P->vxBodyParts[bpNeck];
	offset = vec2Sub(vxGetPosition(from), C);

	glTranslatef(vxGetPosition(P->vxBodyParts[bpNeck]).x, vxGetPosition(P->vxBodyParts[bpNeck]).y, 0.f);
	//glRotatef(P->Angles.Angles[bpHeadLeft], 0.f, 0.f, 1.f);
	glTranslatef(-offset.x, -offset.y, 0.f);
	if (P->Dir == DIR_RIGHT) glRotatef(180.f, 1.f, 0.f, 0.f);
	glRotatef(ang*((P->Dir == DIR_RIGHT) ? -1.f : 1.f), 0.f, 0.f, 1.f);
	glTranslatef(offset.x, offset.y, 0.f);
	glRotatef(P->Angles.Angles[bpHeadLeft], 0.f, 0.f, 1.f);
	//if (P->Dir == DIR_RIGHT) glScalef(-1.f, 1.f, 1.f);
	glBegin(GL_TRIANGLES);
	glTexCoord2f(0.5f, 0.5f);
	glVertex2f(0.f, 0.f);
	glTexCoord2f(0.6f, 0.6f);
	glVertex2f(-20.f, -20.f);
	glTexCoord2f(0.6f, 0.6f);
	glVertex2f(-20.f, 20.f);
	glEnd();


	glDisable(GL_TEXTURE_2D);
	glPopMatrix();
}


void glDispGrass(Polygon* P, SharedResources* SR)
{
	if (polyGetVxCount(P) < 2) return;
	Vertex *V1 = polyGetVertex(P, 0), *V2 = polyGetVertex(P, 1);
	Vec2 N = vec2Normalized(vec2Ortho(vec2Sub(vxGetPosition(V2), vxGetPosition(V1))));
	float l = vec2Length(vec2Sub(vxGetPosition(V2), vxGetPosition(V1)));

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, shGetTexture(SR, "gr_grass")); //66x18

	glBegin(GL_QUADS);

	glTexCoord2i(0, 0);
	glVertex2f(vxGetPosition(V1).x-N.x*18.f, vxGetPosition(V1).y-N.y*18.f);

	glTexCoord2f(l/66.f, 0);
	glVertex2f(vxGetPosition(V2).x-N.x*18.f, vxGetPosition(V2).y-N.y*18.f);

	glTexCoord2f(l/66.f, 1.f);
	glVertex2f(vxGetPosition(V2).x+N.x*18.f, vxGetPosition(V2).y+N.y*18.f);


	glTexCoord2f(0.f, 1.f);
	glVertex2f(vxGetPosition(V1).x+N.x*18.f, vxGetPosition(V1).y+N.y*18.f);


	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void glDispRope(const Elastic* E, SharedResources* SR)
{
	Vertex *V1 = elGetV1(E), *V2 = elGetV2(E);
	Vec2 N = vec2Normalized(vec2Ortho(vec2Sub(vxGetPosition(V2), vxGetPosition(V1))));
	float elLenght = elGetLength(E);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, shGetTexture(SR, "el_rope")); //84x84; je veux de la taille 15

	glBegin(GL_QUADS);

	glTexCoord2i(0, 0);
	glVertex2f(vxGetPosition(V1).x-N.x*4.f, vxGetPosition(V1).y-N.y*4.f);

	glTexCoord2f(elLenght/8.f, 0.f);
	glVertex2f(vxGetPosition(V2).x-N.x*4.f, vxGetPosition(V2).y-N.y*4.f);

	glTexCoord2f(elLenght/8.f, 1.f);
	glVertex2f(vxGetPosition(V2).x+N.x*4.f, vxGetPosition(V2).y+N.y*4.f);


	glTexCoord2f(0.f, 1.f);
	glVertex2f(vxGetPosition(V1).x+N.x*4.f, vxGetPosition(V1).y+N.y*4.f);


	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void glDispChain(const Rigid* R, SharedResources* SR)
{
	Vertex *V1 = rdGetV1(R), *V2 = rdGetV2(R);
	Vec2 N = vec2Normalized(vec2Ortho(vec2Sub(vxGetPosition(V2), vxGetPosition(V1))));
	float elLenght = rdGetLength(R);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, shGetTexture(SR, "rd_chain")); //84x84; je veux de la taille 10

	glBegin(GL_QUADS);

	glTexCoord2i(0, 0);
	glVertex2f(vxGetPosition(V1).x-N.x*5.f, vxGetPosition(V1).y-N.y*7.5f);

	glTexCoord2f(elLenght/15.f, 0.f);
	glVertex2f(vxGetPosition(V2).x-N.x*7.5f, vxGetPosition(V2).y-N.y*7.5f);

	glTexCoord2f(elLenght/15.f, 1.f);
	glVertex2f(vxGetPosition(V2).x+N.x*7.5f, vxGetPosition(V2).y+N.y*7.5f);


	glTexCoord2f(0.f, 1.f);
	glVertex2f(vxGetPosition(V1).x+N.x*7.5f, vxGetPosition(V1).y+N.y*7.5f);


	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void glDispSpawn(Vec2 Pos, SharedResources* SR)
{
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, shGetTexture(SR, "s_spawn")); //46x42

	glBegin(GL_QUADS);

	glTexCoord2i(0, 0);
	glVertex2f(Pos.x-23.f, Pos.y-21.f);

	glTexCoord2i(1, 0);
	glVertex2f(Pos.x+23.f, Pos.y-21.f);

	glTexCoord2i(1,1);
	glVertex2f(Pos.x+23.f, Pos.y+21.f);

	glTexCoord2i(0, 1);
	glVertex2f(Pos.x-23.f, Pos.y+21.f);


	glEnd();

	glDisable(GL_TEXTURE_2D);
}

