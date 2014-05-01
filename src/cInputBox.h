#pragma once
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

DECLARE_SMART(TextWithBackground, spTextWithBackground);
class TextWithBackground: public ColorRectSprite
{
public:
	TextWithBackground(const string &defText)
	{
		text = new TextActor;
		//text won't handle any touch event
		text->setInputEnabled(false);

		TextStyle style;
		style.color = Color::Black;
		style.hAlign = TextStyle::HALIGN_CENTER;
		style.vAlign = TextStyle::VALIGN_MIDDLE;
		style.multiline = true;
		style.font = Assets::gameResources.getResFont("main")->getFont();
		text->setStyle(style);
		text->setText(defText);

		addChild(text);		
	}

	spTextActor text;

	void sizeChanged(const Vector2 &size)
	{
		text->setSize(size);
	}
};

class TestInputText: public Actor
{
public:
	spInputText _input;
	spTextWithBackground _current;

	TestInputText()	
	{
		_input = new InputText;
		//_input->setAllowedSymbols("1234567890");
		//_input->setDisallowedSymbols("0");
		_input->addEventListener(Event::COMPLETE, CLOSURE(this, &TestInputText::onComplete));

		spTextWithBackground t = new TextWithBackground("click and edit me 1");
		t->setSize(200, 60);
		t->setPosition(100 - t->getWidth()/2, 100);
		t->attachTo(this);
		t->addEventListener(TouchEvent::CLICK, CLOSURE(this, &TestInputText::onClick));

		t = new TextWithBackground("click and edit me 2");		
		t->setSize(200, 60);
		t->setPosition(100- t->getWidth()/2, 170);
		t->attachTo(this);
		t->addEventListener(TouchEvent::CLICK, CLOSURE(this, &TestInputText::onClick));
	}

	void onClick(Event *ev)
	{
		if (_current)
		{
			_current->setColor(Color::White);
		}

		_current = safeSpCast<TextWithBackground>(ev->currentTarget);
		_input->start(_current->text);
		_current->setColor(Color::Red);
	}

	void onComplete(Event *ev)
	{
		if (_current)
		{
			_current->setColor(Color::White);
		}
		_current = 0;
		InputText::stopAnyInput();
	}

	~TestInputText()
	{
		InputText::stopAnyInput();
	}

	void clicked(string id)
	{		
		
	}
};
