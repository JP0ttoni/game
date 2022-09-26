#include <SDL.h>  
#include <SDL_image.h>
#include <stdlib.h>
#include <SDL_timer.h>
#include <stdio.h>
#include <time.h>

#define img_path "images/walk.png"
#define button_path "images/button ply.png"
#define vel 3;


typedef struct
{
	int x;
	int y;
	int w;
	int h;
	SDL_Texture* img;
} plyr;

void move(plyr* player, int up, int down, int lft, int right, SDL_Rect* plyr_spr, int can);
void print(plyr* player, SDL_Renderer* render, SDL_Rect plyr_spr, SDL_Rect input_fr[], int destroy[], int* kill);
void button_(SDL_Texture* button, SDL_Renderer* render, int* map, int ms_x, int ms_y, int click);
void col(int* map, SDL_Rect skeleton);

int main(void)
{
	int w, h, i = 0, j = 0, k = 0, f = 0, p = 0, count, fps = 0, num = 4, num2 = 120, can = 0, map = 0;
	SDL_Init(SDL_INIT_EVERYTHING);

	SDL_Window* janela;

	janela = SDL_CreateWindow("jogo", 50, 50, 500, 500, SDL_WINDOW_FULLSCREEN_DESKTOP);

	SDL_Renderer* render = SDL_CreateRenderer(janela, -1, SDL_RENDERER_ACCELERATED);

	SDL_Surface* surface = IMG_Load(img_path);

	SDL_Texture* img = SDL_CreateTextureFromSurface(render, surface);

	SDL_Texture* background = IMG_LoadTexture(render, "images/background.png");

	SDL_Texture* back2 = IMG_LoadTexture(render, "images/gifback.png");

	SDL_Texture* fire = IMG_LoadTexture(render, "images/fire.png");

	SDL_Texture* sk = IMG_LoadTexture(render, "images/skeleton.png");

	SDL_Texture* button = IMG_LoadTexture(render, button_path);

	SDL_Texture* back_lake = IMG_LoadTexture(render, "images/castle.png");

	SDL_Texture* back_win = IMG_LoadTexture(render, "images/win.png");

	SDL_Texture* back_end = IMG_LoadTexture(render, "images/fim do game.png");

	

	SDL_RenderClear(render);

	//atribui o tamanho e largura da textura
	SDL_QueryTexture(img, NULL, NULL, &w, &h);

	SDL_Rect fire_rect[6];

	SDL_Rect sk_sprt[8];

	SDL_Rect fire_input;

	fire_input.x = 100;
	fire_input.y = 100;
	fire_input.w = 384 / 6;
	fire_input.h = 64;

	SDL_Rect input_fr[1000];

	input_fr[0].x = -50;
	input_fr[0].y = 300;
	input_fr[0].w = 64;
	input_fr[0].h = 64;


	srand(time(NULL));

	for (i = 1; i < 20; i++)
	{
		//num = 4
		if (i < num)
		{
			input_fr[i] = input_fr[0];
			input_fr[i].y = (200 + 64 * i) + rand() % 40;
		}
		if(i >= num && i < num * 2)
		{
			input_fr[i] = input_fr[0];
			input_fr[i].y = (200 + 64 * (i - num)) + rand() % 40;
		}

		if (i >= num * 2 && i < num * 3)
		{
			input_fr[i] = input_fr[0];
			input_fr[i].y = (200 + 64 * (i - num * 2)) + rand() % 40;
		}

		if (i >= num * 3 && i < num * 4)
		{
			input_fr[i] = input_fr[0];
			input_fr[i].y = (200 + 64 * (i - num * 3)) + rand() % 40;
		}

		if (i >= num * 4 && i < 20)
		{
			input_fr[i] = input_fr[0];
			input_fr[i].y = (200 + 64 * (i - num * 4)) + rand() % 40;
		}
	}
	
	for (int i = 0; i < 6; i++)
	{
		//criando uma função para divir o sprite sheet, sem precisar fazer isso manualmente
		SDL_Rect temp;
		temp.x = (384 / 6) * i;
		temp.y =  0;
		temp.w = 384/6;
		temp.h = 64;
		fire_rect[i] = temp;
	}

	for (int i = 1; i < 9; i++)
	{
		//criando uma função para divir o sprite sheet, sem precisar fazer isso manualmente
		SDL_Rect temp1;
		temp1.x = (576 / 9) * i;
		temp1.y = 256 - 64;
		temp1.w = 64;
		temp1.h = 64;
		sk_sprt[i - 1] = temp1;
	}


	
	plyr player;

	player.x = 600;
	player.y = 700;
	player.img = img;
	player.w = w;
	player.h = h;

	int check = 0;

	SDL_Rect plyr_spr;

	plyr_spr.x = 0;
	plyr_spr.y = 0;
	plyr_spr.w = 576 / 9;
	plyr_spr.h = 256 / 4;

	map = 0;
	int ms_x, ms_y, click = 0, destroy[1000], kill = 0;

	//começo do game loop
	while (1)
	{
		if (j == 4)
		{
			i++;
			j = 0;
		}

		if (k == 5)
		{
			f++;
			k = 0;
		}

		if (i > 7)
		{
			i = 0;
		}

		if (f > 5)
		{
			f = 0;
		}
		if (fps == 4)
		{
			fps = 0;
		}

		if(kill == 20 && i == 7)
		{
			kill = 21;
		}

			
		//verifica se algum evento está acontecendo
		SDL_Event evnt;	
		//se tiver algumevento acontecendo, retorna 1, senão retorna 0
		if (SDL_PollEvent(&evnt))
		{

			if (evnt.type == SDL_QUIT)
			{
				break;
			}
			else if (evnt.type == SDL_KEYDOWN && evnt.key.keysym.sym == SDLK_ESCAPE)
			{
				break;
			}

			ms_x = evnt.button.x;
			ms_y = evnt.button.y;
			//printf("%d %d\n", ms_x, ms_y);

			if (evnt.button.type == SDL_MOUSEBUTTONDOWN && evnt.button.button == SDL_BUTTON_LEFT)
			{
				click = 1;
			}
			else
			{
				click = 0;
			}


			if (map > 0)
			{
				if (evnt.type == SDL_KEYDOWN && evnt.key.keysym.sym == SDLK_w)
				{
					//player.y -= vel;
					if (player.y > 0 - 17)
					{
						if (map != 2)
						{
							move(&player, 1, 0, 0, 0, &plyr_spr, can);
						}
						else
						{
							if (player.y + 17 > 271)
							{
								move(&player, 1, 0, 0, 0, &plyr_spr, can);
							}
						}
					}

					can = 1;


				}
				else if (evnt.type == SDL_KEYDOWN && evnt.key.keysym.sym == SDLK_a)
				{
					//player.x -= vel;
					if (player.x + 17 > 0)
					{
						if (map != 2)
						{
							move(&player, 0, 0, 1, 0, &plyr_spr, can);
						}
						else
						{
							if (player.x + 17 > 469)
							{
								move(&player, 0, 0, 1, 0, &plyr_spr, can);
							}
						}
					}
					can = 2;
				}
				else if (evnt.type == SDL_KEYDOWN && evnt.key.keysym.sym == SDLK_s)
				{
					//player.y += vel;
					if (player.y + h/4 < 768)
					{

						move(&player, 0, 1, 0, 0, &plyr_spr, can);
					}
					can = 3;
				}
				else if (evnt.type == SDL_KEYDOWN && evnt.key.keysym.sym == SDLK_d)
				{
					//player.x += vel;
					if (player.x + w/9 - 17 < 1366)
					{
						if (map != 2)
						{
							move(&player, 0, 0, 0, 1, &plyr_spr, can);
						}
						else 
						{
							if (player.x + w / 9 - 17 < 854)
							{
								move(&player, 0, 0, 0, 1, &plyr_spr, can);
							}
						}
					}
					can = 4;
				}

				//setando animação para quando soltar a tecla

				if (evnt.type == SDL_KEYUP && evnt.key.keysym.sym == SDLK_w)
				{
					plyr_spr.x = 0;
					plyr_spr.y = 0;
				}
				else if (evnt.type == SDL_KEYUP && evnt.key.keysym.sym == SDLK_a)
				{
					plyr_spr.x = 0;
					plyr_spr.y = 64;
				}
				else if (evnt.type == SDL_KEYUP && evnt.key.keysym.sym == SDLK_s)
				{
					plyr_spr.x = 0;
					plyr_spr.y = 64 * 2;
				}
				else if (evnt.type == SDL_KEYUP && evnt.key.keysym.sym == SDLK_d)
				{
					plyr_spr.x = 0;
					plyr_spr.y = 64 * 3;
				}

			}
		}
			//mapa 0
		if (map == 0)
		{
			SDL_RenderClear(render);

			SDL_RenderCopy(render, back2, NULL, NULL);

			button_(button, render, &map, ms_x, ms_y, click);

			SDL_RenderPresent(render);

		}
		//mapa de inicio
		if (map == 2)
		{
			SDL_Rect next;
			next.x = 658;
			next.y = 271;
			next.w = w / 9;
			next.h = 10;
			SDL_RenderClear(render);
			SDL_RenderCopy(render, back_lake, NULL, NULL);
			print(&player, render, plyr_spr, input_fr, input_fr, &kill);
			SDL_RenderPresent(render);
			SDL_Rect next2;
			next2.x = player.x;
			next2.y = player.y;
			next2.w = player.w / 9;
			next2.h = player.h / 4;
			if (SDL_HasIntersection(&next2, &next))
			{
				SDL_Delay(250);
				player.x = 580;
				player.y = 650;
				map = 1;
			}
			//quadrado x: 658 y: 271 // limite x: 469 x:854
			

		}
			//mapa 1
			if (map == 1)
			{

				SDL_RenderClear(render);

				SDL_RenderCopy(render, background, NULL, NULL);

				SDL_RenderCopy(render, fire, &fire_rect[f], &fire_input);

				for (count = 0; count < 20; count++)
				{
					//num2 = 96
					if (destroy[count] != 1)
					{
						SDL_RenderCopy(render, sk, &sk_sprt[i], &input_fr[count]);
					}
					if (count == 0 && fps == 3 && destroy[count] != 1)
					{
						input_fr[count].x += 4;
					}

					if (count > 0 && fps == 3 && destroy[count] != 1) //&& ((input_fr[count - 1].x - input_fr[count].x) >= 50))
					{
						input_fr[count].x += 4;
					}

					///*
					if (p < num2 && count > 0 && count < num)
					{
						input_fr[count].x = -50;
					}

					if (p < num2 * 2 && count >= num && count < num * 2)
					{
						input_fr[count].x = -50;
					}

					if (p < num2 * 3 && count >= num * 2 && count < num * 3)
					{
						input_fr[count].x = -50;
					}

					if (p < num2 * 4 && count >= num * 3 && count < num * 4)
					{
						input_fr[count].x = -50;
					}

					if (p < num2 * 5 && count >= num * 4 && count < 20)
					{
						input_fr[count].x = -50;
					}
					//*/
					
					col(&map, input_fr[count]);

				}

				print(&player, render, plyr_spr, input_fr, destroy, &kill);

				SDL_RenderPresent(render);


				j++;
				fps++;
				k++;
				p++;

				if (kill >= 21)
				{
					SDL_Delay(350);
					map = 3;
				}

			}
		//mapa final
			if (map == 3)
			{
				SDL_RenderClear(render);

				SDL_RenderCopy(render, back_win, NULL, NULL);

				SDL_RenderPresent(render);
			}
		//mapa game over 
			if (map == 4)
			{
				SDL_RenderClear(render);

				SDL_RenderCopy(render, back_end, NULL, NULL);

				SDL_RenderPresent(render);
			}



			SDL_Delay(1000 / 30);
	}

	SDL_DestroyTexture(img);
	SDL_DestroyRenderer(render);
	SDL_DestroyWindow(janela);

	
}



