#include <iostream>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdio.h>
#include <stdbool.h>
#include <list>
using namespace std;

const int LARGURA_TELA = 640;
const int ALTURA_TELA = 680;
 
ALLEGRO_DISPLAY *janela = NULL;
ALLEGRO_EVENT_QUEUE *fila_eventos = NULL;
ALLEGRO_BITMAP *nave = NULL;
ALLEGRO_BITMAP *tela_inicial = NULL;
ALLEGRO_BITMAP *aliens = NULL;
ALLEGRO_FONT *fonte = NULL;
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_SAMPLE *sample = NULL;
ALLEGRO_SAMPLE *fundo = NULL;
ALLEGRO_SAMPLE *bonus = NULL;
ALLEGRO_SAMPLE *morte = NULL;
ALLEGRO_AUDIO_STREAM *musica = NULL;

const float FPS = 60;
bool inicializar();

class Aliens{
public:
	int value;
	int posX;
	int posY;
	bool alive;

	Aliens(){
		value = 0;
		alive = true;
		aliens = al_load_bitmap("spriteSI.png");
	}

	void setValue(int v){
		value = v;
	}
	
	int getValue(){
		return value;
	}

	int getPosX(){
		return posX;
	}

	int getPosY(){
		return posY;
	}

	void setPosX(int px){
		posX = px;
	}

	void setPosY(int py){
		posY = py;
	}
	int getAlive(){
		return alive;
	}

	void setAlive( bool a){
		alive = a;
	}
};

class Shot{
public:
	int posX;
	int posY;
	bool active;
	int maxShot;

	Shot(){
		maxShot = 12;
	}

	int getPosX(){
		return posX;
	}

	int getPosY(){
		return posY;
	}

	void setPosX(int px){
		posX = px;
	}

	void setPosY(int py){
		posY = py;
	}

	void setActive(bool ac){
		active = ac;
	}

	bool getActive(){
		return active;
	}

};

class Barreira{
public:
	int posX;
	int posY;
	int kind;
	bool active;
	
	Barreira(){
		active = true;
		kind = 0;
	}

	int getPosX(){
		return posX;
	}

	int getPosY(){
		return posY;
	}

	void setPosX(int px){
		posX = px;
	}

	void setPosY(int py){
		posY = py;
	}
	
	void setKind(int k){
		kind = k;
	}

	int getKind(){
		return kind;
	}

	bool getActive(){
		return active;
	}
		
	void setActive(bool ac){
		active = ac;
	}
};

class Nave{
public:
	int posX;
	int posY;
	int speed;
	int life;
	
	
	Nave(){
		nave = al_load_bitmap("spriteSI.png");
		life = 3;
		
		
	}

	int getPosX(){
		return posX;
	}

	int getPosY(){
		return posY;
	}

	void setPosX(int px){
		posX = px;
	}

	void setPosY(int py){
		posY = py;
	}

	void setLife(int l){
		life = l;
	}

	int getLife(){
		return life;
	}

	void draw(){
		//al_draw_bitmap_region(nave,474,83,44,32,posX,posY,0);
		al_draw_bitmap_region(nave,357,20,43,27,posX,posY,0);

		for(int i=0; i<life; i++){
			al_draw_bitmap_region(nave,357,20,43,27,20+i*55,ALTURA_TELA-50,0);
		}
	}

	void right(){
		if(posX+speed < LARGURA_TELA-93){
			posX = posX+speed;
			al_draw_bitmap_region(nave,357,20,43,27,posX,posY,0);
		}
	}
		
	void left(){
		if(posX+speed > 50 ){
			posX = posX-speed;
			al_draw_bitmap_region(nave,357,20,43,27,posX,posY,0);
		}
	}

	void setSpeed(int s){
		speed = s;
	}

	int getSpeed(){
		return speed;
	}

	
};

class SpaceInvaders{
public:
	int score;
	int speedAliens;
	int nAliens;
	int e;
	
	Aliens **a;
	Aliens especial;
	Barreira *b;
	Nave n;
	Shot *shot;
	Shot *shotAliens;


