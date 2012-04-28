#include "Animation.h"
#include <Objects/Player.h>

void aniInit(Animation* A, AnimType Type, AnimTriggers Triggers, Bool Repeat)
{
	A->States = newDynArr();
	A->Ended = FALSE;
	A->Repeat = Repeat;
	A->CurrentState = 0;
	A->Force = 0.75f;
	A->Friction = 0.6f;
	A->Type = Type;
	assert(Triggers != 0);
	A->Triggers = Triggers;
	if (A->Triggers == ANIM_ALL_TRIGGERS)
		A->TriggerCount = 10;
	else
	{
		unsigned char i =0;
		while (Triggers>0)
		{
			if (Triggers%2 == 1)
				i++;
			Triggers>>=1;
		}
		A->TriggerCount = i;
	}
}

void aniFree(Animation* A)
{
	unsigned int i;
	for (i=0; i<daGetSize(A->States); i++)
		free(daGet(A->States, i));
	
	delDynArr(A->States);
}

Animation* newAnimation(AnimType Type, AnimTriggers Triggers, Bool Repeat)
{
	Animation* A;
	A = (Animation*) malloc(sizeof(Animation));
	aniInit(A, Type, Triggers, Repeat);
	return A;
}

void delAnimation(Animation* A)
{
	aniFree(A);
	free(A);
}


Bool aniLoadFromFile(Animation* A, const char* File)
{
	FILE* f;
	f = fopen(File, "r");
	if (f != NULL)
	{
		//[nb of states] [AnimType] [Triggers] [repeat]
		unsigned int type, count, triggers, repeat;
		fscanf(f, "%u %u %u %u\n", &count, &type, &triggers, &repeat);
		A->Type = (AnimType)type;
		A->Triggers = (AnimTriggers)triggers;
		A->Repeat = (Bool)repeat;
		
		printf("type: %u, triggers: %u, nb states: %u\n", type, triggers, count);
		
		while (count>0)
		{
			if (A->Type == ANIM_POSITIONS)
			{
				AnimPositions Pos; unsigned int free, free2;
				
				Vec2* vec;
				
				vec = &Pos.Neck;
				fscanf(f, "%u %f %u %f #Neck\n",&free, &vec->x, &free2, &vec->y);
				if (free)
					vec->x = ANIM_FREE;
				if (free2)
					vec->y = ANIM_FREE;
				
				vec = &Pos.Head;
				fscanf(f, "%u %f %u %f #Head\n",&free, &vec->x, &free2, &vec->y);
				if (free)
					vec->x = ANIM_FREE;
				if (free2)
					vec->y = ANIM_FREE;
				
				vec = &Pos.LeftArm1;
				fscanf(f, "%u %f %u %f #LeftArm1\n",&free, &vec->x, &free2, &vec->y);
				if (free)
					vec->x = ANIM_FREE;
				if (free2)
					vec->y = ANIM_FREE;
				
				vec = &Pos.LeftArm2;
				fscanf(f, "%u %f %u %f #LeftArm2\n",&free, &vec->x, &free2, &vec->y);
				if (free)
					vec->x = ANIM_FREE;
				if (free2)
					vec->y = ANIM_FREE;
				
				vec = &Pos.RightArm1;
				fscanf(f, "%u %f %u %f #RightArm1\n",&free, &vec->x, &free2, &vec->y);
				if (free)
					vec->x = ANIM_FREE;
				if (free2)
					vec->y = ANIM_FREE;
				
				vec = &Pos.RightArm2;
				fscanf(f, "%u %f %u %f #RightArm2\n",&free, &vec->x, &free2, &vec->y);
				if (free)
					vec->x = ANIM_FREE;
				if (free2)
					vec->y = ANIM_FREE;
				
				vec = &Pos.LeftLeg1;
				fscanf(f, "%u %f %u %f #LeftLeg1\n",&free, &vec->x, &free2, &vec->y);
				if (free)
					vec->x = ANIM_FREE;
				if (free2)
					vec->y = ANIM_FREE;
				
				vec = &Pos.LeftLeg2;
				fscanf(f, "%u %f %u %f #LeftLeg2\n",&free, &vec->x, &free2, &vec->y);
				if (free)
					vec->x = ANIM_FREE;
				if (free2)
					vec->y = ANIM_FREE;
				
				vec = &Pos.RightLeg1;
				fscanf(f, "%u %f %u %f #RightLeg1\n",&free, &vec->x, &free2, &vec->y);
				if (free)
					vec->x = ANIM_FREE;
				if (free2)
					vec->y = ANIM_FREE;
				
				vec = &Pos.RightLeg2;
				fscanf(f, "%u %f %u %f #RightLeg2\n",&free, &vec->x, &free2, &vec->y);
				if (free)
					vec->x = ANIM_FREE;
				if (free2)
					vec->y = ANIM_FREE;
				
				aniAddPositionState(A, &Pos);
			}
			else
			{
				AnimAngles Ang; unsigned int free;
				
				float* a;
				
				a = &Ang.Neck;
				fscanf(f, "%u %f #Neck\n",&free, a);
				if (free)
					*a = ANIM_FREE;
				
				a = &Ang.Head;
				fscanf(f, "%u %f #Head\n",&free, a);
				if (free)
					*a = ANIM_FREE;
				
				a = &Ang.LeftArm1;
				fscanf(f, "%u %f #LeftArm1\n",&free, a);
				if (free)
					*a = ANIM_FREE;
				
				a = &Ang.LeftArm2;
				fscanf(f, "%u %f #LeftArm2\n",&free, a);
				if (free)
					*a = ANIM_FREE;
				
				a = &Ang.RightArm1;
				fscanf(f, "%u %f #RightArm1\n",&free, a);
				if (free)
					*a = ANIM_FREE;
				
				a = &Ang.RightArm2;
				fscanf(f, "%u %f #RightArm2\n",&free, a);
				if (free)
					*a = ANIM_FREE;
				
				a = &Ang.LeftLeg1;
				fscanf(f, "%u %f #LeftLeg1\n",&free, a);
				if (free)
					*a = ANIM_FREE;
				
				a = &Ang.LeftLeg2;
				fscanf(f, "%u %f #LeftLeg2\n",&free, a);
				if (free)
					*a = ANIM_FREE;
				
				a = &Ang.RightLeg1;
				fscanf(f, "%u %f #RightLeg1\n",&free, a);
				if (free)
					*a = ANIM_FREE;
				
				a = &Ang.RightLeg2;
				fscanf(f, "%u %f #RightLeg2\n",&free, a);
				if (free)
					*a = ANIM_FREE;
				
				aniAddAngleState(A, &Ang);
			}
			
			count--;
		}
		
	}
	else
		return TRUE;
	
	fclose(f);
	return FALSE; /* No Error */
}