void move(plyr* player, int up, int down, int lft, int right, SDL_Rect* plyr_spr, int can)
{
	//movendo o jogador para a direita
	player->x += right * vel;
	if (right > 0)
	{
		plyr_spr->x += 64;
		plyr_spr->y = 64 * 3;
		if (can != 4)
		{
			plyr_spr->x = 0;
			plyr_spr->y = 64 * 3;
		}
		if (plyr_spr->x >= 576 )
		{
			plyr_spr->x = 64;
		}
	}
	//movendo o jogador para a esquerda
	player->x -= lft * vel;
	if (lft > 0)
	{
		plyr_spr->x += 64;
		plyr_spr->y = 64;
		if (can != 2)
		{
			plyr_spr->x = 0;
			plyr_spr->y = 64;
		}
		if (plyr_spr->x >= 576)
		{
			plyr_spr->x = 64;
		}
	}

	//movendo o jogador para cima
	player->y -= up * vel;
	if (up > 0)
	{
		plyr_spr->x += 64;
		plyr_spr->y = 0;
		if (can != 1)
		{
			plyr_spr->x = 0;
			plyr_spr->y = 0;
		}
		if (plyr_spr->x >= 576)
		{
			plyr_spr->x = 64;
		}
	}

	//movendo o jogador para baixo
	player->y += down * vel;
	if (down > 0)
	{
		plyr_spr->x += 64;
		plyr_spr->y = 64 * 2;
		if (can != 3)
		{
			plyr_spr->x = 0;
			plyr_spr->y = 64 * 2;
		}
		if (plyr_spr->x >= 576)
		{
			plyr_spr->x = 64;
		}
	}
 
}

