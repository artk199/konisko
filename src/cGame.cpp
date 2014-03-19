#include "oxygine-framework.h"
#include "cGame.h"
#include "cAssets.h"

using namespace oxygine;

//DECLARE_SMART is helper, it does forward declaration and declares intrusive_ptr typedef for your class
DECLARE_SMART(cGame, spcGame);


Resources gameResources;


//---Konstruktor, ustawia wartosci poczatkowe dla klas glownych
cGame::cGame(){
	//wczytanie danych
	assets = new cAssets;

	//load xml file with resources definition
	assets->LoadResources();

	//wystartowanie testowej aplikacji
	start();
};


//---Funkcja ustawia wartosci poczatkowe dla klasy
void cGame::Game_Initialize(){

	//lets create our client code simple actor
	//prefix 'sp' here means it is intrusive Smart Pointer
	//it would be deleted automatically when you lost ref to it	
	spcGame actor = new cGame;
	//and add it to RootActor as child
	getRoot()->addChild(actor);
}

//---Funkcja czysci pamiec po zakonczeniu zycia klasy
void cGame::Game_Destroy(){
	gameResources.free();
}

//---Funkcja aktualizujaca czynnosci klasy
void cGame::Game_Update(){
	
}

//---************************************************FUNKCJE TESTOWE DLA APLIKACJI (Z SZABLONU)
void cGame::displayClicked(Event *event){
		//user clicked to button
		_text->setText("wtf5!");

		//lets create and run sprite with animation
		runSprite();
};

void cGame::start(){	
		//create Button actor
		spButton button = new Button();
		//add it as child to current actor
		addChild(button);

		//you will find 'button' resource definition in res.xml
		//button has 3 columns for each state: Normal, Pressed, Overed
		button->setResAnim(assets->gameResources.getResAnim("button"));
		//centered button at screen	
		Vector2 pos = getRoot()->getSize()/2 - button->getSize()/2;
		button->setPosition(pos);
		button->setInputChildrenEnabled(false);

		//handle click to button
		EventCallback cb = CLOSURE(this, &cGame::displayClicked);
		button->addEventListener(TouchEvent::CLICK, cb);

		//second part

		//create Actor with Text and it to button as child
		spTextActor text = new TextActor();
		text->attachTo(button);
		//centered in button
		text->setPosition(button->getSize()/2);

		//initialize text style
		//it would be centered and colored
		TextStyle style;
		style.font = assets->gameResources.getResFont("main")->getFont();
		style.color = Color(72, 61, 139, 255);
		style.vAlign = TextStyle::VALIGN_MIDDLE;
		style.hAlign = TextStyle::HALIGN_CENTER;

		text->setStyle(style);
		text->setText("click\nme!");

		//we will change text later
		_text = text; 
};

void cGame::runSprite(){
		spSprite sprite = new Sprite();		
		addChild(sprite);

		int duration = 500;//500 ms 
		int loops = -1;//infinity loops

		//animation has 7 columns, check res.xml
		ResAnim *animation = assets->gameResources.getResAnim("anim");

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