	SpaceInvaders(){
		a = new Aliens*[6];
		for(int i=0; i<6; i++){	
			a[i] = new Aliens[6];
		}

		shot = new Shot[12];
		for(int i=0; i<12; i++){		
			shot[i].setActive(false);	
		}
		
		e = 1;

		shotAliens = new Shot[2];
		for(int i=0; i<2; i++){		
			shotAliens[i].setActive(false);	
		}

		especial.setValue(100);
		especial.setPosX(0);
		especial.setPosY(90);
		especial.setAlive(false);

		fonte = al_load_font("space_invaders.ttf", 20, 0);
		b = new Barreira[3];

		n.setPosX(LARGURA_TELA/2-47);
		n.setPosY(ALTURA_TELA-90);
		n.setSpeed(6);

		nAliens = 36;
		score = 0;
		aliens = al_load_bitmap("spriteSI.png");
	}

	void initialize(){
		for(int i=0; i<6; i++){
			for(int j=0; j<6; j++){
				if(i == 0){
					al_draw_bitmap_region(aliens,5,20,30,28,50+70*j,130,0);
					a[i][j].setValue(30);
					a[i][j].setPosX(50+ 70*j);
					a[i][j].setPosY(130);
				}
				else if(i == 1){
					al_draw_bitmap_region(aliens,5,20,30,28,50+ 70*j,130+40,0);
					a[i][j].setValue(25);
					a[i][j].setPosX(50+ 70*j);
					a[i][j].setPosY(130+40);
				}				
				else if(i == 2){
					al_draw_bitmap_region(aliens,92,20,37,30,50+ 70*j,130+80,0);
					a[i][j].setValue(20);
					a[i][j].setPosX(50+ 70*j);
					a[i][j].setPosY(130+80);
				}
				else if(i == 3){
					al_draw_bitmap_region(aliens,92,20,37,30,50+ 70*j,130+120,0);
					a[i][j].setValue(15);
					a[i][j].setPosX(50+ 70*j);
					a[i][j].setPosY(130+120);
				}				
				else if(i == 4){
					al_draw_bitmap_region(aliens,186,20,38,33,50+ 70*j,130+160,0);
					a[i][j].setValue(10);
					a[i][j].setPosX(50+ 70*j);
					a[i][j].setPosY(130+160);
				}				
				else if(i == 5){
					al_draw_bitmap_region(aliens,186,20,38,33,50+ 70*j,130+200,0);
					a[i][j].setValue(5);
					a[i][j].setPosX(50+ 70*j);
					a[i][j].setPosY(130+200);
				}
			}
		}

		al_draw_bitmap_region(aliens,410,5,62,46,LARGURA_TELA/4 -31,540,0);
		b[0].setPosX(LARGURA_TELA/4 -31);
		b[0].setPosY(540);
		al_draw_bitmap_region(aliens,410,5,62,46,LARGURA_TELA/4*2-31,540,0);
		b[1].setPosX(LARGURA_TELA/4*2-31);
		b[1].setPosY(540);
		al_draw_bitmap_region(aliens,410,5,62,46,LARGURA_TELA/4*3-31,540,0);
		b[2].setPosX(LARGURA_TELA/4*3-31);
		b[2].setPosY(540);
	}


	void right_nave(){
		n.right();
	}
		
	void left_nave(){
		n.left();
	}

	void tiro(){
		for(int i=0; i<12; i++){		
			if(shot[i].getActive() == false){
				shot[i].setActive(true);
				shot[i].setPosX(n.getPosX() + (43/2));
				shot[i].setPosY(n.getPosY()-20);
				break;
			}
		}
	}

	void draw_nave(){
		n.draw();
	}

	int aliensWalking(int direction){
		for(int i=0; i<6; i++){
			for(int j=0; j<6; j++){
				a[i][j].setPosX(a[i][j].getPosX()+direction);
			}
		}
		if(a[5][5].getPosX() >= 590 -34){
			if(direction == 1){
				for(int i=0; i<6; i++){
					for(int j=0; j<6; j++){
						a[i][j].setPosY(a[i][j].getPosY()+20);
					}
				}
			}
			return -1;
		}
		else if(a[0][0].getPosX() <= 50){
			if(direction == -1){
				for(int i=0; i<6; i++){
					for(int j=0; j<6; j++){
						a[i][j].setPosY(a[i][j].getPosY()+5);
					}
				}
			}
			return 1;
		}
		else
			return direction;
	}