void aniResetEnded(Animation* A)
{
	A->Ended = FALSE;
}

Bool aniIsEnded(const Animation* A)
{
	return A->Ended;
}

unsigned int aniGetStatesCount(const Animation* A)
{
	return daGetSize(A->States);
}

unsigned int aniGetCurrentState(const Animation* A)
{
	return A->CurrentState;
}

void aniAddPositionState(Animation* A, Vec2 Head, Vec2 Neck, Vec2 LeftArm1, Vec2 LeftArm2, Vec2 RightArm1, Vec2 RightArm2, Vec2 LeftLeg1, Vec2 LeftLeg2, Vec2 RightLeg1, Vec2 RightLeg2)
{
	assert(A->Type == ANIM_POSITIONS);
	
	AnimPositions* Pos;
	Pos = (AnimPositions*) malloc(sizeof(AnimPositions));
	Pos->Head = Head;
	Pos->Neck = Neck;
	Pos->LeftArm1 = LeftArm1;
	Pos->LeftArm2 = LeftArm2;
	Pos->RightArm1 = RightArm1;
	Pos->RightArm2 = RightArm2;
	Pos->LeftLeg1 = LeftLeg1;
	Pos->LeftLeg2 = LeftLeg2;
	Pos->RightLeg1 = RightLeg1;
	Pos->RightLeg2 = RightLeg2;
	daAdd(A->States, Pos);
}

