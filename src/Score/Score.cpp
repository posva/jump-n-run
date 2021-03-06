#include "Score.h"

Score* newScore(const char Player[255], const char LvlName[255], const char LvlMD5[255], unsigned int Time)
{
	Score* S = (Score*) malloc(sizeof(Score));
	scInit(S, Player, LvlName, LvlMD5, Time);
	return S;
}

void delScore(Score* S)
{
	scFree(S);
	free(S);
}

void scInit(Score* S, const char Player[255], const char LvlName[255], const char LvlMD5[255], unsigned int Time)
{
	strcpy(S->Player, Player);
	strcpy(S->LvlName, LvlName);
	strcpy(S->LvlMD5, LvlMD5);
	S->Time = Time;
}

Bool scSend(const Score* S)
{
	if(!scIsValid(S)) return 1;
	sf::Http http;
	http.setHost("http://maretverdant.free.fr");

	/* Conversion unsigned iny -> char* */
	char Time[255];
	sprintf(Time, "%d", S->Time);

	char Body[5000];
	Body[0] = '\0';
	strcat(Body, "Player=");
	strcat(Body, S->Player);
	strcat(Body, "&LvlName=");
	strcat(Body, S->LvlName);
	strcat(Body, "&LvlMD5=");
	strcat(Body, S->LvlMD5);
	strcat(Body, "&Time=");
	strcat(Body, Time);

	sf::Http::Request Request("JumpNRun/submit.php");
	Request.setMethod(sf::Http::Request::Post);
	Request.setBody(Body);
	sf::Http::Response Resp = http.sendRequest(Request);

	sf::Http::Response::Status status = Resp.getStatus();
	if (status == sf::Http::Response::Ok)
	{
		// std::cout << Resp.getBody() << std::endl; // Debug
		return 0;
	} else {
		return 1;
	}
}

//unsigned int scCollect(DynArr* DA, const char LvlName[255], const char LvlMD5[255])
DynArr* scCollect(const char LvlName[255], const char LvlMD5[255])
{
	DynArr* DA = newDynArr();
	if(strcmp(LvlName, "") == 0 || strcmp(LvlMD5, "") == 0)
		return DA;
	unsigned int ErrorCode, NbScore, i, Time;
	char *ResString, *Line, Player[255], Hour[255], Date[255];
	sf::Http http;
	http.setHost("http://maretverdant.free.fr");
	char Body[1000];
	Body[0] = '\0';
	strcat(Body, "LvlName=");
	strcat(Body, LvlName);
	strcat(Body, "&LvlMD5=");
	strcat(Body, LvlMD5);

	sf::Http::Request Request("JumpNRun/query.php");
	Request.setMethod(sf::Http::Request::Post);
	Request.setBody(Body);
	sf::Http::Response Response = http.sendRequest(Request);

	if(Response.getStatus() == sf::Http::Response::Ok)
	{
		ResString = (char*) malloc(sizeof(char)*Response.getBody().size());
		strcpy(ResString, Response.getBody().c_str());
		Line = strtok(ResString, "\n");
		sscanf(Line, "%u", &ErrorCode);
		if(ErrorCode == 0)
		{
			Line = strtok(NULL, "\n");
			sscanf(Line, "%u", &NbScore);
			daReserve(DA, NbScore);
			for(i = 0; i < NbScore; i++)
			{
				Line = strtok(NULL, "\n");
				sscanf(Line, "%s | %s %s | %u", Player, Hour, Date, &Time);
				daAdd(DA, newScore(Player, Hour, Date, Time));
			}
		}
		free(ResString);
	}

	return DA;
}

void scCollectFree(DynArr* DA)
{
	unsigned int i;
	for(i = 0; i < daGetSize(DA); i++)
		delScore((Score*) daGet(DA, i));
	daFree(DA);
}

void scFree(Score *S)
{
	strcpy(S->Player, "");
	strcpy(S->LvlName, "");
	strcpy(S->LvlMD5, "");
	S->Time = 0;
}

void scRegressionTest()
{
	DynArr* DA = scCollect("Niveau Simple", "eef982595a3558cb6a4075dfd62ac65e");
	//if(ErrorCode == 0)
	if(daGetSize(DA) != 0)
	{
		printf("Number of Score(s) : %u\n", daGetSize(DA));
		for(unsigned int i = 0; i < daGetSize(DA); i++)
		{
			Score* S = (Score*) daGet(DA, i);
			printf("Player : %s, Hour : %s, Date : %s, Time : %u\n", S->Player, S->LvlName, S->LvlMD5, S->Time);
		}
		scCollectFree(DA);
	}
}

Bool scIsValid(const Score* S)
{
	return ((strcmp(S->Player, "") != 0)
	&& (strcmp(S->LvlName, "") != 0)
	&& (strcmp(S->LvlMD5, "") != 0)
	&& (S->Time > 0));
}