void print(plyr* player, SDL_Renderer* render, SDL_Rect plyr_spr, SDL_Rect input_fr[], int destroy[], int* kill)
{
	//criando um retangulo que vai ser onde iremos imprimir e definir o tamnho e posição do jogador(o retangulo é uma estrutura ja pronta)
	SDL_Rect rect;
	SDL_Rect* hp_rct; 
	hp_rct = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	int i;
	rect.x = player->x;
	rect.y = player->y;
	rect.w = player->w/9;
	rect.h = player->h/4;

	hp_rct = &rect;
	SDL_RenderCopy(render, player->img, &plyr_spr, &rect);
	for (i = 0; i < 20; i++)
	{
		if (SDL_HasIntersection(&rect, &input_fr[i]) && destroy[i] != 1)
		{
			destroy[i] = 1;
			*kill = *kill + 1;
			printf("%d\n", *kill);
		}
	}
	//SDL_HasIntersection(&rect, &input_fr[i])

}

void button_(SDL_Texture* button, SDL_Renderer* render, int* map, int ms_x, int ms_y, int click)
{
	SDL_Rect but_rect;
	SDL_Rect ms_rect;
	ms_rect.x = ms_x;
	ms_rect.y = ms_y;
	ms_rect.w = 3;
	ms_rect.h = 3;

	but_rect.x = (1366/2) - (400/2);
	but_rect.y = (768/2) - (100/2);
	but_rect.w = 275;
	but_rect.h = 115;

	SDL_RenderCopy(render, button, NULL, &but_rect);

	if (SDL_HasIntersection(&but_rect, &ms_rect) && click == 1)
	{
		*map = 2;
    }

}