void aniAddPositionState(Animation* A, const AnimPositions* Positions)
{
	assert(A->Type == ANIM_POSITIONS);
	
	AnimPositions* Pos;
	Pos = (AnimPositions*) malloc(sizeof(AnimPositions));
	Pos->Head = Positions->Head;
	Pos->Neck = Positions->Neck;
	Pos->LeftArm1 = Positions->LeftArm1;
	Pos->LeftArm2 = Positions->LeftArm2;
	Pos->RightArm1 = Positions->RightArm1;
	Pos->RightArm2 = Positions->RightArm2;
	Pos->LeftLeg1 = Positions->LeftLeg1;
	Pos->LeftLeg2 = Positions->LeftLeg2;
	Pos->RightLeg1 = Positions->RightLeg1;
	Pos->RightLeg2 = Positions->RightLeg2;
	daAdd(A->States, Pos);
}

void aniAddAngleState(Animation* A, float Head, float Neck, float LeftArm1, float LeftArm2, float RightArm1, float RightArm2, float LeftLeg1, float LeftLeg2, float RightLeg1, float RightLeg2)
{
	assert(A->Type == ANIM_ANGLES);
	
	AnimAngles* Ang;
	Ang = (AnimAngles*) malloc(sizeof(AnimAngles));
	Ang->Head = Head;
	Ang->Neck = Neck;
	Ang->LeftArm1 = LeftArm1;
	Ang->LeftArm2 = LeftArm2;
	Ang->RightArm1 = RightArm1;
	Ang->RightArm2 = RightArm2;
	Ang->LeftLeg1 = LeftLeg1;
	Ang->LeftLeg2 = LeftLeg2;
	Ang->RightLeg1 = RightLeg1;
	Ang->RightLeg2 = RightLeg2;
	daAdd(A->States, Ang);
}

void aniAddAngleState(Animation* A, const AnimAngles* Angles)
{
	assert(A->Type == ANIM_ANGLES);
	
	AnimAngles* Ang;
	Ang = (AnimAngles*) malloc(sizeof(AnimAngles));
	Ang->Head = Angles->Head;
	Ang->Neck = Angles->Neck;
	Ang->LeftArm1 = Angles->LeftArm1;
	Ang->LeftArm2 = Angles->LeftArm2;
	Ang->RightArm1 = Angles->RightArm1;
	Ang->RightArm2 = Angles->RightArm2;
	Ang->LeftLeg1 = Angles->LeftLeg1;
	Ang->LeftLeg2 = Angles->LeftLeg2;
	Ang->RightLeg1 = Angles->RightLeg1;
	Ang->RightLeg2 = Angles->RightLeg2;
	daAdd(A->States, Ang);
	
	printf("Added angle state with : H: %f, N: %f, LA1:%f, LA2:%f\n", Ang->Head, Ang->Neck, Ang->LeftArm1, Ang->LeftArm2);
}

