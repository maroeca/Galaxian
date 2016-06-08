#include <stdio.h>
#include <stdlib.h>
#include "ConsoleHelper.h"
#include <time.h>

using namespace System;
using namespace System::Text;
using namespace Threading;

struct inimigo {
	float posx, posy;
	bool andandoParaDireita = true;
	bool inimigoAparecendo = true;
};

struct disparo {
	int posx, posy;
	int Vel = 2;
	bool naTela = false;

};

void criarBordaV(int x, int y)
{
	ConsoleHelper::ImprimirASCIIExtended(x, y, ConsoleColor::Black, ConsoleColor::DarkYellow, "€€€€");

}
void criarBordaH(int x, int y)
{
	ConsoleHelper::ImprimirASCIIExtended(x, y, ConsoleColor::Black, ConsoleColor::DarkYellow, "€");
}

void mostrarInimigo(int x, int y)
{
	int iniposx;
	int iniposy;

	iniposx = x;
	iniposy = y;
	ConsoleHelper::ImprimirASCIIExtended(iniposx, iniposy, ConsoleColor::Black, ConsoleColor::White, "\\^/");
	ConsoleHelper::ImprimirASCIIExtended(iniposx, iniposy + 1, ConsoleColor::Black, ConsoleColor::Cyan, "/-\\");

}
void sleep(time_t delay)
{
	time_t timer0, timer1;
	time(&timer0);
	do
	{
		time(&timer1);
	} while ((timer1 - timer0) < delay);
}
int main()
{
	Console::SetWindowSize(80, 40);
	Console::SetBufferSize(80, 40);

	int naveX = 40;
	const int naveY = 33;
	const int maxini = 35;
	struct inimigo ini[maxini];
	int inimigoVoltas = 15;

	float gamespeed = 0.08f;

	struct disparo tiro;
	int tiroPosY;
	int tiroPosX;
	bool tiroNaTela;
	int tiroVoltas;

	tiroVoltas = 3;
	tiroNaTela = false;

	int menorPos = 80;
	int maiorPos = 0;




	/*POSI«√O INICIAL INIMIGOS*/
	for (int i = 0; i < maxini; i++)
	{
		if (i == 0 || i == 1 || i == 3 || i == 5 || i == 6 || i == 7 || i == 13)
		{
			ini[i].inimigoAparecendo = false;
		}
		else
		{
			ini[i].posy = (i / 7) * 3 + 3;
			ini[i].posx = (i % 7) * 6 + 25;
		}
	}

	const int ESTADO_JOGO_MENU = 0;
	const int ESTADO_JOGO_GAMEPLAY = 1;
	const int ESTADO_JOGO_VENCEU = 2;
	const int ESTADO_JOGO_PERDEU = 3;
	int estadoJogo = ESTADO_JOGO_MENU;
	ConsoleKeyInfo tecla;

	do
	{
		switch (estadoJogo)
		{
		case ESTADO_JOGO_MENU:

			Console::Clear();
			Console::SetCursorPosition(30, 15);
			Console::Write("Pressione ENTER para comeÁar");
			if (Console::KeyAvailable)
			{
				tecla = Console::ReadKey(true);
				if (tecla.Key == ConsoleKey::Enter)
				{
					estadoJogo = ESTADO_JOGO_GAMEPLAY;
				}
			}
			break;

		case ESTADO_JOGO_GAMEPLAY:


			for (int i = 0; i < 35; i++)
			{
				if (ini[i].inimigoAparecendo == true)
				{
					if (ini[i].posx > maiorPos)
					{
						maiorPos = ini[i].posx;
					}
					if (ini[i].posx < menorPos)
					{
						menorPos = ini[i].posx;
					}

					if (ini[i].andandoParaDireita == true)
					{
						ini[i].posx += gamespeed;
						if (maiorPos == (Console::BufferWidth) - 8)
						{
							maiorPos = 0;

							for (int i = 0; i < 35; i++)
							{
								if (ini[i].inimigoAparecendo == true)
								{
									ini[i].posx--;
									ini[i].andandoParaDireita = false;
								}
							}
						}
					}
					else
					{
						ini[i].posx -= gamespeed;
						if (menorPos == 4)
						{
							for (int i = 0; i < 35; i++)
							{
								if (ini[i].inimigoAparecendo == true)
								{
									ini[i].posx++;
									ini[i].andandoParaDireita = true;
								}
							}
							menorPos = 80;
						}
					}
				}
			}

			//movimentaÁ„o do tiro
			if (tiro.naTela == true)
			{
				tiro.Vel--;
				if (tiro.Vel == 0)
				{
					tiro.posy--;
					//Colis„o
					for (int i = 0; i < 35; i++)
					{
						if (ini[i].inimigoAparecendo)
						{
							if (tiro.posx >= ini[i].posx && tiro.posx <= (ini[i].posx + 3)
								&& tiro.posy >= ini[i].posy && tiro.posy <= (ini[i].posy + 2))
							{
								ini[i].inimigoAparecendo = false;
								tiro.naTela = false;
							}
						}
					}
					if (tiro.posy <= 0)
					{
						tiro.naTela = false;
					}
					tiro.Vel = 2;
				}
			}

			//leitura dos comandos
			if (Console::KeyAvailable)
			{
				tecla = Console::ReadKey(true);
				if (tecla.Key == ConsoleKey::LeftArrow)
				{
					naveX--;
					if (naveX == 3)
					{
						naveX++;
					}
				}
				else
					if (tecla.Key == ConsoleKey::RightArrow)
					{
						naveX++;
						if (naveX == (Console::BufferWidth - 8))
						{
							naveX--;
						}
					}
					else if (tecla.Key == ConsoleKey::Spacebar)
					{
						if (tiro.naTela == false)
						{
							tiro.naTela = true;
							tiro.posx = naveX + 2;
							tiro.posy = naveY - 1;
						}
					}
			}

			Console::BackgroundColor = ConsoleColor::Black;
			Console::Clear();

			//render dos inimigos
			for (int i = 0; i < maxini; i++)
			{
				if (ini[i].inimigoAparecendo == true)
					mostrarInimigo(int(ini[i].posx), int(ini[i].posy));
			}


			//render da nave			
			ConsoleHelper::ImprimirASCIIExtended(naveX, naveY + 0, ConsoleColor::Black, ConsoleColor::White, " ‹€‹ ");
			ConsoleHelper::ImprimirASCIIExtended(naveX, naveY + 1, ConsoleColor::Black, ConsoleColor::White, "  €  ");
			ConsoleHelper::ImprimirASCIIExtended(naveX, naveY + 1, ConsoleColor::Black, ConsoleColor::White, "€‹€‹€");
			ConsoleHelper::ImprimirASCIIExtended(naveX, naveY + 2, ConsoleColor::Black, ConsoleColor::White, "ﬂ   ﬂ");


			//render borda
			for (int i = 0; i < (Console::BufferHeight - 3); i++)
			{
				criarBordaV(0, i);
				criarBordaV(Console::BufferWidth - 4, i);
			}
			for (int i = 4; i < (Console::BufferWidth - 4); i++)
			{
				criarBordaH(i, 0);
				criarBordaH(i, 36);
			}


			//REnder de tiro
			if (tiro.naTela)
			{
				Console::ForegroundColor = ConsoleColor::White;
				Console::SetCursorPosition(tiro.posx, tiro.posy);
				Console::Write(".");
			}
			break;
		}
		Thread::Sleep(10);
	} while (1);

	return 0;
}