void col(int* map, SDL_Rect skeleton)
{
	SDL_Rect rect;
	rect.x = 1366 + 50;
	rect.y = 223;
	rect.h = 350;
	rect.w = 2;

	if (SDL_HasIntersection(&rect, &skeleton))
	{
		*map = 4;
	}
}

/*
SDL_Window* janela;

	//cria a janela
	janela = SDL_CreateWindow("joguinho", 500, 100, 640, 480, SDL_WINDOW_RESIZABLE);

	//cria a tela de pintura
	SDL_Renderer* renderer = SDL_CreateRenderer(janela, -1, SDL_RENDERER_ACCELERATED);

	//o que vai ser botado no topo da tela de pintura
	SDL_Surface* surface = IMG_Load("images/goblin.png");

	//imagem para ser botado no topo da tela de pintura
	SDL_Texture* img = SDL_CreateTextureFromSurface(renderer, surface);

	//libera o topo da tela de pintura
	SDL_FreeSurface(surface);

	//desenha na tela de pintura
	//SDL_RenderCopy(renderer, img, NULL, NULL);
	//tela de pintura atual
	SDL_RenderPresent(renderer);

	if(i >= num && i < num * 2)
		{
			input_fr[i] = input_fr[0];
			input_fr[i].y = (100 + 64 * (i - num));// +rand() % 32;
		}
*/