void aniUpdate(Animation* A, SPlayer* P, float Step)
{
	unsigned char TriggerCount = 0;
	/* Mise à jour des angles ou des positions */
	if (A->Type == ANIM_POSITIONS)
	{
		AnimPositions* Pos;
		Pos = (AnimPositions*) daGet(A->States, A->CurrentState);
		
		if (Pos->Head.x != ANIM_FREE)
			Wobble(&A->Positions.Head.x, Pos->Head.x, A->Force, A->Friction, &A->Spd[0]);
		if (Pos->Head.y != ANIM_FREE)
			Wobble(&A->Positions.Head.y, Pos->Head.y, A->Force, A->Friction, &A->Spd[1]);
		if (Pos->Neck.x != ANIM_FREE)
			Wobble(&A->Positions.Neck.x, Pos->Neck.x, A->Force, A->Friction, &A->Spd[2]);
		if (Pos->Neck.y != ANIM_FREE)
			Wobble(&A->Positions.Neck.y, Pos->Neck.y, A->Force, A->Friction, &A->Spd[3]);
		
		if (Pos->LeftArm1.x != ANIM_FREE)
			Wobble(&A->Positions.LeftArm1.x, Pos->LeftArm1.x, A->Force, A->Friction, &A->Spd[4]);
		if (Pos->LeftArm1.y != ANIM_FREE)
			Wobble(&A->Positions.LeftArm1.y, Pos->LeftArm1.y, A->Force, A->Friction, &A->Spd[5]);
		if (Pos->LeftArm2.x != ANIM_FREE)
			Wobble(&A->Positions.LeftArm2.x, Pos->LeftArm2.x, A->Force, A->Friction, &A->Spd[6]);
		if (Pos->LeftArm2.y != ANIM_FREE)
			Wobble(&A->Positions.LeftArm2.y, Pos->LeftArm2.y, A->Force, A->Friction, &A->Spd[7]);
		if (Pos->RightArm1.x != ANIM_FREE)
			Wobble(&A->Positions.RightArm1.x, Pos->RightArm1.x, A->Force, A->Friction, &A->Spd[8]);
		if (Pos->RightArm1.y != ANIM_FREE)
			Wobble(&A->Positions.RightArm1.y, Pos->RightArm1.y, A->Force, A->Friction, &A->Spd[9]);
		if (Pos->RightArm2.x != ANIM_FREE)
			Wobble(&A->Positions.RightArm2.x, Pos->RightArm2.x, A->Force, A->Friction, &A->Spd[10]);
		if (Pos->RightArm2.y != ANIM_FREE)
			Wobble(&A->Positions.RightArm2.y, Pos->RightArm2.y, A->Force, A->Friction, &A->Spd[11]);
		
		if (Pos->LeftLeg1.x != ANIM_FREE)
			Wobble(&A->Positions.LeftLeg1.x, Pos->LeftLeg1.x, A->Force, A->Friction, &A->Spd[12]);
		if (Pos->LeftLeg1.y != ANIM_FREE)
			Wobble(&A->Positions.LeftLeg1.y, Pos->LeftLeg1.y, A->Force, A->Friction, &A->Spd[13]);
		if (Pos->LeftLeg2.x != ANIM_FREE)
			Wobble(&A->Positions.LeftLeg2.x, Pos->LeftLeg2.x, A->Force, A->Friction, &A->Spd[14]);
		if (Pos->LeftLeg2.y != ANIM_FREE)
			Wobble(&A->Positions.LeftLeg2.y, Pos->LeftLeg2.y, A->Force, A->Friction, &A->Spd[15]);
		if (Pos->RightLeg1.x != ANIM_FREE)
			Wobble(&A->Positions.RightLeg1.x, Pos->RightLeg1.x, A->Force, A->Friction, &A->Spd[16]);
		if (Pos->RightLeg1.y != ANIM_FREE)
			Wobble(&A->Positions.RightLeg1.y, Pos->RightLeg1.y, A->Force, A->Friction, &A->Spd[17]);
		if (Pos->RightLeg2.x != ANIM_FREE)
			Wobble(&A->Positions.RightLeg2.x, Pos->RightLeg2.x, A->Force, A->Friction, &A->Spd[18]);
		if (Pos->RightLeg2.y != ANIM_FREE)
			Wobble(&A->Positions.RightLeg2.y, Pos->RightLeg2.y, A->Force, A->Friction, &A->Spd[19]);
		
		
		/* Vérification de l'état de l'animation pour passer à létat suivant */
		if (A->Triggers & ANIM_LEFT_LEG2)
			if (ABS(A->Positions.LeftLeg2.x-Pos->LeftLeg2.x) < A->Diff && ABS(A->Positions.LeftLeg2.y-Pos->LeftLeg2.y) < A->Diff)
				TriggerCount++;
		if (A->Triggers & ANIM_RIGHT_LEG2)
			if (ABS(A->Positions.RightLeg2.x-Pos->RightLeg2.x) < A->Diff && ABS(A->Positions.RightLeg2.y-Pos->RightLeg2.y) < A->Diff)
				TriggerCount++;
		if (A->Triggers & ANIM_RIGHT_LEG1)
			if (ABS(A->Positions.RightLeg1.x-Pos->RightLeg1.x) < A->Diff && ABS(A->Positions.RightLeg1.y-Pos->RightLeg1.y) < A->Diff)
				TriggerCount++;
		if (A->Triggers & ANIM_LEFT_LEG1)
			if (ABS(A->Positions.LeftLeg1.x-Pos->LeftLeg1.x) < A->Diff && ABS(A->Positions.LeftLeg1.y-Pos->LeftLeg1.y) < A->Diff)
				TriggerCount++;
		if (A->Triggers & ANIM_LEFT_ARM1)
			if (ABS(A->Positions.LeftArm1.x-Pos->LeftArm1.x) < A->Diff && ABS(A->Positions.LeftArm1.y-Pos->LeftArm1.y) < A->Diff)
				TriggerCount++;
		if (A->Triggers & ANIM_LEFT_ARM2)
			if (ABS(A->Positions.LeftArm2.x-Pos->LeftArm2.x) < A->Diff && ABS(A->Positions.LeftArm2.y-Pos->LeftArm2.y) < A->Diff)
				TriggerCount++;
		if (A->Triggers & ANIM_RIGHT_ARM1)
			if (ABS(A->Positions.RightArm1.x-Pos->RightArm1.x) < A->Diff && ABS(A->Positions.RightArm1.y-Pos->RightArm1.y) < A->Diff)
				TriggerCount++;
		if (A->Triggers & ANIM_RIGHT_ARM2)
			if (ABS(A->Positions.RightArm2.x-Pos->RightArm2.x) < A->Diff && ABS(A->Positions.RightArm2.y-Pos->RightArm2.y) < A->Diff)
				TriggerCount++;
		
		if (A->Triggers & ANIM_NECK)
			if (ABS(A->Positions.Neck.x-Pos->Neck.x) < A->Diff && ABS(A->Positions.Neck.y-Pos->Neck.y) < A->Diff)
				TriggerCount++;
		if (A->Triggers & ANIM_HEAD)
			if (ABS(A->Positions.Head.x-Pos->Head.x) < A->Diff && ABS(A->Positions.Head.y-Pos->Head.y) < A->Diff)
				TriggerCount++;
		
		if (TriggerCount>=A->TriggerCount)
		{
			A->CurrentState++;
			
			if (A->CurrentState >= daGetSize(A->States))
			{
				A->Ended = TRUE;
				if (A->Repeat)
					A->CurrentState = 0;
				else
					A->CurrentState--;
			}
		}
		
	}
	else
	{
		AnimAngles* Ang;
		Ang = (AnimAngles*) daGet(A->States, A->CurrentState);
		
		/* On change la position des vertex du joueur */
		
		if (Ang->Neck != ANIM_FREE)
		{
			Wobble(&A->Angles.Neck, Ang->Neck, A->Force, A->Friction, &A->Spd[1]);
			vxSetPosition(P->Neck, vec2Add(vxGetPosition(P->Base), vec2Rotate(vec2(90.f, 0.f), vec2(0.f, 0.f), -DEG2RAD(A->Angles.Neck))));
		}
			
		if (Ang->Head != ANIM_FREE)
		{
			Wobble(&A->Angles.Head, Ang->Head, A->Force, A->Friction, &A->Spd[0]);
			//la tête est un peu spéciale car on a l'angle du
		}
			
		
		
				
		if (Ang->LeftArm1 != ANIM_FREE)
		{
			Wobble(&A->Angles.LeftArm1, Ang->LeftArm1, A->Force, A->Friction, &A->Spd[2]);
			printf("angle: %f\n", A->Angles.LeftArm1);
			vxSetPosition(P->LeftArm1, vec2Add(vxGetPosition(P->Neck), vec2Rotate(vec2(-35.f, 0.f), vec2(0.f, 0.f), -DEG2RAD(A->Angles.LeftArm1))));
		}
			
		if (Ang->LeftArm2 != ANIM_FREE)
		{
			Wobble(&A->Angles.LeftArm2, Ang->LeftArm2, A->Force, A->Friction, &A->Spd[3]);
			printf("angle: %f\n", A->Angles.LeftArm2);
			vxSetPosition(P->LeftArm2, vec2Add(vxGetPosition(P->LeftArm1), vec2Rotate(vec2(-35.f, 0.f), vec2(0.f, 0.f), -DEG2RAD(A->Angles.LeftArm2))));
		}
			
		
		if (Ang->RightArm1 != ANIM_FREE)
		{
			Wobble(&A->Angles.RightArm1, Ang->RightArm1, A->Force, A->Friction, &A->Spd[4]);
			vxSetPosition(P->RightArm1, vec2Add(vxGetPosition(P->Neck), vec2Rotate(vec2(-35.f, 0.f), vec2(0.f, 0.f), -DEG2RAD(A->Angles.RightArm1))));
		}
			
		if (Ang->RightArm2 != ANIM_FREE)
		{
			Wobble(&A->Angles.RightArm2, Ang->RightArm2, A->Force, A->Friction, &A->Spd[5]);
			vxSetPosition(P->RightArm2, vec2Add(vxGetPosition(P->RightArm1), vec2Rotate(vec2(-35.f, 0.f), vec2(0.f, 0.f), -DEG2RAD(A->Angles.RightArm2))));
		}
			
				
		if (Ang->LeftLeg1 != ANIM_FREE)
			Wobble(&A->Angles.LeftLeg1, Ang->LeftLeg1, A->Force, A->Friction, &A->Spd[6]);
		if (Ang->LeftLeg2 != ANIM_FREE)
			Wobble(&A->Angles.LeftLeg2, Ang->LeftLeg2, A->Force, A->Friction, &A->Spd[7]);
		
		if (Ang->RightLeg1 != ANIM_FREE)
			Wobble(&A->Angles.RightLeg1, Ang->RightLeg1, A->Force, A->Friction, &A->Spd[8]);
		if (Ang->RightLeg2 != ANIM_FREE)
			Wobble(&A->Angles.RightLeg2, Ang->RightLeg2, A->Force, A->Friction, &A->Spd[9]);
		
		
		
		
		/* Vérification de l'état de l'animation pour passer à létat suivant */
		if (A->Triggers & ANIM_LEFT_LEG2)
			if (ABS(A->Angles.LeftLeg2-Ang->LeftLeg2) < A->Diff)
				TriggerCount++;
		if (A->Triggers & ANIM_RIGHT_LEG2)
			if (ABS(A->Angles.RightLeg2-Ang->RightLeg2) < A->Diff)
				TriggerCount++;
		if (A->Triggers & ANIM_RIGHT_LEG1)
			if (ABS(A->Angles.RightLeg1-Ang->RightLeg1) < A->Diff)
				TriggerCount++;
		if (A->Triggers & ANIM_LEFT_LEG1)
			if (ABS(A->Angles.LeftLeg1-Ang->LeftLeg1) < A->Diff)
				TriggerCount++;
		if (A->Triggers & ANIM_LEFT_ARM1)
			if (ABS(A->Angles.LeftArm1-Ang->LeftArm1) < A->Diff)
				TriggerCount++;
		if (A->Triggers & ANIM_LEFT_ARM2)
			if (ABS(A->Angles.LeftArm2-Ang->LeftArm2) < A->Diff)
				TriggerCount++;
		if (A->Triggers & ANIM_RIGHT_ARM1)
			if (ABS(A->Angles.RightArm1-Ang->RightArm1) < A->Diff)
				TriggerCount++;
		if (A->Triggers & ANIM_RIGHT_ARM2)
			if (ABS(A->Angles.RightArm2-Ang->RightArm2) < A->Diff)
				TriggerCount++;
		
		if (A->Triggers & ANIM_NECK)
			if (ABS(A->Angles.Neck-Ang->Neck) < A->Diff)
				TriggerCount++;
		if (A->Triggers & ANIM_HEAD)
			if (ABS(A->Angles.Head-Ang->Head) < A->Diff)
				TriggerCount++;
		
		if (TriggerCount>=A->TriggerCount)
		{
			A->CurrentState++;
			printf("Animation go on !\n");
			if (A->CurrentState >= daGetSize(A->States))
			{
				A->Ended = TRUE;
				if (A->Repeat)
					A->CurrentState = 0;
				else
					A->CurrentState--;
			}
		}				
				
		
	}	
	
	
}