	void score_text(){
		al_draw_bitmap_region(aliens,10,70,165,30,LARGURA_TELA/2-80,10,0);
		al_draw_textf(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA/2,50, ALLEGRO_ALIGN_CENTER, "00000");
		al_draw_textf(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA-40, 10, ALLEGRO_ALIGN_RIGHT, "SCORE<2>");
		al_draw_textf(fonte, al_map_rgb(255, 255, 255), 35, 10, ALLEGRO_ALIGN_LEFT, "SCORE<1>");
		al_draw_textf(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA-70, ALTURA_TELA-40, ALLEGRO_ALIGN_RIGHT, "CREDIT   00");
		al_draw_textf(fonte, al_map_rgb(255, 255, 255), 60, 50, ALLEGRO_ALIGN_LEFT, "%04d", score);
		al_draw_line( 0, ALTURA_TELA-50, LARGURA_TELA, ALTURA_TELA-50, al_map_rgb(0, 255, 0), 1);
	}

	void print(int estado){
		score_text();
		if(estado == 0){
			if(e == 1){
				al_play_sample(fundo, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				e = 0;
			}
			for(int i=0; i<6; i++){
				for(int j=0; j<6; j++){
					if(i == 0){
						if(a[i][j].getAlive() == true)
							al_draw_bitmap_region(aliens,5,20,30,28,a[i][j].getPosX(),a[i][j].getPosY(),0);
					}
					else if(i == 1){
						if(a[i][j].getAlive() == true)
							al_draw_bitmap_region(aliens,5,20,30,28,a[i][j].getPosX(),a[i][j].getPosY(),0);	
					}				
					else if(i == 2){
						if(a[i][j].getAlive() == true)
							al_draw_bitmap_region(aliens,92,20,37,30,a[i][j].getPosX(),a[i][j].getPosY(),0);
					
					}
					else if(i == 3){
						if(a[i][j].getAlive() == true)
							al_draw_bitmap_region(aliens,92,20,37,30,a[i][j].getPosX(),a[i][j].getPosY(),0);
					}				
					else if(i == 4){
						if(a[i][j].getAlive() == true)
							al_draw_bitmap_region(aliens,186,20,38,33,a[i][j].getPosX(),a[i][j].getPosY(),0);
	
					}				
					else if(i == 5){
						if(a[i][j].getAlive() == true)
							al_draw_bitmap_region(aliens,186,20,38,33,a[i][j].getPosX(),a[i][j].getPosY(),0);
					}
				}
			}
		}
		else{
			if(e == 0){
				al_play_sample(fundo, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				e = 1;
			}
			for(int i=0; i<6; i++){
				for(int j=0; j<6; j++){
					if(i == 0){
						if(a[i][j].getAlive() == true)
							al_draw_bitmap_region(aliens,52,20,30,28,a[i][j].getPosX(),a[i][j].getPosY(),0);
					}
					else if(i == 1){
						if(a[i][j].getAlive() == true)
							al_draw_bitmap_region(aliens,52,20,30,28,a[i][j].getPosX(),a[i][j].getPosY(),0);	
					}				
					else if(i == 2){
						if(a[i][j].getAlive() == true)
							al_draw_bitmap_region(aliens,140,20,37,30,a[i][j].getPosX(),a[i][j].getPosY(),0);
					
					}
					else if(i == 3){
						if(a[i][j].getAlive() == true)
							al_draw_bitmap_region(aliens,140,20,37,30,a[i][j].getPosX(),a[i][j].getPosY(),0);
					}				
					else if(i == 4){
						if(a[i][j].getAlive() == true)
							al_draw_bitmap_region(aliens,231,20,38,33,a[i][j].getPosX(),a[i][j].getPosY(),0);
	
					}				
					else if(i == 5){
						if(a[i][j].getAlive() == true)
							al_draw_bitmap_region(aliens,231,20,38,33,a[i][j].getPosX(),a[i][j].getPosY(),0);
					}
				}
			}
		}
		
		for(int i=0; i<12; i++){	 // TIROS NAVE
			if(shot[i].getActive() == true){
				al_draw_bitmap_region(aliens,535,88,8,18,shot[i].getPosX(),shot[i].getPosY(),0);
				shot[i].setPosY(shot[i].getPosY()-15);
				if(shot[i].getPosY() < 0){
					shot[i].setActive(false);
				}
			}
		}

		for(int i=0; i<2; i++){	 // TIROS ALIENS
			if(shotAliens[i].getActive() == true){
				al_draw_bitmap_region(aliens,535,88,8,18,shotAliens[i].getPosX(),shotAliens[i].getPosY(), ALLEGRO_FLIP_VERTICAL);
				shotAliens[i].setPosY(shotAliens[i].getPosY()+8);
				if(shotAliens[i].getPosY() > ALTURA_TELA-60){
					shotAliens[i].setActive(false);
				}
			}
		}

		for(int i=0; i<3; i++){ // BARREIRAS
			if((b[i].getKind() == 0 || b[i].getKind() == 1))
				al_draw_bitmap_region(aliens,410,5,62,46,b[i].getPosX(),b[i].getPosY(),0);
			else if((b[i].getKind() == 2 || b[i].getKind() == 3))
				al_draw_bitmap_region(aliens,624,2,62,47,b[i].getPosX(),b[i].getPosY(),0);
			else if((b[i].getKind() == 4 || b[i].getKind() == 5))
				al_draw_bitmap_region(aliens,623,72,62,46,b[i].getPosX(),b[i].getPosY(),0);
			else if((b[i].getKind() == 6 || b[i].getKind() == 7))
				al_draw_bitmap_region(aliens,556,2,62,46,b[i].getPosX(),b[i].getPosY(),0);
			else if((b[i].getKind() == 8 || b[i].getKind() == 9))
				al_draw_bitmap_region(aliens,483,2,62,46,b[i].getPosX(),b[i].getPosY(),0);
			else{
				b[i].setActive(false);
			}
		}

		if(especial.getAlive() == true){ //BONUS
			al_draw_bitmap_region(aliens,275,18,70,18,especial.getPosX(),especial.getPosY(),0);
		}

	}

	void colision_aliens(){
		for(int i=0; i<12; i++){
			if(shot[i].getActive() == true){
				for(int k=5; k>=0; k--){
					for(int j=0; j<6; j++){
						if(a[k][j].getAlive() == true){
							if(k == 5 || k == 4){
								if(a[k][j].getPosX() < shot[i].getPosX()+4 &&
								   a[k][j].getPosX()+38 > shot[i].getPosX()+4 &&
								   shot[i].getPosY() < a[k][j].getPosY()+15 &&
								   shot[i].getActive() == true){
									a[k][j].setAlive(false);
									shot[i].setActive(false);
									al_draw_bitmap_region(aliens,565,82,39,30,a[k][j].getPosX(),a[k][j].getPosY(),0);
									al_flip_display();
									score = score + a[k][j].getValue();
									break;
								}
							}
							if(k == 3 || k == 2){
								if(a[k][j].getPosX() < shot[i].getPosX()+4 &&
								   a[k][j].getPosX()+37 > shot[i].getPosX()+4 &&
								   shot[i].getPosY() < a[k][j].getPosY()+15 &&
								   shot[i].getActive() == true){
									a[k][j].setAlive(false);
									shot[i].setActive(false);
									al_draw_bitmap_region(aliens,565,82,39,30,a[k][j].getPosX(),a[k][j].getPosY(),0);
									al_flip_display();
									score = score + a[k][j].getValue();
									break;
								}
							}
							if(k == 1 || k == 0){
								if(a[k][j].getPosX() < shot[i].getPosX()+4 &&
								   a[k][j].getPosX()+30 > shot[i].getPosX()+4 &&
								   shot[i].getPosY() < a[k][j].getPosY()+15 &&
								   shot[i].getActive() == true){
									a[k][j].setAlive(false);
									shot[i].setActive(false);
									al_draw_bitmap_region(aliens,565,82,39,30,a[k][j].getPosX(),a[k][j].getPosY(),0);
									al_flip_display();
									score = score + a[k][j].getValue();
									break;
								}
							}
						}
					}
				}
				if(especial.getPosX() < shot[i].getPosX()+4 &&
				   especial.getPosX()+70 > shot[i].getPosX()+4 &&
				   shot[i].getPosY() < especial.getPosY()+18 &&
				   shot[i].getActive() == true){
					especial.setAlive(false);
					especial.setPosX(ALTURA_TELA);
					shot[i].setActive(false);
					al_draw_bitmap_region(aliens,565,82,39,30,especial.getPosX(),especial.getPosY(),0);
					al_flip_display();
					score = score + especial.getValue();
				}
			}
		}
	}

	void shooting_aliens(){
		int s1, s2;
		s1 = rand()%6;

		for(int j=0; j<6; j++){
			for(int i=5; i>=0; i--){
				for(int k=0; k<2; k++){
					if(a[i][j].getAlive() == true &&
						 shotAliens[k].getActive() == false &&
						 j==s1 ){
						shotAliens[k].setActive(true);
						if(a[i][j].getValue() == 5 || a[i][j].getValue() == 10){
							shotAliens[k].setPosX(a[i][j].getPosX() + (30/2));
							shotAliens[k].setPosY(a[i][j].getPosY() + 28);
						}
						else if(a[i][j].getValue() == 15 || a[i][j].getValue() == 20){
							shotAliens[k].setPosX(a[i][j].getPosX() + (36/2));
							shotAliens[k].setPosY(a[i][j].getPosY() + 30);
						}
						else if(a[i][j].getValue() == 25 || a[i][j].getValue() == 30){
							shotAliens[k].setPosX(a[i][j].getPosX() + (38/2));
							shotAliens[k].setPosY(a[i][j].getPosY() +  33);
						}
						return;
					}
				}
			}
		}
	}

	void colision_nave(){
		for(int i=0; i<2; i++){
			if(shotAliens[i].getPosX()+4 > n.getPosX() &&
			   shotAliens[i].getPosX()+4 < n.getPosX() + 43 &&
			   shotAliens[i].getPosY()+9 > n.getPosY() &&
			   shotAliens[i].getActive() == true ){
				
				n.setLife(n.getLife()-1);
				al_draw_bitmap_region(aliens,474,83,44,32,n.getPosX(), n.getPosY(),0);
				al_flip_display();
				al_play_sample(morte, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
				al_rest(1.2);
				shotAliens[i].setActive(false);
			}
		}
	}

	void colision_barreira(){
		for(int i=0; i<3; i++){
			for(int j=0; j<2; j++){	
				if(b[i].getActive() == true &&
				   shotAliens[j].getActive() == true &&
				   shotAliens[j].getPosX()+4 > b[i].getPosX() &&
				   shotAliens[j].getPosX()+4 < b[i].getPosX()+62 &&
				   shotAliens[j].getPosY()+9 > b[i].getPosY()){

					shotAliens[j].setActive(false);
					b[i].setKind(b[i].getKind() + 1);
				}
			}
		}
	}

	bool gameOver(){
		if(n.getLife() == -1){
			al_clear_to_color(al_map_rgb(0,0,0));
			al_draw_textf(fonte, al_map_rgb(255, 255, 255), LARGURA_TELA/2, ALTURA_TELA/2, ALLEGRO_ALIGN_CENTER, "SCORE %04d", score);
			al_flip_display();
			al_rest(10.0);
			return true;
		}
	}

	void bonus(){
		if(especial.getAlive() == true){
			especial.setPosX(especial.getPosX()+3);
		}
		else{
			if(score > 400 && especial.getPosX() < 10){
				especial.setAlive(true);
			}
		}
	}
};

int main(void){
	int f = 0;
    bool sair = false;
	srand( (unsigned)time(NULL) );
   
    if (!inicializar()){
        return -1;
    }

    int tecla = 0;
	int direction = 1;
	int vel = 2;

	al_clear_to_color(al_map_rgb(0,0,0));
	al_flip_display();

	SpaceInvaders si;
	si.initialize();
	al_flip_display();
	al_attach_audio_stream_to_mixer(musica, al_get_default_mixer());
	al_clear_to_color(al_map_rgb(0,0,0));
	tela_inicial = al_load_bitmap("space_invaders_sprite_sheet_by_gooperblooper22.png");
	al_draw_bitmap_region(tela_inicial,0,0,610,180, 20,240, 0);
	al_flip_display();
	al_rest(5.0);

    while (!sair){
		al_clear_to_color(al_map_rgb(0,0,0));
		if(int(al_get_time()*vel)%2 == 0){
			si.print(0);
		}
		else
			si.print(1);
		si.draw_nave();
		al_flip_display();

		//al_play_sample(fundo, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		//al_rest(0.5);
	    //al_set_audio_stream_playing(musica, true);
 

        ALLEGRO_EVENT evento;
		ALLEGRO_TIMEOUT timeout;
        al_init_timeout(&timeout, 0.0005);
       
 		int tem_eventos = al_wait_for_event_until(fila_eventos, &evento, &timeout);

        if (evento.type == ALLEGRO_EVENT_DISPLAY_CLOSE){
            sair = true;
        }
        
        if (evento.type == ALLEGRO_EVENT_KEY_DOWN){
            switch(evento.keyboard.keycode){
		        case ALLEGRO_KEY_SPACE:
		            tecla = 1;
					al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		            break;
		        case ALLEGRO_KEY_LEFT:
		            tecla = 2;
		            break;
		        case ALLEGRO_KEY_RIGHT:
		            tecla = 3;
		            break;
            }
        }
		/*else{
			tecla =0;
		}*/
		
		si.colision_aliens();
		if(int(al_get_time())%7 == 0){
			si.shooting_aliens();
		}
		if(int(al_get_time())%11 == 0){
			si.shooting_aliens();
		}
		/*if(int(al_get_time())%2 == 1 && int(al_get_time()) != f){
			f = int(al_get_time());
			al_play_sample(fundo, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		}*/

		if(tecla == 2)
			si.left_nave();
		else if(tecla == 3)
			si.right_nave();
		else if(tecla == 1){
			si.tiro();
			tecla = 0;
		}
			
		si.colision_nave();
		si.colision_barreira();
		direction = si.aliensWalking(direction);
		si.bonus();
		if(si.gameOver()){
			sair = true;
		}
		
		al_flip_display();
    }
     

	al_destroy_audio_stream(musica);
    al_destroy_sample(sample);
    al_destroy_display(janela);
    al_destroy_event_queue(fila_eventos);

    return 0;
}

bool inicializar(){

    if (!al_init())
    {
        fprintf(stderr, "Falha ao inicializar a Allegro.\n");
        return false;
    }
    al_init_font_addon();
    if (!al_init_ttf_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_ttf.\n");
        return false;
    }
    if (!al_init_image_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on allegro_image.\n");
        return false;
    }
 
    if (!al_install_keyboard())
    {
        fprintf(stderr, "Falha ao inicializar o teclado.\n");
        return false;
    }

    if (!al_init_primitives_addon())
    {
        fprintf(stderr, "Falha ao inicializar add-on de primitivas.\n");
        return false;
    }
    janela = al_create_display(LARGURA_TELA, ALTURA_TELA);
    if (!janela)
    {
        fprintf(stderr, "Falha ao criar janela.\n");
        return false;
    }
     // Inicializa o add-on para utilização de imagens
    al_init_image_addon();
    al_set_window_title(janela, "SPACE INVADERS - JOÃO VICTOR MARINHO BECHARA");
   
    fila_eventos = al_create_event_queue();
    if (!fila_eventos)
    {
        fprintf(stderr, "Falha ao criar fila de eventos.\n");
        al_destroy_display(janela);
        return false;
    }

	if (!al_install_audio())
    {
        fprintf(stderr, "Falha ao inicializar áudio.\n");
        return false;
    }
 
    if (!al_init_acodec_addon())
    {
        fprintf(stderr, "Falha ao inicializar codecs de áudio.\n");
        return false;
    }
 
    if (!al_reserve_samples(1))
    {
        fprintf(stderr, "Falha ao alocar canais de áudio.\n");
        return false;
    }

	sample = al_load_sample("shoot.wav");
    if (!sample)
    {
        fprintf(stderr, "Falha ao carregar sample.\n");
        al_destroy_display(janela);
        return false;
    }

	fundo = al_load_sample("fastinvader1.wav");
    if (!fundo)
    {
        fprintf(stderr, "Falha ao carregar sample.\n");
        al_destroy_display(janela);
        return false;
    }

	morte = al_load_sample("explosion.wav");
    if (!morte)
    {
        fprintf(stderr, "Falha ao carregar sample.\n");
        al_destroy_display(janela);
        return false;
    }
	
	bonus = al_load_sample("ufo_lowpitch.wav");
    if (!bonus)
    {
        fprintf(stderr, "Falha ao carregar sample.\n");
        al_destroy_display(janela);
        return false;
    }

    musica = al_load_audio_stream("fastinvader1.wav", 4, 1024);
    if (!musica)
    {
        fprintf(stderr, "Falha ao carregar audio.\n");
        al_destroy_event_queue(fila_eventos);
        al_destroy_display(janela);
        al_destroy_sample(sample);
        return false;
    }
 
    al_register_event_source(fila_eventos, al_get_keyboard_event_source());
    al_register_event_source(fila_eventos, al_get_display_event_source(janela));
 
    return true;
}
