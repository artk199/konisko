#include "cGame.h"
#include "cUnit.h"

cUnit::cUnit():_game(0)
{

}

void cUnit::init(cGame *game)
{
	//initialize base
	_game = game;
	_view = new Actor;
	_view->attachTo(game);

	//virtual method
	_init();
}

void cUnit::update(const UpdateState &us)
{
	//virtual method was overload in Player
	_update(us);
}