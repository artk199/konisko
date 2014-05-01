/*#pragma once
#include "cUI.h"
#include "InputText.h"
#include "Actor.h"
#include "Button.h"
#include "RenderState.h"
#include "cMap.h"
#include "cPlayer.h"
#include "oxygine-framework.h"
#include "Assets.h"
#include "cGame.h"
#include "cInputBox.h"

cInputBox::cInputBox()	{
	_input = new InputText;
	//_input->setAllowedSymbols("1234567890");
	//_input->setDisallowedSymbols("0");
	_input->addEventListener(Event::COMPLETE, CLOSURE(this, &cInputBox::onComplete));
}

void cInputBox::onClick(Event *ev){
	if (_current) _current->setColor(Color::White);
	

	//_current = safeSpCast<spTextActor>(ev->currentTarget);
	_input->start(_current);
	_current->setColor(Color::Red);
};

void cInputBox::onComplete(Event *ev)
{
	if (_current) _current->setColor(Color::White);
	
	_current = 0;
	InputText::stopAnyInput();
}

cInputBox::~cInputBox(){
	InputText::stopAnyInput();
}

void cInputBox::clicked(string id){		
		
}

void cInputBox::Ustaw(string &nap, int x, int y){
	spTextActor t = cUI::createText(nap, Color::Black);

	t->setPosition(x - t->getWidth()/2,y);
	t->attachTo(this);
	t->addEventListener(TouchEvent::CLICK, CLOSURE(this, &cInputBox::onClick));
};
*/