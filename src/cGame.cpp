#include "cGame.h"
#include "oxygine-framework.h"
#include "Assets.h"
#include "Actor.h"
#include "Button.h"
#include "RenderState.h"
#include "core/STDFileSystem.h"
#include "cNotify.h"
#include "cUI.h"
#include <iostream>


#ifdef __S3E__
#include "s3eKeyboard.h"
#endif

using namespace oxygine;

//DECLARE_SMART is helper, it does forward declaration and declares intrusive_ptr typedef for your class
DECLARE_SMART(cGame, spcGame);

file::STDFileSystem extfs(true);


//---Konstruktor, ustawia wartosci poczatkowe dla klas glownych
cGame::cGame(){
	//Wczytanie asset�w
	Assets::load();

	notifies = new cNotify;
	//ustawienia wyswietlania informacji na ekranie
	memset(notifies->notifies, 0, sizeof(notifies->notifies));
	notifies->ui = new Actor;
	addChild(notifies->ui);

	setSize(getRoot()->getSize());
	delta = 0;

	//Dodanie mapy 
	_map = new cMap(this);

	//Dodanie gracza
	_player = new cPlayer(Vector2(0,0));
	_player->init(this);
	

	Input::instance.addEventListener(Input::event_platform, CLOSURE(this, &cGame::_onPlatform));





	//wystartowanie testowej aplikacji
	start();
};


//---Funkcja ustawia wartosci poczatkowe dla klasy
void cGame::init(){

}
void cGame::_onPlatform(Event *event)
	{
		_onSDLEvent((SDL_Event*)event->userData);
	}
int cGame::_onSDLEvent(SDL_Event *event)
	{
		switch(event->type)
		{
		case SDL_KEYDOWN:
			switch( event->key.keysym.sym ){
                    case SDLK_LEFT:
                        _player->move(Vector2(-64,0));
                        break;
                    case SDLK_RIGHT:
                        _player->move(Vector2(64,0));
                        break;
                    case SDLK_UP:
                        _player->move(Vector2(0,-64));
                        break;
                    case SDLK_DOWN:
                        _player->move(Vector2(0,64));
                        break;
                    default:
                        break;
                }
			break;
		}
	return 0;
}

//---Funkcja czysci pamiec po zakonczeniu zycia klasy
void cGame::destroy(){
	Assets::free();
	delete _map;
	delete _player;
}

//---Funkcja aktualizujaca czynnosci klasy
void cGame::doUpdate(const UpdateState &us)
{
	delta += us.dt;
	if (delta > 1000){
		delta = 0;
		displayClicked(NULL);
	}
}

//---************************************************FUNKCJE TESTOWE DLA APLIKACJI (Z SZABLONU)
void cGame::displayClicked(Event *event){
		notifies->notify("Jakies dluzsze informacje\n nawet w dwoch liniach Trolololololololololo");
		//lets create and run sprite with animation
		runSprite();
};

void cGame::start(){	
	EventCallback c = CLOSURE(this, &cGame::displayClicked);
	addChild(cUI::addButton(400,400, "trololo", c));
	addChild(cUI::addButton(400,500, "Trolololo2", c));
};

void cGame::runSprite(){
		spSprite sprite = new Sprite();		
		addChild(sprite);

		int duration = 500;//500 ms 
		int loops = -1;//infinity loops

		//animation has 7 columns, check res.xml
		ResAnim *animation = Assets::gameResources.getResAnim("tile1");

		//add animation tween to sprite
		//TweenAnim would change animation frames
		sprite->addTween(TweenAnim(animation), duration, loops);

		Vector2 destPos = getRoot()->getSize() - sprite->getSize();
		Vector2 srcPos = Vector2(0, destPos.y);
		//set sprite initial position
		sprite->setPosition(srcPos);		

		//add another tween: TweenQueue
		//TweenQueue is a collection of tweens
		spTweenQueue tweenQueue = new TweenQueue();
		tweenQueue->setDelay(1500);
		//first, move sprite to dest position
		tweenQueue->add(Sprite::TweenPosition(destPos), 1500, 1);
		//then fade it out smoothly
		tweenQueue->add(Sprite::TweenAlpha(0), 1000, 1);
		
		sprite->addTween(tweenQueue);

		//and remove sprite from tree when tweenQueue is empty
		//if you don't hold any references to sprite it would be deleted automatically
		tweenQueue->setDetachActor(true);		
};

